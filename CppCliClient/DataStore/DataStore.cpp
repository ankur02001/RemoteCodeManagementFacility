/////////////////////////////////////////////////////////////////////
//  DataStore.cpp - Stores Catalog Data in map and set             //
//					                  							   //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include"DataStore.h"
#include<iostream>
#include <sstream>
#include <string>

//----< Data Store Default Constructor >-------------------------------
DataStore::DataStore(){
}

//----< Return Path Store Begin >--------------------------------------
DataStore::pathIterator DataStore::pathBegin(){ return setPathStore_.begin(); }

//----< Return Path Store End >----------------------------------------
DataStore::pathIterator DataStore::pathEnd(){ return setPathStore_.end(); }

//----< Return Map Store Begin >----------------------------------------
DataStore::mapDataStoreIterator DataStore::mapDataStoreBegin(){ return mapDataStore_.begin(); }

//----< Return Map Store End >------------------------------------------
DataStore::mapDataStoreIterator DataStore::mapDataStoreEnd(){ return mapDataStore_.end(); }

//----< Saving Path in path Store And file and path ref in map Store >--
void DataStore::savingData(const std::string& filename, const std::string& path){
	if (!isPathExist(path)){
		setPathStore_.insert(path);
	}
	if (!isFileExist(filename)){
		pathIterator tempPathRef = getPathRefernce(path);
		pathList tempList;
		tempList.push_back(tempPathRef);
		mapDataStore_.emplace(filename, tempList);
	}
	else{
		pathIterator tempPathRef = getPathRefernce(path);
		pathList tempPathList = mapDataStore_.find(filename)->second;
		mapDataStore_.erase(filename);
		tempPathList.push_back(tempPathRef);
		mapDataStore_.emplace(filename, tempPathList);
	}
}

//----< Return Path Exist or Not >----------------------------------------
bool DataStore::isPathExist(const std::string& path){
	bool is_in = setPathStore_.find(path) != setPathStore_.end();
	if (is_in){
		return true;
	}
	return false;
}

//----< Return File Exist or Not >----------------------------------------
bool DataStore::isFileExist(const std::string& filename){
	bool is_in = mapDataStore_.find(filename) != mapDataStore_.end();
	if (is_in){
		return true;
	}
	return false;
}

//----< Return Path Reference >-------------------------------------------
DataStore::pathIterator DataStore::getPathRefernce(const std::string& path){
	for (pathIterator pathItr = pathBegin(); pathItr != pathEnd(); ++pathItr){
		if (*pathItr == path){ return pathItr; }
	}
	pathIterator pathtmp;
	return pathtmp;
}

//----< Return vector for Displaying Data Store>---------------------------
DataStore::retVector DataStore::printDataStoreMap(){
	DataStore::retVector tmpVec;
	for (mapDataStoreIterator mapItr = mapDataStore_.begin(); mapItr != mapDataStore_.end(); ++mapItr){
		ElementDisplay ele;
		ele.file_ = mapItr->first;
		pathList tmpListOfPath = mapItr->second;
		for (pathListIterator pathRefItr = tmpListOfPath.begin(); pathRefItr != tmpListOfPath.end(); ++pathRefItr){
			std::string path = *(*pathRefItr);
			ele.map_.emplace(ele.file_, path);
		}
		tmpVec.push_back(ele);
	}
	return tmpVec;
}

//----< Returning map Store Size >-----------------------------------------
int DataStore::DataStoreSize(){
	return mapDataStore_.size();
}

//----< Return vector for Displaying Path Store>---------------------------
DataStore::retVector DataStore::printPathStoreMap(){
	DataStore::retVector tmpVec;
	for (pathIterator pathItr = setPathStore_.begin(); pathItr != setPathStore_.end(); ++pathItr){
		ElementDisplay ele;
		std::string path = *pathItr;
		ele.path_ = path;
		tmpVec.push_back(ele);
	}
	return tmpVec;
}

#ifdef TEST_DATASTORE

//----< Print Header String in Specific Format>---------------------------
void printHeader(std::string header){
	std::cout << "\n";
	std::cout << std::string(75, '#');
	std::cout << "\n" << header;
	std::cout << "\n" << std::string(75, '#');
	std::cout << "\n";
}

//----< Calling DataStore Main >-------------------------------------------
int main(int argc, char* argv[])
{
	try{
		printHeader("Testing Datastore : ");
		DataStore ds;

		std::cout << "\nSaving File one.txt in C:/TextDir ";
		std::cout << "\nSaving File one.txt in E:/CppFiles ";
		std::cout << "\nSaving File two.h in D:/HeaderFileDir/HeaderFiles ";
		std::cout << "\nSaving File three.cppt in E:/CppFiles ";
		std::cout << "\nSaving File two.h in C:/TextDir ";
		std::cout << "\nSaving File two.h in E:/CppFiles \n";

		ds.savingData("one.txt", "C:/TextDir");
		ds.savingData("one.txt", "E:/CppFiles");
		ds.savingData("two.h", "D:/HeaderFileDir/HeaderFiles");
		ds.savingData("three.cpp", "E:/CppFiles");
		ds.savingData("two.h", "C:/TextDir");
		ds.savingData("two.h", "E:/CppFiles");

		std::cout << "\nShowing PathStore at Single time only \n";
		std::vector<ElementDisplay> pathVec = ds.printPathStoreMap();
		for (auto pathVecItr : pathVec){
			std::cout << pathVecItr.showPath();
		}
		std::cout << "\n\nShowing FileStore with respected PathsRefernce in DataStoreMAP \n";
		std::vector<ElementDisplay> dataStoreVec = ds.printDataStoreMap();
		for (auto dataStoreVecItr : dataStoreVec){
			std::cout << dataStoreVecItr.showFileAndPaths();
		}
		printHeader("Testing Datastore End:: ");
	}
	catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif