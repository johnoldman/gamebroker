//
// $Id: StubPersistentStore.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef STUB_PERSISTANT_STORE_H_
#define STUB_PERSISTANT_STORE_H_

#include "PersistentStore.h"

#include <map>
#include <vector>
#include <set>

#include "GameSessionId.h"
#include "TransactionImpl.h"

class StubPersistentStore : public PersistentStore
{
public:
//   StubPersistentStore();
   void addGameInstance(const GameSessionId& id);
   void removeGameInstance(const GameSessionId& id);
public:
   virtual std::auto_ptr<MessageAndResponse> getMessage(const std::string& messageId);
   virtual void storeMessage(const std::string& messageId, const std::string& message, const std::string& response);
   virtual void makeGameInstance(const GameSessionId& id, const std::string& name, const std::string& type, const std::string& tariff);
   virtual void deleteGameInstance(const GameSessionId& id);
   virtual bool gameInstanceExists(const GameSessionId& id);
   virtual PurseId savePurse(const GameSessionId& id, const PurseData&); 
   virtual void deletePurse(const GameSessionId& id, const PurseId& purseid); 
   virtual void saveAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int assetid);
   virtual void updateAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int assetid);
   virtual Transaction startTransaction();
   virtual GameData getGameInstance(const GameSessionId& session);
   virtual std::map<PurseId, PurseData> loadPurses(const GameSessionId& id);
   virtual std::vector<AssetData> loadAssets(const PurseId& purseid);
   void savePurseTransaction(const PurseId& purseid, const PaymentTransaction& pt);
   std::vector<PaymentTransaction> loadTransactions(const PurseId& purseid);
private:
   std::set<GameSessionId> m_gameids;
   class StubPurseData
   {
   public:
      StubPurseData(const PurseData& purseData) : m_purseData(purseData) 
      {
      }
      void add(Amount amount, const std::string& assetclass, int id)
      {
         m_assets.push_back(AssetData(amount, assetclass, id));
      }
      std::vector<AssetData> getAssets() const
      {
         return m_assets;
      }
      PurseData getPurseData() const
      {
         return m_purseData;
      }
   private:
      PurseData m_purseData;
      std::vector<AssetData> m_assets;
   };
   std::map<GameSessionId, std::vector<StubPurseData> > m_Purses;
   std::vector<std::pair<GameSessionId, size_t> > m_PurseIndexes;
   struct transactionType 
   {
      std::string message; 
      std::string response;
   };
   std::map<std::string, transactionType> m_transactionLog;
   std::map<std::string, GameSessionId> m_gameInstances;
   class StubTransaction : public TransactionImpl
   {
   public:
      void commit() {}
   };
};

#endif
