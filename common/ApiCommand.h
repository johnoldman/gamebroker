//
// $Id: ApiCommand.h 2982 2007-11-21 10:05:17Z stephend $
// command message base class 
//
#ifndef API_COMMAND_H_
#define API_COMMAND_H_

#include <string>
#include <map>

#include "MQMessage.h"
#include "ServiceBrokerResponse.h"

class ApiCommand : public MQMessage
{
public:
   explicit ApiCommand(const std::string& messageId);
   ApiCommand(const std::string& messageId, const time_t& eventTime);
   virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const = 0;
   static std::pair<std::string, XmlIterator> parse(const std::string& message);
protected:
   std::string wrapParams(const std::string& params) const;
   std::string wrapCommand(const std::string& commandData) const;
};

#endif // API_COMMAND_H_
