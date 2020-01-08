//
// $Id: testReplyMessageRouting.cpp 3761 2007-12-19 11:39:52Z stephend $
//
#include "testReplyMessageRouting.h"
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
#include "Config.h"
#include "Logger.h"
#include "StubLogger.h"
#include "StubProducerConsumerFactory.h"
#include "ProducerConsumerFactory.h"
#include "ActiveMqConsumerHandler.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testReplyMessageRouting);

static std::string lastErrorMessage;
static std::string lastUserData;

void testReplyMessageRouting::setUp()
{
   m_pFactory = new StubProducerConsumerFactory;
   m_pLogger.reset(new StubLogger);
   m_pGameClient = new GameClientImpl(std::auto_ptr<ProducerConsumerFactory>(m_pFactory), *m_pLogger);
   m_pInstanceReply = new SyncDelayedReply(*m_pGameClient);
}

void testReplyMessageRouting::tearDown()
{
   delete m_pInstanceReply;
   m_pInstanceReply = 0;
   delete m_pGameClient;
   m_pGameClient = 0;
}

void testReplyMessageRouting::testClientToServer()
{
   std::string gameName("gameName");
   std::string gameType("gameType");
   std::string gameTariffId("gameTariffId");

   // we start with no consumers
   CPPUNIT_ASSERT_EQUAL(0, m_pFactory->getNumberOfConsumers());

   std::string gameSessionId_A("gameSessionId_A");
   std::string messId_A = gameName+gameType+gameSessionId_A;
   // verify that calls to registerGameInstance with different gameSessionId results 
   // in consumers with different channels
   std::string registerGameInstanceResponseMessage_A = "<mq:envelope><response>RGI</response><messageId>"
                                        + messId_A + "</messageId><errorCode>0</errorCode></mq:envelope>";

   m_pInstanceReply->sendMessage(registerGameInstanceResponseMessage_A);

   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->registerGameInstance(gameName,gameType,gameSessionId_A,gameTariffId));

   // verify that a consumer has been added
   CPPUNIT_ASSERT_EQUAL(1, m_pFactory->getNumberOfConsumers());

   std::string gameSessionId_B("gameSessionId_B");
   std::string messId_B = gameName+gameType+gameSessionId_B;
   // verify that calls to registerGameInstance with different gameSessionId results 
   // in consumers with different channels
   std::string registerGameInstanceResponseMessage_B = "<mq:envelope><response>RGI</response><messageId>"
                                        + messId_B + "</messageId><errorCode>0</errorCode></mq:envelope>";
   m_pInstanceReply->sendMessage(registerGameInstanceResponseMessage_B);
   CPPUNIT_ASSERT_NO_THROW(m_pGameClient->registerGameInstance(gameName,gameType,gameSessionId_B,gameTariffId));

   // verify another consumer has been added
   CPPUNIT_ASSERT_EQUAL(2, m_pFactory->getNumberOfConsumers());
}
