//
// $Id: StubPersistentStore.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "StubPersistentStore.h"

#include <stdexcept>
#include <boost/lexical_cast.hpp>

void StubPersistentStore::addGameInstance(const GameSessionId& id)
{
   m_gameids.insert(id);
}

void StubPersistentStore::removeGameInstance(const GameSessionId& id)
{
   m_gameids.erase(id);
}

std::auto_ptr<MessageAndResponse> StubPersistentStore::getMessage(const std::string& messageId)
{
   if (m_transactionLog.find(messageId) != m_transactionLog.end())
   {
      // messageId stored - return the message record 
      transactionType transaction = m_transactionLog.find(messageId)->second;
      return std::auto_ptr<MessageAndResponse>(new MessageAndResponse(transaction.message, transaction.response));
   }
   else
   {
      // messageId not stored - return NULL
      return std::auto_ptr<MessageAndResponse>(0);
   }
}

void StubPersistentStore::storeMessage(const std::string& messageId, const std::string& message, const std::string& response)
{
   // store message and response in a struct, and store struct in transaction log
   transactionType transaction;
   transaction.message = message;
   transaction.response = response;   
   m_transactionLog[messageId] = transaction;
}

void StubPersistentStore::makeGameInstance(const GameSessionId& id, const std::string& name, const std::string& type, const std::string& tariff)
{
}

void StubPersistentStore::deleteGameInstance(const GameSessionId& id)
{
   m_gameids.erase(id);
}

bool StubPersistentStore::gameInstanceExists(const GameSessionId& id)
{
   return m_gameids.find(id) != m_gameids.end();
}

PurseId StubPersistentStore::savePurse(const GameSessionId& id, const PurseData& purseData)
{
   m_Purses[id].push_back(purseData);
   m_PurseIndexes.push_back(std::make_pair(id, m_Purses[id].size() - 1));
   return PurseId(m_PurseIndexes.size() - 1);
}

void StubPersistentStore::saveAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int id)
{
   std::pair<GameSessionId, size_t> index = m_PurseIndexes[purseid];
   m_Purses[index.first][index.second].add(amount, assetClassName, id);
}

void StubPersistentStore::updateAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int id)
{
}

std::vector<AssetData> StubPersistentStore::loadAssets(const PurseId& purseid)
{
   if (purseid < (int)m_PurseIndexes.size())
   {
      const std::pair<GameSessionId, size_t> index = m_PurseIndexes[purseid];
      return m_Purses[index.first][index.second].getAssets();
   }
   else
   { 
      throw std::logic_error(std::string("Purse not found ") + boost::lexical_cast<std::string>(purseid));
   }
}

Transaction StubPersistentStore::startTransaction()
{
   return Transaction(boost::shared_ptr<TransactionImpl>(new StubTransaction));
}

GameData StubPersistentStore::getGameInstance(const GameSessionId& id)
{
   return GameData("fred", "bloggs", "tariff");
}

std::map<PurseId, PurseData> StubPersistentStore::loadPurses(const GameSessionId& id)
{
   std::map<PurseId, PurseData> theMap;

   for (std::vector<std::pair<GameSessionId, size_t> >::const_iterator it = m_PurseIndexes.begin(); it != m_PurseIndexes.end(); ++it)
   {
      const std::pair<GameSessionId, size_t> thepair = *it;
      if (thepair.first == id)
      {
         const PurseId pid(thepair.second);
         theMap.insert(std::make_pair(pid, m_Purses[thepair.first][thepair.second].getPurseData()));
      }
   }
   return theMap;
}

void StubPersistentStore::savePurseTransaction(const PurseId& purseid, const PaymentTransaction& pt)
{
}

std::vector<PaymentTransaction> StubPersistentStore::loadTransactions(const PurseId& purseid)
{
   std::vector<PaymentTransaction> txns;
   return txns;
}

void StubPersistentStore::deletePurse(const GameSessionId& id, const PurseId& purseid)
{
}
