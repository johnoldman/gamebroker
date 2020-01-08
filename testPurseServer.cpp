//
// $Id: testPurseServer.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testPurseServer.h"

#include "Config.h"
#include "SkillGameServiceClient.h"
#include "MessageIdGenerator.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPurseServer);

void TestPurseServer::setUp()
{
   std::string brokerURI = "tcp://localhost:61626?wireFormat=openwire&transport.useAsyncSend=true";
   //std::string brokerURI = "tcp://tomcat2:61646?wireFormat=openwire&transport.useAsyncSend=true";
   std::string serverQueue = "ZINIX.SERVICE.SERVER";
   std::string clientQueue = "ZINIX.SERVICE.CLIENT";
   std::string loggerPropertiesFile = "brokerclientlog.properties";
   std::string loggerName("TestGameClient");

   Config config(brokerURI, clientQueue, serverQueue, loggerPropertiesFile, loggerName);

   SglGameAPI::GameClient::initialise(config);
}

void TestPurseServer::tearDown()
{
}

static void beginUserCallback(const std::string &gameSessionId,
                              long userId,
                              const std::string &userSessionId,
                              long gameCost,
                              long gameFloorLimit,
                              long purseTotal)
{
}

static void beginUserError(const std::string &gameSessionId,
                           long userId,
                           const std::string &userSessionId,
                           long gameCost,
                           long gameFloorLimit,
                           enum SglGameAPI::SglServerExceptionType SglServerException,
                           const std::string& SglServerExceptionDescription)
{
}

static void endUserCallback(const std::string &messageId,
                            const std::string &gameSessionId,
                            const std::string &userSessionId)
{
}

static void endUserError(const std::string &messageId,
                         const std::string &gameSessionId,
                         const std::string &userSessionId,
                         enum SglGameAPI::SglServerExceptionType SglServerException,
                         const std::string &SglServerExceptionDescription)
{
}

static void makePaymentCallback(const std::string &messageId,
                                    const std::string &gameSessionId,
                                    const std::string &fromUserSessionId,
                                    const std::string &toUserSessionId,
                                    long amount,
                                    const std::string &eventCode)
{
}

static void makePaymentError(const std::string &messageId,
                             const std::string &gameSessionId,
                             const std::string &fromUserSessionId,
                             const std::string &toUserSessionId,
                             long amount,
                             const std::string &eventCode,
                             enum SglGameAPI::SglServerExceptionType SglServerException,
                             const std::string &SglServerExceptionDescription)
{
}

void TestPurseServer::runTest()
{
   std::string gameName("mettle");
   std::string gameType("death");
   std::string gameTariffId("gameTariffId");
   MessageIdGenerator idGen;
   std::string gameSessionId(++idGen);
   SglGameAPI::GameClient::registerGameInstance(gameName,gameType,gameSessionId,gameTariffId);
   sleep(1);

   const long user1 = 27;
   const long user2 = 28;
   const std::string user1Session("user27game");
   const std::string user2Session("user28game");
   const std::string tariffId("fred");
   long gameCost = 0;
   long gameFloorLimit = 500;

   SglGameAPI::GameClient::beginUserSession(++idGen, gameSessionId, user1, user1Session, tariffId, gameCost, gameFloorLimit, beginUserCallback, beginUserError);
   sleep(1);
   SglGameAPI::GameClient::beginUserSession(++idGen, gameSessionId, user2, user2Session, tariffId, gameCost, gameFloorLimit, beginUserCallback, beginUserError);
   sleep(2);

   for (int i = 0; i < 5; ++i)
   {
      SglGameAPI::GameClient::makePayment(++idGen, gameSessionId, user1Session, user2Session, 1, "SHOT", makePaymentCallback, makePaymentError);
      //sleep(1);
   }
   sleep(2);
   for (int i = 0; i < 5; ++i)
   {
      SglGameAPI::GameClient::makePayment(++idGen, gameSessionId, user2Session, user1Session, 1, "SHOT", makePaymentCallback, makePaymentError);
      //sleep(1);
   }

   SglGameAPI::GameClient::endUserSession(++idGen, gameSessionId, user2Session, endUserCallback, endUserError);
   sleep(2);
   SglGameAPI::GameClient::endUserSession(++idGen, gameSessionId, user1Session, endUserCallback, endUserError);
   sleep(2);

   SglGameAPI::GameClient::deregisterGameInstance(gameSessionId);
   // Allow the async transmission of the last acknowledgement to happen.
   sleep(1);
}
