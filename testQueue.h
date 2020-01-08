#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Queue.h"

class TestQueue : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestQueue);
   CPPUNIT_TEST(runTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
   
protected:
   void runTest();
   
private:
//   Queue<std::string> *queue;
//   void queueTest1();
};
