//
// $Id: testCommands.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class StubProducer;

class TestCommands : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestCommands);
      CPPUNIT_TEST(beginUserSessionTest);
      CPPUNIT_TEST(endUserSessionTest);
      CPPUNIT_TEST(registerGameInstanceTest);
      CPPUNIT_TEST(deRegisterGameInstanceTest);
      CPPUNIT_TEST(makePaymentTest);
      CPPUNIT_TEST(getUserDataTest);
      CPPUNIT_TEST(setUserDataTest);
      CPPUNIT_TEST(makeSystemPaymentTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
   
protected:
   void beginUserSessionTest();
   void endUserSessionTest();
   void registerGameInstanceTest();
   void deRegisterGameInstanceTest();
   void makePaymentTest();
   void getUserDataTest();
   void setUserDataTest();
   void makeSystemPaymentTest();
   
private:
   StubProducer* m_pProducer;
};
