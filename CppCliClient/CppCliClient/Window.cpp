///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//                                                                       //
//  ver 2.1                                                              //
//  Language:      Visual C++ 2013                                       //
//  Platform:      Dell, Windows 8.1                                     //
//  Application:   Scope analysis, OOD Assignment 4, Spring2015          //
//  Author:		   Ankur Pandey                                          //
// Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;
#include <string>
#include <sstream>

//--------------< WPFCppCliDemo Constructor >--------------------
WPFCppCliDemo::WPFCppCliDemo()
{
	checkConnection = false;
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	delete pObjFact;
	srcIP = "localhost";
	srcPort = "8100";
	dstIP = "localhost";
	dstPort = "8080";
	hIP->Text = "localhost";
	hPort->Text = "8080";
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();
	EventHandler();
	basicStartUpSetting();
}

//--------------< WPFCppCliDemo Constructor >--------------------
void WPFCppCliDemo::basicStartUpSetting(){
	// setup Window size
	this->Title = "WPF C++/CLI Demo";
	this->Width = 1200;
	this->Height = 600;
	// setup dockPanel
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);
	// setup Window controls and views
	setUpTabControl();
	setUpUploadFileView();
	setDownDownloadFileView();
	setUpStatusBar();
	setUpConnectionView();
	setUpTextSearchView();
	setUpFileSearchView();
}

//--------------< WPFCppCliDemo EventHandler >--------------------
void WPFCppCliDemo::EventHandler(){
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	hConnect->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::createConnection);
	hDisconnect->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::destroyConnection);

	hUpFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UpBrowseForFolder);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UpLoad);
	hCategories->SelectionChanged += gcnew SelectionChangedEventHandler(this, &WPFCppCliDemo::UploadDropDownList);
	hCategories->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadDDListLoaded);
	hisRecursively->SelectionChanged += gcnew SelectionChangedEventHandler(this,
		&WPFCppCliDemo::UploadDropDownLRecursivelySelection);
	hisRecursively->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadDropDownRecursivelyLoad);

	hDownFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownBrowseForFiles);
	hDownCategories->SelectionChanged += gcnew SelectionChangedEventHandler(this, &WPFCppCliDemo::DownloadDropDownList);
	hDownCategories->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadDDListLoaded);
	hDownButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownLoad);
	hisDownRecursively->SelectionChanged += gcnew SelectionChangedEventHandler(this,
		&WPFCppCliDemo::DownloadDropDownLRecursivelySelection);
	hisDownRecursively->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadDropDownRecursivelyLoad);

	hFSButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchFiles);
	FileSearchPatternCatogries->SelectionChanged += gcnew SelectionChangedEventHandler(this, &WPFCppCliDemo::FileSearchPatternCatogriesSelection);
	FileSearchPatternCatogries->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileSearchPatternCatogriesLoad);
	hFSPattAddButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileSearchAddPattern);
	hFSPattDelButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileSearchDelePattern);
	FileSearchPatternRecursively->SelectionChanged += gcnew SelectionChangedEventHandler(this,
		&WPFCppCliDemo::FileSearchPatternRecursivelySelection);
	FileSearchPatternRecursively->Loaded += gcnew RoutedEventHandler(this, &WPFCppCliDemo::FileSearchPatternRecursiveLoad);

	hTSButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SearchText);
	hTSPattAddButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::TSAddPattern);
	hTSPattDelButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::TSDelePattern);
}

//--------------< WPFCppCliDemo destructor >--------------------
WPFCppCliDemo::~WPFCppCliDemo(){
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}

//----< setting up status bar at the bottom to provide status of current operation> -----
void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "Status";
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 4, 10, 4);
}

//----< setting up Tab Control>-----------------------------------------------------------
void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hGrid->SetRow(hTabControl, 0);
	hConnections->Header = "CONNECTION";
	hUploadFileTab->Header = "UPLOAD-FILES";
	hDownloadFileTab->Header = "DOWNLOAD-FILES";
	hTextSearchTab->Header = "TEXT-SEARCH";
	hFileSearchTab->Header = "FILE-SEARCH";
	hTabControl->Items->Add(hConnections);
	hTabControl->Items->Add(hUploadFileTab);
	hTabControl->Items->Add(hDownloadFileTab);
	hTabControl->Items->Add(hTextSearchTab);
	hTabControl->Items->Add(hFileSearchTab);
}

//----< setting up the Uploadfile tab view >-----------------------------------------------
void WPFCppCliDemo::setUpUploadFileViewHelper(){
	hUploadFileTab->Content = hUploadFileGrid;
	hUploadFileGrid->Margin = Thickness(20);
	hUpFolderBrowseButton->Content = "Select Directory";
	hUpFolderBrowseButton->Height = 30;
	hUpFolderBrowseButton->Width = 220;
	hUpFolderBrowseButton->BorderThickness = Thickness(2);
	hUpFolderBrowseButton->BorderBrush = Brushes::Black;
	hUpFolderBrowserDialog->ShowNewFolderButton = false;
	hUpFolderBrowserDialog->SelectedPath =
		System::IO::Directory::GetCurrentDirectory();
	hUploadButton->Content = "Upload Files";
	hUploadButton->Height = 30;
	hUploadButton->Width = 220;
	hUploadButton->BorderThickness = Thickness(2);
	hUploadButton->BorderBrush = Brushes::Black;
	hUPBorder2->BorderThickness = Thickness(1);
	hUPBorder2->Height = 229;
	hUPBorder2->Margin = Thickness(5);
	hUPBorder2->BorderBrush = Brushes::Black;
	hUPBorder2->Child = hUpListBoxDisplay;
	hUPisRecTitle->Padding = Thickness(5);
	hUPisRecTitle->Text = "Is Search Recursively :";
	hUPisRecTitle->FontSize = 16;
	hUPspaceTitle->Padding = Thickness(5);
	hUPspaceTitle->Text = "";
	hUPspaceTitle->FontSize = 2;
}

