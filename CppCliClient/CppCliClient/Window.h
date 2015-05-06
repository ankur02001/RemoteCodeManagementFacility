#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//                                                                       //
//  ver 2.1                                                              //
//  Language:      Visual C++ 2013                                       //
//  Platform:      Dell, Windows 8.1                                     //
//  Application:   Scope analysis, OOD Assignment 4, Spring2015          //
//  Author:		   Ankur Pandey                                          //
// Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp, 
*  Message.cpp, Message.h
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.1 : 28 Apr 2015
* Commented function
*
*  ver 2.1 : 25 Apr 2015
*  Added functinonality for upload download files
* Added functinality for text Search And file Search
*
* ver 2.1 : 22 Apr 2015
* Added Upload View
* Added Text Search View
* Added File Search View
* Added Connection  View
* Added Download View
*
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   
using namespace System::Windows::Controls;              
using namespace System::Windows::Controls::Primitives;   
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;
using namespace System::Collections;


#include "../MockChannel/MockChannel.h"
#include "../Message/Message.h"
#include <iostream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		//////////////////////////////////////////////////////// 
		//Controls for Window
		///////////////////////////////////////////////////////
		DockPanel^ hDockPanel = gcnew DockPanel();      
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();
		TabItem^ hMessageSend = gcnew TabItem();
		TabItem^ hUploadFileTab = gcnew TabItem();
		TabItem^ hDownloadFileTab = gcnew TabItem();
		TabItem^ hTextSearchTab = gcnew TabItem();
		TabItem^ hFileSearchTab = gcnew TabItem();
		TabItem^ hConnections = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		//////////////////////////////////////////////////////// 
		// Uploading Files View
		///////////////////////////////////////////////////////
		Grid^ hUploadFileGrid = gcnew Grid();
		ListBox^ hUpListBox = gcnew ListBox();
		ListBox^ hUpListBoxDisplay = gcnew ListBox();
		TextBlock^ hUpTextBlock = gcnew TextBlock();
		ComboBox^ hCategories = gcnew ComboBox();
		ComboBox^ hisRecursively = gcnew ComboBox();
		Forms::FolderBrowserDialog^ hUpFolderBrowserDialog
			= gcnew Forms::FolderBrowserDialog();
		Button^ hUpFolderBrowseButton = gcnew Button();
		Button^ hUploadButton = gcnew Button();
		bool isRecursively;
		String^ UploadCategory;
		array<String^>^  UploadPatterns;
		int countUploadFile;
		Border^ hUPBorder2 = gcnew Border();
		TextBlock^ hUPisRecTitle = gcnew TextBlock();
		TextBlock^ hUPspaceTitle = gcnew TextBlock();

		//////////////////////////////////////////////////////// 
		// Downloading Files View
		///////////////////////////////////////////////////////
		Grid^ hDownloadFileGrid = gcnew Grid();
		ListBox^ hDownListBox = gcnew ListBox(); 
		TextBlock^ hDownTextBlock = gcnew TextBlock();
		ListBox^ hDownListBoxDisplay = gcnew ListBox();
		ComboBox^ hDownCategories = gcnew ComboBox();
		ComboBox^ hisDownRecursively = gcnew ComboBox();
		Button^ hDownFolderBrowseButton = gcnew Button();
		String^ DownloadCategory;
		bool isRecursivelyDownload;
		int countDownloadFile;
		Button^ hDownButton = gcnew Button();
		array<String^>^  downPatterns; 
		Border^ hDownDownBorder2 = gcnew Border();
		TextBlock^ hisDownRecTitle1 = gcnew TextBlock();
		TextBlock^ hDownspaceTitle1 = gcnew TextBlock();
		TextBlock^ hdowndownCategoryTitle = gcnew TextBlock();


		//////////////////////////////////////////////////////// 
		// Connecction View
		///////////////////////////////////////////////////////
		Grid^ hConnectionGrid = gcnew Grid();
		ListBox^ hListBox = gcnew ListBox(); 
		Grid^ hConnectionBar = gcnew Grid();
		TextBox^ hIP = gcnew TextBox();
		TextBox^ hPort = gcnew TextBox();
		Button^ hConnect = gcnew Button();
		Button^ hDisconnect = gcnew Button();
		StackPanel^ hConnStackPanel0 = gcnew StackPanel();
		Grid^ hGrid2 = gcnew Grid();
		Border^ hConnectionBorder2 = gcnew Border();
		TextBlock^ hConnectspaceTitle = gcnew TextBlock();
		TextBlock^ hConnectspaceTitle2 = gcnew TextBlock();
		TextBlock^ hConnectspaceTitle3 = gcnew TextBlock();
		Border^ hconnBorder1 = gcnew Border();


		//////////////////////////////////////////////////////// 
		// Text Search View
		///////////////////////////////////////////////////////
		Grid^ hTextSearchGrid = gcnew Grid();
		ListBox^ hTSListBox = gcnew ListBox(); 
		TextBox^ hTSTextBox = gcnew TextBox(); 
		TextBox^ hTSTargetPorts = gcnew TextBox(); 
		Button^ hTSButton = gcnew Button();
		String^ TextToSearch; 
		Button^ hTSPattAddButton = gcnew Button();
		Button^ hTSPattDelButton = gcnew Button();
		ListBox^ hTSPatternDisplay = gcnew ListBox();
		ListBox^ TSPEndPointCatogry = gcnew ListBox();
		ComboBox^ TSPatternRecursively = gcnew ComboBox();
		TextBox^ hTSPattBox = gcnew TextBox();
		TextBlock^ hTSDirTitlee = gcnew TextBlock();
		Border^ hTSBorder1 = gcnew Border();
		StackPanel^ hFTStackPanel01 = gcnew StackPanel();
		TextBlock^ hTSCategoryTitle = gcnew TextBlock();
		TextBlock^ hisTSRecTitle1 = gcnew TextBlock();
		TextBlock^ hTSTextTextTitle = gcnew TextBlock();
		TextBlock^ hisTSPatternTextTitle = gcnew TextBlock();
		Border^ hTSDownBorder3 = gcnew Border();
		Border^ hTSDownBorder2 = gcnew Border();
		int countTextSearch;

		//////////////////////////////////////////////////////// 
		// File Search View
		///////////////////////////////////////////////////////
		Grid^ hFileSearchGrid = gcnew Grid();
		ListBox^ hFSListBox = gcnew ListBox(); 
		TextBox^ hFSPattBox = gcnew TextBox(); 
		Button^ hFSButton = gcnew Button();
		Button^ hFSPattAddButton = gcnew Button();
		Button^ hFSPattDelButton = gcnew Button();
		bool isFSRecursively;
		ListBox^ hFileSearchPatternDisplay = gcnew ListBox();
		ComboBox^ FileSearchPatternCatogries = gcnew ComboBox();
		ComboBox^ FileSearchPatternRecursively = gcnew ComboBox();
		Button^ hFileFolderBrowseButton = gcnew Button();
		String^ FileSearchCategory;
		array<String^>^  FileSearchPatterns;  
		array<String^>^  CattPatterns;
		Border^ hFSBorder1 = gcnew Border();
		TextBlock^ hFSCategoryTitle = gcnew TextBlock();
		TextBlock^ hisFSRecTitle1 = gcnew TextBlock();
		TextBlock^ hisFSPatternTextTitle = gcnew TextBlock();
		TextBlock^ hFSspaceTitle1 = gcnew TextBlock();
		TextBlock^ hFSspaceTitle3 = gcnew TextBlock();
		TextBlock^ hFSspaceTitle2 = gcnew TextBlock();
		//////////////////////////////////////////////////////// 
		//MockChannel references
		///////////////////////////////////////////////////////
		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;
		//////////////////////////////////////////////////////// 
		// Recvr Thread
		///////////////////////////////////////////////////////
		Thread^ recvThread;
		//////////////////////////////////////////////////////// 
		// Port Coonection 
		///////////////////////////////////////////////////////
		String^ srcIP;
		String^ srcPort;
		String^ dstIP;
		String^ dstPort;

	public:
		WPFCppCliDemo();                           // WPFCppCliDemo Constructor 
		~WPFCppCliDemo();                          // WPFCppCliDemo destructor
		void setUpFileSearchView();                // setting up the File Searcch view
		void setUpTextSearchView();                // setting up the Text Search view
		void setUpUploadFileView();                // setting up the Uploadfile  view
		void setDownDownloadFileView();            // setting up Download view
		void setUpConnectionView();                // setting up Coonection view
		void setUpStatusBar();                     // setting up status bar at the bottom to provide status of current operation
		void setUpTabControl();                    // setting up Tab Control

		void addUploadedText(String^ msg);                                            // Displaying Files Uploaded
		void getMessage();                                                            // Get Message from Reciever
		bool checkCommandNULL(std::string cmd);                                       // Checking Command null
		std::string toStdString(String^ pStr);                                        // Convet System String to Std STring
		void clear(Object^ sender, RoutedEventArgs^ args);                            // Clear Status bar
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);                          // Loading Window
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args); // Unloding Window
		void WPFCppCliDemo::EventHandler();                                                           //  WPFCppCliDemo EventHandler 
		void WPFCppCliDemo::basicStartUpSetting();                                                    // WPFCppCliDemo Basic Start up Constructor
		void WPFCppCliDemo::setFSpattern(MessagePassing::Message& msg);                               // Setting File Search Pattern
		void WPFCppCliDemo::setUpUploadFileViewHelper();                                              // setting up the Uploadfile tab view
		void WPFCppCliDemo::setDownDownloadFileViewHelper();                                          // Setting for Download view
		void WPFCppCliDemo::setUpTextSearchViewHelper();                                              // Setting for Text Search view
		void WPFCppCliDemo::setUpConnectionViewHelper();                                              // Setting for Conncction view
		void WPFCppCliDemo::setUpFileSearchViewHelper();                                              // Setting for File Search view
		void WPFCppCliDemo::setUpTextSearchViewAddingButton();                                        // Setting for Text search buttons
		void WPFCppCliDemo::setTSpattern(MessagePassing::Message& msg);                               // Setting for Text search pattern
		void WPFCppCliDemo::AddEndPoint();                                                            // Adding End Point
		void WPFCppCliDemo::searchUPLoadFiles(String^ path, String^ pattern, bool isSearchRecursively); // Searching UploadFiles
		void WPFCppCliDemo::addToTextSearchList(String^ msg);                                           // Display text search 
		bool WPFCppCliDemo::listen();                                                                   // listen to port
		void WPFCppCliDemo::createConnection(Object^ sender, RoutedEventArgs^ args);                    // Create connection with mock channel
	
		void WPFCppCliDemo::DownloadDropDownRecursivelyLoad(Object^ sender, RoutedEventArgs^ args);     // Added Recursively load for Download
		void WPFCppCliDemo::DownloadDropDownLRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args);  // Add Recursively selection Download
		void WPFCppCliDemo::UploadDropDownRecursivelyLoad(Object^ sender, RoutedEventArgs^ args);                    // Add Recursively load for upload
		void WPFCppCliDemo::UploadDropDownLRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args);    // Add REcursivley Selection for Download
		void UploadDDListLoaded(Object^ sender, RoutedEventArgs^ args);                                     // EventHandler for Category Selection for Upload 
		void UploadDropDownList(Object^ sender, SelectionChangedEventArgs^ args);                           //  EventHandler for Category Selection for Upload 
		void UpBrowseForFolder(Object^ sender, RoutedEventArgs^ args);                                      // browse folder for upload
		void UpLoad(Object^ sender, RoutedEventArgs^ args);                                                 // upload files to server
		void DownloadDDListLoaded(Object^ sender, RoutedEventArgs^ args);                                   // EventHandler for Category Load for Download 
		void DownloadDropDownList(Object^ sender, SelectionChangedEventArgs^ args);                         // EventHandler for Category Selection for Download 
		void DownBrowseForFiles(Object^ sender, RoutedEventArgs^ args);                                     // Getting List of files to download
		void DownLoad(Object^ sender, RoutedEventArgs^ args);                                               // Downloading files
		void destroyConnection(Object^ sender, RoutedEventArgs^ args);                                      // Destroy Connection
		void WPFCppCliDemo::addDownloadText(String^ msg);                                                   // Display Download files
		void WPFCppCliDemo::addGetFilesListText(String^ msg);                                               // Display downloadable files list
		void WPFCppCliDemo::FileSearchAddPattern(Object^ sender, RoutedEventArgs^ args);                    // Add Pattern for File Search
		void WPFCppCliDemo::FileSearchDelePattern(Object^ sender, RoutedEventArgs^ args);                   // Del Pattern for File Search
		void WPFCppCliDemo::SearchFiles(Object^ sender, RoutedEventArgs^ args);                             // Search files from server
		void WPFCppCliDemo::FileSearchPatternCatogriesLoad(Object^ sender, RoutedEventArgs^ args);          // Pattern Catogry load for File Search
		void WPFCppCliDemo::FileSearchPatternCatogriesSelection(Object^ sender, SelectionChangedEventArgs^ args);  // Pattern Category select for File Search
		void WPFCppCliDemo::FileSearchPatternRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args); // Recursive Selection for File Search
		void WPFCppCliDemo::FileSearchPatternRecursiveLoad(Object^ sender, RoutedEventArgs^ args);                  // Recursive Load for FIle Search
		void WPFCppCliDemo::addToFileSearchList(String^ msg);                                                       // Display File Search list
		void WPFCppCliDemo::TSAddPattern(Object^ sender, RoutedEventArgs^ args);                                    // Add Pattern to Text Search
		void WPFCppCliDemo::TSDelePattern(Object^ sender, RoutedEventArgs^ args);                                   // Del Pattern from Text Search
		void WPFCppCliDemo::SearchText(Object^ sender, RoutedEventArgs^ args);                                      // Search text from Server
	private:
		String^ toSystemString(std::string& str);
		bool checkConnection;
		size_t listenport;
		size_t targetport;
	};
}


#endif
