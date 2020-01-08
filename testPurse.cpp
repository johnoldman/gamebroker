//
// $Id: testPurse.cpp 3492 2007-12-12 13:16:30Z stephend $
//
#include "testPurse.h"

#include <stdexcept>

#include "Purse.h"
#include "StubLedger.h"
#include "StubTariff.h"
#include "UserSessionId.h"
#include "GameSessionId.h"
#include "NotEnoughMoneyException.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPurse);

void TestPurse::setUp()
{
   m_pLedger = new StubLedger;
   //m_pTariff = new StubTariff;
   m_pStore.reset(new StubPersistentStore);
}

void TestPurse::tearDown()
{
   m_pStore.reset();
   delete m_pLedger;
   m_pLedger = 0;
   // m_pTariff managed by m_pPurse, so already deleted.
}

void TestPurse::runTest()
{
   GameSessionId gsi("hello");
   Purse purse(*m_pStore, gsi, "key", 27, 34, 45, "hello", boost::shared_ptr<ITariff>(new StubTariff), *m_pLedger, "freddy");
   std::vector<Asset> assets;
   Amount amount(27);
   assets.push_back(Asset(amount, 1));
   CPPUNIT_ASSERT_EQUAL(0L, purse.getTotal());
   purse.putAssets(assets, "shot", time(NULL), 1, "ref");
   CPPUNIT_ASSERT_EQUAL(27L, purse.getTotal());

   Purse restoredPurse(*m_pStore, gsi, purse.getId(), 27, "hello", boost::shared_ptr<ITariff>(new StubTariff), *m_pLedger);
   CPPUNIT_ASSERT_EQUAL(27L, restoredPurse.getTotal());
}

void TestPurse::removeAssetsTest()
{
   GameSessionId gsi("hello");
   StubTariff* pTariff = new StubTariff;
   pTariff->addMapping("shot", 1, 2);
   pTariff->addMapping("kill", 1, 3);
   pTariff->addMapping("kill", 3, 2);
   pTariff->addMapping("shot", 2, 1);

   Purse purse(*m_pStore, gsi, "key", 27, 34, 45, "hello", boost::shared_ptr<ITariff>(pTariff), *m_pLedger, "freddy");
   std::vector<Asset> assets;
   Amount amount(27);
   assets.push_back(Asset(amount, 1));
   assets.push_back(Asset(Amount(5), 3));
   purse.putAssets(assets, "startup", time(NULL), 1, "ref");
   CPPUNIT_ASSERT_EQUAL(32L, purse.getTotal());

   std::vector<Asset> removedAssets = purse.removeAssets(Amount(10), "shot", time(NULL), 1, "hello");
   CPPUNIT_ASSERT_EQUAL(22L, purse.getTotal());
   CPPUNIT_ASSERT_EQUAL((size_t)1, removedAssets.size());
   CPPUNIT_ASSERT_EQUAL(Amount(10), removedAssets[0].getAmount());
   CPPUNIT_ASSERT_EQUAL(std::string("fred"), removedAssets[0].getClassName());
   std::vector<Asset> removedAssets2 = purse.removeAssets(Amount(20), "kill", time(NULL), 1, "hello");
   CPPUNIT_ASSERT_EQUAL(2L, purse.getTotal());

   CPPUNIT_ASSERT_THROW(purse.removeAssets(Amount(3), "kill", time(NULL), 1, "hello"), NotEnoughMoneyException);
   CPPUNIT_ASSERT_EQUAL(2L, purse.getTotal());
}
