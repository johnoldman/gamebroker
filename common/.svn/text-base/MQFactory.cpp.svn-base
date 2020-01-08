//
// $Id: MQFactory.cpp 3720 2007-12-18 10:10:03Z stephend $
//
#include "MQFactory.h"
#include "MQConsumer.h"
#include "MQProducer.h"

MQFactory::MQFactory(const std::string& brokerURI, const std::string& producerQueue, const std::string& consumerQueue, Logger& logger)
: brokerURI_(brokerURI)
, producerQueue_(producerQueue)
, consumerQueue_(consumerQueue)
, logger_(logger)
{
}
//
// Both create() factory methods put the queueName last. This is deliberate, as Active/MQ allows queue names to be extended
// with URL-like syntax e.g. FRED.JIM?consumer.retroactive=true. This policy allows such url-type syntax to be usable in the
// configuration file(s) for queue names.
//
std::auto_ptr<GameMessageConsumer> MQFactory::createConsumer(const std::string& gameSessionId) const
{
   return std::auto_ptr<GameMessageConsumer>(new MQConsumer(brokerURI_, gameSessionId + consumerQueue_ , logger_));
}

std::auto_ptr<GameMessageProducer> MQFactory::createProducer(const std::string& gameSessionId) const
{
   return std::auto_ptr<GameMessageProducer>(new MQProducer(brokerURI_, gameSessionId + producerQueue_, consumerQueue_,  logger_));
}
