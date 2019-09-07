#include "adc.h"
#include "delay.h"


void ADC1_Init()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;       //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	ADC_DeInit();       //复位ADC
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //失能DMA
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;        //单独采样模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;       //为使频率再36M以下,所以84M四分频
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  //采样延迟5个周期
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;      //关闭连续转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //数据对齐方式为右对齐
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  //禁止触发检测,使用软件触发
	ADC_InitStructure.ADC_NbrOfConversion = 1;                // 有1个转换再规则序列中,即只转换规则序列1
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;    //分辨率为12dB
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;             //非扫描模式
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
}	
//////////////////获取AD值///////////
u16 ADC_GetValue(u8 ch)       //获取ADC通道值
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_480Cycles);  //ADC1,ch通道,480个周期,提高采样时间可以提高精确度
	
	ADC_SoftwareStartConv(ADC1);          //使能ADC1的软件转换功能
	
	while (!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //等待转换结束
	
	return ADC_GetConversionValue(ADC1);       //返回最后一次ADC1规则组的转换结果
}
////////////////////获取ch通道的转换值,取times次,然后平均
u16 ADC_GetAverage(u8 ch,u8 times)     //得到某个通道给定次数采样的平均值
{
	u32 temp_val=0;
	u8 t;
	for (t=0;t<times;t++)
	{
		temp_val += ADC_GetValue(ch);
		delay_ms(5);
	}
	return temp_val/times;   //返回去times次采样结果的平均值
}






