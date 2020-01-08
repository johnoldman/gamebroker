//
// $Id: testCommands.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testCommands.h"

#include <boost/lexical_cast.hpp>

#include "ApiCommandBeginUserSession.h"
#include "ApiCommandEndUserSession.h"
#include "StubProducer.h"
#include "ApiCommandRegisterGameInstance.h"
#include "ApiCommandMakePayment.h"
#include "ApiCommandDeregisterGameInstance.h"
#include "ApiCommandGetUserData.h"
#include "ApiCommandSetUserData.h"
#include "ApiCommandMakeSystemPayment.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCommands);

void TestCommands::setUp()
{
   m_pProducer = new StubProducer;
}

void TestCommands::tearDown()
{
   delete m_pProducer;
   m_pProducer = 0;
}

void TestCommands::beginUserSessionTest()
{
   const std::string messageId = "wibble";
   const std::string gameSessionId = "fred";
   const long userId = 78;
   const std::string userSessionId = "bill";
   const std::string tariffId = "tariff1";

   long cost = 34;
   long floorLimit = 45;
   ApiCommandBeginUserSession command(messageId, gameSessionId, userId, userSessionId, tariffId, cost, floorLimit);
   command.send(*m_pProducer);

   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>BUS</command><messageId>" + messageId + "</messageId><parameters><gameSessionId>" + gameSessionId + "</gameSessionId><userId>" + boost::lexical_cast<std::string>(userId) + "</userId><userSessionId>" + userSessionId + "</userSessionId><tariffId>" + tariffId + "</tariffId><gameCost>34</gameCost><gameFloorLimit>45</gameFloorLimit></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::registerGameInstanceTest()
{
   const std::string gameName = "hajh";
   const std::string gameType = "jhkjhkj";
   const std::string gameSessionId = "jehfkjh";
   const std::string gameTariffId = "gameTariffId";

   ApiCommandRegisterGameInstance command(gameName,gameType,gameSessionId,gameTariffId);
   command.send(*m_pProducer);
   std::string expectedString = std::string("<mq:envelope>") + "<command>RGI</command><messageId>" + gameName + gameType + gameSessionId + "</messageId><parameters>"
                              + "<gameSessionId>" + gameSessionId + "</gameSessionId>"
                              + "<gameName>" + gameName + "</gameName><gameType>" + gameType + "</gameType>"
                              + "<gameTariffId>" + gameTariffId + "</gameTariffId>"
                              + "</parameters></mq:envelope>";

   CPPUNIT_ASSERT_EQUAL(expectedString, m_pProducer->getMessage());
}

void TestCommands::makePaymentTest()
{
   const std::string messageId = "wibble";
   const std::string gameSessionId = "fred";
   const std::string fromUserSessionId = "jim";
   const std::string toUserSessionId = "bill";
   long amount = 27;
   const std::string eventCode = "eventCode";

   ApiCommandMakePayment command(messageId, gameSessionId, fromUserSessionId, toUserSessionId, amount, eventCode);
   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>MPY</command><messageId>" + messageId + "</messageId><parameters><gameSessionId>" + gameSessionId + "</gameSessionId><fromUserSessionId>" + fromUserSessionId + "</fromUserSessionId><toUserSessionId>" + toUserSessionId + "</toUserSessionId><amount>" + boost::lexical_cast<std::string>(amount) + "</amount><eventCode>" + eventCode + "</eventCode></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::deRegisterGameInstanceTest()
{
   const std::string gameSessionId = "fred";
   ApiCommandDeregisterGameInstance command(gameSessionId);
   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>DGI</command><messageId>" + gameSessionId + "</messageId><parameters><gameSessionId>" + gameSessionId + "</gameSessionId></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::getUserDataTest()
{
   const std::string messageId = "ghwqfhg";
   const std::string gameSessionId = "sdjhfgj";
   long userId = 7657;
   const std::string userDataType = "sdkjfhkjh";

   ApiCommandGetUserData command(messageId, gameSessionId, userId, userDataType);
   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>GUD</command><messageId>ghwqfhg</messageId><parameters><gameSessionId>sdjhfgj</gameSessionId><userId>" + boost::lexical_cast<std::string>(userId) + "</userId><userDataType>sdkjfhkjh</userDataType></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::setUserDataTest()
{
   const std::string messageId = "ghwqfhg";
   const std::string gameSessionId = "sdjhfgj";
   long userId = 76786876;
   const std::string userDataType = "sdkjfhkjh";
   const std::string userData = "5762537623576";

   ApiCommandSetUserData command(messageId, gameSessionId, userId, userDataType, userData);
   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>SUD</command><messageId>ghwqfhg</messageId><parameters><gameSessionId>sdjhfgj</gameSessionId><userId>" + boost::lexical_cast<std::string>(userId) + "</userId><userDataType>sdkjfhkjh</userDataType><userData>5762537623576</userData></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::endUserSessionTest()
{
   const std::string messageId = "ghwqfhg";
   const std::string gameSessionId = "sdjhfgj";
   const std::string userSessionId = "jim";

   ApiCommandEndUserSession command(messageId, gameSessionId, userSessionId);
   CPPUNIT_ASSERT_EQUAL(userSessionId, command.getUserSessionId());
   CPPUNIT_ASSERT_EQUAL(gameSessionId, command.getGameSessionId());

   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>EUS</command><messageId>ghwqfhg</messageId><parameters><gameSessionId>sdjhfgj</gameSessionId><userSessionId>jim</userSessionId></parameters></mq:envelope>"), m_pProducer->getMessage());
}

void TestCommands::makeSystemPaymentTest()
{
   const std::string messageId = "wibble";
   const std::string gameSessionId = "fred";
   const std::string userSessionId = "jim";
   long amount = 27;
   bool payUser = false;
   const std::string eventCode = "eventCode";
   const std::string totalType = "totalType";
   const std::string totalClassification = "totalClassification";

   ApiCommandMakeSystemPayment command(messageId, gameSessionId, userSessionId, payUser, amount, totalType, totalClassification, eventCode);
   command.send(*m_pProducer);
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><command>MSP</command><messageId>wibble</messageId><parameters><gameSessionId>fred</gameSessionId><userSessionId>jim</userSessionId><payUser>0</payUser><amount>27</amount><totalType>totalType</totalType><totalClassification>totalClassification</totalClassification><eventCode>eventCode</eventCode></parameters></mq:envelope>"), m_pProducer->getMessage());
}
