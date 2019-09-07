#include "timer.h"

#define Falling 0     //下降沿测量低电平
#define Rising 1      //上升沿测量高电平


void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStruture;                 //Input Capture设置
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	//////////复用GPIOA_0////////////////
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//////////////TimeBase设置///////////////
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //时钟
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数器模式
	TIM_TimeBaseInitStructure.TIM_Period = arr;                       //自动重装载初值 (period-周期)
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                    //时钟预分频
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);                //初始化TIM3
		

	///////////初始化TIM5输入捕获参数//////////////
	TIM_ICInitStruture.TIM_Channel = TIM_Channel_1;       //选择通道1 IC1映射到TI1上
	TIM_ICInitStruture.TIM_ICFilter = 0x00;              //滤波器为0 不滤波  
	TIM_ICInitStruture.TIM_ICPolarity = TIM_ICPolarity_Rising;      //上升沿触发
	TIM_ICInitStruture.TIM_ICPrescaler = TIM_ICPSC_DIV1;          //分频器不分频
	TIM_ICInitStruture.TIM_ICSelection = TIM_ICSelection_DirectTI;  //直接映射  映射到TI1
	TIM_ICInit(TIM5,&TIM_ICInitStruture);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);             //开启TIM3更新中断和CC1捕获中断
	TIM_Cmd(TIM5,ENABLE);	
	//////////////开启中断/////////////
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

/*////////TIM5_CH1_Cap_STA////////
[7]: 0,没有成功的捕获 1,成功捕获到1次
[6]: 0,没有捕获到变化沿 1,捕获到变化沿
[5:0]: 捕获到的定时器溢出次数   (32位计数器,1us计数器加1,溢出时间:4294s)
*/
u8 TIM5_CH1_Cap_STA=0;            //输入捕获状态
u32 TIM5_CH1_Cap_VAL;             //输入捕获值 (TIM5为32位计数器)

void Input_Capture(u8 string)
{
	if (string==Falling)
	{
		if ((TIM5_CH1_Cap_STA & 0x80)==0)      //未完成捕获周期
		{
			if (TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)     //定时器溢出标志事件
			{
				if (TIM5_CH1_Cap_STA & 0x40)       //捕获到变化沿
				{
					if ((TIM5_CH1_Cap_STA & 0x3F)==0x3F)   //捕获到的电平持续过长.超过数据范围
					{
						TIM5_CH1_Cap_STA |= 0x80;      //过长,停止对当前状态的检测
						TIM5_CH1_Cap_VAL = 0xFFFFFFFF; //VAL为最大值
					}
					else
						TIM5_CH1_Cap_STA++;  //定时器溢出加1
				}
			}			
			else if (TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)        //Input Capture1捕获事件发生
			{
				if (TIM5_CH1_Cap_STA & 0x40)          //已经捕获到一个变化沿
				{
					TIM5_CH1_Cap_STA |= 0x80;        //标记捕获周期结束
					TIM5_CH1_Cap_VAL = TIM5->CCR1;   //获取CCR1当前的定时器值
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //设置为初始变化沿
				}
				else                //第一次发生捕获变化沿
				{
					TIM5_CH1_Cap_STA = 0;        //清空标志位
					TIM5_CH1_Cap_VAL = 0;        //清空总计时器
					TIM5_CH1_Cap_STA |= 0x40;    //标记捕获到一次变化沿
					TIM_Cmd(TIM5,ENABLE);
					TIM5->CNT = 0;               //设置计数器为0
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);   //设置为相反极性来检测整个变化周期
					TIM_Cmd(TIM5,ENABLE);
				}
			}			
		}
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);        //清除捕获标志和溢出中断
	}
	else if (string==Rising)
	{
		if ((TIM5_CH1_Cap_STA & 0x80)==0)      //未完成捕获周期
		{
			if (TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET)     //定时器溢出标志事件
			{
				if (TIM5_CH1_Cap_STA & 0x40)       //捕获到变化沿
				{
					if ((TIM5_CH1_Cap_STA & 0x3F)==0x3F)   //捕获到的电平持续过长.超过数据范围
					{
						TIM5_CH1_Cap_STA |= 0x80;      //过长,停止对当前状态的检测
						TIM5_CH1_Cap_VAL = 0xFFFFFFFF; //VAL为最大值
					}
					else
						TIM5_CH1_Cap_STA++;  //定时器溢出加1
				}
			}			
			else if (TIM_GetITStatus(TIM5,TIM_IT_CC1) != RESET)        //Input Capture1捕获事件发生
			{
				if (TIM5_CH1_Cap_STA & 0x40)          //已经捕获到一个变化沿
				{
					TIM5_CH1_Cap_STA |= 0x80;        //标记捕获周期结束
					TIM5_CH1_Cap_VAL = TIM5->CCR1;   //获取CCR1当前的定时器值
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);   //设置为初始变化沿
				}
				else                //第一次发生捕获变化沿
				{
					TIM5_CH1_Cap_STA = 0;        //清空标志位
					TIM5_CH1_Cap_VAL = 0;        //清空总计时器
					TIM5_CH1_Cap_STA |= 0x40;    //标记捕获到一次变化沿
					TIM_Cmd(TIM5,ENABLE);
					TIM5->CNT = 0;               //设置计数器为0
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //设置为相反极性来检测整个变化周期
					TIM_Cmd(TIM5,ENABLE);
				}
			}			
		}
		TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);        //清除捕获标志和溢出中断
	}
}


void TIM5_IRQHandler()
{
	Input_Capture(Rising);
}





