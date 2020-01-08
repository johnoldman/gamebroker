//
// $Id: ApiCommandEndUserSession.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef _API_COMMAND_END_USER_SESSION_H_
#define _API_COMMAND_END_USER_SESSION_H_

#include "ApiCommandWithGameSessionId.h"

class ApiCommandEndUserSession : public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    ApiCommandEndUserSession(const std::string &messageId, 
                             const std::string &gameSessionId,
                             const std::string &userSessionId);
    explicit ApiCommandEndUserSession(XmlIterator&, const time_t& eventTime);
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    std::string getUserSessionId() const;
    virtual void send(GameMessageProducer&) const;
    std::string getTla() const;    
private:
    std::string userSessionId_;
};

#endif 
