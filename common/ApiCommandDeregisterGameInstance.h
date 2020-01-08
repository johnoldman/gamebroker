//
// $Id: ApiCommandDeregisterGameInstance.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef _API_COMMAND_DEREGISTER_GAME_INSTANCE_H_
#define _API_COMMAND_DEREGISTER_GAME_INSTANCE_H_

#include "ApiCommandWithGameSessionId.h"

class XmlIterator;

class ApiCommandDeregisterGameInstance : public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    explicit ApiCommandDeregisterGameInstance(const std::string &gameSessionId);
    ApiCommandDeregisterGameInstance(XmlIterator& iter, const time_t& eventTime);
    std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    void send(GameMessageProducer& producer) const;
    std::string getTla() const;
};


#endif // _API_COMMAND_DEREGISTER_GAME_INSTANCE_H_

