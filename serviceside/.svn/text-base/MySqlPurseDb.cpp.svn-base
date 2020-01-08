//
// $Id: MySqlPurseDb.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "MySqlPurseDb.h"

#include <boost/lexical_cast.hpp>

#include "GameInstance.h"
#include "GameSessionId.h"
#include "PaymentTransaction.h"

#include <mysql++/mysql++.h>
#include <mysql++/custom.h>
#include <mysql++/transaction.h>

sql_create_3(TransactionLog, 3, 0, std::string, messageid, std::string, message, std::string, response)
sql_create_4(GameInstanceStore, 4, 0, std::string, sessionid, std::string, gameName, std::string, gameType, std::string, gameTariff)
sql_create_4(PurseStore, 4, 0, std::string, gamesessionid, long, userid, std::string, tariffname, std::string, uniquereference)
sql_create_4(PurseAssets, 4, 0, int, purseid, long, amount, std::string, assetclass, int, assetid)
sql_create_10(PurseTransactions, 10, 0, int, purseid, int, amount, int, assetid, time_t, timestamp, std::string, description, int, fromassetid, bool, iscredit, std::string, reference, long, userid, long, otheruserid)

MySqlPurseDb::MySqlTransaction::MySqlTransaction(mysqlpp::Connection& conn)
: m_impl(conn)
{
}

void MySqlPurseDb::MySqlTransaction::commit()
{
   m_impl.commit();
}

MySqlPurseDb::MySqlPurseDb(const std::string& host, const std::string& dbname, const std::string& username, const std::string& password, unsigned short port)
: m_connection(dbname.c_str(), host.c_str(), username.c_str(), password.c_str(), port)
{
}

std::auto_ptr<MessageAndResponse> MySqlPurseDb::getMessage(const std::string& messageid)
{
   mysqlpp::Query query = m_connection.query();
   query << "select message, response from TransactionLog where messageid = " << mysqlpp::quote << messageid;
   mysqlpp::Result res = query.store();

   if (res && res.size() > 0)
   {
      mysqlpp::Row row = res.at(0);
      return std::auto_ptr<MessageAndResponse>(new MessageAndResponse(row["message"].get_string(), row["response"].get_string()));
   }
   else
   {
      return std::auto_ptr<MessageAndResponse>(0);
   }
}

void MySqlPurseDb::storeMessage(const std::string& messageid, const std::string& message, const std::string& response)
{
   TransactionLog logrecord(messageid, message, response);

   mysqlpp::Query query = m_connection.query();
   query.insert(logrecord);
   query.execute();
}

void MySqlPurseDb::makeGameInstance(const GameSessionId& id, const std::string& name, const std::string& type, const std::string& tariff)
{
   GameInstanceStore gi(id.toString(), name, type, tariff);
   mysqlpp::Query query = m_connection.query();
   query.insert(gi);
   query.execute();
}

void MySqlPurseDb::deleteGameInstance(const GameSessionId& id)
{
   mysqlpp::Query query = m_connection.query();
   query << "delete from GameInstanceStore where sessionid = %0q";
   query.parse();
   query.store(id.toString());
}

bool MySqlPurseDb::gameInstanceExists(const GameSessionId& gsi)
{
   mysqlpp::Query query = m_connection.query();
   query << "select count(*) as count from GameInstanceStore where sessionid = %0q";
   query.parse();
   mysqlpp::Result res = query.store(gsi.toString());
   if (res && res.size() > 0)
   {
      mysqlpp::Row row = res.at(0);
      const int count = row["count"];
      return count == 1;
   }
   else
   {
      throw std::runtime_error(std::string("MySqlPurseDb::gameInstanceExists - Problem querying ") + boost::lexical_cast<std::string>(gsi));
   }
}

GameData MySqlPurseDb::getGameInstance(const GameSessionId& gameSessionId)
{
   mysqlpp::Query query = m_connection.query();
   query << "select gameName, gameType, gameTariff from GameInstanceStore where sessionid = %0q";
   query.parse();
   mysqlpp::Result res = query.store(gameSessionId.toString());
   if (res && res.size() > 0)
   {
      mysqlpp::Row row = res.at(0);
      return GameData(row["gameName"].get_string(), row["gameType"].get_string(), row["gameTariff"].get_string());
   }
   else
   {
      throw std::runtime_error(std::string("Can't find ") + boost::lexical_cast<std::string>(gameSessionId));
   }
}