//----< setting up the Uploadfile tab view >-----------------------------------------------
void WPFCppCliDemo::setUpUploadFileView()
{
	setUpUploadFileViewHelper();
	ColumnDefinition^ hColm0Def = gcnew ColumnDefinition();
	hUploadFileGrid->ColumnDefinitions->Add(hColm0Def);
	TextBlock^ hUploadDirTitlee = gcnew TextBlock();
	hUploadDirTitlee->Padding = Thickness(5);
	hUploadDirTitlee->Text = "Files to Upload :";
	hUploadDirTitlee->FontSize = 16;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hUpListBox;
	hBorder1->Height = 430;
	StackPanel^ hStackPanel01 = gcnew StackPanel();
	hStackPanel01->Children->Add(hUploadDirTitlee);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hConnectionGrid->RowDefinitions->Add(hRow1Def);
	hStackPanel01->Children->Add(hBorder1);
	hStackPanel01->Margin = Thickness(2);
	hUploadFileGrid->SetColumn(hStackPanel01, 0);
	hUploadFileGrid->Children->Add(hStackPanel01);
	ColumnDefinition^ hColm1Def = gcnew ColumnDefinition();
	hUploadFileGrid->ColumnDefinitions->Add(hColm1Def);
	TextBlock^ hCategoryTitle = gcnew TextBlock();
	hCategoryTitle->Padding = Thickness(5);
	hCategoryTitle->Text = "Categories Selection:";
	hCategoryTitle->FontSize = 16;
	TextBlock^ hUploadedTitled = gcnew TextBlock();
	hUploadedTitled->Padding = Thickness(5);
	hUploadedTitled->Text = "Uploaded Files:";
	hUploadedTitled->FontSize = 16;
	StackPanel^ hStackPanel11 = gcnew StackPanel();
	hStackPanel11->Margin = Thickness(2);
	hStackPanel11->Children->Add(hCategoryTitle);
	hStackPanel11->Children->Add(hCategories);
	hStackPanel11->Children->Add(hUPisRecTitle);
	hStackPanel11->Children->Add(hisRecursively);
	hStackPanel11->Children->Add(hUpTextBlock);
	hStackPanel11->Children->Add(hUpFolderBrowseButton);
	hStackPanel11->Children->Add(hUPspaceTitle);
	hStackPanel11->Children->Add(hUploadButton);
	hStackPanel11->Children->Add(hUploadedTitled);
	hStackPanel11->Children->Add(hUPBorder2);
	hUploadFileGrid->SetColumn(hStackPanel11, 1);
	hUploadFileGrid->Children->Add(hStackPanel11);
}

//----< Upload  Recursively Selection >-----------------------------------------------
void WPFCppCliDemo::UploadDropDownLRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args)
{
	if (hisRecursively->SelectedItem->ToString() == "YES"){
		isRecursively = true;
	}
	else{
		isRecursively = false;
	}
}

//----< Upload  Recursively Load >-----------------------------------------------
void WPFCppCliDemo::UploadDropDownRecursivelyLoad(Object^ sender, RoutedEventArgs^ args)
{
	array<String^>^ aa = { "YES", "NO" };
	hisRecursively->ItemsSource = aa;
	hisRecursively->SelectedIndex = 0;
}

//----< Upload  Catogries Load >-----------------------------------------------
void WPFCppCliDemo::UploadDDListLoaded(Object^ sender, RoutedEventArgs^ args)
{
	array<String^>^ aa = { "ALL", "CPP", "TEXT", "JAVA", "DOC", "PNG" };
	hCategories->ItemsSource = aa;
	hCategories->SelectedIndex = 0;
}

//----< Upload  Catogries Selection >--------------------------------------------
void WPFCppCliDemo::UploadDropDownList(Object^ sender, SelectionChangedEventArgs^ args)
{
	hUpListBox->Items->Clear();
	UploadCategory = hCategories->SelectedItem->ToString();
	if (UploadCategory == "CPP"){
		array<String^>^ aaa = { "*.cpp", "*.h", "*.vcxproj" };
		UploadPatterns = aaa;
	}
	if (UploadCategory == "ALL"){
		array<String^>^ aaa = { "*.*" };
		UploadPatterns = aaa;
	}
	if (UploadCategory == "TEXT"){
		array<String^>^ aaa = { "*.txt" };
		UploadPatterns = aaa;
	}
	if (UploadCategory == "JAVA"){
		array<String^>^ aaa = { "*.java" };
		UploadPatterns = aaa;
	}
	if (UploadCategory == "DOC"){
		array<String^>^ aaa = { "*.doc", "*.docx" };
		UploadPatterns = aaa;
	}
	if (UploadCategory == "PNG"){
		array<String^>^ aaa = { "*.png" };
		UploadPatterns = aaa;
	}
}

//----< Searching File and Path recursively and transfering to Data Store to save it >---------
void WPFCppCliDemo::searchUPLoadFiles(String^ path, String^ pattern, bool isSearchRecursively)
{
	array<String^>^ files = System::IO::Directory::GetFiles(path, pattern);
	for (int i = 0; i < files->Length; ++i)
		hUpListBox->Items->Add(files[i]);
	if (isSearchRecursively){
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i){
			std::cout << "\n Directories" << toStdString(dirs[i]);
			if (dirs[i] == "." || dirs[i] == ".."){
				continue;
			}
			searchUPLoadFiles(dirs[i], pattern, isSearchRecursively);
		}
	}
}

//----< Displaying Files Uploaded >-------------------------------------------------
void WPFCppCliDemo::addUploadedText(String^ msg)
{
	hUpListBoxDisplay->Items->Add(msg);
	hStatus->Text = "Uploaded:" + msg;
	countUploadFile--;
	if (countUploadFile == 0){
		hStatus->Text = "Finished Uploading Files ";
	}
}

//----< Displaying Files Searched >-------------------------------------------------
void WPFCppCliDemo::addToFileSearchList(String^ msg)
{
	std::stringstream ss(toStdString(msg));
	std::string item;
	std::vector <std::string> vecTemp;
	while (std::getline(ss, item, '@')) {
		vecTemp.push_back(item);
	}
	hFSListBox->Items->Clear();
	array<String^>^ args = gcnew array<String^>(vecTemp.size());
	for (auto val : vecTemp){
		hFSListBox->Items->Add(toSystemString(val));
	}
	hStatus->Text = "Done Search Files with that Pattern ";
}

//----< Displaying Text Searched >-------------------------------------------------
void WPFCppCliDemo::addToTextSearchList(String^ msg)
{
	std::stringstream ss(toStdString(msg));
	std::string item;
	std::vector <std::string> vecTemp;
	while (std::getline(ss, item, '@')) {
		vecTemp.push_back(item);
	}
	array<String^>^ args = gcnew array<String^>(vecTemp.size());
	for (auto val : vecTemp){
		hTSListBox->Items->Add(toSystemString(val));
	}
	countTextSearch--;
	if(countTextSearch <1)
	hStatus->Text = "Done Text Search";
	else
		hStatus->Text = "Still Searching Text ";

}

//----< Displaying Files To be Downloaded >-------------------------------------------------
void WPFCppCliDemo::addGetFilesListText(String^ msg)
{
	std::stringstream ss(toStdString(msg));
	std::string item;
	int index = 0;
	int countIndex = 0;
	std::vector <std::string> vecTemp;
	while (std::getline(ss, item, '@')) {
		vecTemp.push_back(item);
	}
	hDownListBox->Items->Clear();
	array<String^>^ args = gcnew array<String^>(vecTemp.size());
	for (auto val : vecTemp){
		hDownListBox->Items->Add(toSystemString(val));
	}
	hStatus->Text = "Done Getting Files Please choose files ";
}

