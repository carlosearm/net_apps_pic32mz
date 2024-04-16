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

DWORD read_none = 0;
DWORD read_cmd = 0x84000000;
DWORD receive = 0;

ADC_DATA_TYPE ADC_DATA;
ADC_STATES ADC_STATUS;

bool ADC_IsIdle()
{
    return ADC_STATUS == ADC_IDLE;
}

bool ADC_IsComplete()
{
    return ADC_STATUS == ADC_COMPLETE;
}

void ADC_SetIdle()
{
    ADC_STATUS = ADC_IDLE;
}

BYTE ADC_ControlByte()
{
    return ADC_DATA.controlbyte;
}

uint ADC_Channel(int i)
{
    return ADC_DATA.channels[i];
}

void ADC_Initialize(void)
{
    //ADC_CNV_Clear();
    ADC_STATUS = ADC_WAIT_INIT;
    ADC_DATA.controlbyte = 3;
}

void ADC_Reset(void)
{
    ADC_Initialize();
}

void ADC_Scan(void)
{
    if (ADC_STATUS == ADC_IDLE)
    {
        ADC_DATA.index = 0;
        ADC_DATA.channel_0 = 0;
        ADC_DATA.channel_1 = 0;
        ADC_DATA.channel_2 = 0;
        ADC_DATA.channel_3 = 0;
        ADC_DATA.channel_4 = 0;
        ADC_DATA.channel_5 = 0;
        ADC_DATA.channel_6 = 0;
        ADC_DATA.channel_7 = 0;
        ADC_STATUS = ADC_SCANNING;
    }
}

static BYTE GetNextIndex();

void ADC_Tasks()
{
   switch(ADC_STATUS)
    {
        case ADC_WAIT_INIT:
            //Do something to configure ADC at initialization.
            SPI3_WriteRead(&read_cmd, 4, &receive, 4);
            ADC_STATUS = ADC_IDLE;
            break;
        case ADC_IDLE:
        case ADC_COMPLETE:
            break;
        case ADC_SCANNING:
            if (ADC_DATA.controlbyte & (1 << ADC_DATA.index))
            {
                ADC_CNV_Set();
                ADC_CNV_Clear();
                ADC_STATUS = ADC_CONVERTING;
            }
            else
                ADC_DATA.index++;
            break;
       case ADC_CONVERTING:
           if (ADC_BUSY_Get() == 0)
           {
               BYTE next_index = GetNextIndex();
               DWORD cmd = (read_cmd)|(next_index << 27);
               SPI3_WriteRead(&cmd, 4, &receive, 4);
               if (receive == 0) 
               {
                   ADC_STATUS = ADC_SCANNING;
                   break;
               }
               if ((receive >> 24) == (receive & 0x000000FF))
               {
                   int i = (receive >> 11) & 7;
                   ADC_DATA.channels[i] = receive >> 14;
                   //ADC_DATA.channels[ADC_DATA.index] = receive >> 14;
               }
               ADC_STATUS = next_index > ADC_DATA.index ? ADC_SCANNING : ADC_COMPLETE;
               ADC_DATA.index = next_index;
           }
           break;
    }
}

static BYTE GetNextIndex()
{
    if (ADC_DATA.controlbyte == 0) return 0;
    BYTE index = ADC_DATA.index + 1;
    while (((1 << index) & ADC_DATA.controlbyte) == 0)
    {
        index++;
        index %= 8;
    }
    return index;
}

/* *****************************************************************************
 End of File
 */
