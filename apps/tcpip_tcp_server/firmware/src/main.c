/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


unsigned int data = 0x00F00000;
unsigned int write_max_cmd = 0x0000FFFF;
unsigned int write_min_cmd = 0x00000000;
unsigned int toggle_b_cmd = 0x00C00001;
unsigned int global_toggle_cmd = 0x00D00000;
unsigned int update_cmd = 0x00100000;
unsigned int write_update_all_cmd = 0x80A0FFFF;
unsigned int write_update_all_zero_cmd = 0x80A00000;

unsigned int read_control = 0x84000000;
unsigned int read_conv_result = 0x00000000;

//unsigned int buffer;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    DAC1_TGP_Clear();
    DAC1_CLR_Set();
    DAC1_LDAC_Set();


    //ADC_CNV_Clear();


    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        /*
        SPI3_Write(&read_control, 4);
        short triggered = 0;
        while (true)
        {
            //ADC_CNV_Clear();
            if (SWITCH3_Get() != 0)
            {
                ADC_CNV_Clear();
                triggered = 0;
            }
            else
            {
                if (triggered == 0)
                {
                    triggered = 1;
                    ADC_CNV_Set();
                    ADC_CNV_Clear();
                    while (ADC_BUSY_Get() != 0)
                    {
                    }
                    if (SPI3_WriteRead(&read_control, 4, &read_conv_result, 4))
                    {
                        read_conv_result = read_conv_result >> 8;
                        if ((read_conv_result & 0x00003F) == 0x4)
                        {
                            COCOS_BAR_COR_ENABLED_Set();
                            read_conv_result = read_conv_result >> 8;
                            read_conv_result &= 0x0000FFFF;
                            unsigned output1 = write_update_all_zero_cmd | read_conv_result;
                            SPI1_Write(&output1, 4);
                        }
                        else
                        {
                            COCOS_BAR_COR_ENABLED_Clear();
                        }
                    }
                    else
                    {
                        unsigned output2 = write_update_all_zero_cmd | 0x000080FF;
                        SPI1_Write(&output2, 4);  
                    }
                }
            }
        }*/
        
        /*
        uint16_t output = 0;
        uint16_t increment = 1;
        while (true)
        {
            output += increment;
            if (output > 0xFFFF)
                output = 0;
            unsigned int current_value = write_update_all_zero_cmd | output;
            SPI1_Write(&current_value, 4);
        }*/
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

