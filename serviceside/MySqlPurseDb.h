//
// $Id: MySqlPurseDb.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef _MY_SQL_PURSE_DB_H_
#define _MY_SQL_PURSE_DB_H_

#include <boost/noncopyable.hpp>

#include <mysql++/connection.h>
#include <mysql++/transaction.h>

#include "GameSessionId.h"
#include "TransactionImpl.h"
#include "PersistentStore.h"

class PaymentTransaction;

class MySqlPurseDb : public PersistentStore
{
public:
   MySqlPurseDb(const std::string& host, const std::string& dbname, const std::string& username, const std::string& password, unsigned short port = 3306);
   std::auto_ptr<MessageAndResponse> getMessage(const std::string& messageid);
   void storeMessage(const std::string& messageId, const std::string& message, const std::string& response);
   virtual Transaction startTransaction();
   virtual void makeGameInstance(const GameSessionId& id, const std::string& name, const std::string& type, const std::string& tariff);
   virtual void deleteGameInstance(const GameSessionId& id);
   virtual bool gameInstanceExists(const GameSessionId& id);
   virtual PurseId savePurse(const GameSessionId& gsi, const PurseData&);
   virtual void deletePurse(const GameSessionId& gsi, const PurseId& purseid);
   virtual void saveAsset(const PurseId& purseid, const Amount& amount, const std::string& name, int assetid);
   virtual void updateAsset(const PurseId& purseid, const Amount& amount, const std::string& name, int assetid);
   GameData getGameInstance(const GameSessionId& sessionId);
   std::map<PurseId, PurseData> loadPurses(const GameSessionId& gsi);
   std::vector<AssetData> loadAssets(const PurseId& purseid);
   void savePurseTransaction(const PurseId& purseid, const PaymentTransaction& pt);
   std::vector<PaymentTransaction> loadTransactions(const PurseId& purseid);
private:
   mysqlpp::Connection m_connection;
   class MySqlTransaction : public TransactionImpl, public boost::noncopyable
   {
   public:
      MySqlTransaction(mysqlpp::Connection& conn);
      void commit();
   private:
      mysqlpp::Transaction m_impl;
   };
};

#endif
