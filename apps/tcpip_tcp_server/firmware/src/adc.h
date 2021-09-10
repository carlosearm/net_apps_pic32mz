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
    BYTE index;
    union
    {
        uint channels[8];
        struct
        {
            uint channel_0;
            uint channel_1;
            uint channel_2;
            uint channel_3;
            uint channel_4;
            uint channel_5;
            uint channel_6;
            uint channel_7;
        };
    };
}ADC_DATA_TYPE;

ADC_DATA_TYPE ADC_DATA;

typedef enum 
{
    ADC_WAIT_INIT,
    ADC_IDLE,
    ADC_SCANNING,
    ADC_CONVERTING,
    ADC_COMPLETE
} ADC_STATES;

ADC_STATES ADC_STAT;


void ADC_Initialize(void);

void ADC_Reset(void);

void ADC_Tasks(void);

void ADC_Scan(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
