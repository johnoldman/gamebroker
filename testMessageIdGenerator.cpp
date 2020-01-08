//
// $Id: testMessageIdGenerator.cpp 3576 2007-12-13 16:42:19Z stephend $
//
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "MessageIdGenerator.h"
#include "testMessageIdGenerator.h"

CPPUNIT_TEST_SUITE_REGISTRATION(testMessageIdGenerator);

void testMessageIdGenerator::setUp()
{
   uniqueIdTest_01_map.reset(new MapType);
}

void testMessageIdGenerator::tearDown()
{
   uniqueIdTest_01_map.reset();
}

void testMessageIdGenerator::unigueIdTest_01()
{
   // verify TEN different instances generate TEN unique IDs
   MessageIdGenerator gen_01;
   MessageIdGenerator gen_02;
   MessageIdGenerator gen_03;
   MessageIdGenerator gen_04;
   MessageIdGenerator gen_05;
   MessageIdGenerator gen_06;
   MessageIdGenerator gen_07;
   MessageIdGenerator gen_08;
   MessageIdGenerator gen_09;
   MessageIdGenerator gen_10;
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_01, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_02, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_03, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_04, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_05, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_06, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_07, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_08, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_09, 1));
   uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen_10, 1));
   CPPUNIT_ASSERT_EQUAL((size_t)10, uniqueIdTest_01_map->size());
}

void testMessageIdGenerator::unigueIdTest_02()
{
   // verify single instance generates unique IDs
   MessageIdGenerator gen;
   const long numOfIds = 100;
   for(long i=0; i<numOfIds; i++)
   {
      uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen, i));
   }
   CPPUNIT_ASSERT_EQUAL((size_t)numOfIds, uniqueIdTest_01_map->size());
}

void testMessageIdGenerator::overflowTest()
{
   MessageIdGenerator gen;
   for (long i = 0; i < 65536L * 65536L * 4L; ++i)
   {
      if (i % 1000000 == 0)
      {
         std::cout << i / 1000000 << " " << std::flush;
         if (i % 100000000 == 0)
            std::cout << std::endl;
      }
      if (i < 1000000)
      {
         CPPUNIT_ASSERT_MESSAGE(boost::lexical_cast<std::string>(i), uniqueIdTest_01_map->insert(std::pair<std::string, long>(++gen, i)).second);
      }
      else
      {
         CPPUNIT_ASSERT(uniqueIdTest_01_map->find(++gen) == uniqueIdTest_01_map->end());
      }
   }
}
