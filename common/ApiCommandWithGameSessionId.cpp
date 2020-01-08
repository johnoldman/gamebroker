//
// $Id: ApiCommandWithGameSessionId.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include <string>
#include "XmlIterator.h"
#include "ApiCommandWithGameSessionId.h"
#include "GameMessageProducer.h"

#define PARAMETER_STRING "parameters"
#define GAMESESSION_ID_STRING "gameSessionId"

ApiCommandWithGameSessionId::ApiCommandWithGameSessionId(const std::string& messageId,
                                                         const std::string& gameSessionId)
: ApiCommand(messageId)
, gameSessionId_(gameSessionId)
{}

ApiCommandWithGameSessionId::ApiCommandWithGameSessionId(XmlIterator& iter, const time_t& eventTime)
: ApiCommand(iter.extractString("messageId"), eventTime)
, m_paramString(iter.extractXml(PARAMETER_STRING))
{
   XmlIterator params(m_paramString);
   gameSessionId_ = params.extractString(GAMESESSION_ID_STRING);
}

XmlIterator ApiCommandWithGameSessionId::getParamsIterator()
{
   XmlIterator params(m_paramString);
   params.extractString(GAMESESSION_ID_STRING);
   return params;
}

std::string ApiCommandWithGameSessionId::getGameSessionId() const
{
   return gameSessionId_;
}

std::string ApiCommandWithGameSessionId::wrapParamsWithGameSessionId(const std::string& params) const
{
   return wrapCommand( wrapXml(PARAMETER_STRING, wrapString(GAMESESSION_ID_STRING, gameSessionId_) + params) );
}

std::string ApiCommandWithGameSessionId::wrapParamsWithGameSessionId() const
{
   return wrapCommand( wrapXml(PARAMETER_STRING, wrapString(GAMESESSION_ID_STRING, gameSessionId_)) );
}

void ApiCommandWithGameSessionId::sendMessage(GameMessageProducer& producer, const std::string& contents) const
{
   producer.sendMessage(contents, gameSessionId_);
}
