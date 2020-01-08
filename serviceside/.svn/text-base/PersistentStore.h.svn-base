//
// $Id: PersistentStore.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef _PERSISTENT_STORE_H
#define _PERSISTENT_STORE_H

#include <memory>
#include <map>
#include <string>
#include <vector>

#include "MessageAndResponse.h"
#include "Transaction.h"
#include "PurseData.h"
#include "AssetData.h"
#include "PaymentTransaction.h"
#include "PurseId.h"
#include "GameData.h"

class GameInstance;
class GameSessionId;
class Amount;

class PersistentStore
{
public:
   virtual ~PersistentStore() {}
   // Search for a message by message id. Return both original message and its response.
   // if the record does not exist, a null pointer is returned
   virtual std::auto_ptr<MessageAndResponse> getMessage(const std::string& messageId) = 0;
   //
   // if message already exists, it will throw an exception
   // 
   virtual void storeMessage(const std::string& messageId, const std::string& message, const std::string& response) = 0;
   virtual Transaction startTransaction() = 0;
   virtual void makeGameInstance(const GameSessionId& id, const std::string& name, const std::string& type, const std::string& tariff) = 0;
   virtual void deleteGameInstance(const GameSessionId& id) = 0;
   virtual bool gameInstanceExists(const GameSessionId& id) = 0;
   virtual PurseId savePurse(const GameSessionId& gsi, const PurseData&) = 0;
   virtual void deletePurse(const GameSessionId& gsi, const PurseId& purseid) = 0;
   virtual void saveAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int assetid) = 0;
   virtual void updateAsset(const PurseId& purseid, const Amount& amount, const std::string& assetClassName, int assetid) = 0;
   virtual GameData getGameInstance(const GameSessionId&) = 0;
   virtual std::map<PurseId, PurseData> loadPurses(const GameSessionId& gsi) = 0;
   virtual std::vector<AssetData> loadAssets(const PurseId& purseid) = 0;
   virtual void savePurseTransaction(const PurseId& purseid, const PaymentTransaction&) = 0;
   virtual std::vector<PaymentTransaction> loadTransactions(const PurseId& purseid) = 0;
};

#endif
