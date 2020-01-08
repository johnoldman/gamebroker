//
// $Id: testResponses.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include <boost/lexical_cast.hpp>
#include "XmlIterator.h"
#include "RegisterGameInstanceResponse.h"
#include "DeregisterGameInstanceResponse.h"
#include "BeginUserSessionResponse.h"
#include "EndUserSessionResponse.h"
#include "MakePaymentResponse.h"
#include "MakeSystemPaymentResponse.h"
#include "GetUserDataResponse.h"
#include "SetUserDataResponse.h"
#include "StubProducer.h"
#include "testResponses.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestResponses);

void TestResponses::setUp()
{
   m_pProducer = new StubProducer;
}

void TestResponses::tearDown()
{
   delete m_pProducer;
   m_pProducer = 0;
}

void TestResponses::testRegisterInstance()
{
   // sucessful outcome
   std::string messageId = "ID_RGI";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   RegisterGameInstanceResponse response1(messageId);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>RGI</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_RGII";
   errorCode = SglGameAPI::SglServerGameClosed;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "terrible"; 
   RegisterGameInstanceResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>RGI</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testDeregisterInstance()
{ 
   // sucessful outcome
   std::string messageId = "ID_DGI";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   DeregisterGameInstanceResponse response1(messageId);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_DGII";
   errorCode = SglGameAPI::SglServerUnknownUserSessionId;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "rubbish"; 
   DeregisterGameInstanceResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>DGI</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testBeginUserSession()
{
   // sucessful outcome
   std::string messageId = "ID_BUS";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   long purseTotal = 1234;
   std::string strPurseTotal = boost::lexical_cast<std::string>(purseTotal);
   BeginUserSessionResponse response1(messageId,purseTotal);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode><purseTotal>"
                                    + strPurseTotal + "</purseTotal></mq:envelope>"
                                    , m_pProducer->getMessage() );  

   XmlIterator xml(m_pProducer->getMessage());
   const std::string envelopeContents = xml.extractXml("mq:envelope");
   XmlIterator contents(envelopeContents);
   const std::string response = contents.extractString("response");
   BeginUserSessionResponse ourResponse(contents);

   // failure code
   messageId = "ID_BUS1";
   errorCode = SglGameAPI::SglServerUnknownUserSessionId;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "useless"; 
   BeginUserSessionResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>BUS</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testEndUserSession()
{
   // sucessful outcome
   std::string messageId = "ID_EUS";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   EndUserSessionResponse response1(messageId);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>EUS</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_EUS1";
   errorCode = SglGameAPI::SglServerFundsRemaining;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "useless"; 
   EndUserSessionResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>EUS</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testMakePayment()
{
   // sucessful outcome
   std::string messageId = "ID_MPY";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   MakePaymentResponse response1(messageId);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MPY</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_MPY1";
   errorCode = SglGameAPI::SglServerFundsRemaining;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "this player is now skint!"; 
   MakePaymentResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MPY</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testGetUserData()
{
   // sucessful outcome
   std::string messageId = "ID_GUD";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   std::string userData = "UserDataData";
   GetUserDataResponse response1(messageId,userData);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>GUD</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode><userData>"
                                    + userData + "</userData></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_GUD1";
   errorCode = SglGameAPI::SglServerFundsRemaining;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "No No User data!"; 
   GetUserDataResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>GUD</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );

   const std::string awkwardData = "Hello?&goodbye<>";
   GetUserDataResponse awkResp(messageId,awkwardData);
   CPPUNIT_ASSERT_NO_THROW(awkResp.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>GUD</response><messageId>ID_GUD1</messageId><errorCode>0</errorCode><userData>Hello?&amp;goodbye&lt;&gt;</userData></mq:envelope>") ,m_pProducer->getMessage() );
   CPPUNIT_ASSERT_EQUAL(awkwardData, awkResp.getUserData());
}

void TestResponses::testSetUserData()
{
   // sucessful outcome
   std::string messageId = "ID_SUD";
   enum SglGameAPI::SglServerExceptionType errorCode = SglGameAPI::SglSuccess;
   SetUserDataResponse response1(messageId);
   CPPUNIT_ASSERT_NO_THROW(response1.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>SUD</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
   // failure code
   messageId = "ID_SUD1";
   errorCode = SglGameAPI::SglServerFundsRemaining;
   std::string strErrorCode = boost::lexical_cast<std::string>(static_cast<int>(errorCode));
   std::string errorMessage = "the end!"; 
   SetUserDataResponse response2(messageId,errorCode,errorMessage);
   CPPUNIT_ASSERT_NO_THROW(response2.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>SUD</response><messageId>") 
                                    + messageId + "</messageId><errorCode>" 
                                    + strErrorCode + "</errorCode><errorString>"
                                    + errorMessage + "</errorString></mq:envelope>"
                                    , m_pProducer->getMessage() );
}

void TestResponses::testMakeSystemPayment()
{
   std::string messageId = "ID_SUD";
   MakeSystemPaymentResponse resp(messageId);
   CPPUNIT_ASSERT_NO_THROW(resp.send(*m_pProducer));
   CPPUNIT_ASSERT_EQUAL(std::string("<mq:envelope><response>MSP</response><messageId>") 
                                    + messageId + "</messageId><errorCode>0</errorCode></mq:envelope>"
                                    , m_pProducer->getMessage() );
}
