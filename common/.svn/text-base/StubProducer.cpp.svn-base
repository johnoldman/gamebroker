//
// $Id: StubProducer.cpp 3623 2007-12-14 13:43:29Z johno $
//
#include "StubProducer.h"

StubProducer::StubProducer(const std::string& name)
: m_name(name)
{
}

void StubProducer::sendMessage(const std::string& message, const std::string& tag)
{
    message_ = message;
    tag_ = tag;
}

std::string StubProducer::getMessage() const
{
    return message_;
}

std::string StubProducer::getTag() const
{
   return tag_;
}

void StubProducer::clearMessage()
{
    message_ = "";
    tag_ = "";
}

bool StubProducer::isSafeToShutdown() const
{
   // this toggles to show coverage in ServiceBroker::retireOldProducers()
   static bool toggleFlag = true;
   if(toggleFlag)
   {
      toggleFlag = false;
      return false;
   }
   else
   {
      toggleFlag = true;
      return true;
   }
}
