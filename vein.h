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

/* [] END OF FILE */
/*************************************************************************
    > File Name: vein.h
    > Author: leeksir
    > Mail: leek9527@163.com 
    > Created Time: Wed May  9 19:23:15 2018
 ************************************************************************/

#ifndef _VEIN_H_
#define _VEIN_H_
    
#include<stdio.h>
#include <project.h>
#include<string.h>
    
/*******************************************************************************
*        Constants
*******************************************************************************/
#define LED_ON  (0x00u)
#define LED_OFF (!LED_ON)


//命令码
#define CMD_ONE_VS_N 	                0x00		//采集特征1：N比对
#define CMD_ONE_VS_G 	                0x01		//采集特征1：G比对
#define CMD_ONE_VS_ONE 	                0x02		//采集特征1：1比对
#define CMD_REGISTER	                0x03		//注册手指
#define CMD_REG_END		                0x04		//注册结束
#define CMD_DELETE_ONE                  0x05		//删除单个手指
#define CMD_DELETE_ALL	                0x06		//删除所有手指
#define CMD_UPLOAD_ALL_ID	            0x07	    //上传所有手指ID信息
#define CMD_UPLOAD_INFOR                0x08	    //上传制定手指信息
#define CMD_UPLOAD_TEMPLATE             0x09	    //上传指定手指模板
#define CMD_UPLOAD_INFOR_TEMPLATE	    0x0A		//上传指定手指以及对应模板
#define CMD_CREAT_TEMPLATE	            0x0B	    //采集并上传模板
#define CMD_DOWNLOAD_INFOR_TEMPLATES	0x0C	    //下载手指信息头和所有模板
#define CMD_UPLOAD_VERSION	            0x0D	    //获取固件版本号
#define CMD_UPLOAD_COUNT 	            0x0E	    //获取注册手指总数
#define CMD_CHK_FINGER 		            0x0F	    //获取手指状态
#define CMD_UPLOAD_SEQUENCE	            0x10	    //获取设备序列号
#define CMD_SET_BAUD		            0x11	    //设置波特率
#define CMD_SET_DEVID		            0x12	    //设置设备编号

//应答码
#define ERR_SUCCESS		                0x00		//操作成功
#define ERR_FALT		                0x01		//操作失败
#define ERR_TIMEOUT		                0x02		//操作超时
#define ERR_INFOR_FULL	                0x03		//信息头存储空间满
#define ERR_TEMPLATE_FULL	            0x04	    //模板存储空间满
#define ERR_TEMPLATE_OCC 	            0x05	    //注册时两次采集静脉不同
#define ERR_FINGERID_OCC 	            0x06	    //手指FID已存在
#define ERR_FINGERID_NULL	            0x07	    //不存在该手指ID
#define ERR_GROUPID_NULL	            0x08	    //不存在该组号
#define ERR_FLASH		                0x09		//Flash操作错误
#define ERR_NO_FINGER 	                0x0E        //未检测的手指
#define	ERR_REG_BUFFFULL	            0x0F	    //注册缓存区满
#define ERR_TEMPLATE	                0x10		//生成模板不合格
#define ERR_CAP			                0x11		//拍照超时
//波特率
#define BPS_9600	0x01
#define BPS_19200	0x02
#define BPS_57600	0x06
#define BPS_115200 	0x0C

typedef unsigned char uchar;
/*******************************************************************************
*        Global variables
*******************************************************************************/

volatile uint32_t recv_data[10];
volatile uint32_t read_data,read_buf;
uint32_t data_received, data_received_vein;
volatile uint8_t uart_error, uart_error_vein;
volatile int flag_uart, flag_vein;
uint32_t fid;

    
#define ALL_VEIN	 	-1 			//代表所有的静脉
#define DEV_ID 			0xFF 		//静脉模块的设备号
#define CMD_BUF_SIZE 	8			//命令长度
#define RECV_BUF_SIZE 	100			//接受静脉模块返回信息长度
#define XOR_SUM_SIZE 	6			//异或校验和长度
#define RESULT			5			//返回结果位置
#define FINGER_ID_PLACE 4           //静脉ID所在位置
#define FINGER_ADD_CNT  3           //静脉添加时输入指纹的次数

//错误码
#define ERR_OPERATION   -2          //操作失败 



//函数声明
int vein_check();					                        //手指状态检测
int vein_recognition();				                        //静脉识别
int vein_add();					                            //静脉添加
int vein_register_stop();                                   //取消本轮注册
int vein_delete(int f_id);			                        //静脉删除
int vein_cnt();                                             //已注册的静脉数量
uint32_t check_sum(uint32_t *buffer,int len);               //异或校验和
void uart_vein_init(void);				                    //初始化串口
void ISR_UART_vein(void);                                   //静脉串口中断函数
void send_to_uart_vein(int cmd,int dev,int fid);		    //发送命令到静脉模块
void write_vein_to_database(int cmd,int dev, int p3);       //将添加的静脉信息写入数据库
void recv_from_uart_vein();				                    //从静脉模块接受信息
void clear_buf(uint32_t *buf);                              //清空buf 
void handle_error(void);





#endif

