/*******************************************************
*Copyright(C),2020,Shanghai Eastsoft Microelectronics Co.,Ltd.
*�ļ�����iap_dataflash.c
*��  �ߣ�AE Team
*��  ����v1.1��������iDesigner(v4.2.3.166) + ������HRCC(v1.2.0.106)
*��  �ڣ�2020/12/18
*��  ����������IAP��������������ַ��0xC000~0xC1FF
        IAP������ҳ(256����ַ��Ԫ)Ϊ��λ����һ����ִַ�в����������ǲ��������ַ���ڵ�һ��ҳ��
        IAPд����һ����ַ��ԪΪ��λ(һ����ַ��Ԫ��Ӧһ��Word)��ÿҳ����ʱ������2ms��������ַ���ʱ������25us��
		����IAP����������������ֹ�����ܷ��������
        ע�⣺����IAP����ǰ�ر�WDT(������ѡ��WDTENΪDisable)����ʹ��WDT�������ú����WDT���ʱ�䣬�������幷��
*��  ע�����������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
*******************************************************/
#include <hic.h>

#define CLRWDT()   {__Asm CWDT;}        //�궨���幷ָ��

#define STARTADDR    0xC000     //�궨��������IAP������ʼ��ַ
#define ENDADDR      0xC07F     //�궨��������IAP����������ַ
#define WRDATA       0x87654321     //�궨���д������

unsigned char dataerr = 0;      //0��д���������ȣ�1��д�����������

unsigned char CallFlashEn,FlashEwEn;//���������

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

/******************************************************
��������void StartIAP(void)
��  ������ʼIAP������ע�⣺IAP�̶�������ʽ��ʹ��ʱ���ɸı�
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void StartIAP(void)
{
    __asm{
          MOVI 0x55
          MOVA ROMCH
          MOVI 0xFF     //8��NOP������俪ʼ
          SECSEL &FlashEwEn& % 0x80//ѡ��������ڵ�section
          XOR &FlashEwEn& % 0x80,0//0xFF xor 0xAA = 0x55
          SECSEL &CallFlashEn& % 0x80//ѡ��������ڵ�section
          XOR &CallFlashEn& % 0x80,0//��CallFlashEn�ڵ��ú���ǰΪ0x55��0x55 xor 0x55 = 0x00
          JBS PSW,Z//���������Ϊ0x00����ִ����ȷ��������һ��NOP
          GOTO $+3
          NOP//8��NOP����������
          MOVI 0xAA
          MOVA ROMCH
          MOVI 0xFF     //8��NOP������俪ʼ
          SECSEL &FlashEwEn& % 0x80//ѡ��������ڵ�section
          XOR &FlashEwEn& % 0x80,0//0xFF xor 0xAA = 0x55
          SECSEL &CallFlashEn& % 0x80//ѡ��������ڵ�section
          XOR &CallFlashEn& % 0x80,0//��CallFlashEn�ڵ��ú���ǰΪ0x55��0x55 xor 0x55 = 0x00
          JBS PSW,Z//���������Ϊ0x00����ִ����ȷ��������һ��NOP
          GOTO $+3
          NOP//8��NOP����������
          BSS ROMCL,WR     //������̲���
          JBC ROMCL,WR
          goto $-1         //�ȴ���������
        }
}

/*******************************************************
��������void DataFlashErsPage(void)
��  ��������DataFlash��ҳ(256 word)����ַ��Χ��0xC000~0xC1FF
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void DataFlashErsPage(void)
{
	unsigned char gie_bk = GIE;		//����GIE
    CLRWDT();
	while(GIE == 1)GIE = 0;            //����IAPǰ�ر�ȫ���ж�   
    FPEE = 1;           //�洢������ģʽ
    FRAH = 0xC0;
    FRAL = 0x00;
    FRAHN = ~FRAH;      //IAP����/��̱��������ַȡ���߼�
    FRALN = ~FRAL;     
	FlashEwEn = 0xAA;//�������������
    WREN = 1;           //ʹ�ܲ���/��̹���
    StartIAP();         //��ʼIAP����
	CallFlashEn = 0;//�������������
	FlashEwEn = 0;
    ROMCL = 0x00;       //�˳�IAP
	GIE = gie_bk;       //�ָ�ȫ���ж�
}

/*******************************************************
��������unsigned long DataFlashRdData(unsigned int addr)
��  ������Flashָ����ַ������
����ֵ��DataFlash��ַaddr����ַ��Χ��0xC000~0xC1FF
���ֵ����
����ֵ��DataFlashָ����ַ������
*******************************************************/
unsigned long DataFlashRdData(unsigned int addr)
{
	unsigned long data;
	unsigned char gie_bk = GIE;	  //����GIE
    CLRWDT();
	while(GIE == 1)GIE = 0;
	DATARDEN=1;
    FRAH = addr>>8;
    FRAL = addr;
	DATARDTRG=1;
	while(DATARDTRG);
    __Asm TBR;          //����ָ��
    data = ((unsigned long)ROMD1H<<24) | ((unsigned long)ROMD1L<<16) | ((unsigned long)ROMDH<<8) | ROMDL;
    GIE = gie_bk;       //�ָ�ȫ���ж�
	return data;
}

