//
// $Id: ApiCommandGetUserData.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ApiCommandGetUserData.h"
#include "GameMessageProducer.h"
#include "GetUserDataResponse.h"
#include "XmlIterator.h"

std::string ApiCommandGetUserData::getTag()
{
   return "GUD";
}

ApiCommandGetUserData::ApiCommandGetUserData(const std::string &messageId,
                          const std::string &gameSessionId,
                          long userId,
                          const std::string &userDataType)
: ApiCommandWithGameSessionId(messageId, gameSessionId)
, userId_(userId)
, userDataType_(userDataType)
{
}

ApiCommandGetUserData::ApiCommandGetUserData(XmlIterator& iter,
                                             const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   userId_        = params.extractLong("userId");
   userDataType_  = params.extractString("userDataType");
}

void ApiCommandGetUserData::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId( wrapString("userId", userId_)
                                                + wrapString("userDataType", userDataType_)  ); 

    sendMessage(producer, serialised);
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandGetUserData::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new GetUserDataResponse(getMessageId(), e, message));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandGetUserData::makeAckResponse(const std::string& data) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new GetUserDataResponse(getMessageId(), data));
}

std::string ApiCommandGetUserData::getTla() const
{
   return getTag();
}
