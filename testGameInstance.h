//
// $Id: testGameInstance.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GameInstance;
class StubMessageConsumer;
class StubKYR;
class StubLedger;

#include <boost/scoped_ptr.hpp>
#include "StubPersistentStore.h"

class TestGameInstance : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestGameInstance);
      CPPUNIT_TEST(runTest);
      CPPUNIT_TEST(beginUserSessionTest);
      CPPUNIT_TEST(makePaymentTest);
      CPPUNIT_TEST(makeSystemPaymentTest);
      CPPUNIT_TEST(endUserSessionTest);
      CPPUNIT_TEST(loadSaveTest);
   CPPUNIT_TEST_SUITE_END();
public:
   void setUp();
   void tearDown();
protected:
   void runTest();
   void beginUserSessionTest();
   void makePaymentTest();
   void makeSystemPaymentTest();
   void endUserSessionTest();
   void loadSaveTest();
private:
   StubMessageConsumer* m_pConsumer;
   GameInstance* m_pGameInstance;
   StubKYR* m_pKYR;
   StubLedger* m_pLedger;
   boost::scoped_ptr<StubPersistentStore> m_pStore;
};
