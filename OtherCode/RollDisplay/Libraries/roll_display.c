#include "roll_display.h"
#include <string.h>
#include <stdlib.h>

Linkedlist *creatList(int size)       //����һ����ͨ������
{
	int i;
	Linkedlist *head = (Linkedlist*)malloc(sizeof(Linkedlist));
	Linkedlist *temp = head;
	for (i=0;i<size;i++)
	{
		Linkedlist *tail = (Linkedlist*)malloc(sizeof(Linkedlist));  //�ڵ�
		tail->next = NULL;    //ȷ����һ�ε���ʱΪ��
		temp->next = tail;    //ָ����һ���ڵ�
		temp = tail;          //����
	}
	return head;
}

Linkedlist *closeList(Linkedlist *list)				//����ĩβָ��
{
	Linkedlist *temp = list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = list;
	return temp;
}














