/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CORONA_H    /* Guard against multiple inclusion */
#define _CORONA_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "adc.h"
#include "dac.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    CORONA_WAIT_INIT,
    CORONA_IDLE,
    CORONA_ERROR,
    CORONA_MONITOR_READ,
    CORONA_BAR_INIT,
    CORONA_BAR_RAMP,
    CORONA_BAR_DONE,
    CORONA_CHARGING,
    CORONA_MACRO_INIT,
    CORONA_MACRO_READY,
    CORONA_MACRO_START_CHARGING,
    CORONA_MACRO_DONE_CHARGING,
    CORONA_MACRO_DONE,
    CORONA_MICRO_INIT,
    CORONA_MICRO_READY,
    CORONA_MICRO_START_CHARGING,
    CORONA_MICRO_DONE_CHARGING,
    CORONA_MICRO_DONE,
    CORONA_SELF_TEST
}CORONA_STATES;

typedef enum
{
    CORONA_MODE_NONE,
    CORONA_MODE_BAR,
    CORONA_MODE_MACRO,
    CORONA_MODE_MICRO
}CORONA_MODES;

typedef enum 
{
    TEST_STATUS_IDLE,
    TEST_STATUS_START,
    TEST_STATUS_ACTIVE,
    TEST_STATUS_DONE
}TEST_STATUS;

void Corona_Initialize(void);

void Corona_Tasks(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CORONA_H */

/* *****************************************************************************
 End of File
 */
