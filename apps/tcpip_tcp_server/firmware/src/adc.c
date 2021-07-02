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
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "adc.h"

ADC_DATA_TYPE initial_control = {.controlbyte = 1};

DWORD read_none = 0;
DWORD read_cmd = 0x84000000;
DWORD receive = 0;

void ADC_Initialize(void)
{
    //ADC_CNV_Clear();
    ADC_STAT = ADC_WAIT_INIT;
}

static int DoOneReading();

void ADC_Tasks()
{
   switch(ADC_STAT)
    {
        case ADC_WAIT_INIT:
            //Do something to configure ADC at initialization.
            SPI3_WriteRead(&read_cmd, 4, &receive, 4);
            ADC_STAT = ADC_IDLE;
            break;
        case ADC_BUSY:
            break;
        case ADC_IDLE:
            break;
    }
}


static int DoOneReading()
{
    ADC_CNV_Set();
    ADC_CNV_Clear();
    while (ADC_BUSY_Get() != 0)
    {
    }
    return SPI3_WriteRead(&read_cmd, 4, &receive, 4);
    //return SPI3_WriteRead(&read_none, 4, &receive, 4);
}

int ADC_ReadMonitorLines(void)
{
    if (ADC_STAT != ADC_IDLE)
        return 0;
    ADC_STAT = ADC_BUSY;
    DoOneReading();
    //if (receive == 0)
      //  DoOneReading();
    if ((receive >> 24) == (receive & 0x000000FF))
    {
        ADC_DATA.controlbyte = 1;
        ADC_DATA.channel0 = receive >> 14;
        ADC_STAT = ADC_IDLE;
    }
    ADC_STAT = ADC_IDLE;
    return 1;    
}

/* *****************************************************************************
 End of File
 */
