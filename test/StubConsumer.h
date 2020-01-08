//
// $Id: StubConsumer.h 2982 2007-11-21 10:05:17Z stephend $
//
#include <vector>

#include <boost/thread/mutex.hpp>

#include "ActiveMqConsumerHandler.h"

class StubConsumer : public ActiveMqConsumerHandler
{
public:
   StubConsumer();
   // ActiveMqConsumerHandler
   void receive(const std::string& message, const time_t& eventTime);
   void error(const std::string& message);
   // stub methods
   void setThrow(bool bThrow = true);
   void setThrowInt();
public:
   size_t getMessageCount() const;
   std::string getMessage(size_t index) const;
   void clear();
private:
   mutable boost::mutex m_mutex;
   std::vector<std::string> m_messages;
   bool m_bThrow;
   bool m_bThrowInt;
};
