/*********************************************************
*Copyright (C), 2020, Shanghai Eastsoft Microelectronics Co., Ltd
*�ļ���:  adc_offset.c
*��  ��:  AE Team
*��  ��:  v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  ��:  2020/12/25
*��  ��:  ADCͨ����ʾ
          PA7��ģ������˿�
          PB5��TX
          PB6��RX
          ����ת�����ADֵͨ�����ڷ��͵���λ����������9600bps���궨��VDDVREFѡ��VDD��Ϊ�ο���ѹ���ˣ�
          ����ѡ���ڲ�2.048V��Ϊ�ο���ѹ���ˡ�
		  ת��ֵ��ȥoffset����ADC���������offsetΪ802FH���ڲ�2.048V���ο�����8030H��VDD���ο�����8λ���ݣ�
		  IAP��16λ���ݺ������ж��Ƿ�����ߵ�8λȡ����
		  ע�⣺����offset�Ĵ��ڣ�Ϊ��ֹ�����ת��ֵС��offsetʱ��ADC���ֱ�ӹ�0������ADC���ֻ�ܲ⵽(0xFFF-offset)��
*��  ע:  ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
**********************************************************/
#include <hic.h>

#define VDDVREF        //�궨��ѡ��VDD��Ϊ�ο���ѹ��ע�ͱ�����ѡ���ڲ�2.048V��Ϊ�ο���ѹ����
#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��

typedef unsigned char uchar;
typedef unsigned int uint;

uint adc_value;
uint offset_value = 0;  //����ADCУ׼ֵ
uchar str[] = "ͨ�� ��ת��ֵΪ:    \r\n\0";

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
    ANSH = 0xFF;        //ѡ���Ӧ�˿�Ϊ����IO����
    ANSL = 0xFF;        //ѡ���Ӧ�˿�Ϊ����IO����
    PAT = 0x00;         //����IO��Ϊ����͵�ƽ
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;
}

/**********************************************
��������UART_send(uchar *str,uchar ch,uint value)
��  ����UART���ͺ���
����ֵ���ַ���ָ�룬ͨ������ADCת��ֵ
���ֵ����
����ֵ����
**********************************************/
void UART_send(uchar *str,uchar ch,uint value)
{
    *(str+4) = ch+'0';
    *(str+16) = value/1000+'0';
    *(str+17) = value%1000/100+'0';
    *(str+18) = value%100/10+'0';
    *(str+19) = value%10+'0';

    while(*str)
    {
        while(!TRMT1);
        TX1B = *str++;
    }
}

/**********************************************
��������ADC_convert(uchar ch)
��  ����ADCת������
����ֵ��ת��ͨ��ch
���ֵ����
����ֵ��ת����12λ���
**********************************************/
uint ADC_convert(void)
{
    ADTRG = 1;      //����ADת��
    while(ADTRG);
    ADIF = 0;

    return (uint)ADCRH<<8 | ADCRL;
}

/*******************************************************
��������uint GetOffset2048(void)
��  ������ȡ�ο���ѹΪ�ڲ�2.048Vʱoffsetֵ
����ֵ����
���ֵ����
����ֵ��offsetֵ
*******************************************************/
uint GetOffset2048(void)
{
	uint value_tmp = 0;
	unsigned char gie_bk = GIE;		//����GIE

    CLRWDT();

	while(GIE) GIE = 0;
	DATARDEN = 1;

	FRAH = 0x802F >> 8;
	FRAL = 0x802F;
	DATARDTRG = 1;
	while(DATARDTRG) ;	     //�ȴ������
	__Asm TBR;		         //����ָ��
	value_tmp = (ROMDH<<8) | ROMDL;

    ROMCL = 0x00;            //�˳�IAP
	GIE = gie_bk;            //�ָ�ȫ���ж�

	return value_tmp;
}

/*******************************************************
��������uint GetOffsetVDD(void)
��  ������ȡ�ο���ѹΪVDDʱoffsetֵ
����ֵ����
���ֵ����
����ֵ��offsetֵ
*******************************************************/
uint GetOffsetVDD(void)
{
	uint value_tmp = 0;
	unsigned char gie_bk = GIE;		//����GIE

    CLRWDT();

	while(GIE) GIE = 0;
	DATARDEN = 1;

	FRAH = 0x8030 >> 8;
	FRAL = 0x8030;
	DATARDTRG = 1;
	while(DATARDTRG) ;	     //�ȴ������
	__Asm TBR;		         //����ָ��
	value_tmp = (ROMDH<<8) | ROMDL;

    ROMCL = 0x00;            //�˳�IAP
	GIE = gie_bk;            //�ָ�ȫ���ж�

    return value_tmp;
}

