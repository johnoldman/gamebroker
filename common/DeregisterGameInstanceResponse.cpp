//
// $Id: DeregisterGameInstanceResponse.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "DeregisterGameInstanceResponse.h"
#include "GameMessageProducer.h"
#include "XmlIterator.h"

std::string DeregisterGameInstanceResponse::getTag()
{
   return "DGI";
}

std::string DeregisterGameInstanceResponse::getTla() const
{
   return getTag();
}

DeregisterGameInstanceResponse::DeregisterGameInstanceResponse(XmlIterator& xmlResponse)
: ServiceBrokerResponse(xmlResponse)
{
}

DeregisterGameInstanceResponse::DeregisterGameInstanceResponse(const std::string& messageId)
: ServiceBrokerResponse(messageId)
{
}

DeregisterGameInstanceResponse::DeregisterGameInstanceResponse(const std::string& messageId,               
                                const enum SglGameAPI::SglServerExceptionType& errorCode,
                                const std::string& errorString)
: ServiceBrokerResponse(messageId, errorCode, errorString)
{
}

void DeregisterGameInstanceResponse::send(GameMessageProducer& producer) const
{
   // serialise
   producer.sendMessage(serialiseResponse(), "");
}
