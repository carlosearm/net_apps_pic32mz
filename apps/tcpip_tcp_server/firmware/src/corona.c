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

#include "math.h"
#include "corona.h"
#include "system/debug/sys_debug.h"
#include "system/command/sys_command.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

CORONA_STATES CORONA_STATUS;
CORONA_MODES CORONA_MODE;

CORONA_STATES corona_status_backup;

double corona_dose;
GPIO_PIN corona_pin_select;

static SYS_CMD_DEVICE_NODE* pCoronaCmdDevice = 0;
static const void*          CmdIoParam = 0;

static const GPIO_PIN corona_io_pins[] = 
{
    COCOS_BAR_COR_ENABLED_PIN,
    MC_BAR_COR_ENABLED_PIN,
    MACRO_LOW_DOSE_COR_ENABLED_PIN,
    MACRO_HIGH_DOSE_COR_ENABLED_PIN,
    MICRO_LOW_DOSE_COR_ENABLED_PIN,
    MICRO_HIGH_DOSE_COR_ENABLED_PIN,
    CORONA_GRN_RESISTOR_BYPASS_PIN,
    BIAS_MICRO_MODE_SELECT_PIN,
    HIGH_VOLTAGE_ENABLED_PIN,
    CORONA_BIAS_POLARITY_NEG_POS_PIN,
    BIAS_VOLTAGE_ENABLED_PIN,
    HV_POLARITY_NEG_POS_PIN,
    HVPS_ON_INTERLOCKED_PIN
};

static void CoronaReset(void);

static int _Command_Reset(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Monitor_Read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_IOSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Init_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Done_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);

#define         LINE_TERM       "\r\n"
#define         RESP_OK         "+OK."  LINE_TERM
#define         RESP_ERROR      "-ERR." LINE_TERM

static const SYS_CMD_DESCRIPTOR coronaCmdTable[] = 
{
    {"cor_reset",   (SYS_CMD_FNC)_Command_Reset,            ": Reset corona control."},
    {"version",     (SYS_CMD_FNC)_Command_Ver,              ": Get corona version information."},
    {"adcscan",     (SYS_CMD_FNC)_Command_Monitor_Read,     ": Read ADC output."},
    {"ioset",       (SYS_CMD_FNC)_Command_IOSet,            ": Sets GPIO functions."},
    {"init_bar",    (SYS_CMD_FNC)_Command_Init_Bar,         ": Initializes bar corona."},
    {"done_bar",    (SYS_CMD_FNC)_Command_Done_Bar,         ": Finalizes bar corona."}
};

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static int _Command_Reset(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    CoronaReset();
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_OK);
    return 1;
}

static int _Command_IOSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_ERROR "Not enough parameters." LINE_TERM);
        return false;
    }
    int line = atoi(argv[1]);
    if ((line>=0) && (line<13))
    {
        GPIO_PIN pin = corona_io_pins[line];
        GPIO_PinWrite(pin, atoi(argv[2]));
        (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_OK);
        return 1;
    }
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_ERROR "Line index out of range." LINE_TERM);
    return 1;
}

static int _Command_Monitor_Read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    corona_status_backup = CORONA_STATUS;
    pCoronaCmdDevice = pCmdIO;
    CmdIoParam = pCmdIO->cmdIoParam;
    if ((CORONA_STATUS != CORONA_IDLE) && (CORONA_STATUS != CORONA_CHARGING))
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Corona box busy." LINE_TERM);
        return 0;
    }
    CORONA_STATUS = CORONA_MONITOR_READ;
    return 1;
}

static int _Command_Init_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS != CORONA_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Corona box busy." LINE_TERM);
        return 0;
    }
    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    pCoronaCmdDevice = pCmdIO;
    corona_pin_select = atoi(argv[1]) == 1 ? COCOS_BAR_COR_ENABLED_PIN : MC_BAR_COR_ENABLED_PIN;
    corona_dose = atof(argv[2]);
    CORONA_MODE = CORONA_MODE_BAR;
    CORONA_STATUS = CORONA_BAR_INIT;
    return 1;
}

static int _Command_Done_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if ((CORONA_STATUS != CORONA_CHARGING) || (CORONA_MODE != CORONA_MODE_BAR))
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Bar corona not charging." LINE_TERM);
        return 0;
    }
    CORONA_STATUS = CORONA_BAR_DONE;
    return 1;
}

static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_OK "Version Alpha 1." LINE_TERM);
    return 1;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void Corona_Initialize(void)
{
    if (!SYS_CMD_ADDGRP(coronaCmdTable, sizeof(coronaCmdTable)/sizeof(*coronaCmdTable), "corona", ": commands"))
    {
        SYS_ERROR(SYS_ERROR_ERROR, RESP_ERROR "Failed to create Corona Commands." LINE_TERM);
        CORONA_STATUS = CORONA_ERROR;
    }
    else
    {
        CORONA_MODE = CORONA_MODE_NONE;
        CORONA_STATUS = CORONA_WAIT_INIT;
    }
}

static void CoronaReset(void)
{
    DAC_Reset();
    GPIO_PinClear(HIGH_VOLTAGE_ENABLED_PIN);
    GPIO_PinClear(BIAS_VOLTAGE_ENABLED_PIN);
    GPIO_PinClear(HV_POLARITY_NEG_POS_PIN);
    GPIO_PinClear(CORONA_BIAS_POLARITY_NEG_POS_PIN);
    GPIO_PinClear(HVPS_ON_INTERLOCKED_PIN);
    GPIO_PinClear(COCOS_BAR_COR_ENABLED_PIN);
    GPIO_PinClear(MC_BAR_COR_ENABLED_PIN);
    GPIO_PinClear(MACRO_LOW_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MACRO_HIGH_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MICRO_LOW_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MICRO_HIGH_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(CORONA_GRN_RESISTOR_BYPASS_PIN);
    GPIO_PinClear(BIAS_MICRO_MODE_SELECT_PIN);
}

void Corona_Tasks(void)
{
    switch(CORONA_STATUS)
    {
        case CORONA_WAIT_INIT:
            CoronaReset();
            CORONA_STATUS = CORONA_IDLE;
            break;
        case CORONA_IDLE:
            break;
        case CORONA_ERROR:
            break;
        case CORONA_MONITOR_READ:
            if (ADC_STAT == ADC_IDLE)
                ADC_Scan();
            if (ADC_STAT == ADC_COMPLETE)
            {
                float v0 = 10.0 * ADC_DATA.channel_0 / 0x03FFFF;
                float v1 = 10.0 * ADC_DATA.channel_1 / 0x03FFFF;
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK "%f, %f" LINE_TERM, v0, v1);
                ADC_STAT = ADC_IDLE;
                CORONA_STATUS = corona_status_backup;
            }
            break;
        case CORONA_BAR_INIT:
            GPIO_PinWrite(HV_POLARITY_NEG_POS_PIN, corona_dose < 0);
            GPIO_PinSet(corona_pin_select);
            HIGH_VOLTAGE_ENABLED_Set();
            DAC_SetCoronaDose(fabs(corona_dose));
            CORONA_STATUS = CORONA_CHARGING;
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            break;
        case CORONA_BAR_DONE:
            DAC_SetCoronaDoseZero();
            HIGH_VOLTAGE_ENABLED_Clear();
            GPIO_PinClear(corona_pin_select);
            GPIO_PinClear(HV_POLARITY_NEG_POS_PIN);
            CORONA_STATUS = CORONA_IDLE;
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            break;
        case CORONA_CHARGING:
            break;
    }
}

/* *****************************************************************************
 End of File
 */
