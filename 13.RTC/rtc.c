#include "rtc.h"
#include "led.h"
#include "delay.h"
#include "usart.h"


NVIC_InitTypeDef NVIC_InitStructure;

//////////����RTCʱ��/////////
////////����ֵ: ERROR-0   SUCCESS-1
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_TimeStructure.RTC_H12 = ampm;  //am����pm
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);
}

////////��������////////
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	RTC_DateTypeDef RTC_DateStructure;
	
	RTC_DateStructure.RTC_Year = year;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Date = date;
	RTC_DateStructure.RTC_WeekDay = week;
	
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);
}

u8 My_RTC_Init()           //RTC��ʼ��
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry = 0x1FFF;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);   //ʹ���ⲿʱ��ֵ
	PWR_BackupAccessCmd(ENABLE);
	
	if (RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5051)    //�ж��Ƿ�Ϊ��һ��д��
	{
		RCC_LSEConfig(RCC_LSE_ON);
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			retry++;
			delay_ms(10);      //�жϳ�ʱ
		}
		if (retry==0) return 1;   //��ʼ��ʧ��
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    //����RTCʱ��(RTCCLK)Ϊ LSE
		RCC_RTCCLKCmd(ENABLE);    //ʹ��ʱ��
		
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;   //RTC�첽��Ƶϵ��Ϊ0x7F(1~0x7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;    //RTCͬ����Ƶϵ��Ϊ0xFF(1~0xFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;  //24Сʱ��
		RTC_Init(&RTC_InitStructure);
		
		RTC_Set_Time(15,22,30,RTC_H12_AM);        //����Ϊ24Сʱ��
		RTC_Set_Date(18,10,28,7);                   //��������
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5051);  //���üĴ���ֵ
	}
	return 0;
}

/////////����ģʽ///////
void RTC_Set_AlarmA(u8 hour,u8 min,u8 sec,u8 week)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);   //����ǰ�ر�����A
	
	RTC_TimeStructure.RTC_Hours = hour;
	RTC_TimeStructure.RTC_Minutes = min;
	RTC_TimeStructure.RTC_Seconds = sec;
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = week;   //���ڼ�
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;  //����������
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_None;    //��ȷƥ������:ʱ����
	RTC_AlarmStructure.RTC_AlarmTime = RTC_TimeStructure;   //����ʱ�� 
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStructure);   //��������
	 
	RTC_ClearITPendingBit(RTC_IT_ALRA);     //�������A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line7);     //���Line7�ϵ��жϱ�־λ
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);      //��������A���ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);      //��������A
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //������ʽΪ �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;     //ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;       //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;            //�����ȼ�
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
	EXTI_ClearITPendingBit(EXTI_Line17);    //����ж���17��־λ
}




