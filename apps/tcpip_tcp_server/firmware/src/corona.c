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
#include "peripheral/tmr/plib_tmr2.h"
#include "system/debug/sys_debug.h"
#include "system/command/sys_command.h"
#include "corona.h"
#include "dac.h"
#include "adc.h"



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

CORONA_STATES CORONA_STATUS;
CORONA_MODES CORONA_MODE;
MICRO_DONE_STATES MICRO_DONE_STATUS;
TEST_STATUS TEST_STATE;

CORONA_STATES corona_status_backup;

double last_corona_dose;
double target_corona_dose;
double corona_ramp_increment;
double macro_dose;
double micro_dose;
double corona_bias;
double chuck_bias;
bool   positive_macro_dose;
double micro_bias_delay;

uint32_t micro_done_delay_start;
uint32_t micro_done_delay_end;

uint32_t corona_delay;
uint shutter_time;
GPIO_PIN corona_pin_select;

static SYS_CMD_DEVICE_NODE* pCoronaCmdDevice = 0;
static const void*          CmdIoParam = 0;

static const GPIO_PIN corona_io_pins[] = 
{
    COCOS_BAR_COR_ENABLED_PIN,//0
    MC_BAR_COR_ENABLED_PIN,//1
    MACRO_LOW_DOSE_COR_ENABLED_PIN,//2
    MACRO_HIGH_DOSE_COR_ENABLED_PIN,//3
    MICRO_LOW_DOSE_COR_ENABLED_PIN,//4
    MICRO_HIGH_DOSE_COR_ENABLED_PIN,//5
    CORONA_GRN_RESISTOR_BYPASS_PIN,//6
    BIAS_MICRO_MODE_SELECT_PIN,//7
    HIGH_VOLTAGE_ENABLED_PIN,//8
    CORONA_BIAS_POLARITY_NEG_POS_PIN,//9
    BIAS_VOLTAGE_ENABLED_PIN,//10
    HV_POLARITY_NEG_POS_PIN,//11
    HVPS_ON_INTERLOCKED_PIN //12
};

static void CoronaReset(void);
static void MicroDoneTask(void);
static void CoronaTest_Task(void);

