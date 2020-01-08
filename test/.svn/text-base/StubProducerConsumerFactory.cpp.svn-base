//
// $Id: StubProducerConsumerFactory.cpp 3761 2007-12-19 11:39:52Z stephend $
//

#include "StubProducerConsumerFactory.h"

StubProducerConsumerFactory::StubProducerConsumerFactory() : pLastProducer(0), pLastButOneProducer(0), pLastButTwoProducer(0)
, m_producerCount(0)
, m_consumerCount(0)
{}

std::auto_ptr<GameMessageConsumer> StubProducerConsumerFactory::createConsumer(const std::string& gameSessionId) const
{
   ++m_consumerCount;
   return std::auto_ptr<GameMessageConsumer>(new StubMessageConsumer);
}

std::auto_ptr<GameMessageProducer> StubProducerConsumerFactory::createProducer(const std::string& gameSessionId) const
{
   ++m_producerCount;
   pLastButTwoProducer = pLastButOneProducer;
   pLastButOneProducer = pLastProducer;
   pLastProducer = new StubProducer(gameSessionId);
   return std::auto_ptr<GameMessageProducer>(pLastProducer);
}

int StubProducerConsumerFactory::getNumberOfConsumers() const
{
   return m_consumerCount;
}
