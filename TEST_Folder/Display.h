#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////
//  Display.h - Display Message Passing Project                    //
//                                                                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services necessary to display Message Passing Project.

Public Interface:
=================
Display display;                          // To get Display Instance
display.printMessage(Message& msg);       // Print Message
display.printHeader(std::string header);  // Print Header String in Specific Format
display.printString(std::string str);     // Print String

Build Process:
==============
Required files
- Display.h, AppHelpers.h
- Display.cpp, AppHelpers.cpp
- Message.h, Message.cpp

Build commands
- devenv Display.sln

Maintenance History:
====================
ver 1.0 : 11 Apr 2015
- first release
*/

#include<iostream>
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Message/Message.h"
#include <string>

using namespace ApplicationHelpers;

namespace MessagePassing
{
	class Display
	{
	public:
		Display();
		void printMessage(Message& msg);       // Print Message
		void printHeader(std::string header);  // Print Header String in Specific Format
		void printString(std::string str);     // Print String

	private:
	};
}
#endif



