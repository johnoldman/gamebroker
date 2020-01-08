//
// $Id: StubProducer.h 3550 2007-12-13 14:26:04Z johno $
//
#ifndef STUB_PRODUCER_H_
#define STUB_PRODUCER_H_

#include "GameMessageProducer.h"

class StubProducer : public GameMessageProducer
{
public:
   explicit StubProducer(const std::string& name = "");
   void sendMessage(const std::string& message, const std::string& tag);
   std::string getMessage() const;
   std::string getTag() const;
   void clearMessage();
   std::string getName() const { return m_name; }
   bool isSafeToShutdown() const;
private:
   const std::string m_name;
   std::string message_;
   std::string tag_;
};

#endif