//----< Displaying Downloaded Files >-------------------------------------------------
void WPFCppCliDemo::addDownloadText(String^ msg)
{
	hDownListBoxDisplay->Items->Add(msg);
	hStatus->Text = "Downloded :" + msg;
	countDownloadFile--;
	if (countDownloadFile == 0){
		hStatus->Text = "Finished Downloading Files ";
	}
}

//----< checking Command null or not >-------------------------------------------------
bool WPFCppCliDemo::checkCommandNULL(std::string cmd){
	if (cmd != "0"){
		return true;
	}
	return false;
}

//----< getMessage And Transfer it to main thread for  displaying >----------------------
void WPFCppCliDemo::getMessage()
{
	while (true){
		std::cout << "\n  receive thread calling getMessage()";
		MessagePassing::Message msgg;
		pChann_->getMessage(msgg);
		std::string cmd = msgg.getHeader().getDisplayCommand();
		MessagePassing::Command cmd_val = MessagePassing::Command::NONE;
		if (checkCommandNULL(cmd))
			cmd_val = static_cast<MessagePassing::Command>(atoi(cmd.c_str()));
		if (cmd_val == MessagePassing::Command::UploadFile){
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = toSystemString(msgg.getMessage());
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addUploadedText);
			Dispatcher->Invoke(act, args);
		}
		if (cmd_val == MessagePassing::Command::DownLoadFile){
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = toSystemString(msgg.getMessage());
			Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addDownloadText);
			Dispatcher->Invoke(act, args);
		}
		if (cmd_val == MessagePassing::Command::GetFilesList){
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = toSystemString(msgg.getMessage());
			Action<String^>^ act = gcnew Action < String^ >
				(this, &WPFCppCliDemo::addGetFilesListText);
			Dispatcher->Invoke(act, args);
		}
		if (cmd_val == MessagePassing::Command::FileSearch){
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = toSystemString(msgg.getMessage());
			Action<String^>^ act = gcnew Action < String^ >
				(this, &WPFCppCliDemo::addToFileSearchList);
			Dispatcher->Invoke(act, args);
		}
		if (cmd_val == MessagePassing::Command::TextSearch){
			array<String^>^ args = gcnew array<String^>(1);
			args[0] = toSystemString(msgg.getMessage());
			Action<String^>^ act = gcnew Action < String^ >
				(this, &WPFCppCliDemo::addToTextSearchList);
			Dispatcher->Invoke(act, args);
		}
	}
}
//----<setting up the function of the browse folder button >----------
void WPFCppCliDemo::UpBrowseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hUpListBoxDisplay->Items->Clear();
	hUpListBox->Items->Clear();
	hUpListBox->SelectionMode = SelectionMode::Multiple;
	System::Windows::Forms::DialogResult result;
	result = hUpFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hUpFolderBrowserDialog->SelectedPath;
		for (int i = 0; i < UploadPatterns->Length; i++)
		{
			searchUPLoadFiles(path, UploadPatterns[i], isRecursively);
		}
	}
}

//----< uploading FIles to server >---------------------------------
void WPFCppCliDemo::UpLoad(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n Uploading Files Called";
	if (checkConnection){
		if (hUpListBox->SelectedItems->Count > 0)
		{
			countUploadFile = 0;
			for each(String^ value in hUpListBox->SelectedItems)
			{
				countUploadFile++;
				MessagePassing::Message msg;
				msg.getHeader().setCommand(MessagePassing::Command::UploadFile);
				msg.getHeader().setSrcIPAndPort(toStdString(srcIP), toStdString(srcPort));
				msg.getHeader().setDstIPAndPort(toStdString(dstIP), toStdString(dstPort));
				msg.setCatogries(toStdString(UploadCategory));
				msg.getHeader().setDisplayCommand(MessagePassing::Command::UploadFile);
				msg.getHeader().setFileNameAndType(toStdString(value),
					MessagePassing::FileType::binary);
				pChann_->postMessage(msg);
				hStatus->Text = "Uploading";
			}
		}
		else{
			hStatus->Text = "Please Select Files to Upload";
		}
	}
	else
		hStatus->Text = "Connection has not been established";
}

//-----------------< Setting for Download View Helper >----------------------------
void WPFCppCliDemo::setDownDownloadFileViewHelper(){
	hDownloadFileTab->Content = hDownloadFileGrid;
	hDownloadFileGrid->Margin = Thickness(20);
	hDownFolderBrowseButton->Content = "Get File list For Download";
	hDownFolderBrowseButton->Height = 30;
	hDownFolderBrowseButton->Width = 220;
	hDownFolderBrowseButton->BorderThickness = Thickness(2);
	hDownFolderBrowseButton->BorderBrush = Brushes::Black;
	hDownButton->Content = "Download Files";
	hDownButton->Height = 30;
	hDownButton->Width = 220;
	hDownButton->BorderThickness = Thickness(2);
	hDownButton->BorderBrush = Brushes::Black;
	hDownDownBorder2->BorderThickness = Thickness(1);
	hDownDownBorder2->Height = 229;
	hDownDownBorder2->Margin = Thickness(5);
	hDownDownBorder2->BorderBrush = Brushes::Black;
	hDownDownBorder2->Child = hDownListBoxDisplay;
	hdowndownCategoryTitle->Padding = Thickness(5);
	hdowndownCategoryTitle->Text = "Categories Selection:";
	hdowndownCategoryTitle->FontSize = 16;
	hisDownRecTitle1->Padding = Thickness(5);
	hisDownRecTitle1->Text = "Is Search Recursively :";
	hisDownRecTitle1->FontSize = 16;
	hDownspaceTitle1->Padding = Thickness(5);
	hDownspaceTitle1->Text = "";
	hDownspaceTitle1->FontSize = 2;
}

