#include  <hic.h>
#include  "typedef.h"
#include "rw_flash.h"

isp_data_t g_isp_data;   //ͨ��������ͨ��״̬���Ĵ���
extern u32  g_boot_timer;
extern u32  g_frame_timer;

/***************************************************************
��������start_tx
��  ��: ����UART����
����ֵ�����͵��ֽ���
���ֵ����
����ֵ���� 
***************************************************************/
void start_tx(u8 len)
{	
	g_isp_data.tx_cnt = 0;
 	g_isp_data.tx_len = len; 
	TX0B = g_isp_data.tx_buf[0];
	while(1)
	{
		g_isp_data.tx_cnt++;
		if(g_isp_data.tx_cnt > g_isp_data.tx_len)    //������ɣ�������һ������
		{
			while(!TX0IF);
			g_isp_data.triger = 1;      //����״̬��������һ��
			g_isp_data.rx_cnt=0;
			break;
		}
		else
		{
			while(!TX0IF);
			TX0B = g_isp_data.tx_buf[g_isp_data.tx_cnt];
		}
	}
}
/***************************************************************
��������UartReceive
��  ��: UART��������
����ֵ����
���ֵ����
����ֵ���� 
***************************************************************/
void UartReceive(void)
{
	volatile u8   buf8;
	while(1)				//���û�н��յ�����ֱ���˳�
	{	
		if(RX0IF)
		{
			buf8 = RX0B;
			g_boot_timer = Boot_OVERTIME_2;
			if(g_isp_data.syn)  //���ͬ���ɹ�
			{
				if((buf8 == SYN) &&(g_isp_data.step == WAIT_REV_INS))
				{
					g_isp_data.tx_buf[0] = ACK;
					start_tx(0);
					g_isp_data.triger=0;
					break;			
				}
				else
				{					
				
					if(g_isp_data.step == WAIT_REV_INS)
					{
						g_isp_data.rx_len = 1;
						g_isp_data.rx_cnt = 0;
						g_isp_data.step = IN_REV_INS;  //���ڽ���ָ����		
					}
					g_isp_data.rx_buf[g_isp_data.rx_cnt] = buf8;
					g_isp_data.rx_cnt++;
					if(g_isp_data.rx_cnt == 2 && g_isp_data.step == WAIT_REV_PARA1 && g_isp_data.cmd == EX_ERASE)
					{
						g_isp_data.rx_len = (g_isp_data.rx_buf[1]+2) * 2;
					}
					if(g_isp_data.rx_cnt == 1 && g_isp_data.step == WAIT_REV_PARA2 && g_isp_data.cmd == WRITE_M)
					{
						g_isp_data.rx_len = g_isp_data.rx_buf[0] + 2;
					}
					if(g_isp_data.rx_cnt > g_isp_data.rx_len)    //������ɣ�������һ������
					{
							
						g_isp_data.triger = 1;  //����״̬��������һ��
						break;
					}
					
				}
			}
			else if(buf8 == SYN)  //������յ���������ͬ���ɹ�
			{
				g_isp_data.tx_buf[0] = ACK;
				g_isp_data.syn = 1;  //ͬ�����			
				g_isp_data.step = WAIT_REV_INS;
				g_isp_data.triger=0;
				start_tx(0);
				break;	
			}
		}
		else
		{
			break;
		}
	}
}