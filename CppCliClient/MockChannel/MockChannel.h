#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                 //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////
/*
Module Operations :
== == == == == == == == ==
This module defines a class that provides services necessary to for MockChannel Creation.

Public Interface:
== == == == == == == == =
ISendr Interface for Sender Class
IRecvr Interface for Reciver Class
IMockChannel Interface for MockChannel Class
ObjectFactory for creation IMockChannel

Build Process :
=======================
Required files
- MockChannel.cpp, MockChannel.cpp
- Message.h, Message.cpp

Build commands
========================
- devenv MockChannel.sln

Maintenance History :
========================
ver 1.0 : 28 Apr 2015
- first release
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include "../Message/Message.h"

/////////////////////////////////////////////////////////////////////////////
// ISendr Interface for Sender Class
////////////////////////////////////////////////////////////////////////////
struct ISendr
{
	virtual bool connect(std::string ip, std::string port)=0;
	virtual void stop()=0;
	virtual void postMessage(const MessagePassing::Message& msg) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// IRecvr Interface for Reciver Class
////////////////////////////////////////////////////////////////////////////
struct IRecvr
{
	virtual void setPort(size_t portno)=0;
	virtual void listen(int port)=0;
	virtual MessagePassing::Message getMessage() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// IMockChannel Interface for MockChannel Class
////////////////////////////////////////////////////////////////////////////
struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual void postMessage(MessagePassing::Message msg) = 0;
  virtual void getMessage(MessagePassing::Message& msg)=0;
};

/////////////////////////////////////////////////////////////////////////////
// ObjectFactory for creation IMockChannel
////////////////////////////////////////////////////////////////////////////
extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