//----< setting for download view >------------------------------------------------
void WPFCppCliDemo::setDownDownloadFileView()
{
	setDownDownloadFileViewHelper();
	ColumnDefinition^ hdownColm0Def = gcnew ColumnDefinition();
	hDownloadFileGrid->ColumnDefinitions->Add(hdownColm0Def);
	TextBlock^ hDownDirTitlee = gcnew TextBlock();
	hDownDirTitlee->Padding = Thickness(5);
	hDownDirTitlee->Text = "Files to Download :";
	hDownDirTitlee->FontSize = 16;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownListBox;
	hBorder1->Height = 430;
	StackPanel^ hDownStackPanel01 = gcnew StackPanel();
	hDownStackPanel01->Children->Add(hDownDirTitlee);
	hDownStackPanel01->Children->Add(hBorder1);
	hDownStackPanel01->Margin = Thickness(2);
	hDownloadFileGrid->SetColumn(hDownStackPanel01, 0);
	hDownloadFileGrid->Children->Add(hDownStackPanel01);
	ColumnDefinition^ hDownColm1Def = gcnew ColumnDefinition();
	hDownloadFileGrid->ColumnDefinitions->Add(hDownColm1Def);
	TextBlock^ hdownloadedTitled1 = gcnew TextBlock();
	hdownloadedTitled1->Padding = Thickness(5);
	hdownloadedTitled1->Text = "Downloaded Files:";
	hdownloadedTitled1->FontSize = 16;
	StackPanel^ hdownStackPanel11 = gcnew StackPanel();
	hdownStackPanel11->Margin = Thickness(2);
	hdownStackPanel11->Children->Add(hdowndownCategoryTitle);
	hdownStackPanel11->Children->Add(hDownCategories);
	hdownStackPanel11->Children->Add(hisDownRecTitle1);
	hdownStackPanel11->Children->Add(hisDownRecursively);
	hdownStackPanel11->Children->Add(hDownTextBlock);
	hdownStackPanel11->Children->Add(hDownFolderBrowseButton);
	hdownStackPanel11->Children->Add(hDownspaceTitle1);
	hdownStackPanel11->Children->Add(hDownButton);
	hdownStackPanel11->Children->Add(hdownloadedTitled1);
	hdownStackPanel11->Children->Add(hDownDownBorder2);
	hDownloadFileGrid->SetColumn(hdownStackPanel11, 1);
	hDownloadFileGrid->Children->Add(hdownStackPanel11);
}

//----< Download Recursive Selection >------------------------------------------------------
void WPFCppCliDemo::DownloadDropDownLRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args)
{
	hDownListBoxDisplay->Items->Clear();
	hDownListBox->Items->Clear();
	if (hisDownRecursively->SelectedItem->ToString() == "YES"){
		isRecursivelyDownload = true;
	}
	else{
		isRecursivelyDownload = false;
	}
}
//----< Download Recursive Load >------------------------------------------------------
void WPFCppCliDemo::DownloadDropDownRecursivelyLoad(Object^ sender, RoutedEventArgs^ args)
{
	array<String^>^ aa = { "YES", "NO" };
	hisDownRecursively->ItemsSource = aa;
	hisDownRecursively->SelectedIndex = 0;
}

//----< Download Category Load >------------------------------------------------------
void WPFCppCliDemo::DownloadDDListLoaded(Object^ sender, RoutedEventArgs^ args)
{
	array<String^>^ aa = { "ALL", "CPP", "TEXT", "JAVA", "DOC", "PNG" };
	hDownCategories->ItemsSource = aa;
	hDownCategories->SelectedIndex = 0;

}

//----< Download Category Selection >------------------------------------------------------
void WPFCppCliDemo::DownloadDropDownList(Object^ sender, SelectionChangedEventArgs^ args)
{
	hDownListBoxDisplay->Items->Clear();
	hDownListBox->Items->Clear();
	DownloadCategory = hDownCategories->SelectedItem->ToString();
	if (DownloadCategory == "ALL"){
		array<String^>^ aaa = { "*.*" };
		downPatterns = aaa;
	}
	if (DownloadCategory == "CPP"){
		array<String^>^ aaa = { "*.cpp", "*.h", "*.vcxproj" };
		downPatterns = aaa;
	}
	if (DownloadCategory == "TEXT"){
		array<String^>^ aaa = { "*.txt" };
		downPatterns = aaa;
	}
	if (DownloadCategory == "JAVA"){
		array<String^>^ aaa = { "*.java" };
		downPatterns = aaa;
	}
	if (DownloadCategory == "DOC"){
		array<String^>^ aaa = { "*.doc", "*.docx" };
		downPatterns = aaa;
	}
	if (DownloadCategory == "PNG"){
		array<String^>^ aaa = { "*.png" };
		downPatterns = aaa;
	}
}

//----< Downloading Files from Server >------------------------------------------------------
void WPFCppCliDemo::DownLoad(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n Downloading Files";
	if (checkConnection){
		if (hDownListBox->SelectedItems->Count > 0){
			countDownloadFile = 0;
			for each(String^ value in hDownListBox->SelectedItems){
				countDownloadFile++;
				MessagePassing::Message msg;
				msg.getHeader().setCommand(MessagePassing::Command::DownLoadFile);
				msg.getHeader().setSrcIPAndPort(toStdString(srcIP), toStdString(srcPort));
				msg.getHeader().setDstIPAndPort(toStdString(dstIP), toStdString(dstPort));
				msg.setCatogries(toStdString(DownloadCategory));
				msg.getHeader().setDisplayCommand(MessagePassing::Command::DownLoadFile);
				msg.getHeader().setFileNameAndType(toStdString(value),
					MessagePassing::FileType::binary);
				pChann_->postMessage(msg);
				hStatus->Text = "Downloading " + value;
			}
		}
		else{
			hStatus->Text = "Please Select Files to Download";
		}
	}
	else
		hStatus->Text = "Connection has not been established";
}

//---------< Getting List of files to download >------------------------------------------
void WPFCppCliDemo::DownBrowseForFiles(Object^ sender, RoutedEventArgs^ args){
	std::cout << "\n Downloading Files";
	if (checkConnection){
		hDownListBoxDisplay->Items->Clear();
		hDownListBox->Items->Clear();
		MessagePassing::Message msg;
		msg.getHeader().setCommand(MessagePassing::Command::GetFilesList);
		msg.getHeader().setSrcIPAndPort(toStdString(srcIP), toStdString(srcPort));
		msg.getHeader().setDstIPAndPort(toStdString(dstIP), toStdString(dstPort));
		msg.setCatogries(toStdString(DownloadCategory));
		msg.setIsrecursively(isRecursivelyDownload);
		msg.getHeader().setDisplayCommand(MessagePassing::Command::GetFilesList);
		pChann_->postMessage(msg);
		hStatus->Text = "Getting List to Files to be Downloaded";
	}
	else
		hStatus->Text = "Connection has not been established";

}

