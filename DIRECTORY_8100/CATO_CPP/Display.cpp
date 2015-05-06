/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display Message Passing Project                  //
//                                                                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Display.h"

using namespace MessagePassing;

//----< Display Constructor >------------------------------------------------
Display::Display(){
	Verbose v(true);
}

//----< Print String >------------------------------------------------
void MessagePassing::Display::printString(std::string str)
{
	Verbose::show("\n" + str, true);
}

//----< Print Header String in Specific Format>------------------------
void MessagePassing::Display::printHeader(std::string header){
	Verbose::show("\n" + std::string(45, '*'),true);
	Verbose::show("\n" + header, true);
	Verbose::show("\n" + std::string(45, '*'), true);
}

//-------< Prints the message   >--------------------------------
void Display::printMessage(Message& msg)
{
	Verbose::show("\n" + std::string(45, '*'), true);
	Verbose::show(msg.constructHeader(), true);
	Verbose::show("Body:" + msg.getMessage(), true);
	Verbose::show("\n" + std::string(45, '*'), true);
}

#ifdef TEST_DISPLAY
//----< Calling Display Main >-------------------------------------------
int main(int argc, char* argv[])
{
	try{
		std::cout << "\n\n ########################## TESTING DISPLAY PACKAGE #################################### \n\n";
		Display disp;
		std::string test = "TESTING DISPLAY ";
		disp.printHeader(test);
		Message messg;
		std::string srcAddr = "127.0.0.1", srcPort = "8484", targetAddr = "127.0.0.1", targetPort = "8181";
		messg.getHeader().setCommand(MSGSEND);
		messg.getHeader().setDstIPAndPort(targetAddr, targetPort);
		messg.setMessage("Hello I am sender");
		disp.printMessage(messg);
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif

/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display Message Passing Project                  //
//                                                                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Display.h"

using namespace Me