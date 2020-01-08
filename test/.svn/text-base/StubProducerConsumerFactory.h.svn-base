//
// $Id: StubProducerConsumerFactory.h 3528 2007-12-13 11:13:57Z stephend $
//
#ifndef STUB_CONSUMERFACTORY_H_
#define STUB_CONSUMERFACTORY_H_

#include <map>
#include <string>
#include "ProducerConsumerFactory.h"
#include "StubMessageConsumer.h"
#include "StubProducer.h"

class Logger;

class StubProducerConsumerFactory : public ProducerConsumerFactory
{
public:
   mutable StubProducer* pLastProducer;
   mutable StubProducer* pLastButOneProducer;
   mutable StubProducer* pLastButTwoProducer;
   mutable int m_producerCount;
public:
   StubProducerConsumerFactory();
   std::auto_ptr<GameMessageConsumer> createConsumer(const std::string& gameSessionId) const;
   std::auto_ptr<GameMessageProducer> createProducer(const std::string& gameSessionId) const;
   int getNumberOfConsumers() const;
private:
   mutable int m_consumerCount;
};

#endif
