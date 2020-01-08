//
// $Id: testGameAPI.h 3621 2007-12-14 12:38:23Z stephend $
//
#ifndef TEST_GAME_API_HPP_
#define TEST_GAME_API_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/scoped_ptr.hpp>

#include "SyncDelayedReply.h"
#include "SkillGameServiceClient.h"
#include "StubLogger.h"

class StubProducerConsumerFactory;
class GameClientImpl;

class TestGameAPI : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestGameAPI);
      CPPUNIT_TEST(runTest);
      CPPUNIT_TEST(sessionTest);
      CPPUNIT_TEST(getUserDataTest);
      CPPUNIT_TEST(sendReceiveTest);
      CPPUNIT_TEST(makeSystemPaymentTest);
      CPPUNIT_TEST(uniqueMessageIdTest);
   CPPUNIT_TEST_SUITE_END();
public:
   TestGameAPI();
   void setUp();
   void tearDown();

//   void ackCallback();
//   void dataCallback(long data);
//   void exceptionCallback(SglGameAPI::SglExceptionType exception);

protected:
   void runTest();
   void sessionTest();
   void getUserDataTest();
   void sendReceiveTest();
   void makeSystemPaymentTest();
   void uniqueMessageIdTest();

private:
	std::string gameName;
	std::string gameType;
	std::string gameSessionId;
	long userId;
	std::string userSessionId;
	long gameCost;
	long gameFloorLimit;
	std::string fromUserSessionId;
	std::string toUserSessionId;
	long amount;
	std::string eventCode;
	bool payUser;
	std::string totalType;
	std::string systemDataType;
	std::string dataKey;
	std::string userDataType;
private:
   StubProducerConsumerFactory* m_pFactory;
   GameClientImpl* m_pGameClient;
   SyncDelayedReply* beginInstanceReply;
   SyncDelayedReply* endInstanceReply;
   typedef std::map<std::string, long> MapType;
   MapType uniqueMessageIdTestMap;
   boost::scoped_ptr<StubLogger> m_pLogger;
};

#endif // _TEST_GAME_API_HPP_
