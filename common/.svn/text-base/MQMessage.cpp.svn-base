//
// $Id: MQMessage.cpp 3682 2007-12-17 14:08:53Z stephend $
//
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "MQMessage.h"
#include "XmlIterator.h"
#include "StubProducer.h"

#define MQENVELOPE "mq:envelope"

MQMessage::MQMessage(const std::string &messageId, const time_t& eventTime)
: m_messageId(messageId)
, m_eventTime(eventTime)
{
}

MQMessage::MQMessage(const std::string &messageId)
: m_messageId(messageId)
, m_eventTime(time(NULL))
{
}

MQMessage::~MQMessage()
{
}

std::string MQMessage::getMessageId() const
{
   return m_messageId;
}

std::string MQMessage::getSerialised() const
{
   StubProducer producer;
   send(producer);
   return producer.getMessage();
}

std::string MQMessage::wrapEnvelope(const std::string& messageType, const std::string &contents) const
{
   std::string wrappedEnvelope = std::string("<") + MQENVELOPE + "><" 
             + messageType + ">" + getTla() + "</" + messageType + "><messageId>" + m_messageId 
             + "</messageId>" + contents + "</" + MQENVELOPE + ">";
   return wrappedEnvelope;
}
//
// Wrap string inside XML tag, remembering to escape any XML found inside the string
//
std::string MQMessage::wrapString(const std::string &wrapperString, const std::string &insertString)
{
   std::string toBeInserted;
   for (size_t i = 0; i < insertString.length(); ++i)
   {
      const char ch = insertString[i];
      switch (ch)
      {
      case '<':
         toBeInserted += "&lt;";
         break;
      case '>':
         toBeInserted += "&gt;";
         break;
      case '&':
         toBeInserted += "&amp;";
         break;
      default:
         toBeInserted += ch;
      }
   }
   
   return std::string("<") + wrapperString + ">" + toBeInserted + "</" + wrapperString + ">";
}
//
// wrap XML string - so don't escape the contents
//
std::string MQMessage::wrapXml(const std::string &wrapperString, const std::string &insertString)
{
   return std::string("<") + wrapperString + ">" + insertString + "</" + wrapperString + ">";
}

std::string MQMessage::wrapString(const std::string &wrapperString, long insertValue)
{  
    std::string wrappedString = "<" + wrapperString + ">" + boost::lexical_cast<std::string>(insertValue) + "</" + wrapperString + ">";
    return wrappedString;
}

//std::string MQMessage::getSerialisedMessageId() const
//{
//   return wrapString("messageId", m_messageId);
//}
//
// Parse a message and return a string with the envelope contents
// Used by derived classes ApiCommand and ServiceBrokerResponse
// to help decode an incoming message.
//
std::string MQMessage::getEnvelopeContents(const std::string& message)
{
   XmlIterator iter(message);
   return iter.extractXml(MQENVELOPE);
}

time_t MQMessage::getEventTime() const
{
   return m_eventTime;
}
