/////////////////////////////////////////////////////////////////////
//  FileMgr.cpp -    Makes call to the FileSystem and DataStore    //
//			    	classes			        					   //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include <iostream>
#include <sstream>
#include <algorithm>

//----< File Mgr Default Constructor >--------------------------------------------------------
FileMgr::FileMgr(){
}

//----< Iterating over patterns to search files and path >-------------------------------------
void FileMgr::search(const std::string& path, patterns patterns_, bool isSearchRecursively){
	for (auto patt : patterns_){
		searchFiles(path, patt, isSearchRecursively);
	}
}

//----< Searching FilePath recursively and return >---------
void  FileMgr::searchFilePath(std::string path, std::string FileName, std::string pattern,
	bool isSearchRecursively,std::string& FilePath){
	std::string fullPathwithFile = FileSystem::Path::getFullFileSpec(path);
	vectorStore tmpFiles = FileSystem::Directory::getFiles(path, pattern);
	for (auto tmpFile : tmpFiles){
		if (tmpFile == FileName){
			std::string filePath = FileSystem::Path::fileSpec(path, tmpFile);
			std::string fullPathwithFile = FileSystem::Path::getFullFileSpec(filePath);
			std::string fullPath = FileSystem::Path::getPath(fullPathwithFile);
			FilePath = fullPathwithFile;
		}
	}
	if (isSearchRecursively){
		vectorStore dirs = FileSystem::Directory::getDirectories(path);
		for (auto dir : dirs){
			if (dir == "." || dir == ".."){
				continue;
			}
			std::string dirPath = FileSystem::Path::fileSpec(path, dir);
			searchFilePath(dirPath, FileName, pattern, isSearchRecursively, FilePath);
		}
	}
}

//----< Searching File and Path recursively and append to msgBody >---------
void FileMgr::searchFilesAddAppendToMsgBody(std::string& msgBody, std::string path,
	std::string pattern, bool isSearchRecursively){
	vectorStore tmpFiles = FileSystem::Directory::getFiles(path, pattern);
	for (auto tmpFile : tmpFiles){
		msgBody += tmpFile;
		msgBody += "@";
	}
	if (isSearchRecursively){
		vectorStore dirs = FileSystem::Directory::getDirectories(path);
		for (auto dir : dirs){
			if (dir == "." || dir == ".."){
				continue;
			}
			std::string dirPath = FileSystem::Path::fileSpec(path, dir);
			searchFilesAddAppendToMsgBody(msgBody, dirPath, pattern, isSearchRecursively);
		}
	}
}

//----< Searching File and Path recursively and transfering to Data Store to save it >---------
void FileMgr::searchFiles(std::string path, std::string pattern, bool isSearchRecursively)
{
	vectorStore tmpFiles = FileSystem::Directory::getFiles(path, pattern);
	for (auto tmpFile : tmpFiles){
		std::string filePath = FileSystem::Path::fileSpec(path, tmpFile);
		std::string fullPathwithFile = FileSystem::Path::getFullFileSpec(filePath);
		std::string fullPath = FileSystem::Path::getPath(fullPathwithFile);
		Datastore_.savingData(tmpFile, fullPath);
	}

	if (isSearchRecursively){
		vectorStore dirs = FileSystem::Directory::getDirectories(path);
		for (auto dir : dirs){
			if (dir == "." || dir == ".."){
				continue;
			}
			std::string dirPath = FileSystem::Path::fileSpec(path, dir);
			searchFiles(dirPath, pattern, isSearchRecursively);
		}
	}
}

//----< Return vector for Displaying Data Store>------------------------------------------
FileMgr::vectorEle FileMgr::printDataStoreMap(){
	return Datastore_.printDataStoreMap();
}

//----< Returning DataStore Reference >---------------------------------------------------
DataStore&  FileMgr::getDataStoreReference(){
	return Datastore_;
}

//----< Returning map Store Size >--------------------------------------------------------
int  FileMgr::dataStoreSize(){
	return Datastore_.DataStoreSize();
}

#ifdef TEST_FILEMGR

//----< Print Header String in Specific Format>-------------------------------------------
void printHeader(std::string header){
	std::cout << "\n";
	std::cout << std::string(75, '#');
	std::cout << "\n" << header;
	std::cout << "\n" << std::string(75, '#');
	std::cout << "\n";
}

//----< Calling FileMgr Main >----------------------------------------------------------------
int main(int argc, char* argv[]){

	try{
		printHeader("Testing FileMgr : ");
		FileMgr fileManagerCase1;
		std::cout << "\n\nShowing Default Case:: ";
		std::cout << "\n****************************************** ";
		std::string path = ".";
		bool isSearchRecursively = false;
		std::vector< std::string> patterns_;
		patterns_.push_back("*.*");
		std::cout << "\n Searching for files and building Datastore:" << std::endl;
		fileManagerCase1.search(path, patterns_, isSearchRecursively);
		std::cout << "Showing FileStore with respected PathsRefernce in DataStoreMAP \n";
		std::vector< ElementDisplay> dataStoreVec = fileManagerCase1.printDataStoreMap();
		for (auto dataStoreVecItr : dataStoreVec){
			std::cout << "\n" << dataStoreVecItr.showFileAndPaths();
		}

		std::cout << "\nShowing Default And Recursive Case:: ";
		std::cout << "\n****************************************** ";
		FileMgr fileManagerCase2;
		path = ".";
		isSearchRecursively = true;
		patterns_.clear();
		patterns_.push_back("*.*");
		std::cout << "\nSearching for files and building Datastore:" << std::endl;
		fileManagerCase2.search(path, patterns_, isSearchRecursively);
		std::cout << "Showing FileStore with respected PathsRefernce in DataStoreMAP \n";
		dataStoreVec = fileManagerCase2.printDataStoreMap();
		for (auto dataStoreVecItr : dataStoreVec){
			std::cout << "\n" << dataStoreVecItr.showFileAndPaths();
		}
		printHeader("Testing FileMgr End: ");
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;

}
#endif