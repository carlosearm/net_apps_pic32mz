/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Semilab SDI

  @File Name
    ADC.h

  @Summary
    Handles communication with ADC LTC2333-18 using SPI3
 */
/* ************************************************************************** */

#ifndef _ADC_H    /* Guard against multiple inclusion */
#define _ADC_H


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
#include "definitions.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char BYTE;
typedef unsigned int DWORD;

typedef struct
{
    BYTE controlbyte;
    union
    {
        uint channels[8];
        struct
        {
            uint channel0;
            uint channel1;
            uint channel2;
            uint channel3;
            uint channel4;
            uint channel5;
            uint channel6;
            uint channel7;
        };
    };
}ADC_DATA_TYPE;

typedef enum 
{
    ADC_WAIT_INIT,
    ADC_IDLE,
    ADC_BUSY
} ADC_STATES;

ADC_STATES ADC_STAT;

ADC_DATA_TYPE ADC_DATA;

void ADC_Initialize(void);

void ADC_Tasks(void);

int ADC_ReadMonitorLines(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
