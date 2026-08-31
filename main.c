/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC™ Control C1 MCU: CCU4 Capture Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*****************************************************************************/

#include <stdio.h>
#include "cybsp.h"
#include "cy_utils.h"
#include "cy_retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Define macro to enable/disable printing of debug messages */
#define ENABLE_DEBUG_PRINT              (0)

/* Event mapping for CAPTURE_0 */
#define CAPTURE_0_EVENT_RISING_EDGE  CY_CCU4_SLICE_IRQ_ID_EVENT0
#define CAPTURE_0_EVENT_FALLING_EDGE CY_CCU4_SLICE_IRQ_ID_EVENT1

/* Every time that a capture trigger 0 occurs, CCcapt0, the actual value of the 
 * timer is captured into the capture register 1 and the previous value stored in 
 * this register is transferred into capture register 0 */
#define CAPTURE_0_VALUE_RISING_EDGE  1

/* Every time that a capture trigger 1 occurs, CCcapt1, the actual value of the 
 * timer is captured into the capture register 3 and the previous value stored in 
 * this register is transferred into capture register 2 */
#define CAPTURE_0_VALUE_FALLING_EDGE 3

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It performs initial setup of the device, 
* and starts the CCU4 slices. The main while loop checks whether the CAPTURE_0
* has detected a rising or falling edge at its input and prints the corresponding 
* captured timer value.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_HW);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    #if ENABLE_DEBUG_PRINT
    printf("Initialization done\r\n");
    #endif

    /* Start CCU4 slices */
    Cy_CCU4_SLICE_StartTimer(CAPTURE_0_HW);
    Cy_CCU4_SLICE_StartTimer(PWM_0_HW);
  
    /* Print starting sequence on screen */ 
    printf("PSOC™ Control C1 CCU4 Capture example\r\n");
    printf("========================\r\n");
    printf("Starting counters...\r\n");
    
    /* Variables define to get captured timer value on input signal rising and falling edge */
    uint16_t capture_rising;
    uint16_t capture_falling;

    while (1)
    {
        /* Check for a rising edge event in the input signal */
        if (Cy_CCU4_SLICE_GetEvent(CAPTURE_0_HW, CAPTURE_0_EVENT_RISING_EDGE))
        {
            /* Clear event*/
            Cy_CCU4_SLICE_ClearEvent(CAPTURE_0_HW, CAPTURE_0_EVENT_RISING_EDGE);

            /* Get captured timer value on input signal rising edge */
            capture_rising = Cy_CCU4_SLICE_GetCaptureRegisterValue(CAPTURE_0_HW, CAPTURE_0_VALUE_RISING_EDGE);
           
            /* Print captured timer value of input signal rising edge */
            printf("Rising edge: %d\r\n", capture_rising);
        }

        /* Check for a falling edge event in the input signal */
        if (Cy_CCU4_SLICE_GetEvent(CAPTURE_0_HW, CAPTURE_0_EVENT_FALLING_EDGE))
        {
            /* Clear event*/
            Cy_CCU4_SLICE_ClearEvent(CAPTURE_0_HW, CAPTURE_0_EVENT_FALLING_EDGE);
            
            /* Get captured timer value on input signal falling edge */
            capture_falling = Cy_CCU4_SLICE_GetCaptureRegisterValue(CAPTURE_0_HW, CAPTURE_0_VALUE_FALLING_EDGE);
            
            /* Print captured timer value of input signal falling edge */
            printf("Falling edge: %d\r\n", capture_falling);
        }
    }
}

/* [] END OF FILE */
