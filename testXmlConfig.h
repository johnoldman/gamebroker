#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class Config;

class TestXmlConfig : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestXmlConfig);
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