/**********************************************
��������main()
��  ����������,ѡ��AIN7ͨ��������ֵ��UART���͵���λ��
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void main(void) 
{
    uint i = 800;         //��ʱ����
    uint j = 3000;        //��ʱ����
	uchar cnt = 3;

//    RAMclear();
    CLRWDT();
	GPIOInit();

/****************��ʼ��UART*******************/
    PBT5 = 0;       //TX���
    PBT6 = 1;       //RX����
    TX1LEN = 0;     //8λ���ݷ��͸�ʽ
    BRGH1 = 0;      //�����ʵ���ģʽ��������=Fosc/(64*(BRR<7:0>+1))
    BR1R = 25;      //������=16MHz/(64*26)��9600bps
     
/****************��ʼ��ADC********************/
#ifdef VDDVREF
	while(cnt != 0)
	{
        offset_value = GetOffsetVDD();  //��ȡoffsetֵ
		if((offset_value>>8) == ((~offset_value) & 0x00FF)) //�Ƿ�����ߵͰ�λȡ��������ȡ��8λ��Ϊoffset
		{
			offset_value =  offset_value & 0x00FF;

			break;
		}
		else
		{
		    cnt--;

			if(cnt == 0)
			    offset_value = 0;  //��IAP����3�κ��Բ�����ߵ�8λȡ����offset�̶�Ϊ0
		}
	}
    ANSL7 = 0;          //ѡ��AIN7Ϊģ���
    ADCCL |= 0xF0;      //ADCCL<7:4>ѡ��ͨ��
    ADCCL &= 0x7F;      //ѡ��ͨ��7
	ADCCM = 0x4B;       //�ο�ԴVDD,���ο��̶�ѡ��VSS��ת��λ���̶�ѡ��12λ��AD����offset�̶�ѡ��λ1
    ADCCH = 0xC8;       //��λ����;ʱ������FOSC/16
#else
	while(cnt != 0)
	{
        offset_value = GetOffset2048();  //��ȡoffsetֵ
		if((offset_value>>8) == ((~offset_value) & 0x00FF)) //�Ƿ�����ߵͰ�λȡ��������ȡ��8λ��Ϊoffset
		{
			offset_value =  offset_value & 0x00FF;

			break;
		}
		else
		{
		    cnt--;

			if(cnt == 0)
			    offset_value = 0;    //��IAP����3�κ��Բ�����ߵ�8λȡ����offset�̶�Ϊ0
		}
	}
    ANSL7 = 0;          //ѡ��AIN7Ϊģ���
    ADCCL |= 0xF0;      //ADCCL<7:4>ѡ��ͨ��
    ADCCL &= 0x7F;      //ѡ��ͨ��7
	ADCCM = 0x6B;       //�ο�Դ�ڲ��̶�ѡ��2.048V,���ο��̶�ѡ��VSS��ת��λ���̶�ѡ��12λ��AD����offset�̶�ѡ��λ1
    ADCCH = 0xC8;       //��λ����;ʱ������FOSC/16
    VREFEN = 1;         //�ο���ѹģ��ʹ��
	while(j--);         //�ȴ�300us
	VREF_CHOPEN = 1;    //�ڲ��ο�����ʹ�ܵ�ѹն����
#endif

	ADC_LP_EN = 1;      //ADC�͹��ı���̶�ʹ��
    PAT7 = 1;
    SMPS = 1;           //Ӳ�����Ʋ�����ADTRG=1ʱ����AD����ת��
    ADEN = 1;           //ʹ��ADCģ��
    while(i--);         //ʹ��AD�������ڵ�һ��ת��֮ǰ��ʱ80us����

    ADC_convert();      //�״�ת��ֵ��׼ȷ������

    while(1)
    {
		adc_value = ADC_convert();

		if(adc_value > offset_value)   //ADת��ֵ����offsetֵ���ȥoffset������ADC�����0
		    adc_value -= offset_value;
		else
		    adc_value = 0;

		TX1EN = 1;      //�򿪷���
        UART_send(str,7,adc_value);
        for(i = 0;i<50000;i++);        //��ʱ
        for(i = 0;i<50000;i++);
        for(i = 0;i<50000;i++);
        for(i = 0;i<50000;i++);
        for(i = 0;i<50000;i++);
        for(i = 0;i<50000;i++);
        TX1EN = 0;      //�رշ���

        CLRWDT();
    }
}