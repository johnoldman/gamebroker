//
// $Id: LedgerService.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef LEDGER_SERVICE_H_
#define LEDGER_SERVICE_H_

#include <vector>
#include <string>
#include "Asset.h"
#include "ledgerLedgerServiceSOAP11BindingProxy.h"
#include "Ledger.h"

class Payment;

class LedgerService : public Ledger
{
public:
   explicit LedgerService(const std::string& ledgerUrl);

   // _ns3__payPurse
   // _ns3__payPurseResponse
   // Vector of ns2__AssetTransaction* with length 0..unbounded
   // std::vector<ns2__AssetTransaction* > return_
   // throws SGTException
   std::vector<Asset> payPurse( long userId, 
                                const std::string& tariffId, 
                                long amount, 
                                const std::string& lockerId, 
                                const std::string& messageId );

   // class _ns3__payWallet
   // Element param2 of type "http://beans.ledger.sgt.com/xsd":Payment.
   // ns2__Payment*                        param2
   // class _ns3__payWalletResponse
   // throws SGTException
   long payWallet(long userId, const std::string& tariffId, const Payment& payment);

private:
   void checkSoapCode(int code, const std::string& module);
private:
   const std::string m_url;
   LedgerServiceSOAP11BindingProxy m_Impl;
};

#endif
