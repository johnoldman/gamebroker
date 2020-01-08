//
// $Id: testTsq.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include <ctime>

#include <boost/thread/thread.hpp>
#include "testTsq.h"

#include "SyncDelayedReply.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestTsq);

void TestTsq::setUp()
{   
   m_pQueue = new ThreadSafeQueue<int>(2);
   m_pSyncHelper = new SyncHelper(m_pQueue);
   m_pSyncReply = new SyncDelayedReply(*m_pSyncHelper, 2);
}

void TestTsq::tearDown()
{
   delete m_pSyncReply;
   m_pSyncReply = 0;
   delete m_pSyncHelper;
   m_pSyncHelper = 0;
   delete m_pQueue;
   m_pQueue = 0;
}

void TestTsq::runTest()
{
   m_pQueue->write(5);
   time_t then = time(0);
   m_pQueue->write(7);
   // The queue is full at this point.
   time_t now = time(0);

   // Need to loop a few times, otherwise this test will fail randomly and unrepeatedly
   // which is incredibly bad behaviour for a confidence unit test
   unsigned int loopCount = 0;
   while (loopCount++ < 5 && now != then)
   {
      m_pQueue->blockingRead();
      m_pQueue->blockingRead();
      m_pQueue->write(5);
      then = time(0);
      m_pQueue->write(7);
      now = time(0);
   }
   CPPUNIT_ASSERT_EQUAL(now, then);

   m_pSyncReply->sendMessage("hello");
   m_pQueue->write(6);
   time_t later = time(0);
   // The queue should have blocked, so that the time has moved on
   CPPUNIT_ASSERT(later > now);
}
