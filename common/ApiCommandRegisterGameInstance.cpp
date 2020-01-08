//
// $Id: ApiCommandRegisterGameInstance.cpp 3226 2007-11-29 17:16:16Z johno $
//
#include "ApiCommandRegisterGameInstance.h"
#include "GameMessageProducer.h"
#include "RegisterGameInstanceResponse.h"
#include "XmlIterator.h"

std::string ApiCommandRegisterGameInstance::getTag()
{
   return "RGI";
}

ApiCommandRegisterGameInstance::ApiCommandRegisterGameInstance(const std::string &gameName,
                                                               const std::string &gameType,
                                                               const std::string &gameSessionId,
                                                               const std::string &gameTariffId)
: ApiCommandWithGameSessionId(gameName + gameType + gameSessionId,gameSessionId)
, gameName_(gameName)
, gameType_(gameType)
, gameTariffId_(gameTariffId)
{
}

ApiCommandRegisterGameInstance::ApiCommandRegisterGameInstance(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   gameName_ = params.extractString("gameName");
   gameType_ = params.extractString("gameType");
   gameTariffId_ = params.extractString("gameTariffId");
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandRegisterGameInstance::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new RegisterGameInstanceResponse(getMessageId(), e, message));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandRegisterGameInstance::makeAckResponse() const
{
   return std::auto_ptr<ServiceBrokerResponse>(new RegisterGameInstanceResponse(getMessageId()));
}

void ApiCommandRegisterGameInstance::send(GameMessageProducer& producer) const
{
   std::string serialised = wrapParamsWithGameSessionId(wrapString("gameName", gameName_)
                                                      + wrapString("gameType", gameType_)
                                                      + wrapString("gameTariffId", gameTariffId_) );

   sendMessage(producer, serialised);
}

std::string ApiCommandRegisterGameInstance::getTla() const
{
   return getTag();
}
