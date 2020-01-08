//
// $Id: testMQMessage.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testMQMessage.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestMQMessage);

#include "MQMessage.h"

class TestMessage : public MQMessage
{
public:
   static std::string testWrap() { return wrapString("hello", "<goodbye>"); }
   static std::string testWrap2() { return wrapString("hello", "goodbye cruel world & all that"); }
};

void TestMQMessage::setUp()
{
}

void TestMQMessage::tearDown()
{
}

void TestMQMessage::runTest()
{
   CPPUNIT_ASSERT_EQUAL(std::string("<hello>&lt;goodbye&gt;</hello>"), TestMessage::testWrap());
   CPPUNIT_ASSERT_EQUAL(std::string("<hello>goodbye cruel world &amp; all that</hello>"), TestMessage::testWrap2());
}
