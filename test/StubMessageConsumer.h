//
// $Id: StubMessageConsumer.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef STUB_MESSAGE_CONSUMER_H_
#define STUB_MESSAGE_CONSUMER_H_

#include "GameMessageConsumer.h"

class StubMessageConsumer : public GameMessageConsumer
{
public:
   void setConsumer(ActiveMqConsumerHandler* pConsumer) {}
};

#endif
