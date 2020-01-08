#include "testMessageStack.h"

#include "MessageStack.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestMessageStack);

void TestMessageStack::setUp()
{
   m_pStack = new MessageStack<int>();
}

void TestMessageStack::tearDown()
{
   delete m_pStack;
   m_pStack = 0;
}

void TestMessageStack::runTest()
{
   const std::string messageId("fred");

   m_pStack->addMessage(messageId, 1);
   CPPUNIT_ASSERT_THROW(m_pStack->addMessage(messageId, 2), SglStackDuplicateMessageIdException);

   CPPUNIT_ASSERT_EQUAL(1, m_pStack->removeMessage(messageId));
   CPPUNIT_ASSERT_THROW(m_pStack->removeMessage(messageId), SglStackUnknownMessageIdException);
}
