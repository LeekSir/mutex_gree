/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/*************************************************************************
    > File Name: vein.c
    > Author: leeksir
    > Mail: leek9527@163.com 
    > Created Time: Wed May  9 11:00:17 2018
 ************************************************************************/
/* ========================================
                               _(\_/) 
                             ,((((^`\
                            ((((  (6 \ 
                          ,((((( ,    \
      ,,,_              ,(((((  /"._  ,`,
     ((((\\ ,...       ,((((   /    `-.-'
     )))  ;'    `"'"'""((((   (      
    (((  /            (((      \
     )) |                      |
    ((  |        .       '     |
    ))  \     _ '      `t   ,.')
    (   |   y;- -,-""'"-.\   \/  
    )   / ./  ) /         `\  \
       |./   ( (           / /'
       ||     \\          //'|
       ||      \\       _//'||
       ||       ))     |_/  ||
       \_\     |_/          ||
       `'"                  \_\
                            `'"
 * ========================================*/

#include"vein.h"

uint32_t cmd_buf[CMD_BUF_SIZE] = {0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x0D}; 
uint32_t recv_buf[RECV_BUF_SIZE];
/*******************************************************************************
* Function Name: vein_check
********************************************************************************
*
* Summary:
* 
*  1.check vein status
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int vein_check()
{
	//检测手指状态
    send_to_uart_vein(CMD_CHK_FINGER, DEV_ID, 0);
    if(ERR_SUCCESS == recv_data[RESULT])
    {
        return ERR_SUCCESS;
    }
    return ERR_OPERATION;
}


/*******************************************************************************
* Function Name: vein_recognition
********************************************************************************
*
* Summary:
* 1.1:N vein recognition
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/


int vein_recognition()
{
    send_to_uart_vein(CMD_ONE_VS_N, DEV_ID, 0);    
    if(ERR_SUCCESS == recv_data[RESULT])
    {
        uint32_t fid_tmp;
        fid_tmp = recv_data[FINGER_ID_PLACE];
        fid_tmp <<= 8;
        fid_tmp |= recv_data[FINGER_ID_PLACE-1];
        return fid_tmp;
    }
    return ERR_OPERATION; 
}

/*******************************************************************************
* Function Name: vein_type
********************************************************************************
*
* Summary:
* 1.vein add 
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int vein_add()
{
	static uint32_t add_cnt = FINGER_ADD_CNT;
    //发送注册命令
    send_to_uart_vein(CMD_REGISTER, DEV_ID, fid);
    if(ERR_SUCCESS == recv_data[RESULT])
    {
        add_cnt--;
        if(0 == add_cnt)
        {
            add_cnt = FINGER_ADD_CNT;
            write_vein_to_database(CMD_REG_END, DEV_ID, 1);
            if(ERR_SUCCESS == recv_data[RESULT])
            {
                return fid;
            }
        }
        return ERR_SUCCESS;
    }    
	return ERR_OPERATION;
}

/*******************************************************************************
* Function Name: vein_type
********************************************************************************
*
* Summary:
* 1.vein register stop 取消本轮注册 
*
* Parameters:
*  None
*
* Return:
*  int
*
*******************************************************************************/
int vein_register_stop()
{
    write_vein_to_database(CMD_REG_END, DEV_ID, 0);
    if(ERR_SUCCESS == recv_data[RESULT])
    {
        return ERR_SUCCESS;
    }
    return ERR_OPERATION;
}

/*******************************************************************************
* Function Name: vein_delete
********************************************************************************
*
* Summary:
* 1.delete vein
*
* Parameters:
*  int
*
* Return:
*  int
*
*******************************************************************************/
int vein_delete(int f_id)
{
	if(ALL_VEIN != f_id)
	{
        //根据fid删除指静脉
        send_to_uart_vein(CMD_DELETE_ONE, DEV_ID, f_id);
        if(ERR_SUCCESS == recv_data[RESULT])
        {
            return ERR_SUCCESS;
        }
	}
	else
	{
        //删除所有的指静脉
        send_to_uart_vein(CMD_DELETE_ALL, DEV_ID, 0);
        if(ERR_SUCCESS == recv_data[RESULT])
        {
            return ERR_SUCCESS;
        }
	}
	return ERR_OPERATION;
}


/*******************************************************************************
* Function Name: check_sum 
********************************************************************************
*
* Summary:
* 1.check sum 
*
* Parameters:
*  uchar, int
*
* Return:
*  int
*
*******************************************************************************/
uint32_t check_sum(uint32_t *buffer,int len)
{
	uchar checksum = 0;
	int i = 0;
	for(; i<len; i++)
	{
		checksum ^= buffer[i];
	}
	return checksum & 0xFF;
}

