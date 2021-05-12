/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  spi_slave.c
*��  ��:  AE Team
*��  ��:  v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  ��:  2020/12/28
*��  ��:  SPI�ӻ�ģʽ��ʾ
          PA6��NSS
          PB4��SCK
          PB5��MOSI
          PB6��MISO
          �ӻ��жϷ�ʽ��д���ݣ�������д����txbuf�е����ݣ������������ݴ�������rxbuf�С�
*��  ע:  ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��
#define RXBUF_SIZE 8       //�궨����ջ���������
#define TXBUF_SIZE 8       //�궨�巢�ͻ���������

typedef unsigned char uchar;
typedef unsigned int uint;

uchar rxbuf[RXBUF_SIZE];
uint rxcnt = 0;

uchar txbuf[TXBUF_SIZE] = {0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78};
uint txcnt = 0;

/******************************************************
��������void RAMclear(void)
��  ����RAM���������㣬RAM����ַ0x0000~0x03FF
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void RAMclear(void)
{
    for (IAAH=0; IAAH<=0x03; IAAH++)
    {
        for (IAAL=0; IAAL<0xFF; IAAL++)
            IAD = 0x00;
        IAD = 0x00;
    }
}

/******************************************************
��������void GPIOInit(void)
��  �������ȳ�ʼ������δ�õ���IO��Ϊ����͵�ƽ
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void GPIOInit(void)
{
    ANSL = 0xFF;        //ѡ���Ӧ�˿�Ϊ����IO����
	ANSH = 0xFF;
    PAT = 0x00;         //����IO��Ϊ����͵�ƽ
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;
    PAT6 = 1;       //����NSS��Ϊ����
    PBT4 = 1;       //����SCK��Ϊ����
    PBT5 = 1;       //����MOSI��Ϊ����
}

/**********************************************
��������void Init_spi(void)
��  ����SPI��ʼ������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void Init_spi(void)
{
    SPIRST = 1;     //��λSPI
    while(SPIRST);
    SPICON0 = 0xF0;     //CKS = Fosc/16����շ�������������
    SPICON1 &= ~(3<<6);
    SPICON1 |= 0<<6;      //0:�����ط��ͣ��ȣ����½��ؽ��գ��󣩣�
    MS = 1;     //0������ģʽ��1���ӻ�ģʽ
}

/**********************************************
��������void isr(void) interrupt 
��  ���������жϷ������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void isr(void) interrupt
{
    if (RBIE == 1 && RBIF == 1)     //SPI�����ж�
    {
		RBIF = 0;                   //������жϱ�־

        rxbuf[rxcnt++] = SPIRBR;

        if (rxcnt > RXBUF_SIZE - 1)
            rxcnt = 0;

        CLRWDT();
    }

	if(TBIE == 1 && TBIF == 1)      //SPI�����ж�
	{
		TBIF = 0;                   //�巢���жϱ�־

	    SPITBW = txbuf[txcnt++];

        if (txcnt > TXBUF_SIZE - 1)
            txcnt = 0;

        CLRWDT();
	}

    SPI_GIEIF = 0;  //���жϱ�־
}

/**********************************************
��������void main(void) 
��  ����������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void main(void) 
{
//    RAMclear();
    GPIOInit();
    Init_spi();
    RBIE = 1;       //�򿪽����ж�
    TBIE = 1;       //�򿪷����ж�
    SPI_GIEIE = 1;  //��SPI�ж�
    SPI_GIEIF = 0;  //���жϱ�־
    GIE = 1;        //�����ж�
    REN = 1;        //�򿪽���
    SPIEN = 1;      //SPIʹ��
    while(1)
    {
        CLRWDT();
    }
}
