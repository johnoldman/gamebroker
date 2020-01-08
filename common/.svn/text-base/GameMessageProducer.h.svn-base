//
// $Id: GameMessageProducer.h 3550 2007-12-13 14:26:04Z johno $
//
#ifndef GAME_MESSAGE_PRODUCER_H_
#define GAME_MESSAGE_PRODUCER_H_

#include <string>

class GameMessageProducer
{
public:
   virtual ~GameMessageProducer() {}
   // send message <message> with tag <tag> (extra data, interpreted as JMSXGroupID + part of JMSReplyTo in JMS)
   virtual void sendMessage(const std::string& message, const std::string& tag) = 0;
   virtual bool isSafeToShutdown() const = 0;
};

#endif
