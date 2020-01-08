//
// $Id: MQFactory.h 3488 2007-12-12 12:40:44Z stephend $
//
#ifndef MQFACTORY_H_
#define MQFACTORY_H_

class Logger;

#include "ProducerConsumerFactory.h"

class MQFactory : public ProducerConsumerFactory
{
public:
   MQFactory(const std::string& brokerURI, const std::string& producerQueue, const std::string& consumerQueue, Logger& logger);
   std::auto_ptr<GameMessageConsumer> createConsumer(const std::string& gameSessionId) const;
   std::auto_ptr<GameMessageProducer> createProducer(const std::string& gameSessionId) const;
private:
   const std::string brokerURI_;
   const std::string producerQueue_;
   const std::string consumerQueue_;
   Logger& logger_;
};

#endif
