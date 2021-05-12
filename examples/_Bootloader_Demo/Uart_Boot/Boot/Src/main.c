/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  main.c
*��  ��:  AE Team
*��  ��:  V1.00�� ������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  ��:  2020/07/23
*��  ��:  Bootloaderʾ������
		  PA0��ģ��BootPIN
          PA1��TX
		  PA2��RX
		  �������� �����ʣ�9600  ����λ��8λ  ֹͣλ��1λ  У�飺��У��
		  ��ʾ������ο�Ӧ�ñʼǡ�AN112_UserManual_ES-UART-BOOT��
*��  ע:  ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#include "typedef.h"
#include "rw_flash.h"
#include "string.h"
#include "uart.h"
#include  "sysinit.h"
#include "systick.h"
#include <hic.h>

extern isp_data_t g_isp_data;
extern u32  g_boot_timer;
extern u8   g_boot;
void uart_proc_fsm(void);
void fsm_init(void);
void UartReceive(void);

/**********************************************
��������RAM_clear
��  ����RAM����0
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void RAM_clear(void)
{
	for(IAAH=0; IAAH<=0x03; IAAH++)
	{
		for(IAAL=0;IAAL<0xFF;IAAL++)IAD=0;
		IAD=0;
	}
}
/**********************************************
�������� main
��  ����������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void main()
{
    Init_MCU();
	fsm_init();
    while(1)
    {
        Timer_Proc();     //4ms���Ĵ���
		UartReceive();
        while(g_isp_data.triger == 1)
		{
			uart_proc_fsm();  //isp����״̬������
		}
		if(g_boot == 1)  //��Ҫ��������flash
		{
			Reg_reset();
			RAM_clear();
			PCRH=0x08;
			PCRL=0x00;
			//ASM_GOTO(USR_APP_ENTER);			//�ж�������App�����ǽ���Bootloader
		}
    }
}
