#ifndef SERVEREXECUTIVE_H
#define SERVEREXECUTIVE_H
/////////////////////////////////////////////////////////////////////
//  ExecutiveServer.h - Execute Server side                        //
//                                                                 //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services to provide application for server side

Public Interface:
=================
ExecutiveServer(std::size_t srcport, std::string targetport, Socket::IpVer ipv);  // Constuctor for executive server side
~ExecutiveServer();                                                               // Destructor
void stop();                                                                      // stops the sender
void start();                                                                     // Starts the executive server side
void run();                                                                       // Run Server
bool checkCommandNULL(std::string cmd);                                           // Check Command is Null or not
ExecutiveServer::~ExecutiveServer();                                              // ExecutiveServer Destructor
void passMessage(Message message);                                                // Pass Message
Message getMessage();                                                             // Get Message

Build Process :
== == == == == == ==
Required files
- Sockets.h, Sockets.cpp,
-Receiver.h, Receiver.cpp,
-AppHelpers.h, AppHelpers.cpp,
-BlockingQueue.h, BlockingQueue.cpp,
-Display.h, Display.cpp,
-Message.h, Message.cpp
- Dispatcher.h, Dispatcher.cpp

Build commands
- devenv ExecutiveServer.vcxproj / debug rebuild

Maintenance History :
== == == == == == == == == ==
ver 2.1 : 28 April 2015
Added checkCommandNULL(std::string cmd) for Checking Command is Null or not

ver 1.0 : 11 April 2015
- first release
*/
#include <iostream>
#include <string>
#include "../Message/Message.h"
#include "../Sockets/Sockets.h"
#include "../Display/Display.h"
#include "../Reciever/Reciever.h"
#include "../Sender/Sender.h"
#include "../MockChannel/Cpp11-BlockingQueue.h"

using namespace std;
namespace RemoteCodeManagementFacility
{
	class ExecutiveServer
	{
	public:
		using BQueue = BlockingQueue < Message >;
		ExecutiveServer::ExecutiveServer(size_t srcport, 
			std::string targetport, Socket::IpVer ipv); // Executive Constructor
		void start();                                   // Starts the receiver and sender
		void stop();                                    // signal server thread to stop 
		void run();                                     // Run Server 
		bool checkCommandNULL(std::string cmd);         // Check Command is Null or not 
		ExecutiveServer::~ExecutiveServer();            // ExecutiveServer Destructor
		void passMessage(Message message);              // Pass Message
		Message getMessage();                           // Get Message
	private:
		Display display;
		SocketSystem* ss;
		std::thread* thread_;
		Sender* pSendr_;
		Receiver* pRecvr_;
		bool stop_;
		Message msg_;
		std::string sourceIP;
		size_t sourcePort;
	};
}
#endif
