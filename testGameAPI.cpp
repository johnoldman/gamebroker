//
// $Id: testGameAPI.cpp 3621 2007-12-14 12:38:23Z stephend $
//
#include <string>
#include <iostream>
#include "SkillGameServiceClient.h"
#include "testGameAPI.h"
#include "StubProducer.h"
#include "GameClientImpl.h"
#include "SyncDelayedReply.h"
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "BeginUserSessionResponse.h"
#include "EndUserSessionResponse.h"
#include "MakePaymentResponse.h"
#include "GetUserDataResponse.h"
#include "SetUserDataResponse.h"

#include "ActiveMqConsumerHandler.h"
#include "StubProducerConsumerFactory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestGameAPI);

#include <iostream>

TestGameAPI::TestGameAPI()
:gameName("Mettle")
,gameType("Killing")
,gameSessionId("gameIdsendMessage_01")
,userId(6567)
,userSessionId("bert_01_id2")
,gameCost(100)
,gameFloorLimit(10)
,fromUserSessionId("barny_05")
,toUserSessionId("todd_09")
,amount(2)
,eventCode("event_code_aaa")
,payUser(true)
,totalType("total_type_98")
,systemDataType("sysDataType_009922")
,dataKey("data_key_101")
,userDataType("user_data_type_9978897")
{
}

static long lastPurseTotal;
static enum SglGameAPI::SglServerExceptionType lastError;
static std::string lastErrorMessage;
static std::string lastUserData;

void TestGameAPI::setUp()
{
   m_pFactory = new StubProducerConsumerFactory;
   m_pLogger.reset(new StubLogger);
   m_pGameClient = new GameClientImpl(std::auto_ptr<ProducerConsumerFactory>(m_pFactory), *m_pLogger);
   beginInstanceReply = new SyncDelayedReply(*m_pGameClient);
   endInstanceReply = new SyncDelayedReply(*m_pGameClient);
   lastPurseTotal = 0;
   lastError = SglGameAPI::SglSuccess;
}

void TestGameAPI::tearDown()
{
   delete beginInstanceReply;
   beginInstanceReply = 0;
   delete endInstanceReply;
   endInstanceReply = 0;
   delete m_pGameClient;
   m_pGameClient = 0;
}

static void beginUserSessionCallback(const std::string &gameSessionId,
                                     long userId,
                                     const std::string &userSessionId,
                                     long gameCost,
                                     long gameFloorLimit,
                                     long purseTotal)
{
   lastPurseTotal = purseTotal;
   lastError = SglGameAPI::SglSuccess;
}

static void beginUserSessionExceptionCallback(const std::string &gameSessionId,
                                              long userId,
                                              const std::string &userSessionId,
                                              long gameCost,
                                              long gameFloorLimit,
                                              enum SglGameAPI::SglServerExceptionType sglServerException,
                                              const std::string& sglServerExceptionDescription)
{
   lastError = sglServerException;
   lastErrorMessage = sglServerExceptionDescription;
}

static void endUserSessionCallback(const std::string &messageId,
                                   const std::string &gameSessionId,
                                   const std::string &userSessionId)
{
   lastError = SglGameAPI::SglSuccess;
}

static void endUserSessionExceptionCallback(const std::string &messageId,
                                   const std::string &gameSessionId,
                                   const std::string &userSessionId,
                                   enum SglGameAPI::SglServerExceptionType sglServerException,
                                   const std::string& sglServerExceptionDescription)
{
   lastError = sglServerException;
   lastErrorMessage = sglServerExceptionDescription;
}

static void makePaymentCallback(const std::string &messageId,
                      const std::string &gameSessionId,
                      const std::string &fromUserSessionId,
                      const std::string &toUserSessionId,
                      long amount,
                      const std::string &eventCode)
{
}

static void makePaymentExceptionCallback(const std::string &messageId,
                               const std::string &gameSessionId,
                               const std::string &fromUserSessionId,
                               const std::string &toUserSessionId,
                               long amount,
                               const std::string &eventCode,
                                         enum SglGameAPI::SglServerExceptionType SglServerException,
                                         const std::string &SglServerExceptionDescription)
{
   lastError = SglServerException;
   lastErrorMessage = SglServerExceptionDescription;
}

static void makeSystemPaymentCallback(const std::string &messageId,
                                      const std::string &gameSessionId,
                                      const std::string &userSessionId,
                                      bool payUser,
                                      long amount,
                                      const std::string &totalType,
                                      const std::string &totalClassification,
                                      const std::string &eventCode)
{
}

