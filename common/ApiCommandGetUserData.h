//
// $Id: ApiCommandGetUserData.h 3076 2007-11-23 15:23:41Z stephend $
//
#ifndef _API_COMMAND_GET_USER_DATA_H_
#define _API_COMMAND_GET_USER_DATA_H_

#include "ApiCommandWithGameSessionId.h"

class ApiCommandGetUserData: public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    ApiCommandGetUserData(const std::string &messageId, 
                          const std::string &gameSessionId, 
                          long userId, 
                          const std::string &userDataType);
    explicit ApiCommandGetUserData(XmlIterator&, const time_t& eventTime);
    std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    std::auto_ptr<ServiceBrokerResponse> makeAckResponse(const std::string& data) const;
    void send(GameMessageProducer& producer) const;
    long getUserId() const { return userId_; }
    std::string getUserDataType() const { return userDataType_; }
    std::string getTla() const;    
private:
    long userId_;
    std::string userDataType_;
};

#endif // _API_COMMAND_GET_USER_DATA_H_

