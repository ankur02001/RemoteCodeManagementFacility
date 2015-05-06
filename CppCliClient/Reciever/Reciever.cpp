/////////////////////////////////////////////////////////////////////
//  Reciever.cpp -  Recieves Information end point in blocking call//
//                And send back to client                          //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include "Reciever.h"

using namespace ApplicationHelpers;
using namespace MessagePassing;
using namespace  RemoteCodeManagementFacility;

//-------< Checking Command is None or not >-------------------------
int ClientHandler::checkCommandNULL(std::string cmd){
	if (cmd != "0"){
		return true;
	}
	return false;
}

//-------< Process text Search >-------------------------------------
void ClientHandler::processTextSearch(Socket& socket_, Message& message){
	disp.printHeader(" Processing  Text Search Command for Client " + message.getHeader().getSrcPort() + " Dst="+
		message.getHeader().getDstPort());
	std::string srcPort = message.getHeader().getDstPort();
	std::string catogariespath_ = filePath + "_" + message.getHeader().getDstPort();
	FileMgr fileManager;
	message.getHeader().setCommand(Command::Acknowledge);
	message.getHeader().setDisplayCommand(Command::TextSearch);
	message.getHeader().setDstIPAndPort(message.getHeader().getSrcIP(), message.getHeader().getSrcPort());
	message.getHeader().setSrcIPAndPort(message.getHeader().getDstIP(), message.getHeader().getDstPort());
	std::vector<std::string> pattern_ = message.getPattern();
	if (pattern_.empty()){
		pattern_.push_back("*.*");
	}
	bool isRecursive = (message.getIsRecursively() == 1) ? true : false;
	fileManager.search(catogariespath_, pattern_, isRecursive);
	Cataloger Catalog(fileManager);
	std::vector< ElementDisplay> printFileStore =
		Catalog.searchUserText(pattern_, message.getHeader().getTextSearch());
	std::string msgBody = "";
	for (auto filesCat : printFileStore){
		msgBody += "@Text Found on " + srcPort + "@";
		msgBody += filesCat.getFileAndPaths();
	}
	msgBody += "@";
	message.setMessage(msgBody);
	bq_.enQ(message);
	disp.printHeader(" Sending Text Searched to Client" + message.getHeader().getDstPort());
}

//-------< Process File Search >--------------------------------------------
void ClientHandler::processFileSearch(Socket& socket_, Message& message){
	disp.printHeader(" Processing  Files Search Command From Client " + message.getHeader().getSrcPort());
	std::string filePath_ = filePath + "_" + message.getHeader().getDstPort();
	FileMgr fileManager;
	std::string catogariespath_ = getCatogryPath(message);
	message.getHeader().setCommand(Command::Acknowledge);
	message.getHeader().setDisplayCommand(Command::FileSearch);
	message.getHeader().setDstIPAndPort(message.getHeader().getSrcIP(), message.getHeader().getSrcPort());
	message.getHeader().setSrcIPAndPort(message.getHeader().getDstIP(), message.getHeader().getDstPort());
	std::vector<std::string> pattern_ = message.getPattern();
	if (pattern_.empty())
		pattern_.push_back("*.*");
	bool isRecursive = (message.getIsRecursively() == 1) ? true : false;
	fileManager.search(catogariespath_, pattern_, isRecursive);
	Cataloger Catalog(fileManager);
	std::vector< ElementDisplay> printFileStore = Catalog.printFileStore();
	std::string msgBody = "@";
	for (auto filesCat : printFileStore)
		msgBody += filesCat.getFileAndPaths();
	msgBody += "@";
	message.setMessage(msgBody);
	bq_.enQ(message);
	disp.printHeader(" Sending File Searched to Client" + message.getHeader().getDstPort());
}

//-------< Process Get Catogry Path from Extension >------------------------------
std::string ClientHandler::getCatogryPathFromExt(Message& message){
	std::string filePath_ = filePath + "_" + message.getHeader().getDstPort();
	std::string catogariespath_ = filePath_;
	if (message.getCatogries() == "ALL")
	{
		catogariespath_ = filePath_;
		std::string fileExt = FileSystem::Path::getExt(message.getHeader().getFileName());
		if (fileExt == "cpp" || fileExt == "h" || fileExt == "vcxproj")
			catogariespath_ = filePath_ + "//" + "CATO_CPP";
		if (fileExt == "txt")
			catogariespath_ = filePath_ + "//" + "CATO_TEXT";
		if (fileExt == "java")
			catogariespath_ = filePath_ + "//" + "CATO_JAVA";
		if (fileExt == "doc" || fileExt == "docx")
			catogariespath_ = filePath_ + "//" + "CATO_DOC";
		if (fileExt == "png")
			catogariespath_ = filePath_ + "//" + "CATO_PNG";
	}
	return catogariespath_;
}

