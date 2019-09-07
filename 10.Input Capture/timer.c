#include "timer.h"

#define Falling 0     //�½��ز����͵�ƽ
#define Rising 1      //�����ز����ߵ�ƽ


void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStruture;                 //Input Capture����
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	//////////����GPIOA_0////////////////
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//////////////TimeBase����///////////////
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //ʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ�����ģʽ
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //�Զ���װ�س�ֵ (period-����)
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //ʱ��Ԥ��Ƶ
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);                //��ʼ��TIM3
		

	///////////��ʼ��TIM5���벶�����//////////////
	TIM_ICInitStruture.TIM_Channel = TIM_Channel_1;       //ѡ��ͨ��1 IC1ӳ�䵽TI1��
	TIM_ICInitStruture.TIM_ICFilter = 0x00;              //�˲���Ϊ0 ���˲�  
	TIM_ICInitStruture.TIM_ICPolarity = TIM_ICPolarity_Rising;      //�����ش���
	TIM_ICInitStruture.TIM_ICPrescaler = TIM_ICPSC_DIV1;          //��Ƶ������Ƶ
	TIM_ICInitStruture.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ֱ��ӳ��  ӳ�䵽TI1
	TIM_ICInit(TIM5,&TIM_ICInitStruture);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);             //����TIM3�����жϺ�CC1�����ж�
	TIM_Cmd(TIM5,ENABLE);	
	//////////////�����ж�/////////////
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*////////TIM5_CH1_Cap_STA////////
[7]: 0,û�гɹ��Ĳ��� 1,�ɹ�����1��
[6]: 0,û�в��񵽱仯�� 1,���񵽱仯��
[5:0]: ���񵽵Ķ�ʱ���������   (32λ������,1us��������1,���ʱ��:4294s)
*/
u8 TIM5_CH1_Cap_STA=0;            //���벶��״̬
u32 TIM5_CH1_Cap_VAL;             //���벶��ֵ (TIM5Ϊ32λ������)

void Input_Capture(u8 string)
{
	if (string==Falling)
	{
		if ((TIM5_CH1_Cap_STA & 0x80)==0)      //δ��ɲ�������
		{
			if (TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)     //��ʱ�������־�¼�
			{
				if (TIM5_CH1_Cap_STA & 0x40)       //���񵽱仯��
				{
					if ((TIM5_CH1_Cap_STA & 0x3F)==0x3F)   //���񵽵ĵ�ƽ��������.�������ݷ�Χ
					{
						TIM5_CH1_Cap_STA |= 0x80;      //����,ֹͣ�Ե�ǰ״̬�ļ��
						TIM5_CH1_Cap_VAL = 0xFFFFFFFF; //VALΪ���ֵ
					}
					else
						TIM5_CH1_Cap_STA++;  //��ʱ�������1
				}
			}			
			else if (TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)        //Input Capture1�����¼�����
			{
				if (TIM5_CH1_Cap_STA & 0x40)          //�Ѿ�����һ���仯��
				{
					TIM5_CH1_Cap_STA |= 0x80;        //��ǲ������ڽ���
					TIM5_CH1_Cap_VAL = TIM5->CCR1;   //��ȡCCR1��ǰ�Ķ�ʱ��ֵ
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //����Ϊ��ʼ�仯��
				}
				else                //��һ�η�������仯��
				{
					TIM5_CH1_Cap_STA = 0;        //��ձ�־λ
					TIM5_CH1_Cap_VAL = 0;        //����ܼ�ʱ��
					TIM5_CH1_Cap_STA |= 0x40;    //��ǲ���һ�α仯��
					TIM_Cmd(TIM5,ENABLE);
					TIM5->CNT = 0;               //���ü�����Ϊ0
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);   //����Ϊ�෴��������������仯����
					TIM_Cmd(TIM5,ENABLE);
				}
			}			
		}
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);        //��������־������ж�
	}
	else if (string==Rising)
	{
		if ((TIM5_CH1_Cap_STA & 0x80)==0)      //δ��ɲ�������
		{
			if (TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)     //��ʱ�������־�¼�
			{
				if (TIM5_CH1_Cap_STA & 0x40)       //���񵽱仯��
				{
					if ((TIM5_CH1_Cap_STA & 0x3F)==0x3F)   //���񵽵ĵ�ƽ��������.�������ݷ�Χ
					{
						TIM5_CH1_Cap_STA |= 0x80;      //����,ֹͣ�Ե�ǰ״̬�ļ��
						TIM5_CH1_Cap_VAL = 0xFFFFFFFF; //VALΪ���ֵ
					}
					else
						TIM5_CH1_Cap_STA++;  //��ʱ�������1
				}
			}			
			else if (TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)        //Input Capture1�����¼�����
			{
				if (TIM5_CH1_Cap_STA & 0x40)          //�Ѿ�����һ���仯��
				{
					TIM5_CH1_Cap_STA |= 0x80;        //��ǲ������ڽ���
					TIM5_CH1_Cap_VAL = TIM5->CCR1;   //��ȡCCR1��ǰ�Ķ�ʱ��ֵ
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);   //����Ϊ��ʼ�仯��
				}
				else                //��һ�η�������仯��
				{
					TIM5_CH1_Cap_STA = 0;        //��ձ�־λ
					TIM5_CH1_Cap_VAL = 0;        //����ܼ�ʱ��
					TIM5_CH1_Cap_STA |= 0x40;    //��ǲ���һ�α仯��
					TIM_Cmd(TIM5,ENABLE);
					TIM5->CNT = 0;               //���ü�����Ϊ0
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //����Ϊ�෴��������������仯����
					TIM_Cmd(TIM5,ENABLE);
				}
			}			
		}
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);        //��������־������ж�
	}
}


void TIM5_IRQHandler()
{
	Input_Capture(Rising);
}





