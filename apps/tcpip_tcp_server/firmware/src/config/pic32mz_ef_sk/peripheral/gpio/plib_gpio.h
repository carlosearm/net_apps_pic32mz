/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for HV_POLARITY_NEG_POS pin ***/
#define HV_POLARITY_NEG_POS_Set()               (LATGSET = (1<<15))
#define HV_POLARITY_NEG_POS_Clear()             (LATGCLR = (1<<15))
#define HV_POLARITY_NEG_POS_Toggle()            (LATGINV= (1<<15))
#define HV_POLARITY_NEG_POS_OutputEnable()      (TRISGCLR = (1<<15))
#define HV_POLARITY_NEG_POS_InputEnable()       (TRISGSET = (1<<15))
#define HV_POLARITY_NEG_POS_Get()               ((PORTG >> 15) & 0x1)
#define HV_POLARITY_NEG_POS_PIN                  GPIO_PIN_RG15

/*** Macros for HVPS_ON_INTERLOCKED pin ***/
#define HVPS_ON_INTERLOCKED_Set()               (LATASET = (1<<5))
#define HVPS_ON_INTERLOCKED_Clear()             (LATACLR = (1<<5))
#define HVPS_ON_INTERLOCKED_Toggle()            (LATAINV= (1<<5))
#define HVPS_ON_INTERLOCKED_OutputEnable()      (TRISACLR = (1<<5))
#define HVPS_ON_INTERLOCKED_InputEnable()       (TRISASET = (1<<5))
#define HVPS_ON_INTERLOCKED_Get()               ((PORTA >> 5) & 0x1)
#define HVPS_ON_INTERLOCKED_PIN                  GPIO_PIN_RA5

/*** Macros for DAC2_LDAC pin ***/
#define DAC2_LDAC_Set()               (LATJSET = (1<<13))
#define DAC2_LDAC_Clear()             (LATJCLR = (1<<13))
#define DAC2_LDAC_Toggle()            (LATJINV= (1<<13))
#define DAC2_LDAC_OutputEnable()      (TRISJCLR = (1<<13))
#define DAC2_LDAC_InputEnable()       (TRISJSET = (1<<13))
#define DAC2_LDAC_Get()               ((PORTJ >> 13) & 0x1)
#define DAC2_LDAC_PIN                  GPIO_PIN_RJ13

/*** Macros for DAC2_CLR pin ***/
#define DAC2_CLR_Set()               (LATJSET = (1<<14))
#define DAC2_CLR_Clear()             (LATJCLR = (1<<14))
#define DAC2_CLR_Toggle()            (LATJINV= (1<<14))
#define DAC2_CLR_OutputEnable()      (TRISJCLR = (1<<14))
#define DAC2_CLR_InputEnable()       (TRISJSET = (1<<14))
#define DAC2_CLR_Get()               ((PORTJ >> 14) & 0x1)
#define DAC2_CLR_PIN                  GPIO_PIN_RJ14

/*** Macros for DAC2_TGP pin ***/
#define DAC2_TGP_Set()               (LATJSET = (1<<15))
#define DAC2_TGP_Clear()             (LATJCLR = (1<<15))
#define DAC2_TGP_Toggle()            (LATJINV= (1<<15))
#define DAC2_TGP_OutputEnable()      (TRISJCLR = (1<<15))
#define DAC2_TGP_InputEnable()       (TRISJSET = (1<<15))
#define DAC2_TGP_Get()               ((PORTJ >> 15) & 0x1)
#define DAC2_TGP_PIN                  GPIO_PIN_RJ15

/*** Macros for ADC_BUSY pin ***/
#define ADC_BUSY_Set()               (LATBSET = (1<<11))
#define ADC_BUSY_Clear()             (LATBCLR = (1<<11))
#define ADC_BUSY_Toggle()            (LATBINV= (1<<11))
#define ADC_BUSY_OutputEnable()      (TRISBCLR = (1<<11))
#define ADC_BUSY_InputEnable()       (TRISBSET = (1<<11))
#define ADC_BUSY_Get()               ((PORTB >> 11) & 0x1)
#define ADC_BUSY_PIN                  GPIO_PIN_RB11

