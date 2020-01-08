//
// $Id: Transaction.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef TRANSACTION_H
#define TRANSACTION_H

class TransactionImpl;

#include <boost/shared_ptr.hpp>

class Transaction
{
public:
   Transaction(const boost::shared_ptr<TransactionImpl>& impl);
   void commit();
private:
   boost::shared_ptr<TransactionImpl> m_pImpl;
};

#endif
