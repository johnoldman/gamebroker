//
// $Id: SyncDelayedReply.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef SYNC_DELAYED_REPLY_H_
#define SYNC_DELAYED_REPLY_H_
// 
// Class to send message to a ActiveMqConsumerHandler after a configurable
// delay of sleepTime seconds. Allows test code to transmit an expected
// reply before sending the request.
//
#include <string>
#include <boost/thread.hpp>

#include "ThreadSafeQueue.h"
#include "ThreadSafeBool.h"

class ActiveMqConsumerHandler;

class SyncDelayedReply
{
public:
   SyncDelayedReply(ActiveMqConsumerHandler &gameClient, unsigned int sleepTime = 1);
   ~SyncDelayedReply();
   void sendMessage(const std::string &message);
private:
   void runThread();
private:
   ThreadSafeQueue<std::string> threadSafeQueue;
   ActiveMqConsumerHandler& m_GameClient;
   std::auto_ptr<boost::thread> sendThread;
   ThreadSafeBool askedToStop;
   const unsigned int m_sleepTime;
};

#endif