/*** Macros for ADC_CNV pin ***/
#define ADC_CNV_Set()               (LATKSET = (1<<1))
#define ADC_CNV_Clear()             (LATKCLR = (1<<1))
#define ADC_CNV_Toggle()            (LATKINV= (1<<1))
#define ADC_CNV_OutputEnable()      (TRISKCLR = (1<<1))
#define ADC_CNV_InputEnable()       (TRISKSET = (1<<1))
#define ADC_CNV_Get()               ((PORTK >> 1) & 0x1)
#define ADC_CNV_PIN                  GPIO_PIN_RK1

/*** Macros for SWITCH3 pin ***/
#define SWITCH3_Set()               (LATDSET = (1<<0))
#define SWITCH3_Clear()             (LATDCLR = (1<<0))
#define SWITCH3_Toggle()            (LATDINV= (1<<0))
#define SWITCH3_OutputEnable()      (TRISDCLR = (1<<0))
#define SWITCH3_InputEnable()       (TRISDSET = (1<<0))
#define SWITCH3_Get()               ((PORTD >> 0) & 0x1)
#define SWITCH3_PIN                  GPIO_PIN_RD0

/*** Macros for DAC1_CLR pin ***/
#define DAC1_CLR_Set()               (LATDSET = (1<<12))
#define DAC1_CLR_Clear()             (LATDCLR = (1<<12))
#define DAC1_CLR_Toggle()            (LATDINV= (1<<12))
#define DAC1_CLR_OutputEnable()      (TRISDCLR = (1<<12))
#define DAC1_CLR_InputEnable()       (TRISDSET = (1<<12))
#define DAC1_CLR_Get()               ((PORTD >> 12) & 0x1)
#define DAC1_CLR_PIN                  GPIO_PIN_RD12

/*** Macros for DAC1_TGP pin ***/
#define DAC1_TGP_Set()               (LATDSET = (1<<13))
#define DAC1_TGP_Clear()             (LATDCLR = (1<<13))
#define DAC1_TGP_Toggle()            (LATDINV= (1<<13))
#define DAC1_TGP_OutputEnable()      (TRISDCLR = (1<<13))
#define DAC1_TGP_InputEnable()       (TRISDSET = (1<<13))
#define DAC1_TGP_Get()               ((PORTD >> 13) & 0x1)
#define DAC1_TGP_PIN                  GPIO_PIN_RD13

/*** Macros for DAC1_LDAC pin ***/
#define DAC1_LDAC_Set()               (LATJSET = (1<<0))
#define DAC1_LDAC_Clear()             (LATJCLR = (1<<0))
#define DAC1_LDAC_Toggle()            (LATJINV= (1<<0))
#define DAC1_LDAC_OutputEnable()      (TRISJCLR = (1<<0))
#define DAC1_LDAC_InputEnable()       (TRISJSET = (1<<0))
#define DAC1_LDAC_Get()               ((PORTJ >> 0) & 0x1)
#define DAC1_LDAC_PIN                  GPIO_PIN_RJ0

/*** Macros for COCOS_BAR_COR_ENABLED pin ***/
#define COCOS_BAR_COR_ENABLED_Set()               (LATFSET = (1<<1))
#define COCOS_BAR_COR_ENABLED_Clear()             (LATFCLR = (1<<1))
#define COCOS_BAR_COR_ENABLED_Toggle()            (LATFINV= (1<<1))
#define COCOS_BAR_COR_ENABLED_OutputEnable()      (TRISFCLR = (1<<1))
#define COCOS_BAR_COR_ENABLED_InputEnable()       (TRISFSET = (1<<1))
#define COCOS_BAR_COR_ENABLED_Get()               ((PORTF >> 1) & 0x1)
#define COCOS_BAR_COR_ENABLED_PIN                  GPIO_PIN_RF1

/*** Macros for MC_BAR_COR_ENABLED pin ***/
#define MC_BAR_COR_ENABLED_Set()               (LATKSET = (1<<7))
#define MC_BAR_COR_ENABLED_Clear()             (LATKCLR = (1<<7))
#define MC_BAR_COR_ENABLED_Toggle()            (LATKINV= (1<<7))
#define MC_BAR_COR_ENABLED_OutputEnable()      (TRISKCLR = (1<<7))
#define MC_BAR_COR_ENABLED_InputEnable()       (TRISKSET = (1<<7))
#define MC_BAR_COR_ENABLED_Get()               ((PORTK >> 7) & 0x1)
#define MC_BAR_COR_ENABLED_PIN                  GPIO_PIN_RK7

