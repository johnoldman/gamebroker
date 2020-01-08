//
// $Id: Amount.h 3484 2007-12-12 11:57:57Z stephend $
//
// class to represent an amount of (something - money, points etc)
//
#ifndef AMOUNT_H_
#define AMOUNT_H_

class Amount
{
public:
   explicit Amount(long amount)
   : m_amount(amount)
   {
   }
   Amount& operator=(const Amount& other)
   {
      m_amount = other.m_amount;
      return *this;
   }
   Amount& operator+=(const Amount& other)
   {
      m_amount += other.m_amount;
      return *this;
   }
   Amount& operator-=(const Amount& other)
   {
      m_amount -= other.m_amount;
      return *this;
   }
   operator long() const
   {
      return m_amount;
   }
private:
   long m_amount;
};

#endif
