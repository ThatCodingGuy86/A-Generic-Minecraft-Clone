#pragma once

#include <windows.h>
#include <iostream>
#include <string>

void err(std::string str)
{
	if (PLATFORM_NAME == "Windows")
	{
		HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(c, FOREGROUND_RED);

		std::cout << str;

		// I *think* this is the default? Maybe?
		SetConsoleTextAttribute(c, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
	}

}

