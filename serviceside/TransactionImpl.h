//
// $Id: TransactionImpl.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef TRANSACTION_IMPL_H_
#define TRANSACTION_IMPL_H_

class TransactionImpl
{
public:
   virtual ~TransactionImpl() {}
   virtual void commit() = 0;
};

#endif
