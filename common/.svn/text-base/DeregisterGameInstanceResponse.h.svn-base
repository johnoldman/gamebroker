#ifndef _DEREGISTER_GAME_INSTANCE_RESPONSE_H_
#define _DEREGISTER_GAME_INSTANCE_RESPONSE_H_
//
// $Id: DeregisterGameInstanceResponse.h 3484 2007-12-12 11:57:57Z stephend $
//
#include "GameMessageProducer.h"
#include "ServiceBrokerResponse.h"

class ResponseMessageFactory;

class DeregisterGameInstanceResponse : public ServiceBrokerResponse
{
public:
   explicit DeregisterGameInstanceResponse(XmlIterator& xmlResponse);
   DeregisterGameInstanceResponse(const std::string& messageId);
   DeregisterGameInstanceResponse(const std::string& messageId,               
                         const enum SglGameAPI::SglServerExceptionType& errorCode,
                         const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
   void send(GameMessageProducer& producer) const;   
};

#endif