/*** Macros for MACRO_LOW_DOSE_COR_ENABLED pin ***/
#define MACRO_LOW_DOSE_COR_ENABLED_Set()               (LATGSET = (1<<1))
#define MACRO_LOW_DOSE_COR_ENABLED_Clear()             (LATGCLR = (1<<1))
#define MACRO_LOW_DOSE_COR_ENABLED_Toggle()            (LATGINV= (1<<1))
#define MACRO_LOW_DOSE_COR_ENABLED_OutputEnable()      (TRISGCLR = (1<<1))
#define MACRO_LOW_DOSE_COR_ENABLED_InputEnable()       (TRISGSET = (1<<1))
#define MACRO_LOW_DOSE_COR_ENABLED_Get()               ((PORTG >> 1) & 0x1)
#define MACRO_LOW_DOSE_COR_ENABLED_PIN                  GPIO_PIN_RG1

/*** Macros for MACRO_HIGH_DOSE_COR_ENABLED pin ***/
#define MACRO_HIGH_DOSE_COR_ENABLED_Set()               (LATGSET = (1<<0))
#define MACRO_HIGH_DOSE_COR_ENABLED_Clear()             (LATGCLR = (1<<0))
#define MACRO_HIGH_DOSE_COR_ENABLED_Toggle()            (LATGINV= (1<<0))
#define MACRO_HIGH_DOSE_COR_ENABLED_OutputEnable()      (TRISGCLR = (1<<0))
#define MACRO_HIGH_DOSE_COR_ENABLED_InputEnable()       (TRISGSET = (1<<0))
#define MACRO_HIGH_DOSE_COR_ENABLED_Get()               ((PORTG >> 0) & 0x1)
#define MACRO_HIGH_DOSE_COR_ENABLED_PIN                  GPIO_PIN_RG0

/*** Macros for MICRO_LOW_DOSE_COR_ENABLED pin ***/
#define MICRO_LOW_DOSE_COR_ENABLED_Set()               (LATASET = (1<<6))
#define MICRO_LOW_DOSE_COR_ENABLED_Clear()             (LATACLR = (1<<6))
#define MICRO_LOW_DOSE_COR_ENABLED_Toggle()            (LATAINV= (1<<6))
#define MICRO_LOW_DOSE_COR_ENABLED_OutputEnable()      (TRISACLR = (1<<6))
#define MICRO_LOW_DOSE_COR_ENABLED_InputEnable()       (TRISASET = (1<<6))
#define MICRO_LOW_DOSE_COR_ENABLED_Get()               ((PORTA >> 6) & 0x1)
#define MICRO_LOW_DOSE_COR_ENABLED_PIN                  GPIO_PIN_RA6

/*** Macros for MICRO_HIGH_DOSE_COR_ENABLED pin ***/
#define MICRO_HIGH_DOSE_COR_ENABLED_Set()               (LATASET = (1<<7))
#define MICRO_HIGH_DOSE_COR_ENABLED_Clear()             (LATACLR = (1<<7))
#define MICRO_HIGH_DOSE_COR_ENABLED_Toggle()            (LATAINV= (1<<7))
#define MICRO_HIGH_DOSE_COR_ENABLED_OutputEnable()      (TRISACLR = (1<<7))
#define MICRO_HIGH_DOSE_COR_ENABLED_InputEnable()       (TRISASET = (1<<7))
#define MICRO_HIGH_DOSE_COR_ENABLED_Get()               ((PORTA >> 7) & 0x1)
#define MICRO_HIGH_DOSE_COR_ENABLED_PIN                  GPIO_PIN_RA7

/*** Macros for CORONA_GRN_RESISTOR_BYPASS pin ***/
#define CORONA_GRN_RESISTOR_BYPASS_Set()               (LATESET = (1<<1))
#define CORONA_GRN_RESISTOR_BYPASS_Clear()             (LATECLR = (1<<1))
#define CORONA_GRN_RESISTOR_BYPASS_Toggle()            (LATEINV= (1<<1))
#define CORONA_GRN_RESISTOR_BYPASS_OutputEnable()      (TRISECLR = (1<<1))
#define CORONA_GRN_RESISTOR_BYPASS_InputEnable()       (TRISESET = (1<<1))
#define CORONA_GRN_RESISTOR_BYPASS_Get()               ((PORTE >> 1) & 0x1)
#define CORONA_GRN_RESISTOR_BYPASS_PIN                  GPIO_PIN_RE1

