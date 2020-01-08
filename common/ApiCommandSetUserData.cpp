//
// $Id: ApiCommandSetUserData.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ApiCommandSetUserData.h"
#include "GameMessageProducer.h"
#include "SetUserDataResponse.h"
#include "XmlIterator.h"

std::string ApiCommandSetUserData::getTag()
{
   return "SUD";
}

std::string ApiCommandSetUserData::getTla() const
{
   return getTag();
}

ApiCommandSetUserData::ApiCommandSetUserData(const std::string &messageId, 
                          const std::string &gameSessionId,
                          long userId,
                          const std::string &userDataType,
                          const std::string &userData)
: ApiCommandWithGameSessionId(messageId, gameSessionId)
, userId_(userId)
, userDataType_(userDataType)
, userData_(userData)
{
}

ApiCommandSetUserData::ApiCommandSetUserData(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   userId_        = params.extractLong("userId");
   userDataType_  = params.extractString("userDataType");
   userData_      = params.extractString("userData");
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandSetUserData::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new SetUserDataResponse(getMessageId(), e, message));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandSetUserData::makeAckResponse() const
{
   return std::auto_ptr<ServiceBrokerResponse>(new SetUserDataResponse(getMessageId()));
}

void ApiCommandSetUserData::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId(
                 wrapString("userId", userId_)
               + wrapString("userDataType", userDataType_)
               + wrapString("userData", userData_)
                     ); 

    sendMessage(producer, serialised);
}
