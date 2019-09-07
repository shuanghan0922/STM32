#include "dma.h"
#include "delay.h"

////DMA把数据从外设传到定义的内存
////可用于将数据从外设传输到数组



void myDMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channelx,u32 PeripheralBaseAddr,u32 Memory0BaseAddr,u16 BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if ((u32)DMA_Streamx>(u32)DMA2)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
	
	////////配置DMA//////////
	DMA_InitStructure.DMA_Channel = Channelx;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;         //普通模式
	DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr; 
	DMA_InitStructure.DMA_Memory0BaseAddr =Memory0BaseAddr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = BufferSize;          //数据传输量
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;   //数据传输优先级
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   //FIFO模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; //FIFO阈值
	
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    //存储器单次突发传输模式
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //存储器数据大小为字节
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //存储器增量模式
	
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   //外设单次突发传输模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   //外设数据大小为字节
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //外设增量模式
	DMA_Init(DMA_Streamx,&DMA_InitStructure);
}

/////////////开启一次DMA数据传输///////////
void myDMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,u16 BufferSize)
{
	DMA_Cmd(DMA_Streamx,DISABLE);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
	DMA_SetCurrDataCounter(DMA_Streamx,BufferSize);     //设置数据传输量
	DMA_Cmd(DMA_Streamx,ENABLE);
}



