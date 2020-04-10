// GPRO-FW-Launcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//prototype/declaration
int foo(int bar);

#include <stdio.h>
#include <stdlib.h>

int main()
{
	int test = foo(9000);
	printf("\n %d \n", test);
	system("pause");
}