static void makeSystemPaymentExceptionCallback(const std::string &messageId,
                                               const std::string &gameSessionId,
                                               const std::string &userSessionId,
                                               bool payUser,
                                               long amount,
                                               const std::string &totalType,
                                               const std::string &totalClassification,
                                               const std::string &eventCode,
                                               enum SglGameAPI::SglServerExceptionType SglServerException,
                                               const std::string &SglServerExceptionDescription)
{
   lastError = SglServerException;
   lastErrorMessage = SglServerExceptionDescription;
}

static void getUserDataCallback(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    long userId,
                                    const std::string &userDataType,
                                    const std::string &userData)
{
   lastUserData = userData;
}

static void getUserDataExceptionCallback(const std::string &messageId,
                                         const std::string &gameSessionId,
                                         long userId,
                                         const std::string &userDataType,
                                         enum SglGameAPI::SglServerExceptionType SglServerException,
                                         const std::string &SglServerExceptionDescription)
{
   lastError = SglServerException;
   lastErrorMessage = SglServerExceptionDescription;
}

static void setUserDataCallback(const std::string &messageId,
		                const std::string &gameSessionId,
		                long userId,
                                const std::string &userDataType,
                                const std::string &userData)
{
}

static void setUserDataExceptionCallback(const std::string &messageId,
		                         const std::string &gameSessionId,
		                         long userId,
                                         const std::string& userDataType,
                                         const std::string& userData,
                                         enum SglGameAPI::SglServerExceptionType SglServerException,
                                         const std::string &SglServerExceptionDescription)
{
   lastError = SglServerException;
   lastErrorMessage = SglServerExceptionDescription;
}


void TestGameAPI::sessionTest()
{
   std::string messId = gameName+gameType+gameSessionId;
   std::string gameTariffId("gameTariffId");
   std::string registerGameInstanceResponseMessage = "<mq:envelope><response>RGI</response><messageId>"
                                       + messId + "</messageId><errorCode>0</errorCode></mq:envelope>";
   beginInstanceReply->sendMessage(registerGameInstanceResponseMessage);

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->registerGameInstance(gameName,gameType,gameSessionId,gameTariffId));

   m_pFactory->pLastProducer->clearMessage();
   const std::string messageId("a");
   const std::string gameSessionId("b");
   const long userId = 23;
   const std::string userSessionId("df");
   const std::string tariffId("tariff1");
   long gameCost = 43;
   long gameFloorLimit = 67;

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->beginUserSession(messageId,
                                                           gameSessionId,
                                                           userId,
                                                           userSessionId,
                                                           tariffId,
                                                           gameCost,
                                                           gameFloorLimit,
                                                           beginUserSessionCallback,
                                                           beginUserSessionExceptionCallback));

   lastPurseTotal = 0;
   lastError = SglGameAPI::SglServerUnknownUserSessionId;
   m_pGameClient->receive(std::string("<mq:envelope><response>BUS</response><messageId>") + messageId + "</messageId><errorCode>0</errorCode><purseTotal>27</purseTotal></mq:envelope>", time(NULL));
   CPPUNIT_ASSERT_EQUAL(27L, lastPurseTotal);
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->beginUserSession(messageId,
                                                           gameSessionId,
                                                           userId,
                                                           userSessionId,
                                                           tariffId,
                                                           gameCost,
                                                           gameFloorLimit,
                                                           beginUserSessionCallback,
                                                           beginUserSessionExceptionCallback));
   lastPurseTotal = 0;

   m_pGameClient->receive(std::string("<mq:envelope><response>BUS</response><messageId>") + messageId + "</messageId><errorCode>4</errorCode><errorString>The world has ended</errorString></mq:envelope>", time(NULL));
   CPPUNIT_ASSERT_EQUAL(0L, lastPurseTotal);
   // This error corresponds to error code 4 in above message
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglServerUnknownUserSessionId, lastError);
   CPPUNIT_ASSERT_EQUAL(std::string("The world has ended"), lastErrorMessage);
}

void TestGameAPI::getUserDataTest()
{
   lastUserData = "";
   const std::string messageId("a");
   const std::string gameSessionId("b");
   long userId = 666L;
   const std::string userDataType("df");
   m_pGameClient->getUserData(messageId, gameSessionId, userId, userDataType, getUserDataCallback, getUserDataExceptionCallback);
   m_pGameClient->receive(std::string("<mq:envelope><response>GUD</response><messageId>") + messageId + "</messageId><errorCode>0</errorCode><userData>wibblewobble</userData></mq:envelope>", time(NULL));
   CPPUNIT_ASSERT_EQUAL(std::string("wibblewobble"), lastUserData);
}





