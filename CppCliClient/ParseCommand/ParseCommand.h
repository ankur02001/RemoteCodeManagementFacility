#ifndef PARSE_H
#define PARSE_H
/////////////////////////////////////////////////////////////////////
//  ParseCommand.h- Helps in parsing Command line arguments 	   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a class that provides services necessary to parse
a command line arguments
It identifies a set of files by parsing the following command line  arguments:
1. a Path
2. One or more file pattern
3. /s, if present, the entire directory rooted at the path is searched
4. /d, if present, shows duplicate files and path.
5. /f<search Text>, if present, shows search Text in cataloger files.

Public Interface:
=================

ParseCmd pCmd;                                                   // Get ParseCmd Instance
pCmd.parseCmdLine(int argc, char* argv[]);                       // parses command line arguments
bool val = pCmd.parseUserCommand(std::string parseSearchText);   // Parses User command line arguments
bool val = pCmd.getisSearchRecursively();                        // Return bool for is recursive search required
bool val = pCmd.getIsDisplayNumofFileAndDirect();                // Return bool for is Display Num of Files and Direc required
bool val = pCmd.getIsDuplicateSearch();                          // Return bool for is duplicate print required
bool val = pCmd.getIsSearchtext();                               // Return bool for is Search Text required
std::string str = pCmd.getUserInputString();                     // Return Command Input
std::vector< std::string> vec = pCmd.displayParse();             // Display Parse commands
std::vector< std::string> vec = pCmd.getPatterns();              // Return Patterns after cmd line parsing
std::string path = pCmd.getPath();                               // Return Path stored after cmd line parsing
std::string text = pCmd.getSearchtext();                         // Return search text to be searched

Build Process:
==============
Required files
- FileSystem.h, FileSystem.cpp
Build commands
- devenv ParseCommand.vcxproj

Maintenance History:
====================
ver 1.0 : 8 Feb 2015
- first release
*/

#include <iostream>
#include <vector>
#include "../FileSystem/FileSystem.h"

class ParseCmd
{
public:

	using patterns = std::vector < std::string > ;
	using vectorStore = std::vector < std::string > ;

	void parseCmdLine(int argc, char* argv[]);
	bool parseUserCommand(std::string parseSearchText);
	vectorStore displayParse();
	std::string getPath();
	vectorStore getPatterns();
	bool getisSearchRecursively();
	bool getIsDisplayNumofFileAndDirect();
	bool getIsDuplicateSearch();
	bool getIsSearchtext();
	std::string getSearchtext();
	std::string getUserInputString();
	ParseCmd();

private:

	bool isSearchRecursively;
	bool isSearchtext;
	bool isDuplicateSearch;
	bool isDisplayNumofFileAndDirect;

	std::string path_;
	std::string searchText;
	patterns patterns_;
	void userInputCommand(std::string parseSearchText, std::size_t last);
	bool checkOptions(std::string tmpWord);
	void checkingDefaultCase(bool isOptionFDpresent);
	bool userCommandInitial(std::string tmpWord, bool& getfirst);
	std::string userInputString;
};
#endif
