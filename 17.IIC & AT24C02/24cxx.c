#include "24cxx.h"
#include "delay.h"

void AT24CXX_Init()
{
	IIC_Init();
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)		//从AT24CXX器件读出一个字节数据
{
	u8 temp;
	IIC_Start();
	if (EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);	//发送高地址
	}
	else 
		IIC_Send_Byte(0xA0+((ReadAddr/256)<<1));	//发送器件地址0xA0,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr%256);	//发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0xA1);			//进入接收模式
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop();				//停时
	return temp;
}

void AT24CXX_WriteOneByte(u16 WriteAddr,u8 WriteData)			//向AT24CXX器件写一个字节数据
{
	IIC_Start();
	if (EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0xA0);
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}
	else 
		IIC_Send_Byte(0xA0+((WriteAddr/256)<<1));
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr%256);		//发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteData);
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(10);
}

void AT24CXX_WriteLenByte(u16 WriteAddr,u32 WriteData,u8 Len)		//向AT24CXX写入指定Len长度的数据
{
	u8 t;
	for (t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(WriteData>>(8*t))&0xFF);
	}
}

u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)						//向AT24CXX读取指定长度的数据
{
	u8 t;
	u32 temp=0;
	for (t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
	return temp;
}

u8 AT24CXX_Check(void)												//检查AT24CXX是否异常
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255);								//读取防止在地址255中的数据,如果有数据则说明无误
	if (temp==0x55)
		return 0;
	else
	{
		AT24CXX_WriteOneByte(255,0x55);
		temp = AT24CXX_ReadOneByte(255);
		if (temp==0x55)
			return 0;
	}
	return 1;
}

void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 ReadNum)				//在AT24CXX器件中从指定的地址开始读出指定个数的数据
{
	while (ReadNum)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		ReadNum--;
	}
}

void ATCXX_Write(u16 WriteAddr,u8 *pBuffer,u16 WriteNum)			//在AT24CXX里面的指定地址开始写入指定个数的数据
{
	while(WriteNum--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}