/*******************************************************************************
* Function Name: send_to_uart_vein
********************************************************************************
*
* Summary:
* 1.send cmd to vein uart
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void send_to_uart_vein(int cmd,int dev, int f_id)
{
    int tmp1 = f_id, tmp2 = f_id;
    memset((void*)recv_data, 0, RECV_BUF_SIZE);
    cmd_buf[1] = cmd;
    cmd_buf[2] = dev;
    if(f_id != 0)
    {
        tmp1 &= 0xff;
        tmp2 >>= 8;
        tmp2 &= 0xff;
        cmd_buf[3] = tmp1;
        cmd_buf[4] = tmp2;
    }
	uint32_t xor_sum = check_sum(cmd_buf, XOR_SUM_SIZE);
	cmd_buf[XOR_SUM_SIZE] = xor_sum;
    
    int i = 0;
    for(;i<CMD_BUF_SIZE;i++)
        Cy_SCB_UART_Put(UART_vein_HW,cmd_buf[i]);
        
    for(i= 1;i<CMD_BUF_SIZE-1;i++)
        cmd_buf[i] = 0;
    while(flag_vein != 8);
    flag_vein = 0;
    //*************************************************print——返回信息
    /*for(i=0;i<CMD_BUF_SIZE;i++)
        Cy_SCB_UART_Put(UART_HW,recv_data[i]);
    */    
    
}

/*******************************************************************************
* Function Name: write_vein_to_database
********************************************************************************
*
* Summary:
* 1.write vein to database
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void write_vein_to_database(int cmd,int dev, int p3)
{
    memset((void*)recv_data, 0, RECV_BUF_SIZE);
    cmd_buf[1] = cmd;
    cmd_buf[2] = dev;
    cmd_buf[5] = p3;
	uint32_t xor_sum = check_sum(cmd_buf, XOR_SUM_SIZE);
	cmd_buf[XOR_SUM_SIZE] = xor_sum;
    
    int i = 0;
    for(;i<CMD_BUF_SIZE;i++)
        Cy_SCB_UART_Put(UART_vein_HW,cmd_buf[i]);
        
    
}

/*******************************************************************************
* Function Name: recv_from_uart_vein
********************************************************************************
*
* Summary:
* 1.recv return massage from vein uart
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void recv_from_uart_vein()
{

}

/*******************************************************************************
* Function Name: clear_recv_buf
********************************************************************************
*
* Summary:
* 1.clean buf
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void clear_buf(uint32_t *buf)
{
    int j = 0;
    for(;j<8;j++)
    {
        buf[j] = 0;
    }
}

/*******************************************************************************
* Function Name: vein_cnt()
********************************************************************************
*
* Summary:
* 1.vein count number
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
int vein_cnt()
{
    send_to_uart_vein(CMD_UPLOAD_COUNT, DEV_ID, 0);
    if(ERR_SUCCESS == recv_data[RESULT])
    {
        uint32_t vein_cnt;
        vein_cnt = recv_data[FINGER_ID_PLACE];
        vein_cnt <<= 8;
        vein_cnt |= recv_data[FINGER_ID_PLACE-1];
        return vein_cnt;
    }
    return ERR_OPERATION; 
}

//以下是串口中断和初始化函数
/***************************************************************************//**
* Function Name: ISR_UART_vein
********************************************************************************
*
* Summary:
* This function is uart_vein interrupt  
* get uart_read_data 
* 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void ISR_UART_vein(void){
    
    
     /* Check for "RX fifo not empty interrupt" */
    if((UART_vein_HW->INTR_RX_MASKED & SCB_INTR_RX_MASKED_NOT_EMPTY_Msk ) != 0)
	{
        /* Clear UART "RX fifo not empty interrupt" */
		UART_vein_HW->INTR_RX = UART_vein_HW->INTR_RX & SCB_INTR_RX_NOT_EMPTY_Msk;        
         
        /* Get the character from terminal */
     
        read_buf = Cy_SCB_UART_Get(UART_vein_HW);       
        recv_data[flag_vein++] = read_buf;
                
        data_received_vein = 1; 
        /* Update data_received flag */
             
	}   
    else
    {
        /* Error if any other interrupt occurs */
        uart_error_vein = 1;
    }

}

/***************************************************************************//**
* Function Name: uart_vein_init
********************************************************************************
*
* Summary:
* vein init
*  
* 
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void uart_vein_init(void){
    
         
     cy_en_scb_uart_status_t init_status;
        
    /* Start UART operation. */
    init_status = Cy_SCB_UART_Init(UART_vein_HW, &UART_vein_config, &UART_vein_context);
    if(init_status!=CY_SCB_UART_SUCCESS)
    {
        handle_error();
    }
    Cy_SCB_UART_Enable(UART_vein_HW);  
    
    UART_vein_HW->INTR_RX_MASK = SCB_INTR_RX_MASK_NOT_EMPTY_Msk;
    
    /* Interrupt Settings for UART */    
    Cy_SysInt_Init(&UART_vein_SCB_IRQ_cfg, ISR_UART_vein);
    
    /* Enable the interrupt */
    NVIC_EnableIRQ(UART_vein_SCB_IRQ_cfg.intrSrc);
    /* Initialize flags */
    
    data_received_vein = 0;
    uart_error_vein = 0;
    

}

/***************************************************************************//**
* Function Name: handle_error
********************************************************************************
*
* Summary:
* This function processes unrecoverable errors such as any component 
* initialization errors etc. In case of such error the system will switch on 
* RED_LED_ERROR and stay in the infinite loop of this function.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void handle_error(void)
{
     /* Disable all interrupts */
    __disable_irq();
    
    /* Switch on error LED */
    Cy_GPIO_Write(RED_LED_ERROR_0_PORT, RED_LED_ERROR_0_NUM, LED_ON);
    while(1u) {}
}

/* [] END OF FILE */