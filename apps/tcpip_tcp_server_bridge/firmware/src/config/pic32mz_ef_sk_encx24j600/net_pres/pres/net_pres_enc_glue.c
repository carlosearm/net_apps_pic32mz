 /*******************************************************************************
 Source file for the Net Pres Encryption glue functions to work with Harmony


  Summary:


  Description:

*******************************************************************************/

/*****************************************************************************
 Copyright (C) 2013-2018 Microchip Technology Inc. and its subsidiaries.

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


#include "net_pres_enc_glue.h"
#include "net_pres/pres/net_pres_transportapi.h"
#include "net_pres/pres/net_pres_certstore.h"

#include "config.h"
#include "wolfssl/ssl.h"
#include "wolfssl/wolfcrypt/logging.h"
#include "wolfssl/wolfcrypt/random.h"



NET_PRES_EncProviderObject net_pres_EncProviderStreamServer0 =
{
    .fpInit =    0,
    .fpDeinit =  0,
    .fpOpen =    0,
    .fpConnect = 0,
    .fpClose =   0,
    .fpWrite =   0,
    .fpWriteReady =   0,
    .fpRead =    0,
    .fpReadReady = 0,
    .fpPeek =    0,
    .fpIsInited = 0,
    .fpOutputSize = 0,
    .fpMaxOutputSize = 0,

};
NET_PRES_EncProviderObject net_pres_EncProviderStreamClient0 = 
{
    .fpInit =    0,
    .fpDeinit =  0,
    .fpOpen =    0,
    .fpConnect = 0,
    .fpClose =   0,
    .fpWrite =   0,
    .fpWriteReady =   0,
    .fpRead =    0,
    .fpReadReady = 0,
    .fpPeek =    0,
    .fpIsInited = 0,
    .fpOutputSize = 0,
    .fpMaxOutputSize = 0,
};
	