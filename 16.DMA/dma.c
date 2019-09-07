#include "dma.h"
#include "delay.h"

////DMA�����ݴ����贫��������ڴ�
////�����ڽ����ݴ����贫�䵽����



void myDMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channelx,u32 PeripheralBaseAddr,u32 Memory0BaseAddr,u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if ((u32)DMA_Streamx>(u32)DMA2)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
	
	////////����DMA//////////
	DMA_InitStructure.DMA_Channel = Channelx;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         //��ͨģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr; 
	DMA_InitStructure.DMA_Memory0BaseAddr =Memory0BaseAddr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = BufferSize;          //���ݴ�����
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;   //���ݴ������ȼ�
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   //FIFOģʽ
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //FIFO��ֵ
	
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    //�洢������ͻ������ģʽ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�洢�����ݴ�СΪ�ֽ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //�洢������ģʽ
	
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   //���赥��ͻ������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //�������ݴ�СΪ�ֽ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //��������ģʽ
	DMA_Init(DMA_Streamx,&DMA_InitStructure);
}

/////////////����һ��DMA���ݴ���///////////
void myDMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,u16 BufferSize)
{
	DMA_Cmd(DMA_Streamx,DISABLE);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
	DMA_SetCurrDataCounter(DMA_Streamx,BufferSize);     //�������ݴ�����
	DMA_Cmd(DMA_Streamx,ENABLE);
}



