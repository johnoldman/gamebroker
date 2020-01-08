//
// $Id: ApiCommandMakePayment.h 2982 2007-11-21 10:05:17Z stephend $
//
#ifndef _API_COMMAND_MAKE_PAYMENT_H_
#define _API_COMMAND_MAKE_PAYMENT_H_

#include "ApiCommandWithGameSessionId.h"

class ApiCommandMakePayment : public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    ApiCommandMakePayment(const std::string &messageId, 
                          const std::string &gameSessionId, 
                          const std::string &fromUserSessionId, 
                          const std::string &toUserSessionId, 
                          long amount, 
                          const std::string &eventCode);
    explicit ApiCommandMakePayment(XmlIterator&, const time_t& eventTime);    
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    void send(GameMessageProducer& producer) const;

    std::string getFromUserSessionId() const { return fromUserSessionId_; }
    std::string getToUserSessionId() const { return toUserSessionId_; }
    long getAmount() const { return amount_; }
    std::string getEventCode() const { return eventCode_; }
    std::string getTla() const;
private:
    std::string fromUserSessionId_;
    std::string toUserSessionId_;
    long amount_; 
    // The 'reason' for the payment (e.g. 'A shot B')
    std::string eventCode_;
};


#endif // _API_COMMAND_MAKE_PAYMENT_H_