/*******************************************************
��������void DataFlashWrData(unsigned int addr, unsigned long word)
��  ����дDataFlashָ����ַ�����ݣ���ַ��Χ��0xC000~0xC1FF
����ֵ��DataFlash��ַaddr����д������word
���ֵ����
����ֵ��0:д��ʧ�ܣ�1:д��ɹ�
*******************************************************/
unsigned char DataFlashWrData(unsigned int addr, unsigned long word)
{
    unsigned char rmdh,rmdl,rmd1h,rmd1l;      //��ʱ���ROMDH��ROMDLֵ
	unsigned char gie_bk = GIE;	  //����GIE
    CLRWDT();
	while(GIE == 1)GIE = 0;            //����IAPǰ�ر�ȫ���ж�
    FPEE = 0;           //�洢�����ģʽ
    ROMD1H = word>>24;    //дWord
    ROMD1L = word>>16;
    ROMDH = word>>8;
    ROMDL = word;
    FRAH = addr>>8;     //IAP��̵�ַ
    FRAL = addr;
    FRAHN = ~FRAH;      //IAP����/��̱��������ַȡ���߼�
    FRALN = ~FRAL;       
	FlashEwEn = 0xAA;//�������������
    WREN = 1;           //ʹ�ܱ��
    StartIAP();         //��ʼIAP����
	CallFlashEn = 0;//�������������
	FlashEwEn = 0;
    rmdh = ROMDH++;
    rmdl = ROMDL++;
    rmd1h = ROMD1H++;
    rmd1l = ROMD1L++;
	DATARDEN=1;      //DATA������ʹ��
    FRAH = addr>>8;
    FRAL = addr;
	DATARDTRG=1;     //DATA����������
	while(DATARDTRG);
    __Asm TBR;          //����ָ��
    if (ROMDH!=rmdh || ROMDL!=rmdl || ROMD1H!=rmd1h || ROMD1L!=rmd1l)
	{
        ROMCL = 0x00;       //�˳�IAP
		return 0;
	}
	FRAH = 0xFF;//ָ�����õĵ�ַ�ռ�
	FRAL = 0xFF;//ָ�����õĵ�ַ�ռ�
	ROMDH = 0xFF;//���ݳ�ʼ��Ϊ0xFF
	ROMDL = 0xFF;//���ݳ�ʼ��Ϊ0xFF
	ROMD1H = 0xFF;//���ݳ�ʼ��Ϊ0xFF
	ROMD1L = 0xFF;//���ݳ�ʼ��Ϊ0xFF
    ROMCL = 0x00;       //�˳�IAP
	GIE = gie_bk;       //�ָ�ȫ���ж�
    return 1;
}

/*******************************************************
��������void main(void)
��  ������������������IAP��������������ַ��0xC000~0xC1FF
����ֵ����
���ֵ����
����ֵ����
*******************************************************/
void main(void)
{
    unsigned char i;
    unsigned long j;
    unsigned int dtflashaddr;       //��ַ����������DataFlash��������д����

    RAMclear();
    GPIOInit();

/************IAP��������������Flash������(256Word)****/
	CallFlashEn = 0x55;			//CallFlashEn��ֵ0x55��������ִ�в�����̲���
    DataFlashErsPage();
    CLRWDT();

/******************IAPд�벢����У��******************/
    for (dtflashaddr=STARTADDR; dtflashaddr<=ENDADDR; dtflashaddr++)
    {
		CallFlashEn = 0x55;		//CallFlashEn��ֵ0x55��������ִ�в�����̲���
        i = DataFlashWrData(dtflashaddr,WRDATA);
        if (i != 1)
            dataerr = 1;        //����У�����
    }
    ROMCL = 0x00;       //�˳�IAP
    CLRWDT();

/******************IAP����У��(ʾ��)******************/
    for (dtflashaddr=STARTADDR; dtflashaddr<=ENDADDR; dtflashaddr++)
    {
        j = DataFlashRdData(dtflashaddr);
        if (j != WRDATA)
            dataerr = 1;        //����У�����
    }
    ROMCL = 0x00;       //�˳�IAP
    CLRWDT();

    while (1)
    {
        CLRWDT();
    }
}