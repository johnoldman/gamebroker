//
// $Id: testPurseServer.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/scoped_ptr.hpp>

class TestPurseServer : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestPurseServer);
      CPPUNIT_TEST(runTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void runTest();
private:
};
