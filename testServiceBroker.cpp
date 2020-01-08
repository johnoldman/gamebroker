//
// $Id: testServiceBroker.cpp 3624 2007-12-14 14:09:19Z johno $
//
#include "testServiceBroker.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCommandBeginUserSession.h"
#include "ApiCommandEndUserSession.h"
#include "ApiCommandMakePayment.h"
#include "ApiCommandMakeSystemPayment.h"
#include "ApiCommandSetUserData.h"
#include "ApiCommandGetUserData.h"
#include "Amount.h"
#include "StubLedger.h"
#include "StubTariff.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestServiceBroker);

void TestServiceBroker::setUp()
{
   m_producerFactory.reset(new StubProducerConsumerFactory);
   m_pConsumer.reset(new StubMessageConsumer);
   m_pStore.reset(new StubPersistentStore);
   m_pKyr.reset(new StubKYR);
   m_pLedger.reset(new StubLedger);
   m_pLogger.reset(new StubLogger);
   m_pUserService.reset(new StubUserService);
   m_pServiceBroker = new ServiceBroker(*m_producerFactory, *m_pConsumer, *m_pStore, *m_pKyr, *m_pLedger, *m_pLogger, *m_pUserService);
   m_pProducer = m_producerFactory->pLastProducer;
}

void TestServiceBroker::tearDown()
{
   delete m_pServiceBroker;
   m_pServiceBroker = 0;
   m_pUserService.reset();
   m_pLogger.reset();
   m_pConsumer.reset();
   m_pStore.reset();
   m_pProducer = 0;
}

