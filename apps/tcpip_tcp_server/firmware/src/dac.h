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

#ifndef _DAC_H    /* Guard against multiple inclusion */
#define _DAC_H


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
    
    void DAC_Reset(void);
    
    void DAC_SetCoronaDose(double dose);
    
    void DAC_SetCoronaDoseZero(void);
    
    void DAC_SetBiasValue(double dose);
    
    void DAC_SetChuckBiasValue(double dose);
    
    //void DAC_DisableMux(void);
    
    //void DAC_SetMuxOut(uint vout);
    
    void DAC_PowerDown();

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
