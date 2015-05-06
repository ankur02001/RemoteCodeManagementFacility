/////////////////////////////////////////////////////////////////////
//  ParseCommand.cpp - Helps in parsing Command line arguments 	   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include "ParseCommand.h"
#include <string>

//----< parses command line arguments Constructor>--------------------
ParseCmd::ParseCmd(){
	isSearchRecursively = false;
	isSearchtext = false;
	isDuplicateSearch = false;
	isDisplayNumofFileAndDirect = false;
}

// ----< Parses User command line arguments>--------------------------
bool ParseCmd::parseUserCommand(std::string parseSearchText){
	userInputString = parseSearchText;
	isDuplicateSearch = false;
	isDisplayNumofFileAndDirect = false;
	isSearchtext = false;
	patterns_.clear();
	searchText = "";
	bool getfirst = true;
	if (parseSearchText[0] != '\"'){
		return false;
	}
	std::size_t found = parseSearchText.find_first_of("\"");
	if (found == std::string::npos){
		return false;
	}
	std::size_t first = parseSearchText.find("\"");
	std::size_t last = parseSearchText.find_last_of("\"");
	if (last == std::string::npos || (first == 0 && last == 0) ){
		return false;
	}
	searchText = parseSearchText.substr(first + 1, last - (first + 1));
	if (searchText.size() == 0){
		return false;
	}
	isSearchtext = true;
	last++;
	patterns_.push_back("*.*");
	userInputCommand(parseSearchText, last);
	return true;
}

// ----< user Input Command stoing pattern >------------------------------------------
void ParseCmd::userInputCommand(std::string parseSearchText, std::size_t last){
	while (parseSearchText[last] != '\0'){
		std::string tmpWord = "";
		while (parseSearchText[last] != ' ' && parseSearchText[last] != '\0'){
			tmpWord += parseSearchText[last];
			last++;
		}
		if (tmpWord != ""){
			if (patterns_.size() == 1 && patterns_[0] == "*.*"){
				patterns_.pop_back();
				patterns_.push_back(tmpWord);
			}
			else{
				patterns_.push_back(tmpWord);
			}
		}
		last++;
	}
}

// ----< Return Command Input >---------------------------------------------------
std::string ParseCmd::getUserInputString(){
	return userInputString;
}

// ----< Restricting option check for user command after cat build up>------------
bool  ParseCmd::checkOptions(std::string tmpWord){
	std::size_t foundOptionS = tmpWord.find("/s");
	if (foundOptionS != std::string::npos){
		return true;
	}
	std::size_t foundOptiond = tmpWord.find("/d");
	if (foundOptiond != std::string::npos){
		return true;
	}
	return false;
}

// ----< parses command line arguments>--------------------------------
void ParseCmd::parseCmdLine(int argc, char* argv[])
{
	userInputString = "";
	bool isOptionFDpresent = true;
	for (int i = 1; i < argc; i++){
		std::string tmpArg = argv[i];
		userInputString += tmpArg;
		userInputString += " ";
		std::size_t found = tmpArg.find("/f");
		if (found != std::string::npos){
			isSearchtext = true;
			isOptionFDpresent = false;
			unsigned first = tmpArg.find_first_of("\"");
			unsigned last = tmpArg.find_last_of("\"");
			searchText = tmpArg.substr(first + 3, last - (first + 1));
			continue;
		}
		if (tmpArg == "/s"){
			isSearchRecursively = true;
		}
		else if (tmpArg == "/d"){
			isDuplicateSearch = true;
			isOptionFDpresent = false;
		}
		else if (path_.empty()){
			path_ = FileSystem::Path::getFullFileSpec(argv[i]);
		}
		else{
			patterns_.push_back(tmpArg);
		}
	}
	checkingDefaultCase(isOptionFDpresent);
}

// ----< Checking Default cases >--------------------------------------
void ParseCmd::checkingDefaultCase(bool isOptionFDpresent){
	if (path_.empty()){
		path_ = FileSystem::Path::getFullFileSpec(".");  // default path
	}
	if (patterns_.empty()){
		patterns_.push_back("*.*"); // default pattern
	}
	if (isOptionFDpresent){
		isDisplayNumofFileAndDirect = true;
	}
}

// ----< Return bool for is Display Num of Files and Direc required >----
bool ParseCmd::getIsDisplayNumofFileAndDirect(){
	return isDisplayNumofFileAndDirect;
}

