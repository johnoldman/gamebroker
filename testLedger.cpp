//
// $Id: testLedger.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testLedger.h"
#include "Payment.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLedger);

void TestLedger::setUp()
{
   m_pLedger.reset(new LedgerService("http://localhost:8080/axis2/services/LedgerService"));
}

void TestLedger::tearDown()
{
   m_pLedger.reset();
}

void TestLedger::runTest()
{
   //m_pLedger->payPurse(45, "tariff", 8976, "locxker", "messageid");
   //m_pLedger->payWallet(45, "tariff", Payment());
}
