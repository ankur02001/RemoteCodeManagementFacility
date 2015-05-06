/////////////////////////////////////////////////////////////////////
//  Cataloger.h - Makes call to the FileManager which then         //
//               calls to DataStore classes                        //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include "Cataloger.h"
#include <fstream>
#include <iostream>

//----< Cataloger constructor with filMgr Ref>-----------------------
Cataloger::Cataloger(FileMgr& fm) : fileMgr_(fm){
}

//----< Cheching File Already Found >--------------------------------
bool Cataloger::checkFileFound(std::string filename, std::string pattern){
	for (auto mapItr : filesFound_){
		if (mapItr.first == filename && mapItr.second == pattern){
			return true;
		}
	}
	return false;
}

//----< Cheching if to be search all Files Already Found >-----------
bool Cataloger::issearchAllFiles(vectorStore& patterns_){
	bool searchAllFiles = false;
	for (auto patt : patterns_){
		std::size_t foundOptionS = patt.find("*.*");
		if (foundOptionS != std::string::npos){
			searchAllFiles = true;
		}
	}
	return searchAllFiles;
}

//----< Iterating FileStore and Storing it as vector of display element and returning it >----
Cataloger::vectorEle Cataloger::printFileStore(){
	vectorEle tmpVec;
	for (mapDataStoreIterator mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		mapItr != fileMgr_.getDataStoreReference().mapDataStoreEnd(); ++mapItr){
		ElementDisplay ele;
		std::string file = mapItr->first;
		ele.file_ = file;
		pathRef pathRef = mapItr->second;
		for (pathRefIterator pathRefItr = pathRef.begin(); pathRefItr != pathRef.end(); ++pathRefItr){
			std::string path = *(*pathRefItr);
			ele.map_.emplace(file, path);
			ele.path_ = path;
		}
		tmpVec.push_back(ele);
	}
	return tmpVec;
}

//----< Iterating PathStore and Storing it as vector of display element and returning it >----
Cataloger::vectorEle Cataloger::printPathStore(){
	vectorEle tmpVec;
	for (pathIterator pathItr = fileMgr_.getDataStoreReference().pathBegin();
		pathItr != fileMgr_.getDataStoreReference().pathEnd(); ++pathItr){
		ElementDisplay ele;
		ele.path_ = *pathItr;
		tmpVec.push_back(ele);
	}
	return tmpVec;
}

//----< Searching user text along with specifc patterns provided if not then defaut *.* >----
Cataloger::vectorEle Cataloger::searchUserText(vectorStore& patterns_, std::string searchText)
{
	vectorEle tmpVec;
	int dataStoreSize = fileMgr_.dataStoreSize();
	int dataStoreCount = 0;
	filesFound_.clear();

	bool searchAllFiles = issearchAllFiles(patterns_);

	for (mapDataStoreIterator mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		mapItr != fileMgr_.getDataStoreReference().mapDataStoreEnd(); ++mapItr)
	{
		ElementDisplay ele;
		std::string filename = mapItr->first;
		bool fileFound = false;
		std::string fileExt = FileSystem::Path::getExt(filename);
		for (std::vector<std::string>::iterator itr = patterns_.begin(); itr != patterns_.end(); itr++){
			if ((*itr).find(fileExt) != std::string::npos || searchAllFiles){
				if (checkFileFound(filename, *itr)){
					continue;
				}
				pathRef tmpPathRef = mapItr->second;
				pathRefIterator pathRefItr = tmpPathRef.begin();
				std::string filePath = *(*pathRefItr);
				bool fileFound = textSearch(searchText, filePath, filename);
				if (fileFound){
					ele.file_ = filename;
					addPathStore(filename, ele);
					tmpVec.push_back(ele);
					filesFound_.emplace(filename, *itr);
				}
			}
		}
		if ((dataStoreCount < dataStoreSize)){
			dataStoreCount++;
			mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		}
	}
	return tmpVec;
}

