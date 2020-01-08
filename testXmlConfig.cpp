#include "testXmlConfig.h"

#include "Config.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestXmlConfig);

void TestXmlConfig::setUp()
{
   m_pConfig = new Config("test/StubApiBroker.xml");
}

void TestXmlConfig::tearDown()
{
   delete m_pConfig;
   m_pConfig = 0;
}

void TestXmlConfig::runTest()
{
   CPPUNIT_ASSERT_EQUAL(std::string("BertyBroker"), m_pConfig->getBrokerUri());
   CPPUNIT_ASSERT_EQUAL(std::string("QueueingClient"), m_pConfig->getClientQueueName());
   CPPUNIT_ASSERT_EQUAL(std::string("ServingServer"), m_pConfig->getServerQueueName());
   CPPUNIT_ASSERT_EQUAL(std::string("LoggerPropertiesProperties"), m_pConfig->getLoggerPropertiesFile());
   CPPUNIT_ASSERT_EQUAL(std::string("Loggernamename"), m_pConfig->getLoggerName());
}
