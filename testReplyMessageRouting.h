//
// $Id: testReplyMessageRouting.h 3761 2007-12-19 11:39:52Z stephend $
//
#ifndef TEST_REPLY_MESSAGE_ROUTING_H_
#define TEST_REPLY_MESSAGE_ROUTING_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/scoped_ptr.hpp>

#include "SyncDelayedReply.h"
#include "SkillGameServiceClient.h"

class StubProducerConsumerFactory;
class GameClientImpl;
class StubConsumerFactory;
class StubLogger;

class testReplyMessageRouting : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(testReplyMessageRouting);
   CPPUNIT_TEST(testClientToServer);
   CPPUNIT_TEST_SUITE_END();
public:
   void setUp();
   void tearDown();
protected:
   void testClientToServer();
private:
   StubProducerConsumerFactory* m_pFactory; 
   StubConsumerFactory* m_pConsumerFactory;
   GameClientImpl* m_pGameClient;
   SyncDelayedReply* m_pInstanceReply;
   typedef std::map<std::string, long> MapType;
   MapType m_uniqueMessageIdTestMap;
   boost::scoped_ptr<StubLogger> m_pLogger;
};

#endif
