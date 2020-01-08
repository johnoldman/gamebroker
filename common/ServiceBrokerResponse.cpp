//
// $Id: ServiceBrokerResponse.cpp 2791 2007-11-14 18:24:18Z stephend $
//
#include <boost/lexical_cast.hpp>
#include "ServiceBrokerResponse.h"
#include "MQMessage.h"
#include "XmlIterator.h"

#define MESSAGE_STRING "response"

ServiceBrokerResponse::ServiceBrokerResponse(XmlIterator& xmlIterator)
: MQMessage(xmlIterator.extractString("messageId"))
, m_error(static_cast<SglGameAPI::SglServerExceptionType>(xmlIterator.extractLong("errorCode")))
{
   if (m_error != SglGameAPI::SglSuccess)
   {
      m_errorString = xmlIterator.extractString("errorString");
   }
}

ServiceBrokerResponse::ServiceBrokerResponse(const std::string& messageId)
: MQMessage(messageId)
, m_error(SglGameAPI::SglSuccess)
, serialisedErrorData(wrapString("errorCode",0))
{
}

ServiceBrokerResponse::ServiceBrokerResponse(const std::string& messageId,               
                                             const enum SglGameAPI::SglServerExceptionType& errorCode,
                                             const std::string& errorString)
: MQMessage(messageId)
, m_error(errorCode)
, serialisedErrorData(wrapString("errorCode", m_error) + wrapString("errorString", errorString))
{
}

SglGameAPI::SglServerExceptionType ServiceBrokerResponse::getError() const
{
   return m_error;
}

std::string ServiceBrokerResponse::getErrorString() const
{
   return m_errorString;
}

bool ServiceBrokerResponse::isSuccess() const
{
   return m_error == SglGameAPI::SglSuccess;
}

std::string ServiceBrokerResponse::getSerialisedTagCode() const
{
   return wrapString("response", getTla());
}

std::string ServiceBrokerResponse::getSerialisedErrorData() const
{
   return serialisedErrorData;
}

std::string ServiceBrokerResponse::serialiseResponse() const
{
   std::string serialised = wrapEnvelope(MESSAGE_STRING, serialisedErrorData);
   return serialised;
}

std::string ServiceBrokerResponse::serialiseResponse(std::string serialisedUserData) const
{
   std::string serialised = wrapEnvelope(MESSAGE_STRING, serialisedErrorData + serialisedUserData);
   return serialised;
}

std::pair<std::string, XmlIterator> ServiceBrokerResponse::parse(const std::string& message)
{
   XmlIterator contents(getEnvelopeContents(message));
   const std::string command = contents.extractString(MESSAGE_STRING);

   return std::pair<std::string, XmlIterator>(command, contents);
}