void TestServiceBroker::runTest()
{
   const time_t now(time(NULL));
   ApiCommandRegisterGameInstance rgi("name", "type", "sessionid", "gameSessionId");
   StubProducer producer;
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>RGI</response><messageId>nametypesessionid</messageId><errorCode>0</errorCode></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // cause a SglServerDuplicateGameSessionId exception
   ApiCommandRegisterGameInstance rgi2("another_name", "another_type", "sessionid", "gameSessionId");
   rgi2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT(m_producerFactory->pLastButOneProducer != 0);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>RGI</response><messageId>another_nameanother_typesessionid</messageId><errorCode>7</errorCode><errorString>sessionid</errorString></mq:envelope>"), m_producerFactory->pLastButOneProducer->getMessage());

   std::string messageId("messageId");
   std::string gameSessionId("sessionid");
   long userId = 666L;
   std::string userSessionId("userSessionId");
   std::string tariffId("tariffId");
   long gameCost = 7687L;
   long gameFloorLimit = 675675L;
   ApiCommandBeginUserSession bus(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>" + messageId + "</messageId><errorCode>0</errorCode><purseTotal>0</purseTotal></mq:envelope>"), m_producerFactory->pLastButOneProducer->getMessage());

   ApiCommandDeregisterGameInstance dri(gameSessionId);
   dri.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(2, m_producerFactory->m_producerCount);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>sessionid</messageId><errorCode>0</errorCode></mq:envelope>"), m_producerFactory->pLastButOneProducer->getMessage());

   // send a second deregister message - expect a 'no such session' message
   ApiCommandDeregisterGameInstance dri2("sessionid_001");
   dri2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   //CPPUNIT_ASSERT_EQUAL(3, m_producerFactory->m_producerCount);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>sessionid_001</messageId><errorCode>1</errorCode><errorString>sessionid_001</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // this time it will fail as the game session has been destroyed
   messageId = "messageId_02";
   ApiCommandBeginUserSession buss(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   buss.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>" + messageId + "</messageId><errorCode>1</errorCode><errorString>sessionid</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // this time it will fail as the game session has been destroyed
   messageId = "messageId_03";
   ApiCommandEndUserSession eus(messageId, gameSessionId, userSessionId);
   eus.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>EUS</response><messageId>" + messageId + "</messageId><errorCode>1</errorCode><errorString>sessionid</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // this time it will fail as the game session has been destroyed
   messageId = "messageId_04";
   std::string fromUserSessionId("The killer");
   std::string toUserSessionId("The killed");
   long amount = 100;
   std::string eventCode("Killer shot killed");
   ApiCommandMakePayment mpy(messageId, gameSessionId, fromUserSessionId, toUserSessionId, amount, eventCode);
   mpy.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), now);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MPY</response><messageId>" + messageId + "</messageId><errorCode>1</errorCode><errorString>sessionid</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::testEndUserSession()
{
   std::string gameName("nameOfTheGame");
   std::string gameType("typeOfTheGame");
   std::string gameTariffId("gameTariffId");
   std::string gameSessionId("idOfTheGameSession");
   long userId = 9090909L;
   std::string userSessionId("idOfTheUserSession");
   std::string tariffId("idOfTheTarrif");
   long gameCost = 1000L;
   long gameFloorLimit = 9999L;

   // register a game instance
   StubProducer producer;
   ApiCommandRegisterGameInstance rgi(gameName, gameType, gameSessionId, gameTariffId);
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // begin a user session
   std::string messageId("messageId_777");
   ApiCommandBeginUserSession bus(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // test that end user session works
   messageId = "messageId_778";
   ApiCommandEndUserSession eus(messageId, gameSessionId, userSessionId);
   eus.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>EUS</response><messageId>"
      + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>")
      , m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::testMakePayment()
{
   std::string gameName("yetAnotherNameOfTheGame");
   std::string gameType("yetAnotherTypeOfTheGame");
   std::string gameSessionId("yetAnotherIdOfTheGameSession");
   std::string gameTariffId("gameTariffId");
   long killerUserId = 9090908L;
   long killedUserId = 9090909L;
   std::string killerUserSessionId("idOfTheKillerUserSession");
   std::string killedUserSessionId("idOfTheKilledUserSession");
   std::string tariffId("tariffId_098");
   long gameCost = 100L;
   long gameFloorLimit = 999L;
   long amount = 10L;
   std::string eventCode("eventCode_098");

   // register a game instance
   StubProducer producer;
   ApiCommandRegisterGameInstance rgi(gameName, gameType, gameSessionId, gameTariffId);
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // first we put something into the victim's ledger
   boost::shared_ptr<StubTariff> tariff(new StubTariff);
   tariff->addMapping(eventCode, 1, 2);
   m_pKyr->addTariff(tariffId, tariff);
   m_pLedger->addToPurse(killedUserId, Amount(1000), 1);

   // begin a user session for the killer
   std::string messageId("messageId_333");
   ApiCommandBeginUserSession bus1(messageId, gameSessionId, killerUserId, killerUserSessionId, tariffId, gameCost, gameFloorLimit);
   bus1.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // begin a user session for the killed
   messageId = "messageId_334";
   ApiCommandBeginUserSession bus2(messageId, gameSessionId, killedUserId, killedUserSessionId, tariffId, gameCost, gameFloorLimit);
   bus2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // killer shoots killed
   messageId = "messageId_335";
   ApiCommandMakePayment mpy(messageId, gameSessionId, killedUserSessionId, killerUserSessionId, amount, eventCode);
   mpy.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MPY</response><messageId>" + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>")
      , m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::testMakeSystemPayment()
{
   const std::string gameName("andYetAnotherAnotherNameOfTheGame");
   const std::string gameType("andYetAnotherAnotherTypeOfTheGame");
   const std::string gameSessionId("andYetAnotherAnotherIdOfTheGameSession");
   const std::string gameTariffId("tariff1");
   const std::string tariffId("tariff1");
   const std::string eventCode("HELLO");
   const std::string totalClassification("totalClassification");
   const std::string totalType("JACKPOT");
   long gameCost = 10L;
   long gameFloorLimit = 100L;

   // register a game instance
   StubProducer producer;
   ApiCommandRegisterGameInstance rgi(gameName, gameType, gameSessionId, gameTariffId);
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // first we put something into player's ledger
   boost::shared_ptr<StubTariff> tariff(new StubTariff);
   tariff->addMapping(eventCode, 1, 2);
   tariff->addMapping(eventCode, 2, 2);
   m_pKyr->addTariff(tariffId, tariff);
   long playerId = 007;
   m_pLedger->addToPurse(playerId, Amount(1000), 1);

   // begin a user session
   const std::string messageId_01("messageId_778");
   std::string playerSessionId("idOfThePlayerUserSession");
   ApiCommandBeginUserSession bus(messageId_01, gameSessionId, playerId, playerSessionId, tariffId, gameCost, gameFloorLimit);
   bus.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // player pays the game
   long amount = 10L;

   const std::string messageId_02("messageId_779");
   bool payUserA = false;
   ApiCommandMakeSystemPayment msp1(messageId_02, gameSessionId, playerSessionId, payUserA, amount, totalType, totalClassification, eventCode);
   msp1.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MSP</response><messageId>" + messageId_02 + "</messageId><errorCode>0</errorCode></mq:envelope>") , m_producerFactory->pLastProducer->getMessage());

   // game pays a player
   const std::string messageId_03("messageId_780");
   bool payUserB = true;
   ApiCommandMakeSystemPayment msp2(messageId_03, gameSessionId, playerSessionId, payUserB, amount, totalType, totalClassification, eventCode);
   msp2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MSP</response><messageId>" + messageId_03 + "</messageId><errorCode>0</errorCode></mq:envelope>") , m_producerFactory->pLastProducer->getMessage());

   // player pays the game with insuficient funds
   long largeAmount = 1000000000000L;
   const std::string messageId_04("messageId_781");
   bool payUserC = false;
   ApiCommandMakeSystemPayment msp3(messageId_04, gameSessionId, playerSessionId, payUserC, largeAmount, totalType, totalClassification, eventCode);
   msp3.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MSP</response><messageId>" + messageId_04 + "</messageId><errorCode>13</errorCode><errorString>Event Code HELLO Purse::removeAssets  Not Enough Money in Purse Id 0</errorString></mq:envelope>") , m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::testGetSet()
{
   std::string gameSessionId("sessionid");
   std::string gameTariffId("gameTariffId");
   ApiCommandRegisterGameInstance rgi("name", "type", gameSessionId, gameTariffId);
   StubProducer producer;
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>RGI</response><messageId>nametypesessionid</messageId><errorCode>0</errorCode></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   std::string messageId = "messageId_05";
   std::string userDataType("userDataType");
   std::string userData("userData?&<>");
   long userId = 4;
   ApiCommandSetUserData sud(messageId, gameSessionId, userId, userDataType, userData);
   sud.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>SUD</response><messageId>messageId_05</messageId><errorCode>0</errorCode></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_EQUAL(userData, m_pUserService->obtainSetDataField(userId, userDataType));

   producer.clearMessage();

   messageId = "uytuytuytuyt";
   ApiCommandGetUserData gud(messageId, gameSessionId, userId, userDataType);
   gud.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>GUD</response><messageId>uytuytuytuyt</messageId><errorCode>0</errorCode><userData>hello</userData></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::transactionLogTest()
{
   std::string gameName("gameName");
   std::string gameType("gameType");
   std::string messageId("LogTestmessageId");
   std::string gameSessionId("sessionid");
   std::string gameTariffId("gameTariffId");
   long userId = 666L;
   std::string userSessionId("userSessionId");
   std::string tariffId("tariffId");
   long gameCost = 42L;
   long gameFloorLimit = 999L;

   // register a game instance
   ApiCommandRegisterGameInstance rgi(gameName, gameType, gameSessionId, gameTariffId);
   StubProducer producer;
   rgi.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // send BUS command to put a record in database
   ApiCommandBeginUserSession bus1(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus1.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   // message response should indicate success
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>" + messageId + "</messageId><errorCode>0</errorCode><purseTotal>0</purseTotal></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // verify that message IS in database
   std::auto_ptr<MessageAndResponse> mar2 = m_pStore->getMessage(messageId);
   CPPUNIT_ASSERT(mar2.get() != 0);

   // send identical command
   ApiCommandBeginUserSession bus2(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));

   // message response should indicate success - but not functional change
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>" + messageId + "</messageId><errorCode>0</errorCode><purseTotal>0</purseTotal></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());

   // note that if the ApiCommandBeginUserSession was processed then the message would have contained an error message warning about a SglServerDuplicateGameSessionId

   // send identical command but with different data
   userId++;
   ApiCommandBeginUserSession bus3(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus3.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   // message response should indicate tragic failure - same message id, different message
   CPPUNIT_ASSERT_EQUAL(3, m_producerFactory->m_producerCount);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>LogTestmessageId</messageId><errorCode>11</errorCode><errorString>New Message = &lt;mq:envelope&gt;&lt;command&gt;BUS&lt;/command&gt;&lt;messageId&gt;LogTestmessageId&lt;/messageId&gt;&lt;parameters&gt;&lt;gameSessionId&gt;sessionid&lt;/gameSessionId&gt;&lt;userId&gt;667&lt;/userId&gt;&lt;userSessionId&gt;userSessionId&lt;/userSessionId&gt;&lt;tariffId&gt;tariffId&lt;/tariffId&gt;&lt;gameCost&gt;42&lt;/gameCost&gt;&lt;gameFloorLimit&gt;999&lt;/gameFloorLimit&gt;&lt;/parameters&gt;&lt;/mq:envelope&gt; compared to old Message = &lt;mq:envelope&gt;&lt;command&gt;BUS&lt;/command&gt;&lt;messageId&gt;LogTestmessageId&lt;/messageId&gt;&lt;parameters&gt;&lt;gameSessionId&gt;sessionid&lt;/gameSessionId&gt;&lt;userId&gt;666&lt;/userId&gt;&lt;userSessionId&gt;userSessionId&lt;/userSessionId&gt;&lt;tariffId&gt;tariffId&lt;/tariffId&gt;&lt;gameCost&gt;42&lt;/gameCost&gt;&lt;gameFloorLimit&gt;999&lt;/gameFloorLimit&gt;&lt;/parameters&gt;&lt;/mq:envelope&gt; and old  Response = &lt;mq:envelope&gt;&lt;response&gt;BUS&lt;/response&gt;&lt;messageId&gt;LogTestmessageId&lt;/messageId&gt;&lt;errorCode&gt;0&lt;/errorCode&gt;&lt;purseTotal&gt;0&lt;/purseTotal&gt;&lt;/mq:envelope&gt;</errorString></mq:envelope>"), m_producerFactory->pLastButOneProducer->getMessage());
   // note that if the ApiCommandBeginUserSession was processed then the message would have contained an error message warning about a SglServerDuplicateGameSessionId
}

void TestServiceBroker::driFailoverTest()
{
   // deregister a game instance
   std::string gameSessionId("gameSessionId_from_axed_server");
   std::string gameName("gameName");
   std::string gameType("gameType");

   // verify attempt to end game session not on local store but is on persistant store results in success
   m_pStore->addGameInstance(GameSessionId(gameSessionId));
   ApiCommandDeregisterGameInstance dri_1(gameSessionId);
   StubProducer producer;
   dri_1.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>"
                                     + gameSessionId + "</messageId><errorCode>0</errorCode></mq:envelope>")
                                     , m_producerFactory->pLastProducer->getMessage());

   // verify attempt to end game session not on local store or persistant store still results in failure
   gameSessionId = "gameSessionId_NOT_from_axed_server";
   ApiCommandDeregisterGameInstance dri_2(gameSessionId);
   dri_2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>"
                                     + gameSessionId + "</messageId><errorCode>1</errorCode><errorString>"
                                     + gameSessionId + "</errorString></mq:envelope>")
                                     , m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::busFailoverTest()
{
   // begin a user session
   std::string gameSessionId("gameSesId_from_axed_server");
   std::string gameName("gameName");
   std::string gameType("gameType");
   std::string messageId("messageId");
   long userId = 666L;
   std::string userSessionId("userSessionId");
   std::string tariffId("tariffId");
   long gameCost = 7687L;
   long gameFloorLimit = 675675L;

   // set up a game session to the persistant store that's NOT in local store
   m_pStore->addGameInstance(GameSessionId(gameSessionId));

   // verify attempt to begin a user session not on local store but is on persistant store results in success
   ApiCommandBeginUserSession bus1(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   StubProducer producer;
   bus1.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>"
      + messageId
      + "</messageId><errorCode>0</errorCode><purseTotal>0</purseTotal></mq:envelope>")
      , m_producerFactory->pLastProducer->getMessage());

    // verify attempt to begin user session not on local store or persistant store still results in failure
   gameSessionId = "gameSesId_NOT_from_axed_server";
   messageId = "another_messageId";
   ApiCommandBeginUserSession bus2(messageId, gameSessionId, userId, userSessionId, tariffId, gameCost, gameFloorLimit);
   bus2.send(producer);
   m_pServiceBroker->receive(producer.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>"
      + messageId
      + "</messageId><errorCode>1</errorCode><errorString>"
      + gameSessionId
      + "</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage() );
}

void TestServiceBroker::endGameSessionTest()
{
   const std::string gameName("gameName");
   const std::string gameType("gameType");
   const std::string gameSessionId("wibbblewibblewibble");
   const std::string gameTariffId("gameTariffId");

   // register a game instance
   const ApiCommandRegisterGameInstance rgi(gameName, gameType, gameSessionId, gameTariffId);
   m_pServiceBroker->onRegisterGameInstance(rgi);

   // Remove it
   const ApiCommandDeregisterGameInstance dri(gameSessionId);
   m_pServiceBroker->onDeregisterGameInstance(dri);
   StubProducer serial;
   dri.send(serial);

   // Second de-register has to be via receive(), as onDeregisterGameInstance assumes that item exists.
   m_pServiceBroker->receive(serial.getMessage(), time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>" + gameSessionId + "</messageId><errorCode>1</errorCode><errorString>wibbblewibblewibble</errorString></mq:envelope>"), m_producerFactory->pLastProducer->getMessage());
}

void TestServiceBroker::correctReplyTest()
{
   // Need to send two messages to server on channels X & Y and prove that they come back on
   // the channels that they were sent on.
   const std::string gameName("gameName");
   const std::string gameType("gameType");
   const std::string gameSessionId_A("wibbblewibblewibbleA");
   const std::string gameSessionId_B("wibbblewibblewibbleB");
   const std::string gameSessionId_C("wibbblewibblewibbleC");
   const std::string gameSessionId_D("wibbblewibblewibbleD");
   const std::string gameTariffId("gameTariffId");

   // register a game instance
   const ApiCommandRegisterGameInstance rgi_A(gameName, gameType, gameSessionId_A, gameTariffId);
   m_pServiceBroker->onRegisterGameInstance(rgi_A);
   CPPUNIT_ASSERT(0 != m_producerFactory->pLastProducer);
   CPPUNIT_ASSERT_EQUAL(gameSessionId_A, m_producerFactory->pLastProducer->getName());


   // the following tests coverage in ServiceBroker::retireOldProducers()
   const ApiCommandRegisterGameInstance rgi_B(gameName, gameType, gameSessionId_B, gameTariffId);
   m_pServiceBroker->onRegisterGameInstance(rgi_B);

   const ApiCommandRegisterGameInstance rgi_C(gameName, gameType, gameSessionId_C, gameTariffId);
   m_pServiceBroker->onRegisterGameInstance(rgi_C);

   const ApiCommandDeregisterGameInstance dgi_A(gameSessionId_A);
   m_pServiceBroker->onDeregisterGameInstance(dgi_A);

   const ApiCommandDeregisterGameInstance dgi_B(gameSessionId_B);
   m_pServiceBroker->onDeregisterGameInstance(dgi_B);

   const ApiCommandDeregisterGameInstance dgi_C(gameSessionId_C);
   m_pServiceBroker->onDeregisterGameInstance(dgi_C);
}
