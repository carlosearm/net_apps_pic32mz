/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */

#include "math.h"
#include "dac.h"

DWORD ADDR_0 = 0x00000000;
DWORD ADDR_1 = 0x00010000;
DWORD ADDR_2 = 0x00020000;
DWORD ADDR_3 = 0x00030000;

DWORD CMD_WRITE_SPAN         = 0x00600000; //write span to n
DWORD CMD_WRITE_SPAN_ALL     = 0x00E00000; //write span to all
DWORD CMD_WRITE_UPDATE       = 0x00300000; //write code to n, update n
DWORD CMD_WRITE_UPDATE_ALL   = 0x00A00000; //write code to all, update all
DWORD CMD_POWER_DOWN_CHANNEL = 0x00400000; // Power down output.
DWORD CMD_POWER_DOWN_CHIP    = 0x00500000; // Power down chip.

DWORD DAC_MIN_VALUE = 0x00000000;
DWORD DAC_MAX_VALUE = 0x0000FFFF;

DWORD DAC_SPAN_0_5   = 0x00000000;
DWORD DAC_SPAN_0_10  = 0x00000001;
DWORD DAC_SPAN_10_10 = 0X00000003;

DWORD last_bias_value;

/*
DWORD DAC_MUX_CMD   = 0x00B00000;
DWORD DAC_MUX_VOUT0 = 0x00000011;
DWORD DAC_MUX_VOUT1 = 0x00000011;
DWORD DAC_MUX_VOUT2 = 0x00000015;
DWORD DAC_MUX_VOUT3 = 0x00000016;
DWORD DAC_MUX_REF   = 0x00000019;
DWORD DAC_MUX_VPLUS = 0x0000001B;
*/
/*
unsigned int data = 0x00F00000;
unsigned int write_max_cmd = 0x0000FFFF;
unsigned int write_min_cmd = 0x00000000;
unsigned int toggle_b_cmd = 0x00C00001;
unsigned int global_toggle_cmd = 0x00D00000;
unsigned int update_cmd = 0x00100000;
unsigned int write_update_all_cmd = 0x80A0FFFF;
unsigned int write_update_all_zero_cmd = 0x80A00000;

static uint update_all_zero_cmd = 0x80A00000;
 */

void DAC_Reset(void)
{
    last_bias_value = 0;
    DWORD cmd = CMD_WRITE_SPAN_ALL | DAC_SPAN_0_10;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    cmd = CMD_WRITE_UPDATE_ALL | DAC_MIN_VALUE;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    DAC_SetCoronaDoseZero();
    //DAC_DisableMux();
}

void DAC_SetCoronaDose(double dose)
{
    DWORD value = DAC_MAX_VALUE * dose / 10.0;
    value &= DAC_MAX_VALUE; //safety thing
    DWORD cmd = CMD_WRITE_SPAN | ADDR_0 | DAC_SPAN_0_10;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    cmd = CMD_WRITE_UPDATE | ADDR_0 | value;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
}

void DAC_SetCoronaDoseZero(void)
{
    /*DWORD cmd = CMD_WRITE_UPDATE | ADDR_0 | DAC_MIN_VALUE;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);*/
    DWORD cmd = CMD_WRITE_SPAN | ADDR_0 | DAC_SPAN_10_10;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    cmd = CMD_WRITE_UPDATE | ADDR_0 | 0x799A;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
}

void DAC_SetBiasValue(double dose)
{
    DWORD value = DAC_MAX_VALUE * dose / 10.0;
    value &= DAC_MAX_VALUE;
    DWORD cmd = CMD_WRITE_UPDATE | ADDR_2 | value;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    last_bias_value = value;
    /*if (value <= last_bias_value)
    {
        DWORD cmd = CMD_WRITE_UPDATE | ADDR_2 | value;
        SPI1_Write(&cmd, 4);
        last_bias_value = value;
    }
    else
    {
        while(value > last_bias_value)
        {
            DWORD cmd = CMD_WRITE_UPDATE | ADDR_2 | last_bias_value;
            SPI1_Write(&cmd, 4);
            last_bias_value++;
            CORETIMER_DelayUs(1);
        }
    }*/
}

void DAC_SetChuckBiasValue(double dose)
{
    if (fabs(dose) <= 10)
    {
        DWORD cmd = CMD_WRITE_SPAN | ADDR_3 | DAC_SPAN_10_10;
        SPI1_Write(&cmd, 4);
        CORETIMER_DelayUs(1);
        dose = dose / 2.0 + 5.0;
        DWORD value = DAC_MAX_VALUE * dose /10.0;
        value &= DAC_MAX_VALUE;
        cmd = CMD_WRITE_UPDATE | ADDR_3 | value;
        SPI1_Write(&cmd, 4);
        CORETIMER_DelayUs(1);
    }
}

void DAC_PowerDown()
{
    DWORD cmd = CMD_POWER_DOWN_CHANNEL | ADDR_0 | ADDR_1 | ADDR_2 | ADDR_3;
    SPI1_Write(&cmd, 4);
    CORETIMER_DelayUs(1);
    SPI1_Write(&CMD_POWER_DOWN_CHIP, 4);
    CORETIMER_DelayUs(1);
}

/*    
void DAC_DisableMux(void)
{
    DWORD cmd = DAC_MUX_CMD;// | DAC_MUX_VOUT0;
    SPI1_Write(&cmd, 4);
}
    
void DAC_SetMuxOut(uint vout)
{
    DWORD cmd = DAC_MUX_CMD;
    switch(vout)
    {
        case 0  :
            cmd |= DAC_MUX_VOUT0;
            break;
        case 1  :
            cmd |= DAC_MUX_VOUT1;
            break;
        case 2  :
            cmd |= DAC_MUX_VOUT2;
            break;
        case 3  :
            cmd |= DAC_MUX_VOUT3;
            break;
    }
    SPI1_Write(&cmd, 4);
}
*/

/* *****************************************************************************
 End of File
 */
