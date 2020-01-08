//
// $Id: MakePaymentResponse.h 2791 2007-11-14 18:24:18Z stephend $
//
#ifndef _MAKE_PAYMENT_RESPONSE_H_
#define _MAKE_PAYMENT_RESPONSE_H_

#include <string>

#include "GameMessageProducer.h"
#include "ResponseWithNoData.h"

class XmlIterator;

class MakePaymentResponse : public ResponseWithNoData
{
public:
   explicit MakePaymentResponse(XmlIterator& xmlResponse);
   explicit MakePaymentResponse(const std::string& messageId);
   MakePaymentResponse(const std::string& messageId,               
                         const enum SglGameAPI::SglServerExceptionType& errorCode,
                         const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
};

#endif