//-------< Process Get Catogry Path >------------------------------
std::string ClientHandler::getCatogryPath(Message& message){
	std::string filePath_ = filePath + "_" + message.getHeader().getDstPort();
	std::string catogariespath_ = filePath_;
	if (message.getCatogries() == "ALL"){
		catogariespath_ = filePath_;
	}
	if (message.getCatogries() == "CPP")
		catogariespath_ = filePath_ + "//" + "CATO_CPP";

	if (message.getCatogries() == "TEXT")
		catogariespath_ = filePath_ + "//" + "CATO_TEXT";

	if (message.getCatogries() == "JAVA")
		catogariespath_ = filePath_ + "//" + "CATO_JAVA";

	if (message.getCatogries() == "DOC")
		catogariespath_ = filePath_ + "//" + "CATO_DOC";

	if (message.getCatogries() == "PNG")
		catogariespath_ = filePath_ + "//" + "CATO_PNG";

	return catogariespath_;
}

//-------< Process Get Catogry Pattern >------------------------------
std::vector<std::string> ClientHandler::getCatogryPattern(Message& message){
	std::vector<std::string> pattern_;
	if (message.getCatogries() == "ALL")
		pattern_.push_back("*.*");
	if (message.getCatogries() == "CPP"){
		pattern_.push_back("*.cpp");
		pattern_.push_back("*.h");
		pattern_.push_back("*.vcxproj");
	}
	if (message.getCatogries() == "TEXT")
		pattern_.push_back("*.txt");
	if (message.getCatogries() == "JAVA")
		pattern_.push_back("*.java");
	if (message.getCatogries() == "DOC"){
		pattern_.push_back("*.doc");
		pattern_.push_back("*.docx");
	}
	if (message.getCatogries() == "PNG")
		pattern_.push_back("*.png");
	return pattern_;
}

//-------< Process Get File Search >------------------------------
void ClientHandler::processGetFilesSearch(Socket& socket_, Message& message){
	disp.printHeader(" Processing Get Files recieved from Client " + message.getHeader().getSrcPort() + " from Server" + message.getHeader().getDstPort());
	std::string catogariespath_ = getCatogryPath(message);
	message.getHeader().setCommand(Command::Acknowledge);
	message.getHeader().setDisplayCommand(Command::GetFilesList);
	message.getHeader().setDstIPAndPort(message.getHeader().getSrcIP(), message.getHeader().getSrcPort());
	message.getHeader().setSrcIPAndPort(message.getHeader().getDstIP(), message.getHeader().getDstPort());
	std::vector<std::string> pattern_ = getCatogryPattern(message);
	std::string msgBody = "";
	FileMgr fileManager;
	bool isRecursive = (message.getIsRecursively() == 1) ? true : false;
	for (auto patt_ : pattern_){
		fileManager.searchFilesAddAppendToMsgBody(msgBody, catogariespath_, patt_, isRecursive);
	}
	message.setMessage(msgBody);
	bq_.enQ(message);
	disp.printHeader(" Returning Files for Download to Client " + message.getHeader().getDstPort() + " from Server" + message.getHeader().getSrcPort());
}

//-------< Process Download Search >------------------------------
void ClientHandler::processDownload(Socket& socket_, Message& message){
	disp.printHeader(" Downloading Command recieved from Client " + message.getHeader().getSrcPort() + " from Server" + message.getHeader().getDstPort() +
		" for File " + FileSystem::Path::getName(message.getHeader().getFileName()));
	Message msg;
	std::string catogariespath_ = getCatogryPath(message);
	msg.getHeader().setCommand(Command::UploadFile);
	msg.getHeader().setDisplayCommand(Command::DownLoadFile);
	msg.setCatogries(message.getCatogries());
	msg.getHeader().setDstIPAndPort(message.getHeader().getSrcIP(), message.getHeader().getSrcPort());
	msg.getHeader().setSrcIPAndPort(message.getHeader().getDstIP(), message.getHeader().getDstPort());
	FileMgr fileManager;
	std::string pattern = "*.*";
	std::string fileNameWithPath = "";
	fileManager.searchFilePath(catogariespath_, message.getHeader().getFileName(), pattern, true, fileNameWithPath);
	if (fileNameWithPath != ""){
		msg.getHeader().setFileNameAndType(fileNameWithPath, atoi(message.getHeader().getFileType().c_str()));
		bq_.enQ(msg);
	}
	disp.printHeader(" Downloading File Recieved from client " + message.getHeader().getDstPort() + " from Server" + message.getHeader().getSrcPort());
}

