/******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.10
*
* Description: This example demonstrates the basic UART transmit and receive 
*              operation using user ISR.
*
* Related Document: CE219656.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer kit
*
******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation.
******************************************************************************
* This software, including source code, documentation and related materials
* ("Software") is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and 
* foreign), United States copyright laws and international treaty provisions. 
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the 
* Cypress source code and derivative works for the sole purpose of creating 
* custom software in support of licensee product, such licensee product to be
* used only in conjunction with Cypress's integrated circuit as specified in the
* applicable agreement. Any reproduction, modification, translation, compilation,
* or representation of this Software except as specified above is prohibited 
* without the express written permission of Cypress.
* 
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes to the Software without notice. 
* Cypress does not assume any liability arising out of the application or use
* of Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use as critical components in any products 
* where a malfunction or failure may reasonably be expected to result in 
* significant injury or death ("ACTIVE Risk Product"). By including Cypress's 
* product in a ACTIVE Risk Product, the manufacturer of such system or application
* assumes all risk of such use and in doing so indemnifies Cypress against all
* liability. Use of this Software may be limited by and subject to the applicable
* Cypress software license agreement.
*****************************************************************************/


//#include <stdio.h>
#include "vein.h"

/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
void handle_error(void);
void ISR_UART(void);


/***************************************************************************//**
* Function Name: ISR_UART
********************************************************************************
*
* Summary:
*  This function is registered to be called when UART interrupt occurs.
*  (Note that only RX fifo not empty interrupt is unmasked)
*  Whenever there is a data in UART RX fifo, Get that data and Put it into 
*  UART TX fifo which will be transmitted to terminal
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None
*
*******************************************************************************/
int num = 0;

void ISR_UART(void)
{
    /* Check for "RX fifo not empty interrupt" */
    if((UART_HW->INTR_RX_MASKED & SCB_INTR_RX_MASKED_NOT_EMPTY_Msk ) != 0)
	{
        /* Clear UART "RX fifo not empty interrupt" */
		UART_HW->INTR_RX = UART_HW->INTR_RX & SCB_INTR_RX_NOT_EMPTY_Msk;        
         
        /* Get the character from terminal */
		read_data = Cy_SCB_UART_Get(UART_HW);
        
        data_received = 1; 
        /* Update data_received flag */
             
	}   
    else
    {
        /* Error if any other interrupt occurs */
        uart_error = 1;
    }
}




void uart_init(void){
    
    flag_vein = 0;
    flag_uart = 0;
    fid = 0;    
    cy_en_scb_uart_status_t init_status;
    
        
    /* Start UART operation. */
    init_status = Cy_SCB_UART_Init(UART_HW, &UART_config, &UART_context);
    if(init_status!=CY_SCB_UART_SUCCESS)
    {
        handle_error();
    }
    Cy_SCB_UART_Enable(UART_HW); 
    
    /* Unmasking only the RX fifo not empty interrupt bit */
    UART_HW->INTR_RX_MASK = SCB_INTR_RX_MASK_NOT_EMPTY_Msk;
    
    /* Interrupt Settings for UART */    
    Cy_SysInt_Init(&UART_SCB_IRQ_cfg, ISR_UART);
    
    /* Enable the interrupt */
    NVIC_EnableIRQ(UART_SCB_IRQ_cfg.intrSrc);
    data_received = 0;
    uart_error = 0;  
    
     

}


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main function performs the following actions:
*   1. Sets up UART component.
*   2. If initialization of UART component fails then switch on RED_LED_ERROR.
*   3. UART sends text header into the serial terminal.
*   4. Unmask UART RX fifo not empty interrupt
*   5. Enable UART interrupts
*   6. Wait for UART RX fifo not empty interrupt
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int main(void)
{
    
    uart_init();
    uart_vein_init();
    
    /* Enable global interrupts. */
    __enable_irq();
    
    
    Cy_SCB_UART_Put(UART_HW,vein_cnt());
    
    for(;;)
    {
        
        /* Handle received data from terminal */
        if(data_received == 1)
        {
            data_received = 0;
            
            /* Echo the character to terminal */
            while (0UL == Cy_SCB_UART_Put(UART_vein_HW,read_data))
            {
            }
        }
        
        /* Handle UART error */
        if(uart_error == 1)
        {
            handle_error();
        }
        
        
        if(data_received_vein == 1)
        {
            data_received_vein = 0;
            //flag_vein = 0;
            
            /* Echo the character to terminal */
            //while (0UL == Cy_SCB_UART_Put(UART_HW,read_buf))          
            {
            }
           
            
        }
        
        
        /*Handle UART error */
        if(uart_error_vein == 1)
        {
            handle_error();
        }
    }
}


/* [] END OF FILE */

