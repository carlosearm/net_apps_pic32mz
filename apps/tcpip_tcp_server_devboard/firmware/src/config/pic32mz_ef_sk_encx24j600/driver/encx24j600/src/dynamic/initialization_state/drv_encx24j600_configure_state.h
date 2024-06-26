/*******************************************************************************
  ENCx24J600 Configure State Machine
  Company:
    Microchip Technology Inc.

  File Name:
    drv_encx24j600_configure_state.h
  Summary:

  Description:
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*****************************************************************************
 Copyright (C) 2014-2018 Microchip Technology Inc. and its subsidiaries.

Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software 
and any derivatives exclusively with Microchip products. It is your 
responsibility to comply with third party license terms applicable to your 
use of third party software (including open source software) that may 
accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE 
FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN 
ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, 
THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************/

// DOM-IGNORE-END
#ifndef _DRV_ENCX24J600_CONFIGURE_STATE_H_
#define _DRV_ENCX24J600_CONFIGURE_STATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "system_config.h"


struct _DRV_ENCX24J600_DriverInfo;

typedef enum
{
    DRV_ENCX24J600_CS_SET_EGPRDPT,
    DRV_ENCX24J600_CS_SET_ERXRDPT,
    DRV_ENCX24J600_CS_SET_EUDARDPT,
    DRV_ENCX24J600_CS_SET_EGPWRPT,
    DRV_ENCX24J600_CS_SET_ERXWRPT,
    DRV_ENCX24J600_CS_SET_EUDAWRPT,
    DRV_ENCX24J600_CS_SET_ERXST,
    DRV_ENCX24J600_CS_SET_ERXTAIL,
    DRV_ENCX24J600_CS_SET_PHCON1,
    DRV_ENCX24J600_CS_SET_EUDAST,
    DRV_ENCX24J600_CS_SET_EUDAND,
    DRV_ENCX24J600_CS_SET_ERXFCON,
    DRV_ENCX24J600_CS_READ_MACON2,
    DRV_ENCX24J600_CS_WRITE_MAADR1,
    DRV_ENCX24J600_CS_WRITE_MAADR2,
    DRV_ENCX24J600_CS_WRITE_MAADR3,
    DRV_ENCX24J600_CS_READ_MAADR1,
    DRV_ENCX24J600_CS_READ_MAADR2,
    DRV_ENCX24J600_CS_READ_MAADR3,
    DRV_ENCX24J600_CS_WAIT_FOR_MACON2,
    DRV_ENCX24J600_CS_SET_MACON2,
    DRV_ENCX24J600_CS_SET_PHANA,
    DRV_ENCX24J600_CS_SET_EIE,
    DRV_ENCX24J600_CS_WAIT_FOR_MAADR3,
}DRV_ENCX24J600_CONFIGURE_STATES;

typedef enum
{
    DRV_ENCX24J600_CS_OP_SET_EGPRDPT,
    DRV_ENCX24J600_CS_OP_SET_MACON2 = DRV_ENCX24J600_CS_OP_SET_EGPRDPT,
    DRV_ENCX24J600_CS_OP_SET_ERXRDPT,
    DRV_ENCX24J600_CS_OP_SET_PHANA_1 = DRV_ENCX24J600_CS_OP_SET_ERXRDPT,
    DRV_ENCX24J600_CS_OP_SET_EUDARDPT,
    DRV_ENCX24J600_CS_OP_SET_PHANA_2 = DRV_ENCX24J600_CS_OP_SET_EUDARDPT,
    DRV_ENCX24J600_CS_OP_SET_EGPWRPT,
    DRV_ENCX24J600_CS_OP_SET_EIE = DRV_ENCX24J600_CS_OP_SET_EGPWRPT,
    DRV_ENCX24J600_CS_OP_SET_ERXWRPT,
    DRV_ENCX24J600_CS_OP_SET_EUDAWRPT,
    DRV_ENCX24J600_CS_OP_SET_ERXST,
    DRV_ENCX24J600_CS_OP_SET_ERXTAIL,
    DRV_ENCX24J600_CS_OP_SET_PHCON1,
    DRV_ENCX24J600_CS_OP_SET_EUDAST,
    DRV_ENCX24J600_CS_OP_SET_EUDAND,
    DRV_ENCX24J600_CS_OP_SET_ERXFCON,
    DRV_ENCX24J600_CS_OP_READ_MACON2,
    DRV_ENCX24J600_CS_OP_WRITE_MAADR1,
    DRV_ENCX24J600_CS_OP_WRITE_MAADR2,
    DRV_ENCX24J600_CS_OP_WRITE_MAADR3,
    DRV_ENCX24J600_CS_OP_READ_MAADR1,
    DRV_ENCX24J600_CS_OP_READ_MAADR2,
    DRV_ENCX24J600_CS_OP_READ_MAADR3,

}DRV_ENCX24J600_CONFIG_STATE_OPS;

typedef struct _DRV_ENCX24J600_CONFIGURE_STATE_INFO
{
    DRV_ENCX24J600_CONFIGURE_STATES state;
    uintptr_t waitForMacon2Op;
    uintptr_t waitForMaddr1Op;
    uintptr_t waitForMaddr2Op;
    uintptr_t waitForMaddr3Op;
}DRV_ENCX24J600_CONFIGURE_STATE_INFO;

int32_t DRV_ENCX24J600_ConfigStateTask(struct _DRV_ENCX24J600_DriverInfo * pDrvInst);
int32_t DRV_ENCX24J600_ConfigStateEnter(struct _DRV_ENCX24J600_DriverInfo * pDrvInst);
int32_t DRV_ENCX24J600_ConfigStateExit(struct _DRV_ENCX24J600_DriverInfo * pDrvInst);



#ifdef __cplusplus
}
#endif


#endif
