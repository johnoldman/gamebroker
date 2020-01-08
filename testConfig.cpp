#include "testConfig.h"

#include "Config.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestConfig);

void TestConfig::setUp()
{
   m_pConfig = new Config("freddy", "frog", "serverqueue", "jim", "Purse.logger");
}

void TestConfig::tearDown()
{
   delete m_pConfig;
   m_pConfig = 0;
}

void TestConfig::runTest()
{
   CPPUNIT_ASSERT_EQUAL(std::string("freddy"), m_pConfig->getBrokerUri());
   CPPUNIT_ASSERT_EQUAL(std::string("frog"), m_pConfig->getClientQueueName());
   CPPUNIT_ASSERT_EQUAL(std::string("serverqueue"), m_pConfig->getServerQueueName());
   CPPUNIT_ASSERT_EQUAL(std::string("jim"), m_pConfig->getLoggerPropertiesFile());
}
