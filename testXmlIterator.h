#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class XmlIterator;

class TestXmlIterator : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestXmlIterator);
      CPPUNIT_TEST(goodTest);
      CPPUNIT_TEST(badTest);
      CPPUNIT_TEST(spaceTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
   
protected:
   void goodTest();
   void badTest();
   void spaceTest(); 
private:
   XmlIterator* m_pIter;
};
