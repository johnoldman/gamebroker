//
// $Id: ApiCommandSetUserData.h 3076 2007-11-23 15:23:41Z stephend $
//
#ifndef API_COMMAND_SET_USER_DATA_H_
#define API_COMMAND_SET_USER_DATA_H_

#include "ApiCommandWithGameSessionId.h"

class ApiCommandSetUserData: public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    ApiCommandSetUserData(const std::string &messageId, 
                          const std::string &gameSessionId, 
                          long userId, 
                          const std::string &userDataType, 
                          const std::string &userData);
    explicit ApiCommandSetUserData(XmlIterator&, const time_t& eventTime);
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    void send(GameMessageProducer& producer) const;
    long getUserId() const { return userId_; }
    std::string getUserDataType() const { return userDataType_; }
    std::string getUserData() const { return userData_; }
    std::string getTla() const;
private:
    long userId_;
    std::string userDataType_;
    std::string userData_;
};

#endif // _API_COMMAND_SET_USER_DATA_H_