void TestGameAPI::runTest()
{
   // registerGameInstance
   std::string gameSessionId("gameSessionId");
   std::string gameTariffId("gameTariffId");
   std::string messId = gameName+gameType+gameSessionId;
   std::string registerGameInstanceResponseMessage = "<mq:envelope><response>RGI</response><messageId>"
                                       + messId + "</messageId><errorCode>0</errorCode></mq:envelope>";
   beginInstanceReply->sendMessage(registerGameInstanceResponseMessage);

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->registerGameInstance(gameName,gameType,gameSessionId,gameTariffId));
   std::string expectedString = std::string("<mq:envelope><command>RGI</command><messageId>")
                                 + gameName + gameType + gameSessionId + "</messageId>"
                                 + "<parameters>"
                                 + "<gameSessionId>" + gameSessionId +"</gameSessionId>"
                                 + "<gameName>" + gameName
                                 +"</gameName><gameType>"
                                 +gameType
                                 +"</gameType>"
                                 + "<gameTariffId>" + gameTariffId + "</gameTariffId>"
                                 + "</parameters></mq:envelope>";
   CPPUNIT_ASSERT_EQUAL(expectedString, m_pFactory->pLastProducer->getMessage());
   m_pFactory->pLastProducer->clearMessage();

   // makePayment
   const std::string messageId("a");
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->makePayment(messageId,
                                                      gameSessionId,
                                                      fromUserSessionId,
                                                      toUserSessionId,
                                                      amount,
                                                      eventCode,
                                                      makePaymentCallback,
                                                      makePaymentExceptionCallback));
   expectedString = std::string("<mq:envelope><command>MPY</command><messageId>")
                                + messageId
                                + "</messageId><parameters><gameSessionId>"
                                + gameSessionId
                                + "</gameSessionId><fromUserSessionId>"
                                + fromUserSessionId
                                + "</fromUserSessionId><toUserSessionId>"
                                + toUserSessionId
                                + "</toUserSessionId><amount>2</amount><eventCode>"
                                + eventCode
                                + "</eventCode></parameters></mq:envelope>";
   CPPUNIT_ASSERT_EQUAL(expectedString, m_pFactory->pLastProducer->getMessage());
   m_pFactory->pLastProducer->clearMessage();

   const std::string messageIdB("b");
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->makePayment(messageIdB,
                                                      gameSessionId,
                                                      fromUserSessionId,
                                                      toUserSessionId,
                                                      amount,
                                                      eventCode,
                                                      makePaymentCallback,
                                                      makePaymentExceptionCallback));
   // simulate an SglServerInsufficiantFunds exception from the server
   m_pGameClient->receive(std::string("<mq:envelope><response>MPY</response><messageId>") + messageIdB + "</messageId><errorCode>5</errorCode><errorString>The player is skint</errorString></mq:envelope>", time(NULL));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglServerInsufficientFunds, lastError);
   CPPUNIT_ASSERT_EQUAL(std::string("The player is skint"), lastErrorMessage);

   // deregisterGameInstance
   messId = gameSessionId;
   expectedString = "<mq:envelope><response>DGI</response><messageId>"
                                + messId + "</messageId><errorCode>0</errorCode></mq:envelope>";
   endInstanceReply->sendMessage(expectedString);
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->deregisterGameInstance(gameSessionId));
   expectedString = std::string("<mq:envelope><command>DGI</command><messageId>")
                                + gameSessionId
                                + "</messageId><parameters><gameSessionId>"
                                + gameSessionId + "</gameSessionId></parameters></mq:envelope>";
   CPPUNIT_ASSERT_EQUAL(expectedString, m_pFactory->pLastProducer->getMessage());
   m_pFactory->pLastProducer->clearMessage();
}

