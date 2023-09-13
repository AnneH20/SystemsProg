/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Emacs
Program Description:
Write a program that asks the user to input strings
when the user types "STOP" program will output the strings backwards
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include "list.h"
#include <string.h>

void append(DoublyLinkedList *list, char string[100])
{
	if (list != NULL)
	{
		struct Node *tmp = (struct Node *) malloc (sizeof(struct Node));
		tmp->prev = list->tail;
		if (list->head == NULL)
		{
			list->head = tmp;
		}
		else
		{
			list->tail->next = tmp;
		}
		list->tail = tmp;
		
		strcpy(tmp->data, string);
	}
}

/*~DoublyLinkedList()
{
	struct Node* tmp = head;

	while (tmp != NULL)
	{
		head = head->next;
		free(tmp);
		tmp = head;
	}
	tail = NULL;
} */

void reverse(DoublyLinkedList *list)
{
	struct Node *tmp = list->tail;
	while (tmp != NULL)
	{
		printf("%s ", tmp->data);
		tmp = tmp->prev;
	}
	printf("\n");
}

