//
// $Id: ActiveMqConsumerHandler.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef MQ_CONSUMER_HANDLER_HPP_
#define MQ_CONSUMER_HANDLER_HPP_

#include <string>

class ActiveMqConsumerHandler
{
public:
   virtual ~ActiveMqConsumerHandler() {}
   virtual void receive(const std::string &message, const time_t& eventTime) = 0;
   virtual void error(const std::string &message) = 0;
private:
};

#endif // MQ_CONSUMER_HANDLER_HPP_

