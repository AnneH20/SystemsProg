#pragma once
/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Visual Studio
Program Description:
This program compare C++ and C
*/

#include <iostream>
#include <string>
using namespace std;

class Node
{
public:
	string data;
	Node* next;
	Node* prev;

	Node() : data(" "), next(nullptr), prev(nullptr) {}
	Node(string data) : data(data), next(nullptr), prev(nullptr) {}
	Node(string data, Node* next, Node* prev) : data(data), next(next), prev(prev) {}

};

class DoublyLinkedList
{
private:
	Node* head;
	Node* tail;

public:
	DoublyLinkedList() : head(nullptr), tail(nullptr) {}
	~DoublyLinkedList();

	void append(string data);
	void clear();
	void reverse();
};