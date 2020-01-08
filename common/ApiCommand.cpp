//
// $Id: ApiCommand.cpp 2982 2007-11-21 10:05:17Z stephend $
//
#include <string>
#include <iostream>
#include "MQMessage.h"
#include "ApiCommand.h"
#include "XmlIterator.h"
#include "StubProducer.h"

#define MESSAGE_STRING "command"

ApiCommand::ApiCommand(const std::string& messageId, const time_t& eventTime)
: MQMessage(messageId, eventTime)
{
}

ApiCommand::ApiCommand(const std::string& messageId)
: MQMessage(messageId)
{
}

std::string ApiCommand::wrapCommand(const std::string& cmdData) const
{
   return wrapEnvelope(MESSAGE_STRING, cmdData); 
}

std::string ApiCommand::wrapParams(const std::string& params) const
{
   return wrapCommand( wrapXml("parameters", params));
}

std::pair<std::string, XmlIterator> ApiCommand::parse(const std::string& message)
{
   XmlIterator contents(getEnvelopeContents(message));
   const std::string command = contents.extractString(MESSAGE_STRING);

   return std::pair<std::string, XmlIterator>(command, contents);
}


