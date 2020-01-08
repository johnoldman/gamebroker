//
// $Id: ProducerConsumerFactory.h 3541 2007-12-13 13:14:59Z stephend $
//
#ifndef PRODUCER_CONSUMER_FACTORY_H_
#define PRODUCER_CONSUMER_FACTORY_H_

#include <string>
#include <memory>
#include <boost/shared_ptr.hpp>

#include "GameMessageConsumer.h"
#include "GameMessageProducer.h"

class ProducerConsumerFactory
{
public:
   boost::shared_ptr<GameMessageConsumer> createSharedConsumer(const std::string& gameSessionId) const;
   boost::shared_ptr<GameMessageProducer> createSharedProducer(const std::string& gameSessionId) const;
   virtual std::auto_ptr<GameMessageConsumer> createConsumer(const std::string& gameSessionId) const = 0;
   virtual std::auto_ptr<GameMessageProducer> createProducer(const std::string& gameSessionId) const = 0;
   virtual ~ProducerConsumerFactory(){}
};

#endif
