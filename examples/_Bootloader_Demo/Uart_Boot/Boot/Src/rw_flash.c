#include  <hic.h>
#include  <string.h>
#include  "typedef.h"
#include  "macrodef.h"
#include  "rw_flash.h"

/**********************************************
��������check_empty
��  ������ѯFlash�Ƿ�Ϊ��
����ֵ��word_addr:Flash��ʼ��ַ;num:��յ��ֽ���
���ֵ����
����ֵ��SUCCESS��FAILED
**********************************************/
u8 check_empty(u16  word_addr, u8 *buf_Addr, u16 num)
{
	u16  i,buf16;
    for(i = 0; i < (num / 2); i++)
    {
		if(i % 2 == 1){

			__Asm  TBR;

			buf16 = (((u16)ROMD1H << 8) + ROMD1L);
			++word_addr;
			buf_Addr += 2;
		}
		if(i % 2 == 0){
			FRAL = word_addr;
			FRAH = word_addr >> 8;
			FRAHN = ~FRAH;
			FRALN = ~FRAL;
			__Asm  TBR;
			buf16 = (((u16)ROMDH << 8) + ROMDL);
			++word_addr;
			buf_Addr += 2;
		}

        if(buf16 != 0xFFFF)
		{
			return  FAILED;
		}
    }
    return  SUCCESS;
}
/**********************************************
��������Read_Cont
��  ����������Flash
����ֵ��word_addr:Flash��ַ; *buf_Addr:���ݱ����ַ; num:���������ֽ���
���ֵ��Flash�е�����
����ֵ��SUCCESS��FAILED
**********************************************/
u8 Read_Cont(u16  word_addr, u8 *buf_Addr, u16 num)
{
    u16  i;
    for(i = 0; i < (num / 2); i++)
    {
		if(i % 2 == 1){
			__Asm  TBR;
			*(u16 *)buf_Addr = (((u16)ROMD1H << 8) + ROMD1L);
			word_addr++;
			buf_Addr += 2;
		}
		if(i % 2 == 0){
			FRAL = word_addr;
			FRAH = word_addr >> 8;
			FRAHN = ~FRAH;
			FRALN = ~FRAL;
			__Asm  TBR;
			*(u16 *)buf_Addr = (((u16)ROMDH << 8) + ROMDL);
			word_addr++;
			buf_Addr += 2;
		}
    }
	ROMCL=0x00;
    return  SUCCESS;
}
/**********************************************
��������StartIAP
��  ����IAP�̶�����
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void StartIAP(void)	//IAP�̶���ʽ
{
	__asm
	{
			  MOVI 0x55
			  MOVA ROMCH
			  MOVI 0x02        //�ȴ�8��ָ������
			  SSUBI 1          //(A)-1->(A)
			  JBS PSW,Z
			  goto $-2
			  MOVI 0xAA
			  MOVA ROMCH
			  MOVI 0x02        //�ȴ�8��->(ָ������
			  SSUBI 1          //(A)-1A)
			  JBS PSW,Z
			  goto $-2
     		  BSS ROMCL,WR     //Flash�洢��ҳ����/�Ա�̴���λ
			  JBC ROMCL,WR
			  goto $-1         //�ȴ���������
	}
}
/**********************************************
��������Erase_Page
��  ��������һҳFlash
����ֵ��page_addr������ҳ���ڵĵ�ַ
���ֵ����
����ֵ��SUCCESS��FAILED
**********************************************/
u8 Erase_Page(u16  page_addr)
{
	GIE = 0 ;
	DATARDEN = 1;
	FPEE=1;
    FRAH = page_addr >> 8;	//��ַ���ֽ�
	FRAL = page_addr;		//��ַ���ֽ�
	FRAHN = ~FRAH;
	FRALN = ~FRAL;
	WREN=1;
	StartIAP();
	//while(WR);	//�ȴ���������
	ROMCL=0x00;
	return SUCCESS;	
}
/**********************************************
��������Write_SingleWord
��  ������Flash������д����
����ֵ��word_addr��Flash��ʼ��ַ;buf_Addr:Ҫд�����ݵ���ʼ��ַ num:���ݸ���
���ֵ����
����ֵ��SUCCESS��FAILED
**********************************************/
u8 Write_Cont(u16  word_addr, u8 *buf_Addr, u16 num)
{
    u16  i;

	GIE = 0;
	ROMCL=0x80;			
	DATARDEN = 1;
	FPEE=0;

    for(i = 0; i < (num / 2); i++)

    {
		if(i % 2 == 0){
		    FRAH = word_addr >> 8;	//��ַ���ֽ�
			FRAL = word_addr;		//��ַ���ֽ�
			FRAHN = ~FRAH;
			FRALN = ~FRAL;
			ROMDH = (*((u16 *)buf_Addr)) >> 8; //
			ROMDL = (*((u16 *)buf_Addr));	 //д��Flash���ֽ�����
			word_addr++;
			buf_Addr += 2;
		}

		if(i % 2 == 1){
			ROMD1H = (*((u16 *)buf_Addr)) >> 8; //
			ROMD1L = (*((u16 *)buf_Addr));	 //д��Flash���ֽ�����
			WREN=1;
			StartIAP();
			word_addr++;
			buf_Addr += 2;
		}
    }
	ROMCL=0x00;
    return  SUCCESS;
}


