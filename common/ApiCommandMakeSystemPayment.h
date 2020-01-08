//
// $Id: ApiCommandMakeSystemPayment.h 3226 2007-11-29 17:16:16Z johno $
//
#ifndef API_COMMAND_MAKE_SYSTEM_PAYMENT_H_
#define API_COMMAND_MAKE_SYSTEM_PAYMENT_H_

#include "ApiCommandWithGameSessionId.h"

class ApiCommandMakeSystemPayment : public ApiCommandWithGameSessionId
{
public:
    static std::string getTag();
    ApiCommandMakeSystemPayment(const std::string &messageId,
                                const std::string &gameSessionId,
                                const std::string &userSessionId,
                                bool payUser,
                                long amount,
                                const std::string &totalType,
                                const std::string &totalClassification,
                                const std::string &eventCode);
    explicit ApiCommandMakeSystemPayment(XmlIterator&, const time_t& eventTime);
    virtual std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;

    void send(GameMessageProducer&) const;
    std::string getUserSessionId() const { return userSessionId_; }
    bool getPayUser() const { return payUser_; }
    long getAmount() const { return amount_; }
    std::string getTotalType() const { return totalType_; }
    std::string getTotalClassification() const { return totalClassification_; }
    std::string getEventCode() const { return eventCode_; }
    std::string getTla() const;
private:
    std::string userSessionId_;
    bool payUser_;
    long amount_;
    std::string totalType_;
    std::string totalClassification_;
    std::string eventCode_;
};

#endif // API_COMMAND_MAKE_SYSTEM_PAYMENT_H_
