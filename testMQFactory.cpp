//
// $Id: testMQFactory.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testMQFactory.h"

#include "StubLogger.h"
#include "MQFactory.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMQFactory);

void testMQFactory::setUp()
{
   brokerUri  = "tcp://localhost:61616?wireFormat=openwire&transport.useAsyncSend=true";
   const std::string producerQueue  = "SUBJECT.TEST.FOO";
   const std::string consumerQueue  = "queue";
   logger = new StubLogger();
   producerFactory = new MQFactory(brokerUri, producerQueue, consumerQueue, *logger);
}

void testMQFactory::tearDown()
{
   delete producerFactory;
   producerFactory = 0;
   delete logger;
   logger = 0;
}

void testMQFactory::testConsumerFactory()
{
   CPPUNIT_ASSERT_NO_THROW(producerFactory->createConsumer(queueName));
}

void testMQFactory::testProducerFactory()
{
   CPPUNIT_ASSERT_NO_THROW(producerFactory->createProducer(queueName));
}

