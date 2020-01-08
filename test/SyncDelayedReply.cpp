//
// $Id: SyncDelayedReply.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "SyncDelayedReply.h"

#include "ActiveMqConsumerHandler.h"

SyncDelayedReply::SyncDelayedReply(ActiveMqConsumerHandler &gameClient, unsigned int sleepTime)
: m_GameClient(gameClient)
, askedToStop(false)
, m_sleepTime(sleepTime)
{
}

SyncDelayedReply::~SyncDelayedReply()
{
   askedToStop.set(true);
   threadSafeQueue.kick();
   if (sendThread.get())
   {
      sendThread->join();
   }
}

void SyncDelayedReply::sendMessage(const std::string &message)
{
   if (!sendThread.get())
   {
      sendThread = std::auto_ptr<boost::thread>(new boost::thread(boost::bind( &SyncDelayedReply::runThread, this )));
   }
   threadSafeQueue.write(message);
}

void SyncDelayedReply::runThread()
{
   while(!askedToStop)
   {
      try 
      {
         std::string sendMessage = threadSafeQueue.blockingRead();
         sleep(m_sleepTime);      
         m_GameClient.receive(sendMessage, time(NULL)); 
      }
      // Need to prevent exception terminating thread.
      catch (...)
      {
      }
   }
}
