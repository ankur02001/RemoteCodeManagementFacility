/////////////////////////////////////////////////////////////////////
//  ExecutiveServer.cpp - Execute Server side                      //
//                                                                 //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////
#include "ExecutiveServer.h"
#include <string>
#include <thread>
#include <iostream>

using namespace RemoteCodeManagementFacility;

//----< Check Command is Null or not >-------------------------------
bool ExecutiveServer::checkCommandNULL(std::string cmd){
	if (cmd != "0"){
		return true;
	}
	return false;
}

//----< ExecutiveServer Constructor >--------------------------------------------------
ExecutiveServer::ExecutiveServer(size_t srcport, std::string targetport, Socket::IpVer ipv) :pRecvr_(new Receiver(srcport)),
pSendr_(new Sender()), sourceIP("localhost"), sourcePort(srcport), 
ss(new SocketSystem()){
	bool stop_ = false;
}

//----< ExecutiveServer Destructor >--------------------------------------------------
ExecutiveServer::~ExecutiveServer(){
	delete pSendr_;
	delete pRecvr_;
	delete ss;
	delete thread_;
}

//----< Run Server >--------------------------------------------------
void ExecutiveServer::run()
{
	Display disp;
	while (true)
	{
		Message msg = pRecvr_->getMessage(); 
		std::string cmd = msg.getHeader().getCommand();
		Command cmd_val = Command::NONE;
		if (checkCommandNULL(cmd))
			cmd_val = static_cast<Command>(atoi(cmd.c_str()));
		if (cmd_val != Command::NONE){
			while (!pSendr_->connect(msg.getHeader().getDstIP(), msg.getHeader().getDstPort())){
				display.printHeader(" Not able to connect to Client Port" + msg.getHeader().getDstPort());
				::Sleep(100);
			}
			disp.printHeader("Passing Message to Sender for transfer to Client ");
			pSendr_->postMessage(msg);
		}
	}
}

//-------<Starts the receiver and sender>------------------------------
void ExecutiveServer::start(){
	thread_ = new thread(&ExecutiveServer::run, this);
	thread_->detach();
	pRecvr_->connect();
	pSendr_->start();
	pSendr_->wait();
}

//----< Pass Message >-----------------------------------------------
void ExecutiveServer::passMessage(Message message){
	msg_ = message;
}

//----< signal server thread to stop >--------------------------------
void ExecutiveServer::stop(){
	::Sleep(3000);
	pSendr_->stop();
}

#ifdef TEST_EXECSERVER
//----< Calling Client Main >-------------------------------------------------------
int main(int argc, char* argv[]){
	try{
		MessagePassing::Display display;
		Verbose v(true);
		MessagePassing::Message msg;
		if (argc < 5){
			display.printHeader("Invalid Entry, Please provide Command, SourcePort, Destination Port and File Name \n");
			return 0;
		}
		std::string message;
		for (int i = 0; i < argc; i++){
			message += argv[i];
			message += ";";
		}
		msg.messageInterpret(message);
		if ((msg.getHeader().getSrcPort() == "") || msg.getHeader().getDstPort() == "" || msg.getHeader().getFileName() == ""){
			display.printHeader("Invalid number of input \n");
			return 0;
		}
		std::string target_Port = msg.getHeader().getDstPort();
		size_t srcPortt = stoi(msg.getHeader().getSrcPort());
		string FileName = msg.getHeader().getFileName();
		ExecutiveServer s(srcPortt, target_Port, Socket::IpVer::IP4);
		s.start();
		ApplicationHelpers::Cosmetic cosmetic;
		::Sleep(2000);
		s.stop();
	}catch (std::exception& ex){
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	return 0;
}
#endif