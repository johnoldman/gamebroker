//
// $Id: testPurseDb.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/scoped_ptr.hpp>
#include "MySqlPurseDb.h"
#include "StubLedger.h"
#include "StubKYR.h"

class TestPurseDb : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestPurseDb);
      CPPUNIT_TEST(messageTest);
      CPPUNIT_TEST(gameInstanceTest);
      CPPUNIT_TEST(purseTest);
      CPPUNIT_TEST(assetTest);
      CPPUNIT_TEST(transactionTest);
      CPPUNIT_TEST(deleteGameTest);
      CPPUNIT_TEST(deletePurseTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void messageTest();
   void gameInstanceTest();
   void purseTest();
   void assetTest();
   void transactionTest();
   void deleteGameTest();
   void deletePurseTest();
private:
   void makeGameInstance();
   void saveRandomAsset();
private:
   boost::scoped_ptr<StubLedger> m_pLedger;
   boost::scoped_ptr<StubKYR> m_pKyr;

   boost::scoped_ptr<MySqlPurseDb> m_pDb;
   boost::scoped_ptr<Transaction> m_pTransaction;
};