/*** Macros for BIAS_MICRO_MODE_SELECT pin ***/
#define BIAS_MICRO_MODE_SELECT_Set()               (LATGSET = (1<<14))
#define BIAS_MICRO_MODE_SELECT_Clear()             (LATGCLR = (1<<14))
#define BIAS_MICRO_MODE_SELECT_Toggle()            (LATGINV= (1<<14))
#define BIAS_MICRO_MODE_SELECT_OutputEnable()      (TRISGCLR = (1<<14))
#define BIAS_MICRO_MODE_SELECT_InputEnable()       (TRISGSET = (1<<14))
#define BIAS_MICRO_MODE_SELECT_Get()               ((PORTG >> 14) & 0x1)
#define BIAS_MICRO_MODE_SELECT_PIN                  GPIO_PIN_RG14

/*** Macros for HIGH_VOLTAGE_ENABLED pin ***/
#define HIGH_VOLTAGE_ENABLED_Set()               (LATGSET = (1<<12))
#define HIGH_VOLTAGE_ENABLED_Clear()             (LATGCLR = (1<<12))
#define HIGH_VOLTAGE_ENABLED_Toggle()            (LATGINV= (1<<12))
#define HIGH_VOLTAGE_ENABLED_OutputEnable()      (TRISGCLR = (1<<12))
#define HIGH_VOLTAGE_ENABLED_InputEnable()       (TRISGSET = (1<<12))
#define HIGH_VOLTAGE_ENABLED_Get()               ((PORTG >> 12) & 0x1)
#define HIGH_VOLTAGE_ENABLED_PIN                  GPIO_PIN_RG12

/*** Macros for CORONA_BIAS_POLARITY_NEG_POS pin ***/
#define CORONA_BIAS_POLARITY_NEG_POS_Set()               (LATGSET = (1<<13))
#define CORONA_BIAS_POLARITY_NEG_POS_Clear()             (LATGCLR = (1<<13))
#define CORONA_BIAS_POLARITY_NEG_POS_Toggle()            (LATGINV= (1<<13))
#define CORONA_BIAS_POLARITY_NEG_POS_OutputEnable()      (TRISGCLR = (1<<13))
#define CORONA_BIAS_POLARITY_NEG_POS_InputEnable()       (TRISGSET = (1<<13))
#define CORONA_BIAS_POLARITY_NEG_POS_Get()               ((PORTG >> 13) & 0x1)
#define CORONA_BIAS_POLARITY_NEG_POS_PIN                  GPIO_PIN_RG13

