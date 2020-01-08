//
// $Id: ApiCommandMakeSystemPayment.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ApiCommandMakeSystemPayment.h"
#include "XmlIterator.h"
#include "MakeSystemPaymentResponse.h"

ApiCommandMakeSystemPayment::ApiCommandMakeSystemPayment(const std::string &messageId,
                                const std::string &gameSessionId,
                                const std::string &userSessionId,
                                bool payUser,
                                long amount,
                                const std::string &totalType,
                                const std::string &totalClassification,
                                const std::string &eventCode)
    : ApiCommandWithGameSessionId(messageId, gameSessionId)
    , userSessionId_(userSessionId)
    , payUser_(payUser)
    , amount_(amount)
    , totalType_(totalType)
    , totalClassification_(totalClassification)
    , eventCode_(eventCode)
{
}

std::string ApiCommandMakeSystemPayment::getTag()
{
   return "MSP";
}

std::string ApiCommandMakeSystemPayment::getTla() const
{
   return "MSP";
}

ApiCommandMakeSystemPayment::ApiCommandMakeSystemPayment(XmlIterator& iter, const time_t& eventTime)
: ApiCommandWithGameSessionId(iter, eventTime)
{
   XmlIterator params(getParamsIterator());
   userSessionId_ = params.extractString("userSessionId");
   payUser_       = params.extractLong("payUser");
   amount_        = params.extractLong("amount");
   totalType_     = params.extractString("totalType");
   totalClassification_ = params.extractString("totalClassification");
   eventCode_     = params.extractString("eventCode");
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandMakeSystemPayment::makeAckResponse() const
{
   return std::auto_ptr<ServiceBrokerResponse>(new MakeSystemPaymentResponse(getMessageId()));
}

std::auto_ptr<ServiceBrokerResponse> ApiCommandMakeSystemPayment::makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const
{
   return std::auto_ptr<ServiceBrokerResponse>(new MakeSystemPaymentResponse(getMessageId(), e, message));
}

void ApiCommandMakeSystemPayment::send(GameMessageProducer& producer) const
{
    std::string serialised = wrapParamsWithGameSessionId( wrapString("userSessionId", userSessionId_)
                                                        + wrapString("payUser", payUser_)
                                                        + wrapString("amount", amount_)
                                                        + wrapString("totalType", totalType_)
                                                        + wrapString("totalClassification", totalClassification_)
                                                        + wrapString("eventCode", eventCode_)  );

    sendMessage(producer, serialised);
}
