//
// $Id: ApiCommandDeregisterGameInstance.cpp 2982 2007-11-21 10:05:17Z stephend $
//
#include "ApiCommandDeregisterGameInstance.h"
#include "GameMessageProducer.h"
#include "DeregisterGameInstanceResponse.h"
#include "XmlIterator.h"

std::string ApiCommandDeregisterGameInstance::getTag()
{
   return "DGI";
}

std::string ApiCommandDeregisterGameInstance::getTla() const
{
   return getTag();
}

ApiCommandDeregisterGameInstance::ApiCommandDeregisterGameInstance(const std::string &gameSessionId)
: ApiCommandWithGameSessionId(gameSessionId, gameSessionId) // gameSessionId is also messageId
{
}

// SPD - Dummy implementation
ApiCommandDeregisterGameInstance::ApiCommandDeregisterGameInstance(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandDeregisterGameInstance::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const 
{
   return std::auto_ptr<ServiceBrokerResponse>(new DeregisterGameInstanceResponse(getMessageId(), e, message));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandDeregisterGameInstance::makeAckResponse() const 
{
   return std::auto_ptr<ServiceBrokerResponse>(new DeregisterGameInstanceResponse(getMessageId()));
}

void ApiCommandDeregisterGameInstance::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId();

    sendMessage(producer, serialised);
}
