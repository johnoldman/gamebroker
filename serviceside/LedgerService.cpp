//
// $Id: LedgerService.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include <string>
#include <vector>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "Payment.h"
#include "LedgerService.h"
#include "checkSoapCode.h"

void LedgerService::checkSoapCode(int soapCode, const std::string& funcName)
{
   return ::checkSoapCode(m_Impl.soap_fault(), soapCode, funcName);
}

LedgerService::LedgerService(const std::string& ledgerUrl)
: m_url(ledgerUrl)
{
   m_Impl.soap_endpoint = m_url.c_str();
}

std::vector<Asset> LedgerService::payPurse(long userId, 
                                           const std::string& tariffId, 
                                           long amount, 
                                           const std::string& lockerId, 
                                           const std::string& messageId )
{
   _ns3__payPurse* payPurseRequest = soap_new__ns3__payPurse(&m_Impl, -1);
   payPurseRequest->param0 = &userId;
   payPurseRequest->param1 = soap_new_std__string(&m_Impl, -1);
   *payPurseRequest->param1 = tariffId;
   payPurseRequest->param2 = &amount;
   payPurseRequest->param3 = soap_new_std__string(&m_Impl, -1);
   *payPurseRequest->param3 = lockerId;
   payPurseRequest->param4 = soap_new_std__string(&m_Impl, -1);
   *payPurseRequest->param4 = messageId;

   _ns3__payPurseResponse* payPurseResponse = soap_new__ns3__payPurseResponse(&m_Impl, -1);

   const std::string context = std::string("LedgerService::payPurse") + boost::lexical_cast<std::string>(userId) + ":" + tariffId + ":" + boost::lexical_cast<std::string>(amount);

   checkSoapCode(m_Impl.payPurse(payPurseRequest, payPurseResponse), context);

   if (payPurseResponse->return_.size() == 0)
   {
      throw std::runtime_error(context + " pay purse returned no assets");
   }
   else
   {
      std::vector<Asset> assets;
      for (std::vector<ns2__AssetTransaction*>::const_iterator it = payPurseResponse->return_.begin(); it != payPurseResponse->return_.end(); ++it)
      {
         if (!(*it)->isCredit)
         {
            throw std::runtime_error(context + " non-credit asset returned");
         }
         Asset localAsset(Amount(*(*it)->amount), *(*it)->assetId);
         assets.push_back(localAsset);
      }
      return assets;
   }
}

long LedgerService::payWallet(long userId, const std::string& tariffId, const Payment& payment)
{
   _ns3__payWallet* payWalletRequest = soap_new__ns3__payWallet(&m_Impl, -1);
   payWalletRequest->param0 = &userId;
   payWalletRequest->param1 = soap_new_std__string(&m_Impl, -1);
   *payWalletRequest->param1 = tariffId;
   payWalletRequest->param2 = soap_new_ns2__Payment(&m_Impl, -1);
   long amount = payment.getAmount();
   payWalletRequest->param2->amount = &amount;
   payWalletRequest->param2->userId = &userId;

   for (int i = 0; i < payment.getTransactionCount(); ++i)
   {
      PaymentTransaction pt = payment.getTransaction(i);

      ns2__AssetTransactionLog* pTxnLog = soap_new_ns2__AssetTransactionLog(&m_Impl, -1);
      pTxnLog->amount = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->amount));
      *pTxnLog->amount = pt.getAmount();

      pTxnLog->assetId = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->assetId));
      *pTxnLog->assetId = pt.getAssetId();

      pTxnLog->date = (time_t*)soap_malloc(&m_Impl, sizeof(*pTxnLog->date));
      *pTxnLog->date = pt.getDate();

      pTxnLog->description = soap_new_std__string(&m_Impl, -1);
      *pTxnLog->description = pt.getDescription();

      pTxnLog->fromAsset = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->fromAsset));
      *pTxnLog->fromAsset = pt.getFromAsset();

      //pTxnLog->id = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->id));
      //*pTxnLog->id = pt.getId();

      pTxnLog->isCredit = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->isCredit));
      *pTxnLog->isCredit = pt.isCredit();

      //pTxnLog->paymentId = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->paymentId));
      //*pTxnLog->paymentId = pt.getPaymentId();

      pTxnLog->referance = soap_new_std__string(&m_Impl, -1);
      *pTxnLog->referance = pt.getReference();

      pTxnLog->thirdPartyId = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->paymentId));
      *pTxnLog->thirdPartyId = pt.getThirdPartyId();

      pTxnLog->userId = (LONG64*)soap_malloc(&m_Impl, sizeof(*pTxnLog->paymentId));
      *pTxnLog->userId = pt.getUserId();

      payWalletRequest->param2->assetTransactionLog.push_back(pTxnLog);
   }

   long balance = payment.getBalance();
   payWalletRequest->param2->balance = &balance;
   //time_t clearDate = payment.getClearDate();
   //payWalletRequest->param2->clearDate = &clearDate;
   time_t date = payment.getDate();
   payWalletRequest->param2->clearDate = &date;

   //long id = payment.getId();
   //payWalletRequest->param2->id = &id;

   //time_t lockTimeout = payment.getLockTimeout();
   //payWalletRequest->param2->lockTimeout = &lockTimeout;

   payWalletRequest->param2->lockerId = soap_new_std__string(&m_Impl, -1);
   *payWalletRequest->param2->lockerId = payment.getLockerId();

   payWalletRequest->param2->messageId = soap_new_std__string(&m_Impl, -1);
   *payWalletRequest->param2->messageId = payment.getMessageId();

   int state = payment.getState();
   payWalletRequest->param2->state = &state;

   int type = payment.getType();
   payWalletRequest->param2->type = &type;
   
   _ns3__payWalletResponse* payWalletResponse = soap_new__ns3__payWalletResponse(&m_Impl, -1);

   checkSoapCode(m_Impl.payWallet(payWalletRequest, payWalletResponse), "LedgerService::payWallet");
   // Returns the payment reference number.
   return *payWalletResponse->return_;
}
