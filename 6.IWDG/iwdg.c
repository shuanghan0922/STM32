#include "iwdg.h"



void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG->KR = 0x5555;      //�Ĵ���ʹ��
	IWDG->PR = prer;        //Ԥ��Ƶֵ
	IWDG->RLR = rlr;        //��װ��ֵ
	
	IWDG->KR = 0xAAAA;      //��ʼ��Ϊʹ�ó�ʼֵΪ�趨ֵ  ��Ҫ����װ��
	IWDG->KR = 0xCCCC;      //��ʼ����
}

