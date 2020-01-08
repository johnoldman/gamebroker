//
// $Id: testGameInstance.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testGameInstance.h"

#include "serviceside/GameInstance.h"
#include "StubMessageConsumer.h"
#include "serviceside/PurseNotFoundException.h"
#include "serviceside/DuplicateSessionException.h"
#include "serviceside/DuplicateUserException.h"
#include "serviceside/NotEnoughMoneyException.h"
#include "StubKYR.h"
#include "StubLedger.h"
#include "StubTariff.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestGameInstance);

void TestGameInstance::setUp()
{
   m_pConsumer = new StubMessageConsumer;
   m_pKYR = new StubKYR;
   m_pLedger = new StubLedger;
   m_pStore.reset(new StubPersistentStore);
   m_pGameInstance = new GameInstance(*m_pStore, GameSessionId("hello"), "wibble", "wobble", "tariff1", *m_pKYR, *m_pLedger);
   UserSessionId newSession("a");
   long userid = 27;
   m_pLedger->addToPurse(userid, Amount(40), 1);
   boost::shared_ptr<StubTariff> tariff(new StubTariff);
   tariff->addMapping("HELLO", 1, 2);
   tariff->addMapping("HELLO", 2, 1);
   m_pKYR->addTariff("tariff1", tariff);
   m_pGameInstance->beginUserSession(userid, newSession, "tariff1", 1, 2);
}

void TestGameInstance::tearDown()
{
   delete m_pGameInstance;
   m_pGameInstance = 0;
   m_pStore.reset();
   delete m_pLedger;
   m_pLedger = 0;
   delete m_pKYR;
   m_pKYR = 0;
   //delete m_pConsumer;
   //m_pConsumer = 0;
}

void TestGameInstance::runTest()
{
   // End a non-existant session
   UserSessionId nonExistantSession("x");
   CPPUNIT_ASSERT_THROW(m_pGameInstance->endUserSession(nonExistantSession), PurseNotFoundException);
}

void TestGameInstance::beginUserSessionTest()
{
   UserSessionId newSession("a");
   // Check that starting an identical session isn't allowed.
   CPPUNIT_ASSERT_THROW(m_pGameInstance->beginUserSession(27, newSession, "tariff2", 1, 2), DuplicateSessionException);
   // Once we have a session, it should have created a Purse from the ledger. Don't even have a ledger interface yet though...
}

void TestGameInstance::makePaymentTest()
{
   UserSessionId aSession("a");
   UserSessionId bSession("b");
   std::string eventCode("HELLO");
   Amount amount(27);

   m_pLedger->addToPurse(34, Amount(40), 1);

   CPPUNIT_ASSERT_THROW(m_pGameInstance->makePayment(bSession, bSession, amount, eventCode, time(NULL)), PurseNotFoundException);
   // Check that exception is thrown if second purse doesn't exist either but first one does.
   CPPUNIT_ASSERT_THROW(m_pGameInstance->makePayment(aSession, bSession, amount, eventCode, time(NULL)), PurseNotFoundException);
   // Slightly surprising that the same userid is going into the game session twice... but we don't check for this.
   // CPPUNIT_ASSERT_THROW(m_pGameInstance->beginUserSession(27, bSession, "tariff2", 1, 2), DuplicateUserException);

   // Start a session for the second user
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->beginUserSession(34, bSession, "tariff1", 1, 2));
   // Make a payment from a to b for 27 points event code HELLO.
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makePayment(aSession, bSession, amount, eventCode, time(NULL)));

   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makeSystemPayment(bSession, false, Amount(10), "JACKPOT",  "totalClassification", eventCode, time(NULL)));
}

void TestGameInstance::makeSystemPaymentTest()
{
   long userId = 55L;
   UserSessionId userSessionId("c");
   Amount amount(5);
   std::string totalType("JACKPOT");
   std::string totalClassification("totalClassification");
   std::string eventCode("HELLO");
   std::string tariffId("tariff1");
   long gameCost = 10;
   long gameFloorLimit = 50;

   // put some money in the userId's purse
   m_pLedger->addToPurse(userId, Amount(gameFloorLimit), 1);

   // Start a session for userId
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->beginUserSession(userId, userSessionId, tariffId, gameCost, gameFloorLimit));

   // game starts up with no money - verify that it cannot pay userId
   bool payUser = true;
   CPPUNIT_ASSERT_THROW(m_pGameInstance->makeSystemPayment(userSessionId, payUser, amount, totalType, totalClassification, eventCode, time(NULL)), NotEnoughMoneyException);

   // but userId should have money to pay the game
   payUser = false;
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makeSystemPayment(userSessionId, payUser, amount, totalType,  totalClassification, eventCode, time(NULL)));

   // and, we shoud be able to pay into another game purse
   totalType = "another_JACKPOT";
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makeSystemPayment(userSessionId, payUser, amount, totalType,  totalClassification, eventCode, time(NULL)));

   // and, we shoud be able to pay into another game purse
   totalType = "and_yet_another_JACKPOT";
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makeSystemPayment(userSessionId, payUser, amount, totalType,  totalClassification, eventCode, time(NULL)));
}

void TestGameInstance::endUserSessionTest()
{
   // count purses
   long purseCount = m_pGameInstance->getPurseCount();

   // call to endUserSession should remove a purse
   purseCount--;

   UserSessionId theSession("a");
   m_pGameInstance->endUserSession(theSession);
   CPPUNIT_ASSERT_EQUAL((size_t)purseCount, m_pGameInstance->getPurseCount());
}

void TestGameInstance::loadSaveTest()
{
   long userId = 55L;
   UserSessionId userSessionId("c");
   Amount amount(5);
   std::string totalType("JACKPOT");
   std::string totalClassification("totalClassification");
   std::string eventCode("HELLO");
   std::string tariffId("tariff1");
   long gameCost = 10;
   long gameFloorLimit = 50;

   // put some money in the userId's purse
   m_pLedger->addToPurse(userId, Amount(gameFloorLimit), 1);

   // Start a session for userId
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->beginUserSession(userId, userSessionId, tariffId, gameCost, gameFloorLimit));

   // pay game 5 G$ from user 22
   CPPUNIT_ASSERT_NO_THROW(m_pGameInstance->makeSystemPayment(userSessionId, false, amount, totalType,  totalClassification, eventCode, time(NULL)));

   // copy of game instance
   GameInstance gi(*m_pStore, GameSessionId("hello"), *m_pKYR, *m_pLedger);
   // Make sure that both purses have been loaded correctly by paying the 5 G$ back from game to user
   CPPUNIT_ASSERT_NO_THROW(gi.makeSystemPayment(userSessionId, true, amount, totalType, totalClassification, eventCode, time(NULL)));
}
