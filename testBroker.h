//
// $Id: testBroker.h 3492 2007-12-12 13:16:30Z stephend $
//
#ifndef _TEST_BROKER_H_
#define _TEST_BROKER_H_
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class MQConsumer;
class MQProducer;
class StubConsumer;
class StubLogger;

class TestBroker : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestBroker);
      CPPUNIT_TEST(runTest);
      CPPUNIT_TEST(handlerThrowTest);
   CPPUNIT_TEST_SUITE_END();
public:
   TestBroker() : m_pLogger(0), m_pConsumer(0), m_pProducer(0), m_pMessageHandler(0)
   {
   }
   void setUp();
   void tearDown();

protected:
   void runTest();
   void handlerThrowTest();

private:
   StubLogger* m_pLogger;
   MQConsumer* m_pConsumer;
   MQProducer* m_pProducer;
   StubConsumer* m_pMessageHandler;
};

#endif
