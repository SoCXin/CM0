/*******************************************************
*Copyright(C),2020,Shanghai Eastsoft Microelectronics Co.,Ltd.
*�ļ�����compare_square_output.c
*��  �ߣ�AE Team
*��  ����v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  �ڣ�2020/07/07
*��  �����๦�ܶ�ʱ���������ģʽ��T31_CH1N(PB2)���������PB0�ߵ�ƽʱT31_CH1N�����
*��  ע������������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��

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
	ANSH = 0xFF;        //ѡ���Ӧ�˿�Ϊ����IO����
    PAT = 0x00;         //����IO��Ϊ����͵�ƽ
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;

    PBT0 = 1;           //T31_ETR��Ϊ����
    PBPD0 = 1;          //ʹ������
}

/*******************************************************
��������void main(void)
��  ������������T31_CH1N(PB2)���������PB0�ߵ�ƽʱT31_CH1N�����
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void main(void) 
{
    RAMclear();
    GPIOInit();
    T31C0L = 0x00;        //���ؼ���ģʽ�����ϼ���
    T31C2L = 0x08;        //��ֹ��ģʽ,ʹ���ⲿ�¼����OCnREF
    T31CH1C = 0xB0;       //ʹ������Ƚ�1���㣬������ƥ��T31CH1Rʱ��תOC1REF��CC1ͨ������Ϊ��� 
    T31PINCL = 0x04;      //OC1Nͨ���ߵ�ƽ��Ч��ʹ�����
    T31PRSH = 0x00;       //Ԥ��Ƶϵ��1:16
    T31PRSL = 0x0F;
    T31CNTLDH = 0x01;     //��װ��ֵ����������=T31CNTLD*2*��Ƶ���ʱ������
    T31CNTLDL = 0xF4;
    T31CH1RH = 0x00;      //�ȽϼĴ���1ֵ
    T31CH1RL = 0x08;
    CHOE = 1;           //���ʹ��
    T31EN = 1;          //ʹ�ܼ�����
    while (1)
    {
        CLRWDT();
    }
}