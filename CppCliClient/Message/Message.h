#ifndef MESSG_H
#define MESSG_H
/////////////////////////////////////////////////////////////////////
//  Message.h - Conatin Message Information                        //
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
This module defines a class that provides services to store and fetch Message Information.

Public Interface:
=================
Message Message;                                               // Message  Construction
std::string str = Message.toString(int i);                     // Conversion int to String
Message.messageInterpret(std::string msg);                     // Interpreting Message and storing to header
std::string str = Message.constructHeader();                   // Constructing Message from header and returning string
std::string str = Message.getMessage();                        // Getting Message for transfer
Message.setMessage(std::string msg);                           // Setting Message for transfer
MsgHeader& MsgHeader = Message.getHeader();                    // Returning Header
void parseString(std::string& str, char start);                // Parses the string on the start char
void setIsrecursively(int value);                              // Set is Recursively
bool getIsRecursively();                                       // Get is Recursively
void setPattern(std::string patt);                             // Set Patterns
std::string appendPatterns();                                  // Append Patterns
std::vector <std::string>& getPattern();                       // Get Patterns
std::string getCatogries();                                    // Get Catogries
void setCatogries(std::string catog);                          // Set Catogries

Build Process:
==============
Required files
- Message.h, Message.cpp,
- MsgHeader.h, MsgHeader.cpp,
- AppHelpers.h, AppHelpers.cpp

Build commands
- devenv Message.vcxproj /debug rebuild

Maintenance History:
====================
ver 2.1 : 28 Apr 2015
Added parseString to Parses the string on the start char
Added setIsrecursively to Set is Recursively
Added getIsRecursively to Get is Recursively
Added setPattern to Set Patterns
Added appendPatterns to Append Patterns
Added getPattern to Get Patterns
Added getCatogries to Get Catogries
Added setCatogries to Set Catogries

ver 1.0 : 11 April 2015
- first release
*/
#include <map>
#include <vector>
#include "../MsgHeader/MsgHeader.h"

namespace MessagePassing
{
	class Message
	{
	public:
		using vector = std::vector < std::string > ;
		std::string toString(int i);                       // Conversion int to String
		Message();                                         // Message Constructor
		void messageInterpret(std::string msg);            // Interpreting Message and storing to header 
		std::string constructHeader();                     // Constructing Message from header and returning string
		std::string getMessage();                          // Getting Message for transfer
		void setMessage(std::string msg);                  // Setting Message for transfer
		MsgHeader& getHeader();                            // Returning Header
		~Message(){}                                       // Destructor       
		void parseString(std::string& str, char start);    // Parses the string on the start char 
		void setIsrecursively(int value);                  // Set is Recursively   
		int getIsRecursively();                            // Get is Recursively 
		void setPattern(std::string patt);                 // Set Patterns
		std::string appendPatterns();                      // Append Patterns
		std::vector <std::string>& getPattern();           // Get Patterns
		std::string getCatogries();                        // Get Catogries  
		void setCatogries(std::string catog);              // Set Catogries
	private:    
		MsgHeader msgHeader_;
		std::string catogries_;
		vector patterns;
		std::string message_;
		int isRecursively;
	};
}
#endif



