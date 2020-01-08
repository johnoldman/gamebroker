//
// $Id: testResponses.h 3492 2007-12-12 13:16:30Z stephend $
//
#ifndef _TEST_RESPONSES_H_
#define _TEST_RESPONSES_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "BeginUserSessionResponse.h"
#include "EndUserSessionResponse.h"
#include "MakePaymentResponse.h"
#include "GetUserDataResponse.h"
#include "SetUserDataResponse.h"
#include "StubProducer.h"

class TestResponses : public CPPUNIT_NS::TestFixture
{
   CPPUNIT_TEST_SUITE(TestResponses);
      CPPUNIT_TEST(testRegisterInstance);
      CPPUNIT_TEST(testDeregisterInstance);
      CPPUNIT_TEST(testBeginUserSession);
      CPPUNIT_TEST(testEndUserSession);
      CPPUNIT_TEST(testMakePayment);
      CPPUNIT_TEST(testGetUserData);
      CPPUNIT_TEST(testSetUserData);
      CPPUNIT_TEST(testMakeSystemPayment);
   CPPUNIT_TEST_SUITE_END(); 
public:
   void setUp();
   void tearDown();
protected:
   void testRegisterInstance();
   void testDeregisterInstance();
   void testBeginUserSession();
   void testEndUserSession();
   void testMakePayment();
   void testGetUserData();
   void testSetUserData();
   void testMakeSystemPayment();
private:
   RegisterGameInstanceResponse* regGameInstResp;
   DeregisterGameInstanceResponse* deregGameInstResp;
   BeginUserSessionResponse* beginGameSessResp;
   EndUserSessionResponse* endGameSessResp;
   MakePaymentResponse* makePaymentResp;
   GetUserDataResponse* getUserData;
   SetUserDataResponse* setUserData;
   StubProducer* m_pProducer;
};

#endif

