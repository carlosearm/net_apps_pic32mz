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

/* This section lists the other files that are included in this file.
 */

    #include "DAC.h"

    unsigned int data = 0x00F00000;
    unsigned int write_max_cmd = 0x0000FFFF;
    unsigned int write_min_cmd = 0x00000000;
    unsigned int toggle_b_cmd = 0x00C00001;
    unsigned int global_toggle_cmd = 0x00D00000;
    unsigned int update_cmd = 0x00100000;
    unsigned int write_update_all_cmd = 0x80A0FFFF;
    unsigned int write_update_all_zero_cmd = 0x80A00000;


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
    void DAC_SetMin()
    {
        SPI2_Write(&write_update_all_zero_cmd, 4);
    }
    
    void DAC_SetMax()
    {
        SPI2_Write(&write_update_all_cmd, 4);
    }
    
    void TIMER2_InterruptSvcRoutine(uint32_t status, uintptr_t context)
    {
        SPI2_Write(&write_update_all_zero_cmd, 4);
        TMR2_Stop();
    }
    
    void DAC_Toggle(short time)
    {
        TMR2_PeriodSet(time * 99998U);
        TMR2_CallbackRegister(TIMER2_InterruptSvcRoutine,NULL);
        TMR2_Start();
        SPI2_Write(&write_update_all_cmd, 4);
    }
    
    void Ramp()
    {
        uint16_t output = 0;
        uint16_t increment = 1;
        do
        {
            unsigned int current_value = write_update_all_zero_cmd | output;
            SPI2_Write(&current_value, 4);
            output += increment;
        }
        while (output < 0xFFFF);
    }
    
    void Cycles(short count)
    {
        short i = 0;
        while(i < count)
        {
            Ramp();
            i++;
        }
    }