//----< Setting Listen Parameter >-------------------------------------
bool WPFCppCliDemo::listen(){

	dstIP = hIP->Text;
	dstPort = hPort->Text;
	if (dstIP != "localhost"){
		System::Windows::MessageBox::Show(" dstIP in correct", "Error in DstIP");
		false;
	}
	if (dstPort == "" || toStdString(dstPort).find_first_not_of("0123456789") != std::string::npos){
		System::Windows::MessageBox::Show("Port Should be in Numeric", "Error in DstPort");
		false;
	}
	if (hListBox->SelectedItems->Count > 0){
		for each (String^ value in hListBox->SelectedItems){
			srcPort = value;
		}
	}
	if (srcPort == "" || toStdString(srcPort).find_first_not_of("0123456789") != std::string::npos){
		System::Windows::MessageBox::Show("Listen Port Should be in Numeric", "Error in Listen port");
		false;
	}
	size_t portno = static_cast<size_t> (System::Convert::ToUInt64(srcPort));
	pRecvr_->setPort(portno);
	pRecvr_->listen(portno);
	return true;
}

//----< Create Connection to mockChannel>-------------------------------------
void WPFCppCliDemo::createConnection(Object^ sender, RoutedEventArgs^ args)
{

	if (!checkConnection){
		if(!listen()) return;
		checkConnection = false;
		if (dstIP == "localhost" && (dstPort == "8080" || dstPort == "8081")){
			if (!pSendr_->connect(toStdString(dstIP), toStdString(dstPort))){
				hStatus->Text = "Not able to connect ServerIP ="
					+ dstIP + "  Port = " + dstPort;
				return;
			}
			else{
				hStatus->Text = "Connected to ServerIP =" + dstIP + "  Port = " + dstPort + "Listen port =" + srcPort;
				checkConnection = true;
			}
		}
		else
			hStatus->Text = "Please Enter Correct IP And Port for Server";
		pChann_->start();
	}else
		hStatus->Text = " Connection already established ";
}

//----< Destroying Connection >------------------------------------------------
void WPFCppCliDemo::destroyConnection(Object^ sender, RoutedEventArgs^ args){
	checkConnection = false;
	hIP->Text = "";
	hPort->Text = "";
	pChann_->stop();
	hStatus->Text = "Disconnected from Server " + dstIP + "  Port :" + dstPort;
	hIP->Focus();
	hPort->Focus();
}

//----< setUp ConnectionView Helper >------------------------------------------------
void WPFCppCliDemo::setUpConnectionViewHelper()
{
	hConnectionGrid->Margin = Thickness(20);
	hConnections->Content = hConnectionGrid;
	hListBox->Items->Add(" Listen Ports");
	hListBox->Items->Add("8100");
	hListBox->Items->Add("8200");
	hListBox->Items->Add(" Server Ports");
	hListBox->Items->Add("8080");
	hListBox->Items->Add("8081");
	hListBox->Items->Add("By default it listen to 8100");
	hListBox->Items->Add("To change please select listen Port and connect");
	hconnBorder1->BorderThickness = Thickness(1);
	hconnBorder1->BorderBrush = Brushes::Black;
	hconnBorder1->Height = 350;
	hconnBorder1->Width = 350;
	hconnBorder1->MarginProperty;
	hconnBorder1->Child = hListBox;
	hIP->Width = 150;
	hIP->Height = 30;
	hIP->FontSize = 15;
	hIP->BorderThickness = Thickness(1);
	hIP->BorderBrush = Brushes::Black;
	hPort->Width = 150;
	hPort->Height = 30;
	hPort->FontSize = 15;
	hPort->BorderThickness = Thickness(1);
	hPort->BorderBrush = Brushes::Black;
	hConnectionBorder2->Width = 120;
	hConnectionBorder2->Height = 30;
	hConnectionBorder2->BorderThickness = Thickness(1);
	hConnectionBorder2->BorderBrush = Brushes::Black;
	hConnect->Content = "Connect";
	hConnectionBorder2->Child = hConnect;
	hConnectspaceTitle->Padding = Thickness(5);
	hConnectspaceTitle->Text = "";
	hConnectspaceTitle->Height = 30;
	hConnectspaceTitle->FontSize = 2;
	hConnectspaceTitle2->Padding = Thickness(5);
	hConnectspaceTitle2->Text = "";
	hConnectspaceTitle2->Height = 30;
	hConnectspaceTitle2->FontSize = 2;
}

//----< setUp Connection View >------------------------------------------------
void WPFCppCliDemo::setUpConnectionView()
{
	setUpConnectionViewHelper();
	ColumnDefinition^ hColm0Def = gcnew ColumnDefinition();
	hConnectionGrid->ColumnDefinitions->Add(hColm0Def);
	TextBlock^ hListenPort = gcnew TextBlock();
	hListenPort->Padding = Thickness(5);
	hListenPort->Text = " Current Configuration Parameter:";
	hListenPort->FontSize = 16;
	StackPanel^ hStackPanel01 = gcnew StackPanel();
	hStackPanel01->Margin = Thickness(2);
	hStackPanel01->Children->Add(hListenPort);
	hStackPanel01->Children->Add(hconnBorder1);
	hStackPanel01->HorizontalAlignment
		= System::Windows::HorizontalAlignment::Center;
	hConnectionGrid->SetColumn(hStackPanel01, 0);
	hConnectionGrid->Children->Add(hStackPanel01);
	ColumnDefinition^ hColm1Def = gcnew ColumnDefinition();
	hConnectionGrid->ColumnDefinitions->Add(hColm1Def);
	StackPanel^ hStackPanel02 = gcnew StackPanel();
	hStackPanel02->Margin = Thickness(2);
	TextBlock^ hIPAddreessTitle = gcnew TextBlock();
	hIPAddreessTitle->Padding = Thickness(20);
	hIPAddreessTitle->Text = " Server IP Address :";
	hIPAddreessTitle->FontSize = 20;
	hStackPanel02->HorizontalAlignment
		= System::Windows::HorizontalAlignment::Center;
	hStackPanel02->Children->Add(hIPAddreessTitle);
	hStackPanel02->Children->Add(hIP);
	TextBlock^ hAddressPort = gcnew TextBlock();
	hAddressPort->Padding = Thickness(20);
	hAddressPort->Text = " Server Port :";
	hAddressPort->FontSize = 20;
	hStackPanel02->Children->Add(hAddressPort);
	hStackPanel02->Children->Add(hPort);
	hStackPanel02->Children->Add(hConnectspaceTitle2);
	hStackPanel02->Children->Add(hConnectionBorder2);
	hStackPanel02->Children->Add(hConnectspaceTitle);
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hDisconnect->Content = "Disconnect";
	hBorder3->Child = hDisconnect;
	//hStackPanel02->Children->Add(hBorder3);
	hConnectionGrid->SetColumn(hStackPanel02, 1);
	hConnectionGrid->Children->Add(hStackPanel02);
}

//----< convert form std::string to system string >-------------------------------------
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}

