/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////
/*
Module Operations :
== == == == == == == == ==
This module defines a class that provides services necessary to for MockChannel Creation.
Contains MockChannel for Connecting Sender And Reciever for CLient

Public Interface:
=======================
MockChannel(ISendr* pSendr, IRecvr* pRecvr);                 // pass pointers to Sender and Receiver
void start();                                                // creates thread to read from sendQ and echo back to the recvQ
void stop();                                                 // signal server thread to stop
void postMessage(Message msg);                               // Pass Message to Sender
void getMessage(Message& msg);                               // Get Message from Receiver

Build Process :
=======================
Required files
- MockChannel.cpp, MockChannel.cpp
- Message.h, Message.cpp
- Sender.h, Sender.cpp
- Reciever.h, Reciever.cpp
- Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp

Build commands
========================
- devenv MockChannel.sln

Maintenance History :
========================
ver 1.0 : 28 Apr 2015
- first release
*/
#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../Sender/Sender.h"
#include "../Reciever/Reciever.h"
#include "../Message/Message.h"

using namespace RemoteCodeManagementFacility;

/////////////////////////////////////////////////////////////////////////////
// MockChannel for Connecting Sender And Reciever for CLient
////////////////////////////////////////////////////////////////////////////
class MockChannel : public IMockChannel
{
public:
  using BQueue = BlockingQueue < MessagePassing::Message >;
  
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);                 // pass pointers to Sender and Receiver
  void start();                                                // creates thread to read from sendQ and echo back to the recvQ 
  void stop();                                                 // signal server thread to stop
  void postMessage(MessagePassing::Message msg);                               // Pass Message to Sender
  void getMessage(MessagePassing::Message& msg);                                        // Get Message from Receiver

private:
  SocketSystem* ss;
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  size_t src_port;
  size_t targetport;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------
MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {
	ss  =new SocketSystem;
}

//----< creates thread to read from sendQ and echo back to the recvQ >-------
void MockChannel::start()
{
	Display disp;
	disp.printHeader(" MockChannel starting up");
  thread_ = std::thread(
    [this] {
	  Sender* pSendr = dynamic_cast<Sender*>(pISendr_);
	  Receiver* pRecvr = dynamic_cast<Receiver*>(pIRecvr_);
	pRecvr->connect();
	::Sleep(100);
	pSendr->start();
	pSendr->wait();
	if (stop_){
		return;
	}
	if (pSendr == nullptr || pRecvr == nullptr){
		Display disp;
		disp.printHeader(" failed to start Mock Channel");
		return;
	}
 
  });
  ::Sleep(400);
}

//----< Pass Message to Sender  >--------------------------------------------------
void MockChannel::postMessage(MessagePassing::Message msg){
	::Sleep(100);
	pISendr_->postMessage(msg);
}

//----< Get Message from Receiver  >--------------------------------------------------
void MockChannel::getMessage(MessagePassing::Message& msg){
	::Sleep(100);
	msg= pIRecvr_->getMessage();
}

//----< signal server thread to stop >---------------------------------------------
void MockChannel::stop() { 
	pISendr_->stop();
	stop_ = true; 
}

//----< factory functions createSendr >--------------------------------------------------
ISendr* ObjectFactory::createSendr() { 
	return new Sender; 
}

//----< factory functions createRecvr >--------------------------------------------------
IRecvr* ObjectFactory::createRecvr() { 
	return new Receiver; 
}

//----< factory functions createMockChannel >--------------------------------------------------
IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  Message messg;
  std::string srcAddr = "127.0.0.1", srcPort = "8484", targetAddr = "127.0.0.1", targetPort = "8181";
  messg.getHeader().setCommand(MSGSEND);
  messg.getHeader().setDstIPAndPort(targetAddr, targetPort);
  messg.setMessage(" Hello I am sender");
  pSendr->postMessage(messg);
  pSendr->postMessage(messg);
  Message msg = pRecvr->getMessage();
  msg = pRecvr->getMessage();
  msg = pRecvr->getMessage();
  pMockChannel->stop();
  std::cin.get();
}
#endif
