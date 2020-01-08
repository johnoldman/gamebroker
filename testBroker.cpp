//
// $Id: testBroker.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testBroker.h"

#include "MQConsumer.h"
#include "MQProducer.h"
#include "StubConsumer.h"
#include "StubLogger.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBroker);

void TestBroker::setUp()
{
   m_pLogger = new StubLogger();
   //m_pConsumer = new MQConsumer("tcp://127.0.0.1:61613?wireFormat=stomp&transport.useAsyncSend=true", "SUBJECT.TEST.FOO", *m_pLogger);
   //m_pProducer = new MQProducer("tcp://127.0.0.1:61613?wireFormat=stomp&transport.useAsyncSend=true", "SUBJECT.TEST.FOO", *m_pLogger);
   m_pConsumer = new MQConsumer("tcp://localhost:61616?wireFormat=openwire&transport.useAsyncSend=true", "SUBJECT.TEST.FOO", *m_pLogger);
   m_pProducer = new MQProducer("tcp://localhost:61616?wireFormat=openwire&transport.useAsyncSend=true", "SUBJECT.TEST.FOO", "SUBJECT.TEST.FOO", *m_pLogger);
   m_pMessageHandler = new StubConsumer();
   m_pConsumer->setConsumer(m_pMessageHandler);

   sleep(1);
   // Try and consume old messages from previous tests
   while (m_pMessageHandler->getMessageCount() > 0)
   {
      m_pConsumer->setConsumer(0);
      delete m_pMessageHandler;
      m_pMessageHandler = new StubConsumer();
      m_pConsumer->setConsumer(m_pMessageHandler);
      sleep(1);
   }
}

void TestBroker::tearDown()
{
   //sleep(1);
   if (m_pConsumer)
   {
      m_pConsumer->setConsumer(0);
   }
   delete m_pMessageHandler;
   m_pMessageHandler = 0;

   delete m_pProducer;
   m_pProducer = 0;

   delete m_pConsumer;
   m_pConsumer = 0;
}

void TestBroker::runTest()
{
   m_pProducer->sendMessage("!frog", "");
   // Have to wait a bit for message to be delivered asynchronously...
   sleep(1);
   CPPUNIT_ASSERT_EQUAL((size_t)1, m_pMessageHandler->getMessageCount());

   CPPUNIT_ASSERT_EQUAL(std::string("!frog"), m_pMessageHandler->getMessage(0));

   // Now just check that we really have consumed the message...
   tearDown();
   setUp();
   sleep(1);
   CPPUNIT_ASSERT_EQUAL((size_t)0, m_pMessageHandler->getMessageCount());
}
//
// If receiver throws, message should end up on dead letter queue. This is
// simply done via configuration as a result of rolling back the transaction.
//


void TestBroker::handlerThrowTest()
{
   m_pMessageHandler->setThrow();
   m_pProducer->sendMessage("!frog", "");
   sleep(1);
   CPPUNIT_ASSERT_EQUAL(std::string("StubConsumer::receive throwing"), m_pLogger->getWhat());

   // Prove that MQ still works after throw
   m_pMessageHandler->setThrow(false);
   m_pProducer->sendMessage("!another frog", "");
   sleep(1);

   // this is one zero because the rollback is not implemented
   CPPUNIT_ASSERT_EQUAL((size_t)1, m_pMessageHandler->getMessageCount());
//   CPPUNIT_ASSERT_EQUAL(std::string("!frog"), m_pMessageHandler->getMessage(0));
   CPPUNIT_ASSERT_EQUAL(std::string("!another frog"), m_pMessageHandler->getMessage(0));
   m_pMessageHandler->clear();

   // Verify that logger runs when we throw something random.
   m_pMessageHandler->setThrowInt();
   m_pProducer->sendMessage("!yet another green thing", "");
   sleep(1);
   CPPUNIT_ASSERT_EQUAL(std::string("MQConsumer::runThread"), m_pLogger->getLastUnknown());

   // and check that we can still process messages
   m_pProducer->sendMessage("!a badger for a change", "");
   sleep(1);
   CPPUNIT_ASSERT_EQUAL((size_t)1, m_pMessageHandler->getMessageCount());
//   CPPUNIT_ASSERT_EQUAL(std::string("!yet another green thing"), m_pMessageHandler->getMessage(0));
   CPPUNIT_ASSERT_EQUAL(std::string("!a badger for a change"), m_pMessageHandler->getMessage(0));
}
