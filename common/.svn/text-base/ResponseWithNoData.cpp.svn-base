//
// $Id: ResponseWithNoData.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "ResponseWithNoData.h"
#include "GameMessageProducer.h"

ResponseWithNoData::ResponseWithNoData(XmlIterator& xmlResponse)
: ServiceBrokerResponse(xmlResponse)
{
}

ResponseWithNoData::ResponseWithNoData(const std::string& messageId)
: ServiceBrokerResponse(messageId)
{
}

ResponseWithNoData::ResponseWithNoData(const std::string& messageId,               
                                       const enum SglGameAPI::SglServerExceptionType& errorCode,
                                       const std::string& errorString)
: ServiceBrokerResponse(messageId, errorCode, errorString)
{
}

void ResponseWithNoData::send(GameMessageProducer& producer) const
{
   producer.sendMessage(serialiseResponse(), "");
}

