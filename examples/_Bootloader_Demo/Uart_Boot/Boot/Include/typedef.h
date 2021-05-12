/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd.
*�ļ���:	typedef.h
*��  ��:	AE Team
*��  ��:	V1.01
*��  ��:	2020/07/23
*��  ��:	Bootloader demo ����
*��  ע:    ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#ifndef   __TYPEDEF__H
#define   __TYPEDEF__H
#include  "macrodef.h"

/**********************************************************/
//  add new datatypes by linford
/**********************************************************/
typedef   unsigned  char   u8;
typedef   unsigned  int    u16;
typedef   unsigned  long   u32;

/**********************************************************/
union u_b32b08_t{
	u32 b32;
	u8 b08[4];
};

typedef enum {
	            WAIT_REV_INS = 0,   
	            IN_REV_INS,   
	            END_IN_INS,  
	            WAIT_REV_PARA1,   
	            END_REV_PARA1,    
	            WAIT_REV_PARA2,      
	            IN_EXE_INS,       
	            IN_SEND_OVER,      
	            OTHER    
} isp_step_t; 

typedef struct isp_data_t{
    isp_step_t  step;	//ָ��ʱ����
    u8 syn;      //�Ƿ����ͬ��
	u8 cmd;      //ָ����
	u8 triger;    //��һ��������ɣ�������һ������
	u8 rx_len;   //���ճ���
	u8 tx_len;   //���ͳ���
	u8 rx_cnt;   //���ռ���
	u8 tx_cnt;   //���ͼ���
    union u_b32b08_t para1;  //����1
    union u_b32b08_t para2;  //����2
    u8 rx_buf[132];	//���ջ���
    u8 tx_buf[132];  //���ͻ���
} isp_data_t;
#endif
