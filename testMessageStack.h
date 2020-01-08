#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "MessageStack.h"

class TestMessageStack : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestMessageStack);
   CPPUNIT_TEST(runTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
   
protected:
   void runTest();
   
private:
   MessageStack<int>* m_pStack;
};
