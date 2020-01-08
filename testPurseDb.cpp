//
// $Id: testPurseDb.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include <boost/scoped_ptr.hpp>
#include "testPurseDb.h"
#include <mysql++/mysql++.h>
#include "GameInstance.h"
#include "GameSessionId.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPurseDb);

void TestPurseDb::setUp()
{
   system("mysqladmin -uroot -ppassword -f drop pursedb");
   system("mysqladmin -uroot -ppassword create pursedb");
   system("mysql -uroot -ppassword pursedb <makepursedb.sql");
   m_pDb.reset(new MySqlPurseDb("localhost", "pursedb", "root", "password"));
   m_pKyr.reset(new StubKYR);
   m_pLedger.reset(new StubLedger);
   m_pTransaction.reset(new Transaction(m_pDb->startTransaction()));
}

void TestPurseDb::tearDown()
{
   m_pTransaction.reset();
   m_pDb.reset();
}

void TestPurseDb::messageTest()
{
   // Check that null is returned if message not present.
   std::auto_ptr<MessageAndResponse> nullRet(0);
   CPPUNIT_ASSERT_EQUAL(nullRet.get(), m_pDb->getMessage("fred").get());

   m_pDb->storeMessage("fred", "hello fred", "fred reply");
   // Once message has been stored, we can find it again (!?!)
   CPPUNIT_ASSERT(m_pDb->getMessage("fred").get() != 0);
   CPPUNIT_ASSERT_EQUAL(MessageAndResponse("hello fred", "fred reply"), *m_pDb->getMessage("fred"));

   // Attempting to store message twice results in an exception (due to uniqueness constraints on the DB)
   CPPUNIT_ASSERT_THROW(m_pDb->storeMessage("fred", "hello fred", "fred reply"), mysqlpp::BadQuery);
}

void TestPurseDb::makeGameInstance()
{
   GameInstance gi(*m_pDb, GameSessionId("fred"), *m_pKyr, *m_pLedger);
}

void TestPurseDb::gameInstanceTest()
{
   CPPUNIT_ASSERT_THROW(makeGameInstance(), std::runtime_error);
   const GameSessionId gsi("interestinggamesessionid");
   std::string gameName("name");
   std::string gameType("type");
   std::string gameTariffId("gameTariffId");

   GameInstance gi(*m_pDb, gsi, gameName, gameType, gameTariffId, *m_pKyr, *m_pLedger);
   GameInstance gi2(*m_pDb, gsi, *m_pKyr, *m_pLedger);
   CPPUNIT_ASSERT_EQUAL(gameName, gi2.getName());
   CPPUNIT_ASSERT_EQUAL(gameType, gi2.getType());
}

void TestPurseDb::purseTest()
{
   const GameSessionId gsi("interestinggamesessionid");
   std::string gameName("name");
   std::string gameType("type");
   std::string gameTariffId("gameTariffId");
   GameInstance gi(*m_pDb, gsi, gameName, gameType, gameTariffId, *m_pKyr, *m_pLedger);
   long userid = 27;
   UserSessionId usi("usi");
   std::string tariffid("tariff");
   long gameCost = 3;
   long floorLimit = 5;

   gi.beginUserSession(userid, usi, tariffid, gameCost, floorLimit);

   GameInstance gi2(*m_pDb, gsi, *m_pKyr, *m_pLedger);
   CPPUNIT_ASSERT_EQUAL((size_t)1, gi2.getPurseCount());
}

void TestPurseDb::saveRandomAsset()
{
   m_pDb->saveAsset(PurseId(1), Amount(2), "fred", 1);
}

void TestPurseDb::assetTest()
{
   CPPUNIT_ASSERT_THROW(saveRandomAsset(), mysqlpp::BadQuery);
   GameSessionId gsi("fred");
   m_pDb->makeGameInstance(gsi, "name", "type", "tariff");
   PurseData purseData("key", 27, "xyggt", "saejkdfhkj");

   const PurseId purseid = m_pDb->savePurse(gsi, purseData);
   const std::string assetname("assetname");
   const std::string assetname2("assetname2");

   m_pDb->saveAsset(purseid, Amount(27), assetname, 1);
   CPPUNIT_ASSERT_THROW(m_pDb->saveAsset(purseid, Amount(27), assetname, 1), mysqlpp::BadQuery);
   m_pDb->saveAsset(purseid, Amount(54), assetname2, 2);

   std::vector<AssetData> assets = m_pDb->loadAssets(purseid);
   CPPUNIT_ASSERT_EQUAL(size_t(2), assets.size());
   CPPUNIT_ASSERT_EQUAL(Amount(27), assets[0].getAmount());
   CPPUNIT_ASSERT_EQUAL(Amount(54), assets[1].getAmount());
   CPPUNIT_ASSERT_EQUAL(assetname, assets[0].getAssetClass());
   CPPUNIT_ASSERT_EQUAL(assetname2, assets[1].getAssetClass());

   m_pDb->updateAsset(purseid, Amount(67), assetname, 1);
}

void TestPurseDb::transactionTest()
{
   GameSessionId gsi("fred");
   m_pDb->makeGameInstance(gsi, "name", "type", "tariff");
   PurseData purseData("key", 27, "xyggt", "saejkdfhkj");

   const PurseId purseid = m_pDb->savePurse(gsi, purseData);
   long amount = 3;
   long assetid = 4;
   int fromAsset = 1;
   long userid = 5;
   long otheruserid = 6;
   PaymentTransaction pt(amount, assetid, time(NULL), "desc", fromAsset, true, "reference", userid, otheruserid);
   m_pDb->savePurseTransaction(purseid, pt);

   std::vector<PaymentTransaction> txns = m_pDb->loadTransactions(purseid);
   CPPUNIT_ASSERT_EQUAL((size_t)1, txns.size());
}

void TestPurseDb::deleteGameTest()
{
   GameSessionId gsi("fred");
   CPPUNIT_ASSERT_EQUAL(false, m_pDb->gameInstanceExists(gsi));
   m_pDb->makeGameInstance(gsi, "name", "type", "tariff");
   CPPUNIT_ASSERT_EQUAL(true, m_pDb->gameInstanceExists(gsi));
   m_pDb->deleteGameInstance(gsi);
   CPPUNIT_ASSERT_EQUAL(false, m_pDb->gameInstanceExists(gsi));
}

void TestPurseDb::deletePurseTest()
{
   GameSessionId gsi("fred");
   m_pDb->makeGameInstance(gsi, "name", "type", "tariff");

   PurseData purseData("key", 27, "xyggt", "saejkdfhkj");

   const PurseId purseid = m_pDb->savePurse(gsi, purseData);
   std::map<PurseId, PurseData> purses = m_pDb->loadPurses(gsi);
   CPPUNIT_ASSERT_EQUAL(size_t(1), purses.size());
   m_pDb->deletePurse(gsi, purseid);
   std::map<PurseId, PurseData> purses2 = m_pDb->loadPurses(gsi);
   CPPUNIT_ASSERT_EQUAL(size_t(0), purses2.size());
}
