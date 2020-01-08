//
// $Id: ApiCommandEndUserSession.cpp 2982 2007-11-21 10:05:17Z stephend $
//
#include "ApiCommandEndUserSession.h"
#include "GameMessageProducer.h"
#include "EndUserSessionResponse.h"
#include "XmlIterator.h"

std::string ApiCommandEndUserSession::getTag()
{
   return "EUS";
}

ApiCommandEndUserSession::ApiCommandEndUserSession(const std::string &messageId, 
                             const std::string &gameSessionId,
                             const std::string &userSessionId)
: ApiCommandWithGameSessionId(messageId, gameSessionId)
, userSessionId_(userSessionId)
{
}

ApiCommandEndUserSession::ApiCommandEndUserSession(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   userSessionId_ = params.extractString("userSessionId");
}

std::string ApiCommandEndUserSession::getUserSessionId() const 
{ 
   return userSessionId_; 
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandEndUserSession::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new EndUserSessionResponse(getMessageId(), e, message));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandEndUserSession::makeAckResponse() const
{
   return std::auto_ptr<ServiceBrokerResponse>(new EndUserSessionResponse(getMessageId()));
}

void ApiCommandEndUserSession::send(GameMessageProducer& producer) const
{
    const std::string serialised = wrapParamsWithGameSessionId(wrapString("userSessionId", userSessionId_));
    sendMessage(producer, serialised);
}

std::string ApiCommandEndUserSession::getTla() const
{
   return getTag();
}
