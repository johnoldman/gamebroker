//
// $Id: GameInstance.cpp 3484 2007-12-12 11:57:57Z stephend $
//
// Note there are two sorts of purses being stored - 'user' purses for users, and 'game' purses for jackpots etc
// These are told apart by the store by using key 'USER' for user purses and key 'GAME' + totaltype for the game purses
//
#include "GameInstance.h"
#include "PurseNotFoundException.h"
#include "DuplicateSessionException.h"
#include "IKYRClient.h"
#include "Ledger.h"
#include "PersistentStore.h"
#include "GameData.h"

GameInstance::GameInstance(PersistentStore& store, const GameSessionId& id, const std::string& gameName, const std::string& gameType, const std::string& gameTariffId, IKYRClient& kyr, Ledger& ledger)
: m_store(store)
, m_id(id)
, m_gameName(gameName)
, m_gameType(gameType)
, m_kyr(kyr)
, m_ledger(ledger)
, m_gameUserId(-1L)
, m_gameTariffId(gameTariffId)
{
   m_store.makeGameInstance(m_id, m_gameName, m_gameType, m_gameTariffId);
}

GameInstance::GameInstance(PersistentStore& store, const GameSessionId& id, IKYRClient& kyr, Ledger& ledger)
: m_store(store)
, m_id(id)
, m_kyr(kyr)
, m_ledger(ledger)
, m_gameUserId(-1L)
{
   GameData gameData = store.getGameInstance(id);
   m_gameName = gameData.getName();
   m_gameType = gameData.getType();
   m_gameTariffId = gameData.getTariff();

   // Need to load purse data...
   std::map<PurseId, PurseData> purses = m_store.loadPurses(id);
   for (std::map<PurseId, PurseData>::const_iterator ppurse = purses.begin(); ppurse != purses.end(); ++ppurse)
   {
      boost::shared_ptr<Purse> purse(new Purse(m_store, m_id, ppurse->first, ppurse->second.getUserId(), ppurse->second.getTariffId(), m_kyr.getTariff(ppurse->second.getTariffId()), m_ledger));
      if (ppurse->second.getKey().substr(0, 4) == "GAME")
      {
         m_GamePurses.insert(std::make_pair(ppurse->second.getKey().substr(4), purse));
      }
      else
      {
         m_Purses.insert(std::make_pair(UserSessionId(ppurse->second.getId()), purse));
      }
   }
}

size_t GameInstance::getPurseCount() const
{
   return m_Purses.size();
}

long GameInstance::beginUserSession(long userId, const UserSessionId& sessionId, const std::string& tariffId, long gameCost, long floorLimit)
{
   // Check to see if we already have such a session, and reject it if so.
   std::map<UserSessionId, boost::shared_ptr<Purse> >::const_iterator it = m_Purses.find(sessionId);
   if (it == m_Purses.end())
   {
      boost::shared_ptr<Purse> purse(new Purse(m_store, m_id, "USER", userId, gameCost, floorLimit, tariffId, m_kyr.getTariff(tariffId), m_ledger, sessionId.toString()));
      m_Purses.insert(std::make_pair(sessionId, purse));
      return purse->getTotal();
   }
   else
   {
      throw DuplicateSessionException("GameInstance::beginUserSession", sessionId);
   }
}

void GameInstance::endUserSession(const UserSessionId& sessionId)
{
   std::map<UserSessionId, boost::shared_ptr<Purse> >::iterator it = m_Purses.find(sessionId);
   if (it != m_Purses.end())
   {
      it->second->close(m_id.toString() + sessionId.toString());
      m_Purses.erase(it);
   }
   else
   {
      throw PurseNotFoundException("GameInstance::endUserSession", m_id, sessionId);
   }
}

void GameInstance::transferMoney(const boost::shared_ptr<Purse>& from, const boost::shared_ptr<Purse>& to, const Amount& amount, const std::string& eventCode, const time_t& eventTime, const std::string& reference)
{
   const std::vector<Asset> fromAssets = from->removeAssets(amount, eventCode, eventTime, to->getUserId(), m_id.toString() + reference);
   to->putAssets(fromAssets, eventCode, eventTime, to->getUserId(), m_id.toString() + reference);
}

void GameInstance::makePayment(const UserSessionId& fromSessionId, const UserSessionId& toSessionId, Amount amount, const std::string& eventCode, const time_t& eventTime)
{
   std::map<UserSessionId, boost::shared_ptr<Purse> >::iterator from = m_Purses.find(fromSessionId);
   if (from == m_Purses.end())
   {
      throw PurseNotFoundException("GameInstance::makePayment", m_id, fromSessionId);
   }
   std::map<UserSessionId, boost::shared_ptr<Purse> >::iterator to = m_Purses.find(toSessionId);
   if (to == m_Purses.end())
   {
      throw PurseNotFoundException("GameInstance::makePayment", m_id, toSessionId);
   }
   transferMoney(from->second, to->second, amount, eventCode, eventTime, fromSessionId.toString() + toSessionId.toString());
}

void GameInstance::makeSystemPayment(const UserSessionId& userSessionId, bool payUser, Amount amount, const std::string& totalType, const std::string& totalClassification, const std::string& eventCode, const time_t& eventTime)
{
   // totalClassification selects between game and universe purses
   // As universe purses are not implemented, totalClassification is ignored.

   std::map<std::string, boost::shared_ptr<Purse> >::const_iterator gamePurseIter = m_GamePurses.find(totalType);
   if (gamePurseIter == m_GamePurses.end())
   {
      // game purse not known - make one
      boost::shared_ptr<Purse> purse(new Purse(m_store, m_id, "GAME" + totalType, m_gameUserId, 0L, 0L, m_gameTariffId, m_kyr.getTariff(m_gameTariffId), m_ledger, totalType));

      std::pair<std::map<std::string, boost::shared_ptr<Purse> >::iterator, bool> insertResult = m_GamePurses.insert(std::make_pair(totalType, purse));
      gamePurseIter = insertResult.first;
   }
   const std::map<UserSessionId, boost::shared_ptr<Purse> >::const_iterator userPurseIter = m_Purses.find(userSessionId);
   if (userPurseIter != m_Purses.end())
   {
      if(payUser)
      {
         // game pays game-user
         transferMoney(gamePurseIter->second, userPurseIter->second, amount, eventCode, eventTime, totalType + userSessionId.toString());
      }
      else
      {
         // game-user pays game
         transferMoney(userPurseIter->second, gamePurseIter->second, amount, eventCode, eventTime, userSessionId.toString() + totalType);
      }
   }
   else
   {
      throw PurseNotFoundException("GameInstance::makeSystemPayment", m_id, userSessionId);
   }
}

void GameInstance::destroy()
{
   m_store.deleteGameInstance(m_id);
}