//-------< Process End of File >------------------------------
void ClientHandler::processFileEnd(Socket& socket_, Message& message){
	disp.printHeader(" File End Command recieved . Passing acknowledgement ");
	Message ack;
	ack.getHeader().setCommand(Acknowledge);
	ack.getHeader().setDstIPAndPort(message.getHeader().getSrcIP(), message.getHeader().getSrcPort());
	ack.getHeader().setSrcIPAndPort(message.getHeader().getDstIP(), message.getHeader().getDstPort());
	ack.getHeader().setFileNameAndType(message.getHeader().getFileName(),
		atoi(message.getHeader().getFileType().c_str()));

	if (stoi(message.getHeader().getDisplayCommand()) == Command::DownLoadFile){
		ack.getHeader().setDisplayCommand(DownLoadFile);
		ack.setMessage(" File = " + FileSystem::Path::getName(message.getHeader().getFileName()) +
			" is Download from = " + message.getHeader().getSrcIP() + " Port = "
			+ message.getHeader().getSrcPort());
	}
	else{
		ack.getHeader().setDisplayCommand(UploadFile);
		ack.setMessage(" File = " + FileSystem::Path::getName(message.getHeader().getFileName()) +
			" is uploade to server IP = "
			+ message.getHeader().getDstIP() + " Port = " + message.getHeader().getDstPort());
	}

	disp.printString("ACK sent back to " + message.getHeader().getSrcPort() + " for file: " +
		FileSystem::Path::getName(message.getHeader().getFileName()));
	bq_.enQ(ack);
}

//-------< Process Acknowledge  >------------------------------
void ClientHandler::processAcknowledge(Socket& socket_, Message& message){
	disp.printString(" Acknowledgement Received From : " + message.getHeader().getSrcIP());
	bq_.enQ(message);
}

//-------< Process the messsages of acknowledgement and End of File >------------------------------
void ClientHandler::processMessage(Socket& socket_, Message& message)
{
	std::string cmd = message.getHeader().getCommand();
	Command cmd_val = Command::NONE;
	if (checkCommandNULL(cmd))
		cmd_val = static_cast<Command>(atoi(cmd.c_str()));
	if (cmd_val == FileEnded)
		processFileEnd(socket_, message);

	if (cmd_val == Acknowledge)
		processAcknowledge(socket_, message);

	if (TextSearch == cmd_val)
		processTextSearch(socket_, message);

	if (FileSearch == cmd_val)
		processFileSearch(socket_, message);

	if (cmd_val == GetFilesList)
		processGetFilesSearch(socket_, message);

	if (cmd_val == DownLoadFile)
		processDownload(socket_, message);
}

//-------< Reads data from the Socket >------------------------------
char* ClientHandler::socketBufferHandler(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			break;
		char* buffer = new char[bufferSize_];
		::Sleep(100);
		if (socket_.recv(bufferSize_, buffer))
			return buffer;
		else
			return nullptr;
		if (socket_ == INVALID_SOCKET)
			return nullptr;
	}
	return nullptr;
}

