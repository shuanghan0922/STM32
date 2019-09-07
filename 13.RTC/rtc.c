#include "rtc.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


NVIC_InitTypeDef NVIC_InitStructure;

//////////设置RTC时间/////////
////////返回值: ERROR-0   SUCCESS-1
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_TimeStructure.RTC_H12 = ampm;  //am或者pm
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);
}

////////设置日期////////
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
	
	RTC_DateStructure.RTC_Year = year;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Date = date;
	RTC_DateStructure.RTC_WeekDay = week;
	
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);
}

u8 My_RTC_Init()           //RTC初始化
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry = 0x1FFF;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);   //使能外部时钟值
	PWR_BackupAccessCmd(ENABLE);
	
	if (RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5051)    //判断是否为第一次写入
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			retry++;
			delay_ms(10);      //判断超时
		}
		if (retry==0) return 1;   //初始化失败
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    //设置RTC时钟(RTCCLK)为 LSE
		RCC_RTCCLKCmd(ENABLE);    //使能时钟
		
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;   //RTC异步分频系数为0x7F(1~0x7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;    //RTC同步分频系数为0xFF(1~0xFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;  //24小时制
		RTC_Init(&RTC_InitStructure);
		
		RTC_Set_Time(15,22,30,RTC_H12_AM);        //设置为24小时制
		RTC_Set_Date(18,10,28,7);                   //设置日期
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5051);  //设置寄存器值
	}
	return 0;
}

/////////闹钟模式///////
void RTC_Set_AlarmA(u8 hour,u8 min,u8 sec,u8 week)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);   //开启前关闭闹钟A
	
	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week;   //星期几
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;  //按照星期闹
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;    //精确匹配类型:时分秒
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeStructure;   //闹钟时间 
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);   //设置闹钟
	 
	RTC_ClearITPendingBit(RTC_IT_ALRA);     //清除闹钟A的标志
	EXTI_ClearITPendingBit(EXTI_Line7);     //清除Line7上的中断标志位
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);      //开启闹钟A的中断
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);      //开启闹钟A
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //触发方式为 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;     //使能
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;       //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;            //子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_Alarm_IRQHandler()
{
	if (RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		printf ("ALARM A!\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line17);    //清除中断线17标志位
}




