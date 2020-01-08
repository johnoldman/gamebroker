//
// $Id: testServiceBroker.h 3597 2007-12-14 10:04:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/scoped_ptr.hpp>

#include "ServiceBroker.h"
#include "StubProducer.h"
#include "StubProducerConsumerFactory.h"
#include "StubMessageConsumer.h"
#include "StubPersistentStore.h"
#include "StubKYR.h"
#include "StubLedger.h"
#include "StubLogger.h"
#include "StubUserService.h"

class TestServiceBroker : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestServiceBroker);
      CPPUNIT_TEST(runTest);
      CPPUNIT_TEST(transactionLogTest);
      CPPUNIT_TEST(driFailoverTest);
      CPPUNIT_TEST(busFailoverTest);
      CPPUNIT_TEST(testEndUserSession);
      CPPUNIT_TEST(testGetSet);
      CPPUNIT_TEST(testMakePayment);
      CPPUNIT_TEST(testMakeSystemPayment);
      CPPUNIT_TEST(endGameSessionTest);
      CPPUNIT_TEST(correctReplyTest);
   CPPUNIT_TEST_SUITE_END();
public:
   void setUp();
   void tearDown();
protected:
   void runTest();
   void transactionLogTest();
   void driFailoverTest();
   void busFailoverTest();
   void eusFailoverTest();
   void testGetSet();
   void testEndUserSession();
   void testMakePayment();
   void testMakeSystemPayment();
   void endGameSessionTest();
   void correctReplyTest();

private:
   ServiceBroker* m_pServiceBroker;
   StubProducer* m_pProducer;
   boost::scoped_ptr<StubProducerConsumerFactory> m_producerFactory;
   boost::scoped_ptr<StubPersistentStore> m_pStore;
   boost::scoped_ptr<StubMessageConsumer> m_pConsumer;
   boost::scoped_ptr<StubKYR> m_pKyr;
   boost::scoped_ptr<StubLedger> m_pLedger;
   boost::scoped_ptr<StubLogger> m_pLogger;
   boost::scoped_ptr<StubUserService> m_pUserService;
};