//--------------< Writes the file to destination directory >--------------------
void ClientHandler::writeToFile(Message msg, char* byte){

	std::string path = msg.getHeader().getFileName();
	std::string filename = FileSystem::Path::getName(path);
	disp.printHeader("Writes the file = " + filename + " to " + msg.getHeader().getDstPort());
	std::string command = msg.getHeader().getCommand();
	std::string fileType = msg.getHeader().getFileType();
	std::ofstream ofstream;
	std::string filePath_ = filePath + "_" + msg.getHeader().getDstPort();
	std::string catogariesPath_ = filePath_;
	if (msg.getCatogries() == "ALL")
		catogariesPath_ = getCatogryPathFromExt(msg);
	else
		catogariesPath_ = getCatogryPath(msg);

	if (!(FileSystem::Directory::exists(filePath_)))
		FileSystem::Directory::create(filePath_);
	if (!(FileSystem::Directory::exists(catogariesPath_)))
		FileSystem::Directory::create(catogariesPath_);

	if (fileType == "2")
		ofstream.open(catogariesPath_ + "//" + filename, std::ios::out | std::ios::binary | std::ofstream::app | std::ofstream::ate);
	else if (fileType == "1")
		ofstream.open(catogariesPath_ + "//" + filename, std::ios::out | std::ios::binary | std::ofstream::app | std::ofstream::ate);
	if (ofstream.good()){
		ofstream.write(byte, stoi(msg.getHeader().getBodylength()));
		ofstream.close();
	}
	else{
		disp.printHeader("\n Unable to Write File");
	}
}

//-------< Constructor >------------------------------
Receiver::Receiver() :client_(queue_){}

//-------< Constructor >------------------------------
Receiver::Receiver(int port, Socket::IpVer ipv) : listener_(new SocketListener(port, ipv)),
client_(queue_), port_(port){
}

//-------< Enqueue the messages to the blocking queue >------------------------------
void  Receiver::passMessage(Message msg){
	queue_.enQ(msg);
}

//-------< Client Handler constructor  >---------------------------------------------
ClientHandler::ClientHandler(BlockingQueue<Message>& q) : bq_(q){
	filePath = ".//DIRECTORY";
}

//-------< connectting to the port >--------------------------------------------------
void Receiver::connect()
{
	::Sleep(1000);
	try{
		Verbose v(true);
		listener_->start(client_);
	}
	catch (std::exception ex){
		Verbose::show(ex.what());
	}
}

//-------< connectting to the port >-----------------------------------------------
Message Receiver::getMessage(){
	return queue_.deQ();
}

//-------< Returning Reciever Queue Reference >------------------------------------
Receiver::BQueue& Receiver::queue(){
	return queue_;
}

//-------< new listener  >-------------------------------------------------------
void Receiver::listen(int port){
	listener_ = new SocketListener(port, Socket::IP4);
}

//-------< setting port >--------------------------------------------------------
void Receiver::setPort(size_t port){
	port_ = port;
}


//-------< destructor >---------------------------------------------------------------
Receiver::~Receiver(){
}

//-------< Converts String to Integer >-----------------------------------------------
size_t ClientHandler::stringToInt(std::string str)
{
	std::istringstream iss(str);
	iss >> bufferSize_;
	return bufferSize_;
}

//----< Client Handler thread starts running this  >------------------------------------
void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		if (socket_.bytesWaiting() == 0)
			continue;
		::Sleep(100);
		Message messg;
		std::string recvMsg = socket_.recvString('\n');
		messg.messageInterpret(recvMsg);
		MsgHeader msgHeader = messg.getHeader();
		std::string cmd = msgHeader.getCommand();
		Command cmd_val = Command::NONE;
		if (checkCommandNULL(cmd))
			cmd_val = static_cast<Command>(atoi(cmd.c_str()));
		if (UploadFile == cmd_val){
			bufferSize_ = stringToInt(messg.getHeader().getBodylength());
			char* content = socketBufferHandler(socket_);
			std::string path = messg.getHeader().getFileName();
			std::string filename = FileSystem::Path::getName(path);
			disp.printString("Writing to File : " + filename + " Received from " + messg.getHeader().getSrcPort());
			writeToFile(messg, content);
			continue;
		}
		if (socket_ == INVALID_SOCKET)
			break;
		processMessage(socket_, messg);
	}
	disp.printHeader("ClientHandler socket connection closing");
	socket_.shutDown();
	socket_.close();
	disp.printHeader("ClientHandler thread terminating");
}

#ifdef TEST_RECIEVER
//-------< Reciever main test >---------------------------------------
int main()
{
	try
	{
		Verbose v(true);
		std::cout << "\n\n ########################## TESTING RECIEVER PACKAGE #################################### \n\n";
		SocketSystem ss;
		int clientPort = 8484;
		Receiver rerc(clientPort, Socket::IP6);
		rerc.setPort(clientPort);
		rerc.listen(clientPort);
		rerc.connect();
		Display display;
		display.printHeader("Press ank key:");
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Verbose::show("  Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif

