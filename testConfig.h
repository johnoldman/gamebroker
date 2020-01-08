#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Config;

class TestConfig : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestConfig);
   CPPUNIT_TEST(runTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void runTest();
private:
   Config* m_pConfig;
};
