#include <boost/thread/thread.hpp>
#include "testQueue.h"
#include "Queue.h"

Queue<std::string> queue;

CPPUNIT_TEST_SUITE_REGISTRATION(TestQueue);

void TestQueue::setUp()
{   
}

void TestQueue::tearDown()
{
}

static void queueTest1()
{
   std::string testString;
   std::string key = "key_01";
   CPPUNIT_ASSERT_NO_THROW(testString = queue.getItem(key));
}

static void queueTest2()
{
   std::string testString;
   std::string key = "key_01";   
   CPPUNIT_ASSERT_NO_THROW(queue.putItem(key, testString));
}

void TestQueue::runTest()
{
   boost::thread testThread1(&queueTest1); 
   boost::thread testThread2(&queueTest2); 
   testThread1.join();
   testThread2.join();
}

