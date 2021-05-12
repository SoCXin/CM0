/*****************************************************************
*Copyright (C), 2017, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  iap_flash.h
*��  ��:  AE Team 
*��  ��:  2020/07/23
*��  ��:  ��������ͷ�ļ���������ES7P0693
*��  ע:  �������������ʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
******************************************************************/

#ifndef _ES7P0693_IAP_
#define _ES7P0693_IAP_

#include "ES7P0693.h"
#include <hic.h>

/* �������Ͷ��� */
typedef unsigned char  uchar;		//�޷���8λ���ͱ���
typedef unsigned int   uint;		//�޷���16λ���ͱ��� 
typedef unsigned long  ulong;       //�޷���32λ���ͱ���

typedef unsigned char  u8;          //�޷���8λ���ͱ���
typedef unsigned int   u16;         //�޷���16λ���ͱ��� 
typedef unsigned long  u32;       	//�޷���32λ���ͱ���

typedef unsigned char  INT8U;       //�޷���8λ���ͱ���
typedef signed   char  INT8S;       //�з���8λ���ͱ���
typedef unsigned int   INT16U;      //�޷���16λ���ͱ��� 
typedef signed   int   INT16S;      //�з���16λ���ͱ���
typedef unsigned long  INT32U;      //�޷���32λ���ͱ���
typedef signed   long  INT32S;		//�з���32λ���ͱ���

typedef float          FP32;		//�����ȸ����ͱ���[floating-point type:������]
typedef double         FP64;		//˫���ȸ����ͱ���

/* �����嶨�� */
typedef union 
{
	unsigned char byte;
	struct P8Bit
	{
	   unsigned char bit0:1;	// 1 
	   unsigned char bit1:1;	// 2 
	   unsigned char bit2:1;	// 3 
	   unsigned char bit3:1;	// 4 
	   unsigned char bit4:1;	// 5 
	   unsigned char bit5:1;	// 6 
	   unsigned char bit6:1;	// 7 
	   unsigned char bit7:1;	// 8 
	}bitn;
}BitAccess; 	

/* ���ָ��궨�� */
#define nop()		{__Asm NOP;}	//��
#define clr_wdt()	{__Asm CWDT;}	//�幷
#define IDLE()		{__Asm IDLE;}	//�͹���ģʽ
//#define TBR()		{__Asm TBR;}	//����
#define TBR          __Asm TBR	  //������ָ��

/* ��־λ�궨�� */
#define FALSE		0
#define TRUE		1
#define WRONG		0
#define SUCCESS		1


//FRAH-PAGEѡ��
#define CODEADDRH	0x3F        //CODE������ҳ�ߵ�ַ
//#define CODEADDRH	0x08

#define CODEADDRL	0x00

//FRAH-����ѡ��
#define CODE	0x00

#define WRDATA0H   0x55        //ROMD0��д����ֽ�����(85)
#define WRDATA0L   0xAA        //ROMD0��д����ֽ�����(170)
#define WRDATA1H   0x55        //ROMD1��д����ֽ�����(85)
#define WRDATA1L   0xAA        //ROMD1��д����ֽ�����(170)

#define ERASEDATA 0xFF		  //������ɺ�����(256)

#endif