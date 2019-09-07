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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;       //ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	ADC_DeInit();       //��λADC
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //ʧ��DMA
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;        //��������ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;       //ΪʹƵ����36M����,����84M�ķ�Ƶ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  //�����ӳ�5������
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;      //�ر�����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //���ݶ��뷽ʽΪ�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  //��ֹ�������,ʹ���������
	ADC_InitStructure.ADC_NbrOfConversion = 1;                // ��1��ת���ٹ���������,��ֻת����������1
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;    //�ֱ���Ϊ12dB
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;             //��ɨ��ģʽ
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
}	
//////////////////��ȡADֵ///////////
u16 ADC_GetValue(u8 ch)       //��ȡADCͨ��ֵ
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_480Cycles);  //ADC1,chͨ��,480������,��߲���ʱ�������߾�ȷ��
	
	ADC_SoftwareStartConv(ADC1);          //ʹ��ADC1�����ת������
	
	while (!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));   //�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);       //�������һ��ADC1�������ת�����
}
////////////////////��ȡchͨ����ת��ֵ,ȡtimes��,Ȼ��ƽ��
u16 ADC_GetAverage(u8 ch,u8 times)     //�õ�ĳ��ͨ����������������ƽ��ֵ
{
	u32 temp_val=0;
	u8 t;
	for (t=0;t<times;t++)
	{
		temp_val += ADC_GetValue(ch);
		delay_ms(5);
	}
	return temp_val/times;   //����ȥtimes�β��������ƽ��ֵ
}