//----< Adding Path to Specifc fileName searched >-----------------------------
void Cataloger::addPathStore(std::string fileName, ElementDisplay& ele){

	for (mapDataStoreIterator mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		mapItr != fileMgr_.getDataStoreReference().mapDataStoreEnd(); ++mapItr){
		std::string file = mapItr->first;
		if (file == fileName){
			pathRef pathRef = mapItr->second;
			for (pathRefIterator pathRefItr = pathRef.begin(); pathRefItr != pathRef.end(); ++pathRefItr){
				std::string path = *(*pathRefItr);
				ele.map_.emplace(file, path);
				ele.path_ = path;
			}
		}
	}
}

//----< Searching Number Of Files Found And Directories >------------------
Cataloger::vectorEle Cataloger::displayNumofFileAndDirectories(){
	int numOfFiles = 0;
	int numOfDirectories = 0;
	Cataloger::vectorEle tmpVec;
	// file count
	for (mapDataStoreIterator mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		mapItr != fileMgr_.getDataStoreReference().mapDataStoreEnd(); ++mapItr){
		pathRef pathRef = mapItr->second;
		for (pathRefIterator pathRefItr = pathRef.begin(); pathRefItr != pathRef.end(); ++pathRefItr){
			numOfFiles++;
		}
	}
	// directory count
	for (pathIterator pathItr = fileMgr_.getDataStoreReference().pathBegin();
		pathItr != fileMgr_.getDataStoreReference().pathEnd(); ++pathItr){
		numOfDirectories++;
	}
	ElementDisplay ele;
	ele.numOfFiles = numOfFiles;
	ele.numOfDirectories = numOfDirectories;
	tmpVec.push_back(ele);
	return tmpVec;
}

//----< Searching Duplicate files and Returning as vector of display element >--
Cataloger::vectorEle Cataloger::displayDuplicateFilesAndDirectories(){
	vectorEle tmpVec;
	for (mapDataStoreIterator mapItr = fileMgr_.getDataStoreReference().mapDataStoreBegin();
		mapItr != fileMgr_.getDataStoreReference().mapDataStoreEnd(); ++mapItr){
		bool duplicatePresent = false;
		int countNumPath = 0;
		pathRef tmpPathRef = mapItr->second;
		for (pathRefIterator pathRefItr = tmpPathRef.begin(); pathRefItr != tmpPathRef.end(); ++pathRefItr){
			if (countNumPath >= 1){
				duplicatePresent = true;
			}
			countNumPath++;
		}
		if (duplicatePresent){
			ElementDisplay ele;
			ele.file_ = mapItr->first;
			for (pathRefIterator pathRefItr = tmpPathRef.begin(); pathRefItr != tmpPathRef.end(); ++pathRefItr){
				std::string path = *(*pathRefItr);
				ele.map_.emplace(ele.file_, path);
			}
			tmpVec.push_back(ele);
		}
	}
	return tmpVec;
}

//----< Searching text in file and path provided and Returning true if Found >-----
bool Cataloger::textSearch(const std::string searchtext, const std::string& filePath, const std::string& filename){

	std::string fileSpecification = FileSystem::Path::fileSpec(filePath, filename);
	std::string line;
	std::ifstream inFile(fileSpecification);
	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			if (line.find(searchtext) != std::string::npos)
				return true;
		}
	}
	return false;
}

#ifdef TEST_CATALOGER
#include "../FileMgr/FileMgr.h"
#include "../Display/Display.h"
#include "../ParseCommand/ParseCommand.h"

//----< Print Header String in Specific Format>---------------------------
void printHeader(std::string header){
	std::cout << "\n";
	std::cout << std::string(75, '#');
	std::cout << "\n" << header;
	std::cout << "\n" << std::string(75, '#');
	std::cout << "\n";
}

//----< Calling Cataloger Main >-------------------------------------------
int main(int argc, char* argv[])
{
	try{
		printHeader("Testing Calaoger Package Begin::");
		ParseCmd parser;
		std::cout << "\nParsing Command line arguments: " << std::endl;
		parser.parseCmdLine(argc, argv);
		try{
			FileMgr fileManager;
			std::cout << "Searching for files and building Datastore: " << std::endl;
			fileManager.search(parser.getPath(), parser.getPatterns(), parser.getisSearchRecursively());

		}
		catch (std::exception& ex){
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	printHeader("Testing Calaoger Package End::");
	return 0;
}
#endif
