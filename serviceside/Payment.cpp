//
// $Id: Payment.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "Payment.h"

#include <ctime>

Payment::Payment(int userid, const std::string& messageid)
: m_amount(0), userId_(userid), messageId_(messageid)
{
   date_ = time(NULL);
}

void Payment::addTransaction(const PaymentTransaction& txn)
{
   m_transactions.push_back(txn);
   if (txn.isCredit())
   {
      m_amount += txn.getAmount();
   }
   else
   {
      m_amount -= txn.getAmount();
   }
}
