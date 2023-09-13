/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Visual Studio
Program Description:
This program compare C++ and C
*/

#include "List.h"
#include <iostream>


// Append - Adds to the bottom of the list
void DoublyLinkedList::append(string data)
{
	Node* tmp = new Node(data);
	tmp->prev = tail;
	if (head == nullptr)
	{
		head = tmp;
	}
	else
	{
		tail->next = tmp;
	}
	tail = tmp;
}


DoublyLinkedList::~DoublyLinkedList()
{
	Node* tmp = head;

	while (tmp != nullptr)
	{
		head = head->next;
		delete tmp;
		tmp = head;
	}
	tail = nullptr;
}

void DoublyLinkedList::clear()
{
	Node* tmp = head;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}


void DoublyLinkedList::reverse()
{
	while (tail->next != nullptr)
	{
		tail = tail->next;
	}
	while (tail != head) {
		cout << tail->data << " ";
		tail = tail->prev;
	}
	cout << tail->data << endl;
}