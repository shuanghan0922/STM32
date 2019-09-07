#ifndef _DMA_H
#define _DMA_H
#include "sys.h"

void myDMA_Init(DMA_Stream_TypeDef *DMA_Streamx,u32 Channelx,u32 PeripheralBaseAddr,u32 Memory0BaseAddr,u16 BufferSize);
void myDMA_ENABLE(DMA_Stream_TypeDef *DMA_Streamx,u16 BufferSize);


#endif
