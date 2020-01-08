//
// $Id: testMessageIdGenerator.h 3580 2007-12-13 17:01:54Z stephend $
//
#ifndef _TEST_MESSAGE_ID_GENERATOR_H_
#define _TEST_MESSAGE_ID_GENERATOR_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include <boost/scoped_ptr.hpp>

class testMessageIdGenerator : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(testMessageIdGenerator);
      CPPUNIT_TEST(unigueIdTest_01);
      CPPUNIT_TEST(unigueIdTest_02);
      // Generally don't want to run this test as it takes too long. Had to convert 
      // id generator to a short(65536) to show the problem is decent time on 64-bit platform anyway.
      //CPPUNIT_TEST(overflowTest);
   CPPUNIT_TEST_SUITE_END();
public:
   void setUp();
   void tearDown();
protected:
   void unigueIdTest_01();
   void unigueIdTest_02();
   void overflowTest();
private:
   typedef std::map<std::string, long> MapType;
   boost::scoped_ptr<MapType> uniqueIdTest_01_map;
};

#endif
