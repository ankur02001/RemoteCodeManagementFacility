#ifndef MSGHEADE_H
#define MSGHEADE_H
/////////////////////////////////////////////////////////////////////
//  MsgHeader.h - Conatin Message Header Information               //
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
This module defines a class that provides services to store and fetch Message Header Information.

Public Interface:
=================
MsgHeader MsgHeader;                                               // Message Header Construction
MsgHeader.setParameters(std::string name, std::string value);      // Setting Header Parameters
std::string str = MsgHeader.getSrcIP();                            // Getting Source IP
std::string str = MsgHeader.getSrcPort();                          // Getting Source Port
std::string str = MsgHeader.getDstIP();                            // Getting Destination IP
std::string str = MsgHeader.getDstPort();                          // Getting Destination Port
std::string str = MsgHeader.getCommand();                          // Getting Command
std::string str = MsgHeader.getFileName();                         // Getting File Name
std::string str = MsgHeader.getFileType();                         // Getting File Type
std::string str = MsgHeader.getBodylength();                       // Getting Body Length
MsgHeader.setCommand(int cmd);                                     // Setting Command
MsgHeader.setDstIPAndPort(std::string ip,std::string port);        // Setting Destination IP and Destination Port
MsgHeader.setSrcIPAndPort(std::string ip, std::string port);       // Setting Source IP and Source Port
MsgHeader.setFileNameAndType(std::string FileName, int fileType);  // Setting File Name and File Type
MsgHeader.setBodylength(int length);                               // Setting Body Length
MsgHeader.MsgHeader::setBodylength(size_t len);                    // Setting Body Length
std::string str =MsgHeader.MsgHeader::getDstPort2();               // Setting Destination Port Second
std::string toString(int i);                                       // To String Conversion
std::string getTextSearch();                                       // Getting Text Search
std::string	getDisplayCommand();                                   // Getting Display Command
void setDisplayCommand(int cmd);                                   // Setting Display Command
void setTextSearch(std::string str);                               // Setting Text Search

Build Process:
==============
Required files
- MsgHeader.h, MsgHeader.cpp,

Build commands
- devenv MsgHeader.vcxproj /debug rebuild

Maintenance History:
====================
ver 2.1 : 28 April 2015
Added getTextSearch for Getting Text Search
Added getDisplayCommand for Getting Display Command
Added setDisplayCommand for Setting Display Command
Added setTextSearch(std::string str) for Setting Text Search

ver 1.0 : 11 April 2015
- first release
*/

namespace MessagePassing
{
	class MsgHeader
	{
	public:

		MsgHeader();                                                       // Message Header Construction 
		void setParameters(std::string name, std::string value);           // Setting Header Parameters 
		~MsgHeader(){}
		std::string getSrcIP();                                            // Getting Source IP 
		std::string getSrcPort();                                          // Getting Source Port
		std::string getDstIP();                                            // Getting Destination IP 
		std::string getDstPort();                                          // Getting Destination Port
		std::string getCommand();                                          // Getting Command
		std::string getFileName();                                         // Getting File Name 
		std::string getFileType();                                         // Getting File Type
		std::string getBodylength();                                       // Getting Body Length

		void setCommand(int cmd);                                          // Setting Command 
		void setDstIPAndPort(std::string ip, std::string port);            // Setting Destination IP and Destination Port
		void setSrcIPAndPort(std::string ip, std::string port);            // Setting Source IP and Source Port
		void setFileNameAndType(std::string FileName, int fileType);       // Setting File Name and File Type
		void setBodylength(std::string length);                            // Setting Body Length

		void MsgHeader::setBodylength(size_t len);                         // Setting Body Length
		std::string MsgHeader::getDstPort2();                              // Setting Destination Port Second
		std::string getTextSearch();                                       // Getting Text Search
		std::string	getDisplayCommand();                                   // Getting Display Command
		void setDisplayCommand(int cmd);                                   // Setting Display Command
		void setTextSearch(std::string str);                               // Setting Text Search


	private:
		void setParametersCommand(std::string name, std::string value);        // Setting Parameters  Command
		void setParametersDisplayCommand(std::string name, std::string value); //Setting Parameters Display Command
		void setParametersIPAndPort(std::string name, std::string value);      // Setting Parameters IPAndPort 

		std::string toString(int i);                                           // To String Conversion 
		int command_;
		int displayCommand_;
		std::string srcIP;
		std::string srcPort;
		std::string dstIP;
		std::string dstPort;
		std::string fileName;
		std::string distPort2;
		int FileType_;
		std::string bodyLength_;
		std::string textSearch_;
	};
	enum Command { NONE = 0, UploadFile, DownLoadFile,GetFilesList, FileSearch, TextSearch, MSGSEND, Acknowledge, FileEnded, FileRecieved, StopSending }; // ENUM For command
	enum FileType {NOT =0, binary, text };                                                              // ENUM for File Type
}
#endif