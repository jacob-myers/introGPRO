// GPRO-FW-Launcher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "GPRO_FW/proj-header.h"
#include "GPRO_FW/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main()
{
	while (true)
	{
		int input;
		cout << "Enter a number (0 to quit): ";
		cin >> input;

		if (input == 0)
		{
			break;
		}

		else
		{
			cout << input << " is " << oddOrEven(input) << endl;
		}
	}
}
