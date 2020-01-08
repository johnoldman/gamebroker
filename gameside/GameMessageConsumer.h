//
// $Id: GameMessageConsumer.h 1989 2007-10-23 16:06:18Z stephend $
//
#ifndef GAME_MESSAGE_CONSUMER_H_
#define GAME_MESSAGE_CONSUMER_H_

#include <string>

class ActiveMqConsumerHandler;

class GameMessageConsumer
{
public:
   virtual ~GameMessageConsumer() {}
   virtual void setConsumer(ActiveMqConsumerHandler* pHandler) = 0;
   //virtual void commit() = 0;
};

#endif