//----< Clear Status Mesage >-------------------------------------
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	hStatus->Text = "Cleared message";
}

//----< Add File Search Pattern >-------------------------------------
void WPFCppCliDemo::FileSearchAddPattern(Object^ sender, RoutedEventArgs^ args){
	if (hFSPattBox->Text != ""){
		hFileSearchPatternDisplay->Items->Add(hFSPattBox->Text);
		hFSPattBox->Text = "";
	}
	else{
		hStatus->Text = " Enter value to add ";
	}
}

//----< Delete File Search Pattern >-------------------------------------
void WPFCppCliDemo::FileSearchDelePattern(Object^ sender, RoutedEventArgs^ args){
	if (hFileSearchPatternDisplay->SelectedItems->Count > 0){
		for (int i = hFileSearchPatternDisplay->SelectedItems->Count - 1; i >= 0; i--)
		{
			hFileSearchPatternDisplay->Items->RemoveAt(hFileSearchPatternDisplay->SelectedIndex);
		}
	}
	else{
		hStatus->Text = "Select Items to Clear ";
	}
}

//----< set File Search Pattern >-------------------------------------
void WPFCppCliDemo::setFSpattern(MessagePassing::Message& msg){
	if (hFileSearchPatternDisplay->Items->Count > 0){
		for each (String^ value in hFileSearchPatternDisplay->Items){
			msg.setPattern(toStdString(value));
		}
	}
}

//----< File Search from Server >-------------------------------------
void WPFCppCliDemo::SearchFiles(Object^ sender, RoutedEventArgs^ args){
	std::cout << "\n Search Files";
	hFSListBox->Items->Clear();
	if (checkConnection){
		MessagePassing::Message msg;
		msg.getHeader().setCommand(MessagePassing::Command::FileSearch);
		msg.getHeader().setSrcIPAndPort(toStdString(srcIP), toStdString(srcPort));
		msg.getHeader().setDstIPAndPort(toStdString(dstIP), toStdString(dstPort));
		msg.setCatogries(toStdString(FileSearchCategory));
		msg.setIsrecursively(isFSRecursively);
		setFSpattern(msg);
		msg.getHeader().setDisplayCommand(MessagePassing::Command::FileSearch);
		pChann_->postMessage(msg);
		hStatus->Text = "Searching Files ";
	}
	else
		hStatus->Text = "Connection has not been established";
}

//----< File Search Catogry load >-------------------------------------
void WPFCppCliDemo::FileSearchPatternCatogriesLoad(Object^ sender, RoutedEventArgs^ args){
	array<String^>^ aa = { "ALL", "CPP", "TEXT", "JAVA", "DOC", "PNG" };
	FileSearchPatternCatogries->ItemsSource = aa;
	FileSearchPatternCatogries->SelectedIndex = 0;
}

//----< File Search Catogry Selection >-------------------------------------
void WPFCppCliDemo::FileSearchPatternCatogriesSelection(Object^ sender, SelectionChangedEventArgs^ args){
	hFSListBox->Items->Clear();
	FileSearchCategory = hCategories->SelectedItem->ToString();
	if (FileSearchCategory == "CPP"){
		array<String^>^ aaa = { "*.cpp", "*.h", "*.vcxproj" };
		CattPatterns = aaa;
	}
	if (FileSearchCategory == "ALL"){
		array<String^>^ aaa = { "*.*" };
		CattPatterns = aaa;
	}
	if (FileSearchCategory == "TEXT"){
		array<String^>^ aaa = { "*.txt" };
		CattPatterns = aaa;
	}
	if (FileSearchCategory == "JAVA"){
		array<String^>^ aaa = { "*.java" };
		CattPatterns = aaa;
	}
	if (FileSearchCategory == "DOC"){
		array<String^>^ aaa = { "*.doc", "*.docx" };
		CattPatterns = aaa;
	}
	if (FileSearchCategory == "PNG"){
		array<String^>^ aaa = { "*.png" };
		CattPatterns = aaa;
	}
}

//----< File Search Recursively Selection >-------------------------------------
void WPFCppCliDemo::FileSearchPatternRecursivelySelection(Object^ sender, SelectionChangedEventArgs^ args){
	if (FileSearchPatternRecursively->SelectedItem->ToString() == "YES"){
		isFSRecursively = true;
	}
	else{
		isFSRecursively = false;
	}
}

//----< File Search Recursively Load >-------------------------------------
void WPFCppCliDemo::FileSearchPatternRecursiveLoad(Object^ sender, RoutedEventArgs^ args){
	array<String^>^ aa = { "YES", "NO" };
	FileSearchPatternRecursively->ItemsSource = aa;
	FileSearchPatternRecursively->SelectedIndex = 0;
}

//----< Text Search Following end points >-------------------------------------
void WPFCppCliDemo::AddEndPoint(){
	TSPEndPointCatogry->Items->Add("8080");
	TSPEndPointCatogry->Items->Add("8081");
}

