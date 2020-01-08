//
// $Id: ApiCommandRegisterGameInstance.h 3226 2007-11-29 17:16:16Z johno $
//
#ifndef _API_COMMAND_REGISTER_GAME_INSTANCE_H_
#define _API_COMMAND_REGISTER_GAME_INSTANCE_H_

#include "ApiCommandWithGameSessionId.h"

class XmlIterator;

class ApiCommandRegisterGameInstance : public ApiCommandWithGameSessionId
{
public:
    ApiCommandRegisterGameInstance(const std::string &gameName,
                                   const std::string &gameType,
                                   const std::string &gameSessionId,
                                   const std::string &gameTariffId);
    explicit ApiCommandRegisterGameInstance(XmlIterator&, const time_t& eventTime);
    virtual std::auto_ptr<ServiceBrokerResponse> makeErrorResponse(SglGameAPI::SglServerExceptionType e, const std::string& message) const;
    virtual std::auto_ptr<ServiceBrokerResponse> makeAckResponse() const;
    void send(GameMessageProducer& producer) const;
    std::string getGameName() const { return gameName_; }
    std::string getGameType() const { return gameType_; }
    std::string getGameTariffId() const { return gameTariffId_; }
    static std::string getTag();
    std::string getTla() const;
private:
    std::string gameName_;
    std::string gameType_;
    std::string gameTariffId_;
};

#endif // _API_COMMAND_REGISTER_GAME_INSTANCE_H_

