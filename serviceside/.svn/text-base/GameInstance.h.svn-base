//
// $Id: GameInstance.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef GAME_INSTANCE_H_
#define GAME_INSTANCE_H_

#include <string>
#include <map>

#include <boost/shared_ptr.hpp>

#include "GameMessageConsumer.h"
#include "Purse.h"
#include "UserSessionId.h"
#include "GameSessionId.h"

class IKYRClient;
class Ledger;
class PersistentStore;

class GameInstance
{
public:
   GameInstance(PersistentStore& store, const GameSessionId& id, const std::string& gameName, const std::string& gameType, const std::string& gameTariffId, IKYRClient& kyr, Ledger& ledger);
   GameInstance(PersistentStore& store, const GameSessionId& id, IKYRClient&, Ledger&);
   // Create a purse from the specified user's wallet (from ledger) and associate it with the sessionId.
   long beginUserSession(long userId, const UserSessionId& sessionId, const std::string& tariffId, long gameCost, long floorLimit);
   // Return specified purse contents (if any) back to wallet (ledger)
   void endUserSession(const UserSessionId& sessionId);
   // Event <eventCode> for amount<amount> between player<fromSessionId> and player<toSessionId>
   void makePayment(const UserSessionId& fromSessionId, const UserSessionId& toSessionId, Amount amount, const std::string& eventCode, const time_t& eventTime);
   void makeSystemPayment(const UserSessionId& userSessionId, bool payUser, Amount amount, const std::string &totalType, const std::string& totalClassification, const std::string& eventCode, const time_t& eventTime);
   std::string getName() const { return m_gameName; }
   std::string getType() const { return m_gameType; }
   size_t getPurseCount() const;
   void destroy();
private:
   void transferMoney(const boost::shared_ptr<Purse>& from, const boost::shared_ptr<Purse>& to, const Amount& amount, const std::string& eventCode, const time_t& eventTime, const std::string& reference);
private:
   PersistentStore& m_store;
   const GameSessionId m_id;
   std::string m_gameName;
   std::string m_gameType;
   std::map<UserSessionId, boost::shared_ptr<Purse> > m_Purses;
   std::map<std::string, boost::shared_ptr<Purse> > m_GamePurses;
   IKYRClient& m_kyr;
   Ledger& m_ledger;
   long m_gameUserId;
   std::string m_gameTariffId;
};

#endif
