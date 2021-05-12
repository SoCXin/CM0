/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd.
*�ļ���:	macrodef.h
*��  ��:	AE Team
*��  ��:	V1.01
*��  ��:	2020/07/23
*��  ��:	Bootloader demo ����
*��  ע:    ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#ifndef   __MACRODEF__H
#define   __MACRODEF__H


// ��غ������߼���
#define  ASM_GOTO(X)   __Asm  AJMP  X

/*****����ִ�н��******/
#define  SUCCESS   0X01        //�ɹ�
#define  FAILED	   0X00        //ʧ��

// �����û��������
#define  BOOT_APP_ADDR	(	0x0000	)
#define  USR_APP_ADDR	(	0x0900	)
#define  USR_APP_ENTER	(	0x0800	)

/*********** ����Ӳ����صĺ���/ʵ��************/
// LED ָʾ��
#define LED_INUSED
#define KEY_INUSED

#ifdef LED_INUSED
#define LED_PIN			(PA2)
#define LED_PIN_xS		(PAS2)
#define LED_PIN_xT     (PAT2)
#define LED_ON			(LED_PIN = 0)
#define LED_OFF			(LED_PIN = 1)
#define LED_BLINK		(LED_PIN ^= 1)
#else
#define LED_PIN
#define LED_PIN_xS
#define LED_PIN_xT
#define LED_ON
#define LED_OFF
#define LED_BLINK
#endif

#ifdef KEY_INUSED
#define KEY_PIN			(PC3)
#define KEY_PIN_xS      (PCS3)
#define KEY_PIN_xT		(PCT3 )
#define KEY_PIN_PxPU	(PCPU3 )

#else
#define KEY_PIN
#define KEY_PIN_xS
#define KEY_PIN_xT
#endif

// ����bootloaderʹ�õ�uart����
#define UARTx_INUSED 1

#if UARTx_INUSED == 1
// �ж�ʹ��
#define RXxIE	RX1IE
#define TXxIE	TX1IE
// �жϱ�־
#define	RXxIF	RX1IF
#define	TXxIF	TX1IF
//
#define	TRMTx	TRMT1
// �����־
#define	FERRx	FERR1
#define	OERRx	OERR1
// �豸ʹ��
#define	RXxEN	RX1EN
#define	TXxEN	TX1EN
// ���ͼĴ���/��9λ
#define	TXxB	TX1B
#define	TXxR8	TX1R8
// ���ܼĴ���/��9λ
#define	RXxB	RX1B
#define	RXxR8	RX1R8
#elif UARTx_INUSED == 3
// �ж�ʹ��
#define RXxIE	RX3IE
#define TXxIE	TX3IE
// �жϱ�־
#define	RXxIF	RX3IF
#define	TXxIF	TX3IF
//
#define	TRMTx	TRMT3
// �����־
#define	FERRx	FERR3
#define	OERRx	OERR3
// �豸ʹ��
#define	RXxEN	RX3EN
#define	TXxEN	TX3EN
// ���ͼĴ���/��9λ
#define	TXxB	TX3B
#define	TXxR8	TX3R8
// ���ܼĴ���/��9λ
#define	RXxB	RX3B
#define	RXxR8	RX3R8
#endif

#define  ACK  0x79
#define  NACK 0x1F
#define  GO        0xA1
#define  SYN       0X3F
#define  READ_M    0x91
#define  WRITE_M   0xB1
#define  EX_ERASE  0xC4
#define  CHECK_EMPTY 0xD1

#define  FRAME_INTERVAL  80 //һ֡��������byte���ʱ�����80ms
#define  Boot_OVERTIME_1  20  //��λ�����BootPINΪ�ͣ�����20msû��ͬ���ɹ���ʼ��������
#define  Boot_OVERTIME_2  300

#endif