PurseId MySqlPurseDb::savePurse(const GameSessionId& gsi, const PurseData& purseData)
{
   PurseStore purse(gsi.toString(), purseData.getUserId(), purseData.getTariffId(), purseData.getId());
   mysqlpp::Query query = m_connection.query();
   query.insert(purse);
   mysqlpp::ResNSel res = query.execute();
   return PurseId(res.insert_id);
}

void MySqlPurseDb::deletePurse(const GameSessionId& id, const PurseId& purseid)
{
   mysqlpp::Query query = m_connection.query();
   query << "delete from PurseStore where gamesessionid = %0q and purseid = %1";
   query.parse();
   query.store(id.toString(), static_cast<int>(purseid));
}

std::map<PurseId, PurseData> MySqlPurseDb::loadPurses(const GameSessionId& gsi)
{
   mysqlpp::Query query = m_connection.query();
   query << "select * from PurseStore where gamesessionid = %0q";
   query.parse();
   mysqlpp::Result res = query.store(gsi.toString());
   std::map<PurseId, PurseData> purseData;
   for (size_t i = 0; i < res.size(); ++i)
   {
      mysqlpp::Row row = res.at(i);
      purseData.insert(std::make_pair(PurseId(row["purseid"]), PurseData(row["storagekey"].get_string(), row["userid"], row["tariffname"].get_string(), row["uniquereference"].get_string())));
   }
   return purseData;
}

void MySqlPurseDb::saveAsset(const PurseId& purseid, const Amount& amount, const std::string& assetName, int assetid)
{
   PurseAssets pa(purseid, amount, assetName, assetid);
   mysqlpp::Query insert = m_connection.query();
   insert.insert(pa);
   insert.execute();
}

void MySqlPurseDb::updateAsset(const PurseId& purseid, const Amount& amount, const std::string& assetName, int assetid)
{
   PurseAssets pa(purseid, amount, assetName, assetid);
   mysqlpp::Query query = m_connection.query();
   query << "update PurseAssets set amount = %0 where purseid = %1 and assetclass = %2q and assetid = %3";
   query.parse();
   query.store(static_cast<int>(amount), static_cast<int>(purseid), assetName, assetid);
}

std::vector<AssetData> MySqlPurseDb::loadAssets(const PurseId& purseid)
{
   mysqlpp::Query query = m_connection.query();
   query << "select * from PurseAssets where purseid = %0";
   query.parse();
   mysqlpp::Result res = query.store(static_cast<int>(purseid));
   std::vector<AssetData> assetData;
   for (size_t i = 0; i < res.size(); ++i)
   {
      mysqlpp::Row row = res.at(i);
      assetData.push_back(AssetData(Amount(row["amount"]), row["assetclass"].get_string(), row["assetid"]));
   }
   return assetData;
}

Transaction MySqlPurseDb::startTransaction()
{
   return Transaction(boost::shared_ptr<TransactionImpl>(new MySqlTransaction(m_connection)));
}

void MySqlPurseDb::savePurseTransaction(const PurseId& purseid, const PaymentTransaction& pt)
{
   PurseTransactions storedData(purseid, pt.getAmount(), pt.getAssetId(), pt.getDate(), pt.getDescription(), pt.getFromAsset(), pt.isCredit(), pt.getReference(), pt.getUserId(), pt.getThirdPartyId());
   mysqlpp::Query insert = m_connection.query();
   insert.insert(storedData);
   insert.execute();
}

std::vector<PaymentTransaction> MySqlPurseDb::loadTransactions(const PurseId& purseid)
{
   mysqlpp::Query query = m_connection.query();
   query << "select * from PurseTransactions where purseid = %0";
   query.parse();
   mysqlpp::Result res = query.store(static_cast<int>(purseid));
   std::vector<PaymentTransaction> txns;
   for (size_t i = 0; i < res.size(); ++i)
   {
      mysqlpp::Row row = res.at(i);
      PaymentTransaction pt(row["amount"], row["assetid"], row["timestamp"], row["description"].get_string(), row["fromassetid"], row["iscredit"], row["reference"].get_string(), row["userid"], row["otheruserid"]);
      txns.push_back(pt);
   }
   return txns;
}
