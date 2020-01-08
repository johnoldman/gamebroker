//
// $Id: ApiCommandWithGameSessionId.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef APICOMMANDWITHGAMESESSION_H_
#define APICOMMANDWITHGAMESESSION_H_

#include <string>
#include "ApiCommand.h"

class GameMessageProducer;

class ApiCommandWithGameSessionId : public ApiCommand
{
public:
   std::string getGameSessionId() const;
protected:
   ApiCommandWithGameSessionId(const std::string& messageId,
                               const std::string& gameSessionId);
   explicit ApiCommandWithGameSessionId(XmlIterator& paramsIter, const time_t& eventTime);
   XmlIterator getParamsIterator();
   std::string wrapParamsWithGameSessionId(const std::string& params) const;
   std::string wrapParamsWithGameSessionId() const;
   void sendMessage(GameMessageProducer& producer, const std::string& contents) const;
private:
   std::string m_paramString;
   std::string gameSessionId_;
};

#endif