//----< setting Up FileSearch View Helper >-------------------------------------
void WPFCppCliDemo::setUpFileSearchViewHelper()
{
	hFileSearchTab->Content = hFileSearchGrid;
	hFileSearchGrid->Margin = Thickness(20);
	hFSBorder1->BorderThickness = Thickness(1);
	hFSBorder1->BorderBrush = Brushes::Black;
	hFSBorder1->Child = hFSListBox;
	hFSBorder1->Height = 430;
	hFSCategoryTitle->Padding = Thickness(5);
	hFSCategoryTitle->Text = "Specific Path Selection:";
	hFSCategoryTitle->FontSize = 16;
	hisFSRecTitle1->Padding = Thickness(5);
	hisFSRecTitle1->Text = "Is Search Recursively :";
	hisFSRecTitle1->FontSize = 16;
	hisFSPatternTextTitle->Padding = Thickness(5);
	hisFSPatternTextTitle->Text = "Pattern Text :";
	hisFSPatternTextTitle->FontSize = 16;
	hFSButton->Content = "File Search";
	hFSButton->Height = 30;
	hFSButton->Width = 220;
	hFSButton->BorderThickness = Thickness(2);
	hFSButton->BorderBrush = Brushes::Black;
	hFSspaceTitle1->Padding = Thickness(5);
	hFSspaceTitle1->Text = "";
	hFSspaceTitle1->FontSize = 2;
	hFSspaceTitle2->Padding = Thickness(5);
	hFSspaceTitle2->Text = "";
	hFSspaceTitle2->FontSize = 2;
	hFSspaceTitle3->Padding = Thickness(5);
	hFSspaceTitle3->Text = "";
	hFSspaceTitle3->FontSize = 2;
	hDownButton->Content = "Download Files";
	hDownButton->Height = 30;
	hDownButton->Width = 220;
	hDownButton->BorderThickness = Thickness(2);
	hDownButton->BorderBrush = Brushes::Black;
	hFSPattAddButton->Content = "Add Pattern";
	hFSPattAddButton->Height = 30;
	hFSPattAddButton->Width = 220;
	hFSPattAddButton->BorderThickness = Thickness(2);
	hFSPattAddButton->BorderBrush = Brushes::Black;
	hFSPattDelButton->Content = "Del Pattern";
	hFSPattDelButton->Height = 30;
	hFSPattDelButton->Width = 220;
	hFSPattDelButton->BorderThickness = Thickness(2);
	hFSPattDelButton->BorderBrush = Brushes::Black;

}
//----< setting Up FileSearch View >-------------------------------------
void WPFCppCliDemo::setUpFileSearchView(){
	setUpFileSearchViewHelper();
	ColumnDefinition^ hdownColm0Def = gcnew ColumnDefinition();
	hFileSearchGrid->ColumnDefinitions->Add(hdownColm0Def);
	TextBlock^ hDownDirTitlee = gcnew TextBlock();
	hDownDirTitlee->Padding = Thickness(5);
	hDownDirTitlee->Text = "Files Found :";
	hDownDirTitlee->FontSize = 16;
	StackPanel^ hDownStackPanel01 = gcnew StackPanel();
	hDownStackPanel01->Children->Add(hDownDirTitlee);
	hDownStackPanel01->Children->Add(hFSBorder1);
	hDownStackPanel01->Margin = Thickness(2);
	hFileSearchGrid->SetColumn(hDownStackPanel01, 0);
	hFileSearchGrid->Children->Add(hDownStackPanel01);
	ColumnDefinition^ hDownColm1Def = gcnew ColumnDefinition();
	hFileSearchGrid->ColumnDefinitions->Add(hDownColm1Def);
	hFSPattBox->Height = 30;
	hFSPattBox->Width = 220;
	hFSPattBox->BorderThickness = Thickness(5);
	hFSPattBox->BorderBrush = Brushes::Black;
	TextBlock^ hdownloadedTitled1 = gcnew TextBlock();
	hdownloadedTitled1->Padding = Thickness(5);
	hdownloadedTitled1->Text = "Patterns Added:";
	hdownloadedTitled1->FontSize = 16;
	Border^ hDownBorder2 = gcnew Border();
	hDownBorder2->BorderThickness = Thickness(1);
	hDownBorder2->Height = 100;
	hDownBorder2->Margin = Thickness(5);
	hDownBorder2->BorderBrush = Brushes::Black;
	hDownBorder2->Child = hFileSearchPatternDisplay;
	StackPanel^ hdownStackPanel11 = gcnew StackPanel();
	hdownStackPanel11->Margin = Thickness(2);
	hdownStackPanel11->Children->Add(hFSCategoryTitle);
	hdownStackPanel11->Children->Add(FileSearchPatternCatogries);
	hdownStackPanel11->Children->Add(hisFSRecTitle1);
	hdownStackPanel11->Children->Add(FileSearchPatternRecursively);
	hdownStackPanel11->Children->Add(hisFSPatternTextTitle);
	hdownStackPanel11->Children->Add(hFSPattBox);
	hdownStackPanel11->Children->Add(hFSspaceTitle3);
	hdownStackPanel11->Children->Add(hFSPattAddButton);
	hdownStackPanel11->Children->Add(hFSspaceTitle1);
	hdownStackPanel11->Children->Add(hFSPattDelButton);
	hdownStackPanel11->Children->Add(hFSspaceTitle2);
	hdownStackPanel11->Children->Add(hFSButton);
	hdownStackPanel11->Children->Add(hdownloadedTitled1);
	hdownStackPanel11->Children->Add(hDownBorder2);
	hFileSearchGrid->SetColumn(hdownStackPanel11, 1);
	hFileSearchGrid->Children->Add(hdownStackPanel11);
}

//----<Convert System String to Std string >----------------------
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}
//----< Set Text Search view Helper >----------------------------------------
void WPFCppCliDemo::setUpTextSearchViewHelper()
{
	hTextSearchTab->Content = hTextSearchGrid;
	hTextSearchGrid->Margin = Thickness(20);
	hTSDirTitlee->Padding = Thickness(5);
	hTSDirTitlee->Text = "Text Found :";
	hTSDirTitlee->FontSize = 16;
	hTSBorder1->BorderThickness = Thickness(1);
	hTSBorder1->BorderBrush = Brushes::Black;
	hTSBorder1->Child = hTSListBox;
	hTSBorder1->Height = 430;
	hFTStackPanel01->Children->Add(hTSDirTitlee);
	hFTStackPanel01->Children->Add(hTSBorder1);
	hFTStackPanel01->Margin = Thickness(2);
	hTSCategoryTitle->Padding = Thickness(5);
	hTSCategoryTitle->Text = "Select Multiple End Point:";
	hTSCategoryTitle->FontSize = 16;
	hisTSRecTitle1->Padding = Thickness(5);
	hisTSRecTitle1->Text = "Is Search Recursively :";
	hisTSRecTitle1->FontSize = 16;
	hTSTextTextTitle->Padding = Thickness(5);
	hTSTextTextTitle->Text = "Enter Text To be Search / Regular Expression:";
	hTSTextTextTitle->FontSize = 16;
	hisTSPatternTextTitle->Padding = Thickness(5);
	hisTSPatternTextTitle->Text = "Enter Wild Cards :";
	hisTSPatternTextTitle->FontSize = 16;
	hTSButton->Content = "Text Search";
	hTSButton->Height = 30;
	hTSButton->Width = 220;
	hTSButton->BorderThickness = Thickness(2);
	hTSButton->BorderBrush = Brushes::Black;
	hTSTextBox->Height = 30;
	hTSTextBox->Width = 220;
	hTSTextBox->BorderThickness = Thickness(5);
	hTSTextBox->BorderBrush = Brushes::Black;
	hTSPattBox->Height = 30;
	hTSPattBox->Width = 220;
	hTSPattBox->BorderThickness = Thickness(5);
	hTSPattBox->BorderBrush = Brushes::Black;
}

