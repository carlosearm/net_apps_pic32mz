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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
    /*#include <stddef.h>                     // Defines NULL
    #include <stdbool.h>                    // Defines true
    #include <stdlib.h>                     // Defines EXIT_FAILURE*/
    #include "definitions.h"                // SYS function prototypes

    /* TODO:  Include other files here if needed. */



    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    void DAC_SetMin();
    void DAC_SetMax();
    void DAC_Toggle(short time);
    void Cycles(short count);
    