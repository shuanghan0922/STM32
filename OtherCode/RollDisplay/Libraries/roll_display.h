#ifndef _ROLL_DISPLAY_H
#define _ROLL_DISPLAY_H
#include <sys.h>
#include <string.h>
#include <stdlib.h>

struct List         //½Úµã
{
	uint8_t Data[20];
	struct List *next;
};

typedef struct List Linkedlist;

Linkedlist *creatList(int size);
Linkedlist *closeList(Linkedlist *list);




#endif

