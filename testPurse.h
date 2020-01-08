//
// $Id: testPurse.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class StubLedger;
class Tariff;

#include <boost/scoped_ptr.hpp>
#include "StubPersistentStore.h"

class TestPurse : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestPurse);
      CPPUNIT_TEST(runTest);
      CPPUNIT_TEST(removeAssetsTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void runTest();
   void removeAssetsTest();
private:
   Tariff* m_pTariff;
   StubLedger* m_pLedger;
   boost::scoped_ptr<StubPersistentStore> m_pStore;
};
