//
// $Id: ApiCommandBeginUserSession.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef API_COMMAND_BEGIN_USER_SESSION_H_
#define API_COMMAND_BEGIN_USER_SESSION_H_


#include "ApiCommandWithGameSessionId.h"

class ApiCommandBeginUserSession : public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    virtual std::string getTla() const;
    ApiCommandBeginUserSession(const std::string &messageId, 
                               const std::string &gameSessionId,
                               long userId,
                               const std::string &userSessionId,
                               const std::string &tariffId,
                               long gameCost, 
                               long gameFloorLimit);
    explicit ApiCommandBeginUserSession(XmlIterator&, const time_t&);
    void send(GameMessageProducer& producer) const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    std::auto_ptr<ServiceBrokerResponse> makeAckResponse(long purseTotal) const;

    long getUserId() const { return userId_; }
    std::string getUserSessionId() const { return userSessionId_; }
    std::string getTariffId() const { return tariffId_; }
    long getGameCost() const { return gameCost_; }
    long getFloorLimit() const { return gameFloorLimit_; }

private:
    long userId_;
    std::string userSessionId_;
    std::string tariffId_;
    long gameCost_; 
    long gameFloorLimit_;
};

#endif // API_COMMAND_BEGIN_USER_SESSION_H_
