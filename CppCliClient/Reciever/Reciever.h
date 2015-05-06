#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////
//  Reciever.h -  Recieves Information end point in blocking call  //
//                And send back to client                          //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services to  receives, interprets 
and handles appropriately incoming messages, be it a file or a plain text message.
If a file message is received, the header indicates the length of the body and 
then the bytes are stored in the buffer. Once the end of file of received the 
entire buffer is written together at once thus preventing data to be garbled from multiple
clients sending the same file.

Public Interface:  Class Receiver:
=================
	Receiver(int port, Socket::IpVer ipv, Dispatcher& dispatcher);         // Reciever constructor
	void connect(int port);                                                // connect to the port
	virtual ~Receiver();                                                   // Destructor
	void sendMessage(Message msg);                                         // Enqueue the messages to the blocking queue
	BQueue& queue();                                                       // Returning Reciever Queue Reference

Public Interface:  Class ClientHandler:
=================
-Listens and creates new thread for processing request for each new incoming socket requests on the receiving port
    ClientHandler(BlockingQueue<Message>& q, Dispatcher& dispatcher);       // Client Handler constructor  
	void operator()(Socket& socket_);                                       // Client Handler thread starts running this
	size_t ClientHandler::stringToInt(std::string str);                     // string to int
	void writeToFile(MessageId id, Message messg, char* byte);              // Writes the file to destination directory
	void processMessage(Socket& socket, Message& msg);                      // Process the messsages of acknowledgement and End of File
	char* socketBufferHandler(Socket& socket);                              // Reads data from the Socket
	MessageId getMessageIdentifier(Message&);                               // Generate Identifier of file
	vectorStore getCatogryPattern(Message& message);                        // Process Get Catogry Pattern
	void processTextSearch(Socket& socket_, Message& message);              // Process text Search
	void processFileSearch(Socket& socket_, Message& message);              // Process File Search
	void processGetFilesSearch(Socket& socket_, Message& message);          // Process Get File Search
	void processDownload(Socket& socket_, Message& message);                // Process Download Search
	void processFileEnd(Socket& socket_, Message& message);                 // Process End of File
	std::string getCatogryPath(Message& message);                           // Process Get Catogry Path
	void processAcknowledge(Socket& socket_, Message& message);             // Process Acknowledge
	std::string getCatogryPathFromExt(Message& message);                      // Get Catogry path form Extension of file

Build Process :
== == == == == == ==
Required files
- Sockets.h, Sockets.cpp,
-Receiver.h, Receiver.cpp,
-AppHelpers.h, AppHelpers.cpp,
-BlockingQueue.h, BlockingQueue.cpp,
-Display.h, Display.cpp,
-Message.h, Message.cpp
-Dispatcher.h, Dispatcher.cpp

Build commands
- devenv Receiver.vcxproj / debug rebuild

Maintenance History :
== == == == == == == == == ==
ver 2.1 : 28 April 2015
Added queue() for Returning Reciever Queue Reference
Added getCatogryPattern() to Process Get Catogry Pattern
Added processTextSearch() to Process text Search
Added processFileSearch() to Process File Search
Added processGetFilesSearch() to Process Get File Search
Added processDownload() to Process Download Search
Added processFileEnd() to Process End of File
Added getCatogryPath() to Process Get Catogry Path
Added processAcknowledge() to Process Acknowledge

ver 1.0 : 11 April 2015
- first release
*/
#include "../sockets/sockets.h"
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include "../FileSystem/FileSystem.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Message/Message.h"
#include "../Display/Display.h"
#include "../MockChannel/MockChannel.h"
#include "../MockChannel/Cpp11-BlockingQueue.h"
#include "../Cataloger/Cataloger.h"
#include "../FileMgr/FileMgr.h"
using namespace MessagePassing;

namespace RemoteCodeManagementFacility
{
	/////////////////////////////////////////////////////////////////////////////
	// Server's client handler class
	// - must be callable object so we've built as a functor
	// - passed to SocketListener::start(CallObject& co)
	// - Client handling thread starts by calling operator()
	////////////////////////////////////////////////////////////////////////////
	class ClientHandler  
	{
	public:
		using file = std::string;
		using path = std::string;
		using multimapStore = std::multimap < file, path >;
		using multimapStoreIterator = std::multimap< file, path>::iterator;
		using BQueue = BlockingQueue < Message >;
		using vectorStore = std::vector < std::string > ;

		vectorStore getCatogryPattern(Message& message);                        // Process Get Catogry Pattern
		void processTextSearch(Socket& socket_, Message& message);              // Process text Search 
		void processFileSearch(Socket& socket_, Message& message);              // Process File Search
		void processGetFilesSearch(Socket& socket_, Message& message);          // Process Get File Search
		void processDownload(Socket& socket_, Message& message);                // Process Download Search
		void processFileEnd(Socket& socket_, Message& message);                 // Process End of File
		std::string getCatogryPath(Message& message);                           // Process Get Catogry Path
		void processAcknowledge(Socket& socket_, Message& message);             // Process Acknowledge    
		ClientHandler(BlockingQueue<Message>& q);                               // Client Handler constructor  
		void operator()(Socket& socket_);                                       // Client Handler thread starts running this
		size_t ClientHandler::stringToInt(std::string str);                     // string to int
		void writeToFile( Message messg, char* byte);                           // Writes the file to destination directory
		void processMessage(Socket& socket, Message& msg);                      // Process the messsages of acknowledgement and End of File
		char* socketBufferHandler(Socket& socket);                              // Reads data from the Socket
		int checkCommandNULL(std::string cmd);                                  // Checking Command is None or not
		std::string getCatogryPathFromExt(Message& message);                      // Get Catogry path form Extension of file 
	private:
		Display disp;
		std::string filePath;
		BQueue& bq_;
		size_t bufferSize_;
	};

	////////////////////////////////////////////////////////////////////////
	// Receiver
	// Creates new ClientHandler and listen to define socket
	////////////////////////////////////////////////////////////////////////
	class Receiver : public IRecvr
	{
	public:
		using BQueue = BlockingQueue < Message >;
		Receiver();
		Receiver(int port, Socket::IpVer ipv = Socket::IpVer::IP4); // Reciever constructor
		void connect();                                             // connect to the port
		virtual ~Receiver();                                        // Destructor
		void passMessage(Message msg);                              // Enqueue the messages to the blocking queue
		Message getMessage();                                       // Connectting to the port
		BQueue& queue();                                            // Returning Reciever Queue Reference
		void listen(int port);                                      // New listener
		void setPort(size_t port);                                  // setting port
	private:
		SocketListener* listener_;
		int port_;
		BQueue queue_;
		ClientHandler client_; 
	};
}
#endif