void TestGameAPI::sendReceiveTest()
{
   // beginInstanceReply
   std::string gameName("Mettle");
   std::string gameType("fight");
   std::string gameSessionId("fight");
   std::string gameTariffId("gameTariffId");
   RegisterGameInstanceResponse rgir(gameName + gameType + gameSessionId);
   rgir.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->registerGameInstance(gameName,gameType,gameSessionId,gameTariffId));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // endInstanceReply
   DeregisterGameInstanceResponse dergir(gameSessionId);
   dergir.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->deregisterGameInstance(gameSessionId));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // beginUserSessionReply
   std::string messageId("messageId090909");
   long userId = 90909;
   std::string userSessionId("userSessionId090909");
   std::string tariffId("tariffIduyrtu");
   long gameCost = 100909;
   long gameFloorLimit = 3099459848;
   long purseTotal = 7676767;
   BeginUserSessionResponse busr(messageId, purseTotal);
   busr.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->beginUserSession(messageId,
                                                           gameSessionId,
                                                           userId,
                                                           userSessionId,
                                                           tariffId,
                                                           gameCost,
                                                           gameFloorLimit,
                                                           beginUserSessionCallback,
                                                           beginUserSessionExceptionCallback));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // endUserSessionReply
   messageId = "messageId090";
   gameSessionId = "fight";
   userSessionId = "userSessionId0966909";
   EndUserSessionResponse eusr(messageId);
   eusr.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->endUserSession(messageId,
                                                         gameSessionId,
                                                         userSessionId,
                                                         endUserSessionCallback,
                                                         endUserSessionExceptionCallback));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // makePaymentResponse
   std::string fromUserSessionId("killed");
   std::string toUserSessionId("killer");
   long amount = 200;
   std::string eventCode("shot");
   MakePaymentResponse mpr(messageId);
   mpr.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->makePayment(messageId,
                                                      gameSessionId,
                                                      fromUserSessionId,
                                                      toUserSessionId,
                                                      amount,
                                                      eventCode,
                                                      makePaymentCallback,
                                                      makePaymentExceptionCallback));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // getUserDataResponse
   std::string userData("this is user data");
   GetUserDataResponse gudr(messageId, userData);
   gudr.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->getUserData(messageId,
                                                      gameSessionId,
                                                      userId,
                                                      userDataType,
                                                      getUserDataCallback,
                                                      getUserDataExceptionCallback));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);

   m_pFactory->pLastProducer->clearMessage();

   // setUserDataResponse
   SetUserDataResponse sudr(messageId);
   sudr.send(*m_pFactory->pLastProducer);
   beginInstanceReply->sendMessage(m_pFactory->pLastProducer->getMessage());
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->setUserData(messageId,
                                                      gameSessionId,
                                                      userId,
                                                      userDataType,
                                                      userData,
                                                      setUserDataCallback,
                                                      setUserDataExceptionCallback));
   CPPUNIT_ASSERT_EQUAL(SglGameAPI::SglSuccess, lastError);
}

void TestGameAPI::makeSystemPaymentTest()
{
   const std::string messageId("z");
   const std::string gameSessionId("gameSessionId");
   const std::string userSessionId("userSessionId");
   bool payUser = true;
   long amount = 777L;
   const std::string totalType("typeTotal");
   const std::string totalClassification("totalClassification");
   const std::string eventCode("eventCode");

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->makeSystemPayment(messageId,
                                                            gameSessionId,
                                                            userSessionId,
                                                            payUser,
                                                            amount,
                                                            totalType,
                                                            totalClassification,
                                                            eventCode,
                                                            makeSystemPaymentCallback,
                                                            makeSystemPaymentExceptionCallback));
   std::string expectedString = std::string("<mq:envelope><command>MSP</command><messageId>")
                                + messageId
                                + "</messageId><parameters>"
                                + "<gameSessionId>" + gameSessionId + "</gameSessionId>"
                                + "<userSessionId>" + userSessionId + "</userSessionId>"
                                + "<payUser>1</payUser>"
                                + "<amount>777</amount>"
                                + "<totalType>" + totalType + "</totalType>"
                                + "<totalClassification>" + totalClassification + "</totalClassification>"
                                + "<eventCode>" + eventCode + "</eventCode>"
                                + "</parameters></mq:envelope>";
   CPPUNIT_ASSERT_EQUAL(expectedString, m_pFactory->pLastProducer->getMessage());
   m_pFactory->pLastProducer->clearMessage();
}

void TestGameAPI::uniqueMessageIdTest()
{
   const long testlength = 10;
   for(long i=0; i<testlength; i++)
   {
      uniqueMessageIdTestMap.insert(std::pair<std::string, long>(m_pGameClient->getUniqueMessageId(), i)).second;
   }
   CPPUNIT_ASSERT_EQUAL((size_t)testlength, uniqueMessageIdTestMap.size());
}
