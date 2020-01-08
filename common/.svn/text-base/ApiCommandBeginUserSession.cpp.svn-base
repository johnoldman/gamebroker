//
// $Id: ApiCommandBeginUserSession.cpp 2982 2007-11-21 10:05:17Z stephend $
//
#include "ApiCommandBeginUserSession.h"
#include "GameMessageProducer.h"
#include "BeginUserSessionResponse.h"
#include "XmlIterator.h"

std::string ApiCommandBeginUserSession::getTag()
{
   return "BUS";
}

std::string ApiCommandBeginUserSession::getTla() const
{
   return getTag();
}

ApiCommandBeginUserSession::ApiCommandBeginUserSession(const std::string &messageId, 
                      const std::string &gameSessionId,
                      long userId,
                      const std::string &userSessionId,
                      const std::string &tariffId,
                      long gameCost, 
                      long gameFloorLimit)
    : ApiCommandWithGameSessionId(messageId, gameSessionId)
    , userId_(userId)
    , userSessionId_(userSessionId)
    , tariffId_(tariffId)
    , gameCost_(gameCost)
    , gameFloorLimit_(gameFloorLimit)
{
}

ApiCommandBeginUserSession::ApiCommandBeginUserSession(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   userId_         = params.extractLong("userId");
   userSessionId_  = params.extractString("userSessionId");
   tariffId_       = params.extractString("tariffId");
   gameCost_       = params.extractLong("gameCost");
   gameFloorLimit_ = params.extractLong("gameFloorLimit");
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandBeginUserSession::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new BeginUserSessionResponse(getMessageId(), e, message));
}
   
void ApiCommandBeginUserSession::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId(
                 wrapString("userId", userId_)
               + wrapString("userSessionId", userSessionId_)
               + wrapString("tariffId", tariffId_)
               + wrapString("gameCost", gameCost_)
               + wrapString("gameFloorLimit", gameFloorLimit_)  ); 

    sendMessage(producer, serialised);
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandBeginUserSession::makeAckResponse(long purseTotal) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new BeginUserSessionResponse(getMessageId(), purseTotal));
}
