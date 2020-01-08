//
// $Id: ProducerConsumerFactory.cpp 3541 2007-12-13 13:14:59Z stephend $
//
#include "ProducerConsumerFactory.h"

boost::shared_ptr<GameMessageConsumer> ProducerConsumerFactory::createSharedConsumer(const std::string& gameSessionId) const
{
   return boost::shared_ptr<GameMessageConsumer>(createConsumer(gameSessionId).release());
}

boost::shared_ptr<GameMessageProducer> ProducerConsumerFactory::createSharedProducer(const std::string& gameSessionId) const
{
   return boost::shared_ptr<GameMessageProducer>(createProducer(gameSessionId).release());
}
