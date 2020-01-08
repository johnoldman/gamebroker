//
// $Id: Transaction.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "Transaction.h"
#include "TransactionImpl.h"

Transaction::Transaction(const boost::shared_ptr<TransactionImpl>& impl)
: m_pImpl(impl)
{
}

void Transaction::commit()
{
   m_pImpl->commit();
}