static int _Command_Reset(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_PowerDown(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Discharge(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Monitor_Read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_IOSet(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Init_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Done_Bar(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_SetDose(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_SetBias(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_SetChuckBias(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Init_Macro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Done_Macro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Init_Micro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Done_Micro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Shutter(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Shutter_MS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Shutter_S(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Shutter_Open(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Shutter_Close(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);

#define         LINE_TERM       "\r\n"
#define         RESP_OK         "+OK."  LINE_TERM
#define         RESP_ERROR      "-ERR." 
#define         COMMENT         "CMT."
#define         VERSION_NUMBER  "1.0.5.0"

static const SYS_CMD_DESCRIPTOR coronaCmdTable[] = 
{
    {"cor_reset",       (SYS_CMD_FNC)_Command_Reset,            ": Reset corona control."},
    {"power_down",      (SYS_CMD_FNC)_Command_PowerDown,        ": Power down components."}, 
    {"discharge",       (SYS_CMD_FNC)_Command_Discharge,        ": Discharge relays."}, 
    {"version",         (SYS_CMD_FNC)_Command_Ver,              ": Get corona version information."},
    {"adcscan",         (SYS_CMD_FNC)_Command_Monitor_Read,     ": Read ADC output."},
    {"ioset",           (SYS_CMD_FNC)_Command_IOSet,            ": Sets GPIO functions."},
    {"setdose",         (SYS_CMD_FNC)_Command_SetDose,          ": Sets Corona Dose."},
    {"setbias",         (SYS_CMD_FNC)_Command_SetBias,          ": Sets Corona Bias."},
    {"setchuckbias",    (SYS_CMD_FNC)_Command_SetChuckBias,     ": Sets Chuck Bias."},
    {"init_bar",        (SYS_CMD_FNC)_Command_Init_Bar,         ": Initializes Bar Corona."},
    {"done_bar",        (SYS_CMD_FNC)_Command_Done_Bar,         ": Finalizes Bar corona."},
    {"init_macro",      (SYS_CMD_FNC)_Command_Init_Macro,       ": Initializes Macro Point Source Corona."},
    {"done_macro",      (SYS_CMD_FNC)_Command_Done_Macro,       ": Finalizes Macro Point Source Corona."},
    {"init_micro",      (SYS_CMD_FNC)_Command_Init_Micro,       ": Initializes Micro Point Source Corona."},
    {"done_micro",      (SYS_CMD_FNC)_Command_Done_Micro,       ": Finalizes Micro Point Source Corona."},
    {"shutter",         (SYS_CMD_FNC)_Command_Shutter,          ": Open Close Shutter microseconds."},
    {"shutterms",       (SYS_CMD_FNC)_Command_Shutter_MS,       ": Open Close Shutter milliseconds."},
    {"shutters",        (SYS_CMD_FNC)_Command_Shutter_S,        ": Open Close Shutter seconds."},
    {"openshutter",     (SYS_CMD_FNC)_Command_Shutter_Open,     ": Open shutter for micro."},
    {"closeshutter",    (SYS_CMD_FNC)_Command_Shutter_Close,    ": Close shutter for micro."},
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
    CORONA_STATUS = CORONA_IDLE;
    MICRO_DONE_STATUS = MICRO_DONE_IDLE;
    return 1;
}

static int _Command_PowerDown(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    DAC_PowerDown();
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_OK);
    return 1;
}

static int _Command_Discharge(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    uint32_t delay = 1;
    if (argc > 1)
        delay = atoi(argv[1]);

    MC_BAR_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
    COCOS_BAR_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
    MACRO_LOW_DOSE_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
    MACRO_HIGH_DOSE_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
    MICRO_LOW_DOSE_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
    MICRO_HIGH_DOSE_COR_ENABLED_Set();
    CORETIMER_DelayMs(delay);
            
    COCOS_BAR_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
    MACRO_LOW_DOSE_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
    MACRO_HIGH_DOSE_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
    MICRO_LOW_DOSE_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
    MICRO_HIGH_DOSE_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
    MC_BAR_COR_ENABLED_Clear();
    CORETIMER_DelayMs(delay);
            
    
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
    /*if (argc > 1)
    {
        BYTE control = atoi(argv[1]);
        if (control != 0)
            ADC_DATA.controlbyte = control;
        else
            ADC_Reset();
    }*/
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
    corona_pin_select = atoi(argv[1]) == 1 ? COCOS_BAR_COR_ENABLED_PIN : MC_BAR_COR_ENABLED_PIN;
    target_corona_dose = atof(argv[2]);
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

static int _Command_SetDose(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    if (CORONA_MODE == CORONA_MODE_BAR)
    {
        if (CORONA_STATUS == CORONA_CHARGING)
        {
            target_corona_dose = atof(argv[1]);
            corona_ramp_increment = target_corona_dose > last_corona_dose ? 
                fabs(corona_ramp_increment) : -fabs(corona_ramp_increment);
            CORONA_STATUS = CORONA_BAR_RAMP;
            return 1;
        }
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to change dose." LINE_TERM);
        return 0;
    }
    macro_dose = atof(argv[1]);
    micro_dose = macro_dose;
    //if (CORONA_STATUS == CORONA_MACRO_READY)
    if (macro_dose != 0)
        DAC_SetCoronaDose(fabs(macro_dose));
    else
        DAC_SetCoronaDoseZero();
    (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
    return 1;
}

static int _Command_SetBias(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    corona_bias = atof(argv[1]);
    DAC_SetBiasValue(fabs(corona_bias));
    (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
    return 1;
}

static int _Command_SetChuckBias(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    chuck_bias = atof(argv[1]);
    DAC_SetChuckBiasValue(chuck_bias);
    (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
    return 1;
}

static int _Command_Init_Macro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS != CORONA_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to initialize macro." LINE_TERM);
        return 0;
    }
    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    //corona_pin_select = atoi(argv[1]) == 0 ? MACRO_LOW_DOSE_COR_ENABLED_PIN : MACRO_HIGH_DOSE_COR_ENABLED_PIN;
    corona_pin_select = atoi(argv[1]) == 0 ? MACRO_LOW_DOSE_COR_ENABLED_PIN : MACRO_HIGH_DOSE_COR_ENABLED_PIN;
    macro_dose = atof(argv[2]);
    corona_bias = atof(argv[3]);
    CORONA_STATUS = CORONA_MACRO_INIT;
    return 1;
}

static int _Command_Done_Macro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS != CORONA_MACRO_READY)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to finalize macro." LINE_TERM);
        return 0;
    }
    CORONA_STATUS = CORONA_MACRO_DONE;
    return 1;
}

void OpenMacroShutter(void)
{
    //DAC_SetCoronaDose(fabs(macro_dose)); 
    if (positive_macro_dose)
    {
        //DAC_SetCoronaDose(fabs(macro_dose)); 
        BIAS_VOLTAGE_ENABLED_Set();
        HIGH_VOLTAGE_ENABLED_Set();
    }
    else
    {
        BIAS_VOLTAGE_ENABLED_Set();
        CORETIMER_DelayMs(20);
        DAC_SetBiasValue(fabs(corona_bias));
        CORETIMER_DelayMs(20);
        HIGH_VOLTAGE_ENABLED_Set();
    }
}

void CloseMacroShutter(void)
{
    HIGH_VOLTAGE_ENABLED_Clear();
    if (!positive_macro_dose)
    {
        DAC_SetBiasValue(0);
        //CORETIMER_DelayMs(10);
    }
    BIAS_VOLTAGE_ENABLED_Clear();
    //DAC_SetCoronaDoseZero();
}

void TIMER2_MacroRoutine(uint32_t status, uintptr_t context)
{
    TMR2_Stop();
    CloseMacroShutter();
    CORONA_STATUS = CORONA_MACRO_DONE_CHARGING;
}

void DoMacroShutter(uint32_t conset, uint32_t period)
{
    TMR2_CallbackRegister(TIMER2_MacroRoutine, NULL);
    T2CONCLR = _T2CON_ON_MASK;
    T2CONCLR = 0x70;
    T2CONSET = conset;
    PR2 = (period);
    OpenMacroShutter();
    T2CONSET = _T2CON_ON_MASK;
    CORONA_STATUS = CORONA_MACRO_START_CHARGING;
}

static int _Command_Init_Micro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS != CORONA_IDLE)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to initialize mIcro." LINE_TERM);
        return 0;
    }
    if (argc < 3)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    
    corona_pin_select = atoi(argv[1]) == 0 ? MICRO_LOW_DOSE_COR_ENABLED_PIN : MICRO_HIGH_DOSE_COR_ENABLED_PIN;
    micro_dose = atof(argv[2]);
    corona_bias = atof(argv[3]);
    corona_delay = 1000;
    if (argc > 4)
        corona_delay = atoi(argv[4]);
    CORONA_STATUS = CORONA_MICRO_INIT;
    return 1;
}

static int _Command_Done_Micro(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS != CORONA_MICRO_READY)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to finalize micro." LINE_TERM);
        return 0;
    }
    micro_bias_delay = 0;
    if (argc > 1)
        micro_bias_delay = atoi(argv[1]);
    CORONA_STATUS = CORONA_MICRO_DONE;
    return 1;
}

void OpenMicroShutter(void)
{
    CORONA_BIAS_POLARITY_NEG_POS_Toggle();
    /*if (micro_dose > 0)
        CORONA_BIAS_POLARITY_NEG_POS_Clear();
    else
        CORONA_BIAS_POLARITY_NEG_POS_Set();*/
}

void CloseMicroShutter(void)
{
    CORONA_BIAS_POLARITY_NEG_POS_Toggle();
    /*if (micro_dose > 0)
        CORONA_BIAS_POLARITY_NEG_POS_Set();
    else
        CORONA_BIAS_POLARITY_NEG_POS_Clear();*/
}

void TIMER2_MicroRoutine(uint32_t status, uintptr_t context)
{
    TMR2_Stop();
    CloseMicroShutter();
    CORONA_STATUS = CORONA_MICRO_DONE_CHARGING;
}

void DoMicroShutter(uint32_t conset, uint32_t period)
{
    TMR2_CallbackRegister(TIMER2_MicroRoutine, NULL);
    T2CONCLR = _T2CON_ON_MASK;
    T2CONCLR = 0x70;
    T2CONSET = conset;
    PR2 = (period);
    OpenMicroShutter();
    T2CONSET = _T2CON_ON_MASK;
    CORONA_STATUS = CORONA_MICRO_START_CHARGING;
}

static int _Command_Shutter(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    uint t = atoi(argv[1]);
    if (CORONA_STATUS == CORONA_MACRO_READY)
    {
        DoMacroShutter(0x0, t * 99U);
        return 1;
    }
    else if (CORONA_STATUS == CORONA_MICRO_READY)
    {
        DoMicroShutter(0x0, t * 99U);
        return 1;
    }

    (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to open/close shutter." LINE_TERM);
    return 0;
}

static int _Command_Shutter_MS(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    uint t = atoi(argv[1]);
    if (CORONA_STATUS == CORONA_MACRO_READY)
    {
        DoMacroShutter(0x30, t * 12498U);
        return 1;
    }
    else if (CORONA_STATUS == CORONA_MICRO_READY)
    {
        DoMicroShutter(0x30, t * 12498U);
        return 1;
    }
    return 0;
}

static int _Command_Shutter_S(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (argc < 2)
    {
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not enough arguments." LINE_TERM);
        return 0;
    }
    uint t = atof(argv[1]) * 1000;
    if (CORONA_STATUS == CORONA_MACRO_READY)
    {
        DoMacroShutter(0x50, t * 3123U);
        return 1;
    }
    else if (CORONA_STATUS == CORONA_MICRO_READY)
    {
        DoMicroShutter(0x50, t * 3123U);
        return 1;
    }
    return 0;
}

static int _Command_Shutter_Open(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if (CORONA_STATUS == CORONA_MICRO_READY)
    {
        OpenMicroShutter();
        CORONA_STATUS = CORONA_MICRO_START_CHARGING;
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_OK);
        return 1;
    }
    (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to open shutter." LINE_TERM);
    return 0;
}

static int _Command_Shutter_Close(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    CmdIoParam = pCmdIO->cmdIoParam;
    pCoronaCmdDevice = pCmdIO;
    if ((CORONA_MODE == CORONA_MODE_MICRO) && (CORONA_STATUS == CORONA_CHARGING))
    {
        CloseMicroShutter();
        CORONA_STATUS = CORONA_MICRO_READY;
        (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_OK);
        return 1;
    }
    (*pCmdIO->pCmdApi->msg)(CmdIoParam, RESP_ERROR "Not allowed to close shutter." LINE_TERM);
    return 0;
}


static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, RESP_OK "Version " VERSION_NUMBER  LINE_TERM);
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
    HVPS_ON_INTERLOCKED_Set();
    T2CONCLR = _T2CON_ON_MASK;
    T2CONCLR = 0x70;
    DAC_Reset();
    GPIO_PinClear(HIGH_VOLTAGE_ENABLED_PIN);
    GPIO_PinClear(BIAS_VOLTAGE_ENABLED_PIN);
    GPIO_PinClear(HV_POLARITY_NEG_POS_PIN);
    GPIO_PinClear(CORONA_BIAS_POLARITY_NEG_POS_PIN);
    GPIO_PinClear(COCOS_BAR_COR_ENABLED_PIN);
    GPIO_PinClear(MC_BAR_COR_ENABLED_PIN);
    GPIO_PinClear(MACRO_LOW_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MACRO_HIGH_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MICRO_LOW_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(MICRO_HIGH_DOSE_COR_ENABLED_PIN);
    GPIO_PinClear(CORONA_GRN_RESISTOR_BYPASS_PIN);
    GPIO_PinClear(BIAS_MICRO_MODE_SELECT_PIN);
    CORONA_MODE = CORONA_MODE_NONE;
    TEST_STATE = TEST_STATUS_IDLE;
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
        case CORONA_SELF_TEST:
            CoronaTest_Task();
            break;
        case CORONA_MONITOR_READ:
            if (ADC_IsIdle())
                ADC_Scan();
            if (ADC_IsComplete())
            {
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
                int i;
                for(i=0; i<8; i++)
                {
                    if ((ADC_ControlByte() & (1 << i)) > 0)
                        (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, "%f, ", 10.0 * ADC_Channel(i) / 0x03FFFF);
                }
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, LINE_TERM);
                ADC_SetIdle();
                CORONA_STATUS = corona_status_backup;
            }
            break;
        case CORONA_BAR_INIT:
            DAC_SetCoronaDose(0);
            last_corona_dose = 0;
            corona_ramp_increment = target_corona_dose > last_corona_dose ? 0.01 : -0.01;
            GPIO_PinWrite(HV_POLARITY_NEG_POS_PIN, target_corona_dose < 0);
            GPIO_PinSet(corona_pin_select);
            HIGH_VOLTAGE_ENABLED_Set();
            CORONA_MODE = CORONA_MODE_BAR;
            CORONA_STATUS = CORONA_BAR_RAMP;
            break;
        case CORONA_BAR_RAMP:
            if ((last_corona_dose >= 0) &&
                (2.0*fabs(last_corona_dose - target_corona_dose) > fabs(corona_ramp_increment)))
            {
                last_corona_dose += corona_ramp_increment;
                DAC_SetCoronaDose(fabs(last_corona_dose));
            }
            else
            {
                last_corona_dose = target_corona_dose;
                DAC_SetCoronaDose(fabs(last_corona_dose));
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
                CORONA_STATUS = CORONA_CHARGING;
            }
            break;
        case CORONA_BAR_DONE:
            DAC_SetCoronaDoseZero();
            HIGH_VOLTAGE_ENABLED_Clear();
            GPIO_PinClear(corona_pin_select);
            GPIO_PinClear(HV_POLARITY_NEG_POS_PIN);
            CORONA_MODE = CORONA_MODE_NONE; 
            CORONA_STATUS = CORONA_IDLE;
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            break;
        case CORONA_MACRO_INIT:
            HIGH_VOLTAGE_ENABLED_Clear();
            BIAS_MICRO_MODE_SELECT_Clear();
            GPIO_PinSet(corona_pin_select);
            GPIO_PinWrite(HV_POLARITY_NEG_POS_PIN, macro_dose < 0);
            GPIO_PinWrite(CORONA_BIAS_POLARITY_NEG_POS_PIN, macro_dose < 0);
            positive_macro_dose = macro_dose > 0;
            if (positive_macro_dose)
                DAC_SetBiasValue(fabs(corona_bias));
            else
                DAC_SetBiasValue(0);
            DAC_SetCoronaDose(fabs(macro_dose));
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            CORONA_MODE = CORONA_MODE_MACRO;
            CORONA_STATUS = CORONA_MACRO_READY;
            break;
        case CORONA_MACRO_DONE:
            HIGH_VOLTAGE_ENABLED_Clear();
            //DAC_SetCoronaDoseZero();
            DAC_SetCoronaDose(0);
            DAC_SetBiasValue(0);
            GPIO_PinClear(corona_pin_select);
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            CORONA_MODE = CORONA_MODE_NONE;
            CORONA_STATUS = CORONA_IDLE;
            break;
        case CORONA_MACRO_START_CHARGING:
            CORONA_STATUS = CORONA_CHARGING;
            break;
        case CORONA_MACRO_DONE_CHARGING:
            //CloseMacroShutter();
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            CORONA_STATUS = CORONA_MACRO_READY;
            break;
        case CORONA_MICRO_INIT:
            HIGH_VOLTAGE_ENABLED_Clear();
            BIAS_VOLTAGE_ENABLED_Clear();
          
            CORETIMER_DelayUs(corona_delay);
            BIAS_MICRO_MODE_SELECT_Set();
            CORETIMER_DelayUs(corona_delay);
            GPIO_PinSet(corona_pin_select);
            CORETIMER_DelayUs(corona_delay);
            GPIO_PinWrite(HV_POLARITY_NEG_POS_PIN, micro_dose < 0);
            CORETIMER_DelayUs(corona_delay);
            GPIO_PinWrite(CORONA_BIAS_POLARITY_NEG_POS_PIN, micro_dose > 0);
            
            CORETIMER_DelayUs(corona_delay);
            DAC_SetBiasValue(fabs(corona_bias));
            DAC_SetCoronaDose(fabs(micro_dose));
            
            CORETIMER_DelayUs(corona_delay);
            BIAS_VOLTAGE_ENABLED_Set();
            HIGH_VOLTAGE_ENABLED_Set();
            
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            CORONA_MODE = CORONA_MODE_MICRO;
            CORONA_STATUS = CORONA_MICRO_READY;
            break;
        case CORONA_MICRO_DONE:
            if (MICRO_DONE_STATUS == MICRO_DONE_IDLE)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " micro done started." LINE_TERM);
                MICRO_DONE_STATUS = MICRO_DONE_START;
            }
            MicroDoneTask();
            if (MICRO_DONE_STATUS == MICRO_DONE_COMPLETE)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
                CORONA_STATUS = CORONA_IDLE;
                CORONA_MODE = CORONA_MODE_NONE;
                MICRO_DONE_STATUS = MICRO_DONE_IDLE;
            }
            break;
        case CORONA_MICRO_START_CHARGING:
            CORONA_STATUS = CORONA_CHARGING;
            break;
        case CORONA_MICRO_DONE_CHARGING:
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_OK);
            CORONA_STATUS = CORONA_MICRO_READY;
            break;
        case CORONA_MICRO_READY:
        case CORONA_MACRO_READY:
        case CORONA_CHARGING:
            break;
    }
}

static void MicroDoneTask(void)
{
    switch (MICRO_DONE_STATUS)
    {
        case MICRO_DONE_START:
        case MICRO_DONE_HV_OFF:
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " Turning HV off." LINE_TERM);
            DAC_SetCoronaDoseZero();
            CORETIMER_DelayMs(100);
            HIGH_VOLTAGE_ENABLED_Clear();

            micro_done_delay_start = _CP0_GET_COUNT();
            micro_done_delay_end = (CORE_TIMER_FREQUENCY/1000)*micro_bias_delay;
            MICRO_DONE_STATUS = MICRO_DONE_HV_WAIT;
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " start_delay = %x." LINE_TERM, micro_done_delay_start);
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " end_delay = %x." LINE_TERM, micro_done_delay_end);
            break;
        case MICRO_DONE_HV_WAIT:
            if (_CP0_GET_COUNT() - micro_done_delay_start >= micro_done_delay_end)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " start_delay = %x." LINE_TERM, _CP0_GET_COUNT() - micro_done_delay_start);
                MICRO_DONE_STATUS = MICRO_DONE_BIAS_OFF;
            }
            break;
        case MICRO_DONE_BIAS_OFF:
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " Turning Bias off." LINE_TERM);
            BIAS_VOLTAGE_ENABLED_Clear();
            CORETIMER_DelayMs(100);
            DAC_SetBiasValue(0);
            MICRO_DONE_STATUS = MICRO_DONE_BIAS_WAIT;
            break;
        case MICRO_DONE_BIAS_WAIT:
            MICRO_DONE_STATUS = MICRO_DONE_IO_CLEAR;
            break;
        case MICRO_DONE_IO_CLEAR:
            (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, COMMENT " Clearing IO." LINE_TERM);
            GPIO_PinClear(corona_pin_select);
            CORETIMER_DelayMs(100);
            BIAS_MICRO_MODE_SELECT_Clear();
            MICRO_DONE_STATUS = MICRO_DONE_COMPLETE;
            break;
        case MICRO_DONE_IDLE:
        case MICRO_DONE_COMPLETE:
            break;
    }
}

void CoronaTest_Task(void)
{
    switch (TEST_STATE)
    {
        case TEST_STATUS_START:
            if (CORONA_STATUS != CORONA_IDLE)
            {
                TEST_STATE = TEST_STATUS_DONE;
                (*pCoronaCmdDevice->pCmdApi->print)(CmdIoParam, RESP_ERROR "Not allowed to start self-test." LINE_TERM);
                //ADC_DATA.controlbyte = 1<<7;
            }
            break;
        case TEST_STATUS_ACTIVE:
            break;
        case TEST_STATUS_DONE:
            //ADC_DATA.controlbyte = 3;
            break;
        case TEST_STATUS_IDLE:
            break;
    }
}

/* *****************************************************************************
 End of File
 */
