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

#include "corona.h"
#include "system/debug/sys_debug.h"
#include "system/command/sys_command.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

CORONA_STATES corona_state;
static SYS_CMD_DEVICE_NODE* pCoronaCmdDevice = 0;
static const void*          monitorReadCmdIoParam = 0;

static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);
static int _Command_Monitor_Read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv);


static const SYS_CMD_DESCRIPTOR coronaCmdTable[] = 
{
    {"ver",     (SYS_CMD_FNC)_Command_Ver,              ": Get corona version information"},
    {"read",   (SYS_CMD_FNC)_Command_Monitor_Read,      ": Read ADC output."}
};

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

static int _Command_Monitor_Read(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    if (corona_state != CORONA_IDLE)
        return 0;
    pCoronaCmdDevice = pCmdIO;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "Reading corona monitor.\r\n");
    corona_state = CORONA_MONITOR_READ;
    return 1;
}

static int _Command_Ver(SYS_CMD_DEVICE_NODE* pCmdIO, int argc, char** argv)
{
    pCoronaCmdDevice = pCmdIO;
    monitorReadCmdIoParam = pCmdIO->cmdIoParam;
    const void* cmdIoParam = pCmdIO->cmdIoParam;
    (*pCmdIO->pCmdApi->msg)(cmdIoParam, "version Alpha 1.\r\n");
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
        SYS_ERROR(SYS_ERROR_ERROR, "Failed to create Corona Commands\r\n");
        corona_state = CORONA_ERROR;
    }
    else
        corona_state = CORONA_WAIT_INIT;
}

uint c = -1;

void Corona_Tasks(void)
{
    switch(corona_state)
    {
        case CORONA_WAIT_INIT:
            corona_state = CORONA_IDLE;
            break;
        case CORONA_IDLE:
            if (c == 0)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Corona idle.\r\n");
                c++;
            }
            break;
        case CORONA_ERROR:
            break;
        case CORONA_MONITOR_READ:
            (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Starting ADC reading.\r\n");
            if (ADC_ReadMonitorLines())
            {
                (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Waiting for ADC to complete.\r\n");
                corona_state = CORONA_MONITOR_READING;
                c = 0;
            }
            break;
        case CORONA_MONITOR_READING:
            if (c == 0)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Checking ADC status.\r\n");
                (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Checking ADC value %d.\r\n", ADC_DATA.channel0);
                c++;
            }
            if (ADC_STAT == ADC_IDLE)
            {
                (*pCoronaCmdDevice->pCmdApi->print)(monitorReadCmdIoParam, "Monitor reading completed.\r\n");
                corona_state = CORONA_IDLE;
                c = 0;
            }
            break;
    }
}

/* *****************************************************************************
 End of File
 */
