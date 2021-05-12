/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  iic_slave.c
*��  ��:  AE Team
*��  ��:  v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  ��:  2020/07/09
*��  ��:  I2C�ӻ�ģʽ��ʾ
          PB1��SCL
          PB0��SDA
          I2C�ӻ�ͨ�ţ������շ����������ݴ�������send_data���������ݴ�������rece_data��
*��  ע:  ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��
#define SLAVE_ADDR 0x2D

typedef unsigned char uchar;
typedef unsigned int uint;

uchar send_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uchar rece_data[8] = {0};
uchar send_i = 0;
uchar rece_i = 0;
uchar counter = 0;

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
    PBT0 = 1;       //SDA�˿���Ϊ����
    PBT1 = 1;       //SCL�˿���Ϊ����
}

/**********************************************
��������void isr(void) interrupt 
��  �����жϺ���
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void isr(void) interrupt
{
    if(I2CIE==1 && I2CIF==1)
    {
        if(I2CRBIE==1 && I2CRBIF==1)      //�����ж�
        {
			I2CRBIF = 0;
            rece_data[rece_i] = I2CRB;    //��ȡ����
			rece_i++;
			if(rece_i >= 8)
			    rece_i = 0;
        }
		
		if(I2CTBIE==1 && I2CTBIF==1)
		{
			I2CTBIF = 0;
			I2CTB = send_data[send_i];     //�����ݷŵ����ͻ�����
			send_i++;
			if(send_i >= 8)
			    send_i = 0;
		}
        
        if(I2CSRIE==1 && I2CSRIF==1)    //�յ���ʼλ+��ַƥ��+����Ӧ��λ
        {
            I2CSRIF = 0;                //�����ʼλ+��ַƥ��+����Ӧ��λ�ж�
        }

        if(I2CSPIE==1 && I2CSPIF==1)    //ֹͣλ�ж�
        {
            I2CSPIF = 0;
            I2CRST = 1;         //��λI2C
            while(I2CRST);      //�ȴ���λ���

			//reset param
			I2CSA = SLAVE_ADDR<<1;      //���ôӻ���ַ
            I2CC = 0xE1;	    //I2C��������©���������ACK��I2Cʹ��;
            I2CIEC = 0x4F;		//ʹ�ܵ�ַƥ���жϣ�NACK�жϣ�ֹͣλ�жϣ����տ�/���ͷǿ��ж�
			I2CIE = 1;          //ʹ��I2C���ж�
		    I2CIFC = 0x00;      //�����־λ

			send_i = 0;
			counter = 0;

        }
        if(I2CNAIE==1 && I2CNAIF==1)    //NACK�ж�
        {
            I2CNAIF = 0;
        }
    }
    I2CIF = 0;
}

/**********************************************
��������main() 
��  ����������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void main() 
{
    uint i;
    GPIOInit();
    I2CRST = 1;         //��λI2C
    while(I2CRST);      //�ȴ���λ���
    I2CC = 0xE0;        //I2C��������©���������ACK;
    I2CSA = SLAVE_ADDR<<1;      //�ӻ���ַ
    I2CEN = 1;          //ʹ��I2C
    I2CIEC = 0x4F;      //ʹ�ܵ�ַƥ���жϣ�NACK�жϣ�ֹͣλ�жϣ����տ�/���ͷǿ��ж�
    I2CIE = 1;          //ʹ��I2C���ж�
    I2CIFC = 0x00;      //�����־λ
    GIE = 1;            //�����ж�

    while(1)
    {
        CLRWDT();
    }
}
