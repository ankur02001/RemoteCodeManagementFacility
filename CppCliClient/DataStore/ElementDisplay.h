#ifndef ELEMENT_H
#define ELEMENT_H
/////////////////////////////////////////////////////////////////////
//  ElementDisplay.h - Stores Display Information which is         //
//                     Project Specific                            //
//					                							   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 1, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a struct ElementDisplay that provides services necessary
to pass values as vectors which will be used to display project Specific requirement.

Public Interface:
=================
//Display Files with multiple locations Path
std::string showFileAndPaths();

//Display Path
std::string showPath();

//Display File
std::string showFiles();

//Display File No Of Files and Directories Found
std::string NumberOfFilesAndDirectories();

// file_ = stores filename
// path_ = stores pathname
// map_  = stores file as key and path as values
// numOfFiles = stores No Of Files Found
// numOfDirectories = Stores No of Directories Found

Maintenance History:
====================
ver 1.0 : 8 Feb 2015
- first release
*/

#include <string>
#include <sstream>
#include <map>

struct ElementDisplay
{
	using file = std::string;
	using path = std::string;
	using multimapStore = std::multimap < file, path > ;
	using multimapStoreIterator = std::multimap< file, path>::iterator;

	file file_;
	path path_;
	multimapStore map_;

	unsigned numOfFiles;
	unsigned numOfDirectories;
	

	//----< Display Files with multiple locations Path>------------
	std::string showFileAndPaths()
	{
		std::ostringstream temp;
		temp << "\n File => ";
		temp << file_;
		temp << "\n Path => \n";
		for (multimapStoreIterator pathRefItr = map_.begin(); pathRefItr != map_.end(); ++pathRefItr){
			temp << "         ";
			temp << pathRefItr->second;
			temp << "\n";
		}
		return temp.str();
	}

	std::string getFileAndPaths()
	{
		std::ostringstream temp;
		temp << "@FILE Name@" << file_ << "@Found PATHs@";
		for (multimapStoreIterator pathRefItr = map_.begin(); pathRefItr != map_.end(); ++pathRefItr){
			temp << pathRefItr->second << "@";
		}
		temp << "@";
		return temp.str();
	}

	//----< Display Path>-------------------------------------------
	std::string showPath(){
		std::ostringstream temp;
		temp << "\n => ";
		temp << path_;
		return temp.str();
	}

	//----< Display File>-------------------------------------------
	std::string showFiles()
	{
		std::ostringstream temp;
		temp << "\n  => ";
		temp << file_;
		return temp.str();
	}

	//----< Display File No Of Files and Directories Found>---------
	std::string NumberOfFilesAndDirectories()
	{
		std::ostringstream temp;
		temp << " => ";
		temp << numOfFiles;
		temp << " files find in ";
		temp << numOfDirectories;
		temp << " directories";
		return temp.str();
	}
};
#endif