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

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define SWITCH_PRESSED_STATE        0   // Active LOW switch
#define LED_On()                        LED_Set()
#define LED_Off()                       LED_Clear()
#define SW_Get()                        SWITCH1_Get()

uint8_t txData[]  = "SELF LOOPBACK DEMO FOR SPI!";
uint8_t rxData[sizeof(txData)];

/*unsigned int data = 0x00F00000;
unsigned int write_max_cmd = 0x0000FFFF;
unsigned int write_min_cmd = 0x00000000;
unsigned int toggle_b_cmd = 0x00C00001;
unsigned int global_toggle_cmd = 0x00D00000;
unsigned int update_cmd = 0x00100000;
unsigned int write_update_all_cmd = 0x80A0FFFF;
unsigned int write_update_all_zero_cmd = 0x80A00000;

unsigned int buffer;
*/
int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    //CLR_Clear();
    TGP_Clear();
    CLR_Set();
    LDAC_Set();
    
    
    /*SPI2_Write(&write_update_all_zero_cmd, 4);
    SPI2_Write(&write_update_all_zero_cmd, 4);
    SPI2_Write(&data, 4);
    SPI2_Write(&write_update_all_cmd, 4);
    SPI2_Write(&write_update_all_cmd, 4);
    SPI2_Write(&data, 4);*/

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        /*
         * uint16_t output = 0;
        uint16_t increment = 1;
        while (SW_Get() == SWITCH_PRESSED_STATE)
        {
            output += increment;
            if (output > 0xFFFF)
                output = 0;
            unsigned int current_value = write_update_all_zero_cmd | output;
            SPI2_Write(&current_value, 4);
        }*/
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

