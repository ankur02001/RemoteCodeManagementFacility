/////////////////////////////////////////////////////////////////////
//  Sender.cpp -  Sender Information end point in blocking call    //
//                                                                 //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell, Windows 8.1                               //
//  Application:   Scope analysis, OOD Assignment 3, Spring2015    //
//  Author:		   Ankur Pandey                                    //
/////////////////////////////////////////////////////////////////////

#include "Sender.h"
using namespace  RemoteCodeManagementFacility;
using namespace ApplicationHelpers;

//-------<  send the message string  >--------------------
bool Sender::sendString(Message msg){
	MsgHeader hdr = msg.getHeader();
	std::string cmd = hdr.getCommand();
	Command cmd_val = Command::NONE;
	if (checkCommandNULL(cmd))
		cmd_val = static_cast<Command>(atoi(cmd.c_str()));
	if (UploadFile == cmd_val){
		if (msg.getHeader().getFileName() == "") return false;
		if (!sendBlock(msg)){
			disp.printHeader("Thread Stopped. Not able to send Data");
			return false;
		}
		msg.getHeader().setCommand(FileEnded);
		if (!SocketCon_->sendString(msg.constructHeader(), '\n'))
			return false;
		disp.printHeader("Ended sending of file: " + Path::getName(msg.getHeader().getFileName()) + "");
		disp.printHeader("Waiting for Ack Received from IP: " + msg.getHeader().getSrcIP() + " And File Name : " + Path::getName(msg.getHeader().getFileName()));
	}
	if (TextSearch == cmd_val ||
		GetFilesList == cmd_val ||
		DownLoadFile == cmd_val ||
		FileSearch == cmd_val ||
		Acknowledge == cmd_val){
		disp.printHeader(" Sending Command to Reciever for Processing ");
		if (!SocketCon_->sendString(msg.constructHeader(), '\n')){
			disp.printHeader("Thread Stopped. Not able to send Data");
			return false;
		}
	}

	return true;
}

//-------<  Connect to specified ip and port >--------------------------------
bool Sender::connect(std::string ip, std::string port){
	try{
		ApplicationHelpers::Verbose v(true);
		if (!SocketCon_->connect(ip, atoi(port.c_str())))
		{
			disp.printHeader("Sender couldn't Connect to IP: " + ip + " Port:" + port + "");
			return false;
		}
		dstPort = port;
	}
	catch (std::exception e){
		disp.printHeader(e.what());
		return false;
	}
	return true;
}

//-------< stopping sender   >------------------------------------------------------
void Sender::stop()
{
	Message messg;
	messg.getHeader().setCommand(StopSending);
	queue_.enQ(messg);
}

//-------< Wait sender to compelete    >-------------------------------------------
void Sender::wait(){
	t->join();
}

//-------< destructor  >-----------------------------------------------------------
Sender::~Sender(){
	delete t;
}

//-------< Starting Sender thread over run method >--------------------------------
void Sender::start()
{
	try{
		t = new std::thread(&Sender::run, this);
	}
	catch (std::exception e){
		disp.printHeader(e.what());
	}
}

//-------< Constructs the instance of the Socket  >--------------------------------
Sender::Sender() {
	SocketCon_ = new SocketConnecter();
}

//-------< message send to blocking queue >----------------------------------
void Sender::postMessage(const Message& msg){
	queue_.enQ(msg);
}

//--------< Check Command is Null or not >-----------------------------------
bool Sender::checkCommandNULL(std::string cmd){
	if (cmd != "0"){
		return true;
	}
	return false;
}

//-------< Return Queue Reference >----------- ------------------------------
Sender::BQueue&  Sender::queue() {
	return queue_;
}

//-------< Run the Sender thread >-------------------------------------------
void Sender::run(){
	try{
		do{
			Message msg = queue_.deQ();
			MsgHeader hdr = msg.getHeader();
			std::string cmd = hdr.getCommand();
			if (dstPort != msg.getHeader().getDstPort())
				if (!connect(msg.getHeader().getDstIP(), msg.getHeader().getDstPort()))
					continue;
			dstPort = msg.getHeader().getDstPort();
			Command cmd_val = Command::NONE;
			if (checkCommandNULL(cmd))
				cmd_val = static_cast<Command>(atoi(cmd.c_str()));
			sendString(msg);
			if (StopSending == cmd_val){
				disp.printHeader("Stopping Sender");
				break;
			}
			::Sleep(100);
		} while (true);
	}
	catch (std::exception e){
		e.what();
	}
}

//-------< Sends blocks of file to the socket >---------------------------------------
bool Sender::sendBlock(Message& msg){
	disp.printHeader("Started Sending File: " + msg.getHeader().getFileName() + " to Destination IP : " +
		msg.getHeader().getDstIP() + " And port :" + msg.getHeader().getDstPort());
	MsgHeader hdr = msg.getHeader();
	std::string path = hdr.getFileName();
	File file(path);
	FileInfo fileinfo(path);
	std::string filename = fileinfo.name();
	std::ifstream ifstream(path.c_str(), std::ios::in | std::ios::binary);
	ifstream.seekg(0, std::ios::beg);
	size_t end_byte = 0;
	bool open = ifstream.is_open();
	if (!open){
		disp.printHeader("Unable to open file");
		return false;
	}
	while (open && ifstream.good() && !ifstream.eof()){
		char *block = new char[1024];
		size_t i = 0;
		for (i = 0; i <= 1024; i++)
			block[i] = '\0';
		size_t bytesRead = 0;
		if (block != NULL){
			ifstream.read((char*)block, 1024);
			bytesRead = static_cast<int> (ifstream.gcount());
			std::ostringstream convert;
			convert << bytesRead;
			end_byte = bytesRead;
			msg.getHeader().setBodylength(convert.str());
			if (!SocketCon_->sendString(msg.constructHeader(), '\n')){
				disp.printHeader("Thread Stopped. Not able to send Data");
				break;
			}::Sleep(100);
			if (!SocketCon_->send(bytesRead, block)){
				disp.printHeader("Thread Stopped. Not able to send Data");
				break;
			}::Sleep(100);
		}
	}
	ifstream.close();
	::Sleep(100);
	std::ostringstream convert;
	convert << end_byte;
	msg.getHeader().setBodylength(convert.str());
	return true;
}

#ifdef TEST_SENDER
//-------< Sender main test >---------------------------------------
int main()
{
	int ret = 0;
	Display disp;
	try
	{
		Verbose v(true);
		std::cout << "\n\n ########################## TESTING SENDER PACKAGE #################################### \n\n";
		SocketSystem ss;
		Message messg;
		std::string srcAddr = "127.0.0.1", targetPort = "8484", targetAddr = "localhost", srcPort = "9080";
		std::cout << "\n\n ########################## TESTING UPLOAD FILE #################################### \n\n";
		Message messg2;
		messg2.getHeader().setCommand(UploadFile);
		messg2.getHeader().setDstIPAndPort(targetAddr, targetPort);
		messg2.getHeader().setSrcIPAndPort(targetAddr, srcPort);
		Sender sender2;
		std::string path = "..\\..\\TEST_FOLDER\\TestImage.png";
		messg2.getHeader().setFileNameAndType(path, text);
		int connectCount = 0;
		while (!sender2.connect("localhost", "8484")){
			disp.printString("Couldnot connect to server");
			::Sleep(100);
			connectCount++;
		}
		sender2.postMessage(messg2);
		sender2.start();
		sender2.wait();
		disp.printString("Done Executing");
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		return 1;
	}
	return ret;
}
#endif