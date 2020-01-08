//
// $Id: testTsq.h 3492 2007-12-12 13:16:30Z stephend $
//
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ThreadSafeQueue.h"
#include "ActiveMqConsumerHandler.h"

class SyncDelayedReply;

class TestTsq : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestTsq);
   CPPUNIT_TEST(runTest);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
   
protected:
   void runTest();
private:
   ThreadSafeQueue<int>* m_pQueue;

   class SyncHelper : public ActiveMqConsumerHandler
   {
   public:
      SyncHelper(ThreadSafeQueue<int>* pQueue) : m_pQueue(pQueue) {}
      void receive(const std::string& message, const time_t& eventTime)
      {
         m_pQueue->blockingRead();
      }
      void error(const std::string& message)
      {
      }
   private:
      ThreadSafeQueue<int>* m_pQueue;
   };
   SyncHelper* m_pSyncHelper;
   SyncDelayedReply*     m_pSyncReply;
};
