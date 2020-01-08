//
// $Id: ApiCommandMakePayment.cpp 2982 2007-11-21 10:05:17Z stephend $
//
#include "ApiCommandMakePayment.h"
#include "GameMessageProducer.h"
#include "MakePaymentResponse.h"
#include "XmlIterator.h"

std::string ApiCommandMakePayment::getTag()
{
   return "MPY";
}

ApiCommandMakePayment::ApiCommandMakePayment(const std::string &messageId, 
                          const std::string &gameSessionId, 
                          const std::string &fromUserSessionId, 
                          const std::string &toUserSessionId, 
                          long amount, 
                          const std::string &eventCode)
    : ApiCommandWithGameSessionId(messageId, gameSessionId)
    , fromUserSessionId_(fromUserSessionId)    
    , toUserSessionId_(toUserSessionId)
    , amount_(amount)
    , eventCode_(eventCode)
{
}

ApiCommandMakePayment::ApiCommandMakePayment(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   fromUserSessionId_ = params.extractString("fromUserSessionId");
   toUserSessionId_   = params.extractString("toUserSessionId");
   amount_            = params.extractLong("amount");
   eventCode_         = params.extractString("eventCode");
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandMakePayment::makeAckResponse() const
{
   return std::auto_ptr<ServiceBrokerResponse>(new MakePaymentResponse(getMessageId()));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandMakePayment::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new MakePaymentResponse(getMessageId(), e, message));
}

void ApiCommandMakePayment::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId( wrapString("fromUserSessionId", fromUserSessionId_) 
                                                        + wrapString("toUserSessionId", toUserSessionId_) 
                                                        + wrapString("amount", amount_)
                                                        + wrapString("eventCode", eventCode_)  );

    sendMessage(producer, serialised);
}

std::string ApiCommandMakePayment::getTla() const
{
   return getTag();
}
