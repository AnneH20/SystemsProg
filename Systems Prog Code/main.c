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

#include "list.h"

int main()
{
	char input[100] = { '\0' };
	
	struct DoublyLinkedList *list = (struct DoublyLinkedList *) malloc(sizeof(struct DoublyLinkedList));
	
	printf("Enter phrase. Type STOP to stop. \n");
    scanf("%s",input);

    while (strcmp(input,"STOP") != 0) 
    {
        append(list, input);
        scanf("%s",input);
    }

	printf("The program will now output in reverse. \n");
	reverse(list);
}
