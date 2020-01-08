#ifndef _STUB_CONSUMERFACTORY_H_
#define _STUB_CONSUMERFACTORY_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include "MQFactory.h"
#include "StubConsumer.h"
#include "GameMessageConsumer.h"

class Logger;

class StubConsumerFactory : public MQFactory
{
public:
   StubConsumerFactory(const std::string& brokerURI, const std::string& subject, Logger& logger)
   :MQFactory(brokerURI, subject, logger)
   {}

   boost::shared_ptr<StubConsumer> create(const std::string& gameSessionId)
   {
      return boost::shared_ptr<StubConsumer>(new StubConsumer(getSubject() + gameSessionId));
   }
};

#endif
