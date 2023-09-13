/*
Student Name: Anne Marie Heidebreicht
Student NetID: alh1310
Compiler Used: Visual Studio
Program Description:
This program compare C++ and C
*/

#include "List.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	DoublyLinkedList list;
	string input = " ";
	cout << "This program will read strings until the command STOP is entered." << endl;
	cout << "Enter phrase. Type STOP to stop." << endl;
	while (input != "STOP")
	{
		cin >> input;
		if (input != "STOP")
		{
			list.append(input);
		}
	}	
	cout << "The program will now output in reverse." << endl;
	list.reverse();
	list.~DoublyLinkedList();
}