#include  <hic.h>
#include  "typedef.h"
extern isp_data_t g_isp_data;
u32  g_boot_timer;   //��ʱ���������ʱ��
u32  g_frame_timer;  //֡�����ʱ��ʱ��
u8   g_boot;  //���������ʶ

/**********************************************
��������Timer_Proc
��  ������ʱ��������
����ֵ����
���ֵ����
����ֵ����
**********************************************/
void  Timer_Proc(void)
{
    static u16 t1s_tmp = 0 ;
    if(T8NIF == 1)   //ÿ4ms����һ��
    {
        T8NIF = 0;
        T8N = 130;
        if(PA0 == 0)  //BootPIN����
		{
			if(g_boot_timer != 0)
			{
				g_boot_timer--;
				if(g_boot_timer == 0)
				{
					g_boot = 1;  //��������
				}
			}
		}
		if(g_frame_timer != 0) //֡�ֽڼ��ʱ�䳬ʱ��λ����ʹ�������½���ȴ��������״̬
		{
			g_frame_timer--;
			if(g_frame_timer == 0)
			{
				g_isp_data.step = WAIT_REV_INS;
				//g_isp_data.syn = 0;
				//Init_Uart();
			}
		}
    }
}