//----< Set Text Search view Adding Button and Border >----------------
void WPFCppCliDemo::setUpTextSearchViewAddingButton(){
	hTSPattAddButton->Content = "Add Wild Cards";
	hTSPattAddButton->Height = 30;
	hTSPattAddButton->Width = 220;
	hTSPattAddButton->BorderThickness = Thickness(2);
	hTSPattAddButton->BorderBrush = Brushes::Black;
	hTSPattDelButton->Content = "Del Wild Cards";
	hTSPattDelButton->Height = 30;
	hTSPattDelButton->Width = 220;
	hTSPattDelButton->BorderThickness = Thickness(2);
	hTSPattDelButton->BorderBrush = Brushes::Black;
	hTSDownBorder3->BorderThickness = Thickness(1);
	hTSDownBorder3->Height = 80;
	hTSDownBorder3->Margin = Thickness(5);
	hTSDownBorder3->BorderBrush = Brushes::Black;
	hTSDownBorder3->Child = TSPEndPointCatogry;
	hTSDownBorder2->BorderThickness = Thickness(1);
	hTSDownBorder2->Height = 52;
	hTSDownBorder2->Margin = Thickness(5);
	hTSDownBorder2->BorderBrush = Brushes::Black;
	hTSDownBorder2->Child = hTSPatternDisplay;
	TSPEndPointCatogry->SelectionMode = SelectionMode::Multiple;
}

//----< Set Text Search view >----------------------------------------
void WPFCppCliDemo::setUpTextSearchView(){
	setUpTextSearchViewHelper();
	setUpTextSearchViewAddingButton();
	ColumnDefinition^ hdownColm0Def = gcnew ColumnDefinition();
	hTextSearchGrid->ColumnDefinitions->Add(hdownColm0Def);
	hTextSearchGrid->SetColumn(hFTStackPanel01, 0);
	hTextSearchGrid->Children->Add(hFTStackPanel01);
	ColumnDefinition^ hDownColm1Def = gcnew ColumnDefinition();
	hTextSearchGrid->ColumnDefinitions->Add(hDownColm1Def);
	TextBlock^ hspaceTitle1 = gcnew TextBlock();
	hspaceTitle1->Padding = Thickness(5);
	hspaceTitle1->Text = "";
	hspaceTitle1->FontSize = 2;
	TextBlock^ hspaceTitle2 = gcnew TextBlock();
	hspaceTitle2->Padding = Thickness(5);
	hspaceTitle2->Text = "";
	hspaceTitle2->FontSize = 2;
	AddEndPoint();
	TextBlock^ hspaceTitle3 = gcnew TextBlock();
	hspaceTitle3->Padding = Thickness(5);
	hspaceTitle3->Text = "";
	hspaceTitle3->FontSize = 2;
	TextBlock^ hdownloadedTitled1 = gcnew TextBlock();
	hdownloadedTitled1->Padding = Thickness(5);
	hdownloadedTitled1->Text = "Wild Cards Added:";
	hdownloadedTitled1->FontSize = 16;
	StackPanel^ hdownStackPanel11 = gcnew StackPanel();
	hdownStackPanel11->Margin = Thickness(2);
	hdownStackPanel11->Children->Add(hTSCategoryTitle);
	hdownStackPanel11->Children->Add(hTSDownBorder3);
	hdownStackPanel11->Children->Add(hTSTextTextTitle);
	hdownStackPanel11->Children->Add(hTSTextBox);
	hdownStackPanel11->Children->Add(hisTSPatternTextTitle);
	hdownStackPanel11->Children->Add(hTSPattBox);
	hdownStackPanel11->Children->Add(hspaceTitle3);
	hdownStackPanel11->Children->Add(hTSPattAddButton);
	hdownStackPanel11->Children->Add(hspaceTitle1);
	hdownStackPanel11->Children->Add(hTSPattDelButton);
	hdownStackPanel11->Children->Add(hspaceTitle2);
	hdownStackPanel11->Children->Add(hTSButton);
	hdownStackPanel11->Children->Add(hdownloadedTitled1);
	hdownStackPanel11->Children->Add(hTSDownBorder2);
	hTextSearchGrid->SetColumn(hdownStackPanel11, 1);
	hTextSearchGrid->Children->Add(hdownStackPanel11);
}

//----<Adding Text Search Wild Cards >----------------------------------------
void WPFCppCliDemo::TSAddPattern(Object^ sender, RoutedEventArgs^ args){
	// Add Pattern to FileSearchPatterns

	if (hTSPattBox->Text != ""){
		hTSPatternDisplay->Items->Add(hTSPattBox->Text);
		hTSPattBox->Text = "";
	}
	else{
		hStatus->Text = " Enter value to add ";
	}
}

//----<Deleting Text Search Wild Cards >----------------------------------------
void WPFCppCliDemo::TSDelePattern(Object^ sender, RoutedEventArgs^ args){
	if (hTSPatternDisplay->SelectedItems->Count > 0){
		for (int i = hTSPatternDisplay->SelectedItems->Count - 1; i >= 0; i--)
		{
			hTSPatternDisplay->Items->RemoveAt(hTSPatternDisplay->SelectedIndex);
		}
	}
	else{
		hStatus->Text = "Select Items to Clear ";
	}
}

//----<setting Text Search Wild Cards >----------------------------------------
void WPFCppCliDemo::setTSpattern(MessagePassing::Message& msg){
	if (hTSPatternDisplay->Items->Count > 0){
		for each (String^ value in hTSPatternDisplay->Items){
			msg.setPattern(toStdString(value));
		}
	}
}

//----< Searching Text from different Servers >----------------------------------------
void WPFCppCliDemo::SearchText(Object^ sender, RoutedEventArgs^ args){
	std::cout << "\n Search Files";
	hTSListBox->Items->Clear();
	int countTextSearch = 0;
	if (checkConnection){
		MessagePassing::Message msg;
		msg.getHeader().setCommand(MessagePassing::Command::TextSearch);
		msg.getHeader().setSrcIPAndPort(toStdString(srcIP), toStdString(srcPort));
		msg.getHeader().setDstIPAndPort(toStdString(dstIP), toStdString(dstPort));
		msg.setCatogries(toStdString(FileSearchCategory));
		msg.setIsrecursively(true);
		setTSpattern(msg);
		msg.getHeader().setDisplayCommand(MessagePassing::Command::TextSearch);
		msg.getHeader().setTextSearch(toStdString(hTSTextBox->Text));
		if (TSPEndPointCatogry->SelectedItems->Count > 0){
			if(TSPEndPointCatogry->SelectedItems->Count > 1)
			System::Windows::MessageBox::Show("Searching from multiple End Points ", "");
			for each (String^ value in TSPEndPointCatogry->SelectedItems){
				msg.getHeader().setDstIPAndPort("localhost", toStdString(value));
				pSendr_->postMessage(msg);
				hStatus->Text = "Fetching Files Matching Text/Regex.....";
				countTextSearch++;
			}
		}
		else
			hStatus->Text = "Please Select Atleast one End Point";
	}
	else
		hStatus->Text = "Connection has not been established";
}

//----< Loading Window >----------------------------------------
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}

//----< UnLoading Window >----------------------------------------
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPF Client ");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}