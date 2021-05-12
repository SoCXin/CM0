/*******************************************************
*Copyright(C),2020,Shanghai Eastsoft Microelectronics Co.,Ltd.
*�ļ�����timer_internal_clock.c
*��  �ߣ�AE Team
*��  ����v1.0��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  �ڣ�2020/07/09
*��  �����๦�ܶ�ʱ���ڲ�ʱ��Դ��ʱģʽ����ʱ��1s�����תPA1
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
	ANSH = 0xFF;
    PAT = 0x00;         //����IO��Ϊ����͵�ƽ
    PBT = 0x00;
    PCT = 0x00;
    PA = 0x00;
    PB = 0x00;
    PC = 0x00;
}

/*******************************************************
��������void isr(void) interrupt
��  �����жϷ������
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void isr(void) interrupt
{
    if (UPIS==1 && UPIF==1)        //��ʱ������ж�
    {
        UPIC = 1;       //�������־λ��UPIF����Ҫ��T31IF֮ǰ���
        T31IF = 0;      //�嶨ʱ���ܱ�־λ��UPIF����Ҫ��T31IF֮ǰ���

        PA1 = ~PA1;
    }
}

/*******************************************************
��������void main(void)
��  ��������������ʱ��1s�����תPA1
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void main(void)
{
    RAMclear();
    GPIOInit();
    T31C2L = 0x00;      //��ֹ��ģʽ��ʹ���ڲ�HRC(16mHz)��Ϊ������ʱ��Դ
    T31C0L = 0x00;      //T31CNTLD�Ĵ����޻��壬д��ֵ������Ч�����������ؼ���ģʽ�����ϼ���
    T31CNTLDH = 0xF4;     //��װ��ֵ������������ƥ��ʱ�����¼�
    T31CNTLDL = 0x24;
    T31PRSH = 0x00;       //Ԥ��Ƶϵ��1:16
    T31PRSL = 0x0F;
    T31POS = 0x0F;        //��������T31CNTLD��16��ƥ���¼�ʱ��������ж�  
    UPIE = 1;       //�򿪼�������ж�
    T31IE = 1;      //�����๦�ܶ�ʱ���ж�
    UPIC = 1;       //�������־λ
    T31IF = 0;      //�嶨ʱ���ܱ�־λ
    GIE = 1;        //����ȫ���ж�
    T31EN = 1;      //ʹ�ܼ�����
    while (1)
    {
        CLRWDT();
    }
}