// ----< Return Path stored after cmd line parsing >---------------------
std::string ParseCmd::getPath(){
	return path_;
}

// ----< Return Patterns after cmd line parsing >-------------------------
ParseCmd::vectorStore ParseCmd::getPatterns(){
	return patterns_;
}

// ----< Return bool for is recursive search required >-------------------
bool ParseCmd::getisSearchRecursively(){
	return isSearchRecursively;
}

// ----< Return bool for is duplicate print required >--------------------
bool ParseCmd::getIsDuplicateSearch(){
	return isDuplicateSearch;
}

// ----< Return bool for is Search Text required >------------------------
bool ParseCmd::getIsSearchtext(){
	return isSearchtext;
}

// ----< Return search text to be searched >-------------------------------
std::string ParseCmd::getSearchtext(){
	return searchText;
}

// ----< Display Parse commands >------------------------------------------
ParseCmd::vectorStore ParseCmd::displayParse()
{
	ParseCmd::vectorStore tmpVec;
	tmpVec.push_back("Path = " + path_);
	tmpVec.push_back("isSearchRecursively = " + isSearchRecursively);
	tmpVec.push_back("isSearchtext = " + isSearchtext);
	tmpVec.push_back("isDisplayNumofFileAndDirect = " + isDisplayNumofFileAndDirect);
	tmpVec.push_back("isDuplicateSearch = " + isDuplicateSearch);
	tmpVec.push_back("searchText = " + searchText);
	tmpVec.push_back("Patterns = ");
	for (auto vec : patterns_){
		std::string vecstr = vec.c_str();
		tmpVec.push_back(" " + vecstr);
	}
	return tmpVec;
}

#ifdef TEST_PARSE
//----< Print Header String in Specific Format>---------------------------
void printHeader(std::string header){
	std::cout << "\n";
	std::cout << std::string(75, '#');
	std::cout << "\n" << header;
	std::cout << "\n" << std::string(75, '#');
	std::cout << "\n";
}

//----< Calling Parse main >----------------------------------------------
int main(int argc, char* argv[]){

	try{
		printHeader("Testing ParseCmd::::");
		std::cout << "\n\nShowing Default Case:: ";
		std::cout << "\n****************************************** ";
		ParseCmd parseCase1;
		int argc = 0;
		char *argv[] = { "" };
		parseCase1.parseCmdLine(argc, argv);
		std::cout << "\nDisplay ParseCmds \n";
		std::vector< std::string> parseVec = parseCase1.displayParse();
		for (auto vec : parseVec){
			std::cout << "\n" << vec.c_str();
		}

		std::cout << "\n\nShowing Some Case:: ";
		std::cout << "\n****************************************** ";
		ParseCmd parseCase2;
		int argc2 = 5;
		char *argv2[] = { "D:\\test", "*.h", "*.txt", "/s", "/d" };
		parseCase2.parseCmdLine(argc2, argv2);
		std::cout << "\nDisplay ParseCmds \n";
		std::vector< std::string> parseVec2 = parseCase2.displayParse();
		for (auto vec : parseVec2){
			std::cout << "\n" << vec.c_str();
		}

		std::cout << "\n\nShowing All Cases:: ";
		std::cout << "\n****************************************** ";
		ParseCmd parseCase3;
		int argc3 = 6;
		char *argv3[] = { "D:\\test", "*.h", "*.txt", "/s", "/d", "/f<I m working on OOD Project 1>" };
		parseCase3.parseCmdLine(argc3, argv3);
		std::cout << "\nDisplay ParseCmds \n";
		std::vector< std::string> parseVec3 = parseCase3.displayParse();
		for (auto vec : parseVec3){
			std::cout << "\n" << vec.c_str();
		}
		do{
			ParseCmd parseCase4;
			std::string userInputString;
			std::getline(std::cin, userInputString);
			std::cout << userInputString << std::endl;
			userInputString += " ";

			std::cout << "\nParsing User Command line arguments : \n" << std::endl;
			parseCase4.parseUserCommand(userInputString);
			std::cout << "\nDisplay ParseCmds \n";
			std::vector< std::string> parseVec3 = parseCase3.displayParse();
			for (auto vec : parseVec3){
				std::cout << "\n" << vec.c_str();
			}

		} while (true);
		printHeader("Testing ParseCmd End::");
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif