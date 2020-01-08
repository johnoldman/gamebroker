//
// $Id: BeginUserSessionResponse.h 2791 2007-11-14 18:24:18Z stephend $
//
#ifndef _BEGIN_USER_SESSION_RESPONSE_H_
#define _BEGIN_USER_SESSION_RESPONSE_H_

#include <string>
#include "ServiceBrokerResponse.h"
#include "GameMessageProducer.h"
#include "SglExceptions.h"

class XmlIterator;
class ResponseMessageFactory;

class BeginUserSessionResponse : public ServiceBrokerResponse
{
public:
   explicit BeginUserSessionResponse(XmlIterator& xmlResponse);

   BeginUserSessionResponse(const std::string& messageId,               
            long purseTotal);

   BeginUserSessionResponse(const std::string& messageId,               
            const enum SglGameAPI::SglServerExceptionType& errorCode,
            const std::string& errorString);

   long getPurseTotal() const;
   static std::string getTag();
   void send(GameMessageProducer& producer) const;
   std::string getTla() const;
private:
   long m_purseTotal;
   std::string serialPurseTotal;
};

#endif