/*** Macros for BIAS_VOLTAGE_ENABLED pin ***/
#define BIAS_VOLTAGE_ENABLED_Set()               (LATESET = (1<<2))
#define BIAS_VOLTAGE_ENABLED_Clear()             (LATECLR = (1<<2))
#define BIAS_VOLTAGE_ENABLED_Toggle()            (LATEINV= (1<<2))
#define BIAS_VOLTAGE_ENABLED_OutputEnable()      (TRISECLR = (1<<2))
#define BIAS_VOLTAGE_ENABLED_InputEnable()       (TRISESET = (1<<2))
#define BIAS_VOLTAGE_ENABLED_Get()               ((PORTE >> 2) & 0x1)
#define BIAS_VOLTAGE_ENABLED_PIN                  GPIO_PIN_RE2


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_D = 3,
    GPIO_PORT_E = 4,
    GPIO_PORT_F = 5,
    GPIO_PORT_G = 6,
    GPIO_PORT_H = 7,
    GPIO_PORT_J = 8,
    GPIO_PORT_K = 9,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RA5 = 5,
    GPIO_PIN_RA6 = 6,
    GPIO_PIN_RA7 = 7,
    GPIO_PIN_RA9 = 9,
    GPIO_PIN_RA10 = 10,
    GPIO_PIN_RA14 = 14,
    GPIO_PIN_RA15 = 15,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB6 = 22,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB12 = 28,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RB15 = 31,
    GPIO_PIN_RC1 = 33,
    GPIO_PIN_RC2 = 34,
    GPIO_PIN_RC3 = 35,
    GPIO_PIN_RC4 = 36,
    GPIO_PIN_RC12 = 44,
    GPIO_PIN_RC13 = 45,
    GPIO_PIN_RC14 = 46,
    GPIO_PIN_RC15 = 47,
    GPIO_PIN_RD0 = 48,
    GPIO_PIN_RD1 = 49,
    GPIO_PIN_RD2 = 50,
    GPIO_PIN_RD3 = 51,
    GPIO_PIN_RD4 = 52,
    GPIO_PIN_RD5 = 53,
    GPIO_PIN_RD6 = 54,
    GPIO_PIN_RD7 = 55,
    GPIO_PIN_RD9 = 57,
    GPIO_PIN_RD10 = 58,
    GPIO_PIN_RD11 = 59,
    GPIO_PIN_RD12 = 60,
    GPIO_PIN_RD13 = 61,
    GPIO_PIN_RD14 = 62,
    GPIO_PIN_RD15 = 63,
    GPIO_PIN_RE0 = 64,
    GPIO_PIN_RE1 = 65,
    GPIO_PIN_RE2 = 66,
    GPIO_PIN_RE3 = 67,
    GPIO_PIN_RE4 = 68,
    GPIO_PIN_RE5 = 69,
    GPIO_PIN_RE6 = 70,
    GPIO_PIN_RE7 = 71,
    GPIO_PIN_RE8 = 72,
    GPIO_PIN_RE9 = 73,
    GPIO_PIN_RF0 = 80,
    GPIO_PIN_RF1 = 81,
    GPIO_PIN_RF2 = 82,
    GPIO_PIN_RF3 = 83,
    GPIO_PIN_RF4 = 84,
    GPIO_PIN_RF5 = 85,
    GPIO_PIN_RF8 = 88,
    GPIO_PIN_RF12 = 92,
    GPIO_PIN_RF13 = 93,
    GPIO_PIN_RG0 = 96,
    GPIO_PIN_RG1 = 97,
    GPIO_PIN_RG6 = 102,
    GPIO_PIN_RG7 = 103,
    GPIO_PIN_RG8 = 104,
    GPIO_PIN_RG9 = 105,
    GPIO_PIN_RG12 = 108,
    GPIO_PIN_RG13 = 109,
    GPIO_PIN_RG14 = 110,
    GPIO_PIN_RG15 = 111,
    GPIO_PIN_RH0 = 112,
    GPIO_PIN_RH1 = 113,
    GPIO_PIN_RH2 = 114,
    GPIO_PIN_RH3 = 115,
    GPIO_PIN_RH4 = 116,
    GPIO_PIN_RH5 = 117,
    GPIO_PIN_RH6 = 118,
    GPIO_PIN_RH7 = 119,
    GPIO_PIN_RH8 = 120,
    GPIO_PIN_RH9 = 121,
    GPIO_PIN_RH10 = 122,
    GPIO_PIN_RH11 = 123,
    GPIO_PIN_RH12 = 124,
    GPIO_PIN_RH13 = 125,
    GPIO_PIN_RH14 = 126,
    GPIO_PIN_RH15 = 127,
    GPIO_PIN_RJ0 = 128,
    GPIO_PIN_RJ1 = 129,
    GPIO_PIN_RJ2 = 130,
    GPIO_PIN_RJ3 = 131,
    GPIO_PIN_RJ4 = 132,
    GPIO_PIN_RJ5 = 133,
    GPIO_PIN_RJ6 = 134,
    GPIO_PIN_RJ7 = 135,
    GPIO_PIN_RJ8 = 136,
    GPIO_PIN_RJ9 = 137,
    GPIO_PIN_RJ10 = 138,
    GPIO_PIN_RJ11 = 139,
    GPIO_PIN_RJ12 = 140,
    GPIO_PIN_RJ13 = 141,
    GPIO_PIN_RJ14 = 142,
    GPIO_PIN_RJ15 = 143,
    GPIO_PIN_RK0 = 144,
    GPIO_PIN_RK1 = 145,
    GPIO_PIN_RK2 = 146,
    GPIO_PIN_RK3 = 147,
    GPIO_PIN_RK4 = 148,
    GPIO_PIN_RK5 = 149,
    GPIO_PIN_RK6 = 150,
    GPIO_PIN_RK7 = 151,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
