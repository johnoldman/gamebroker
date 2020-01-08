//
// $Id: MakeSystemPaymentResponse.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef MAKE_SYSTEM_PAYMENT_RESPONSE_H_
#define MAKE_SYSTEM_PAYMENT_RESPONSE_H_

#include <string>

#include "GameMessageProducer.h"
#include "ResponseWithNoData.h"

class XmlIterator;

class MakeSystemPaymentResponse : public ResponseWithNoData
{
public:
   explicit MakeSystemPaymentResponse(XmlIterator& xmlResponse);
   explicit MakeSystemPaymentResponse(const std::string& messageId);
   MakeSystemPaymentResponse(const std::string& messageId,               
                         const enum SglGameAPI::SglServerExceptionType& errorCode,
                         const std::string& errorString);
   static std::string getTag();
   std::string getTla() const;
};

#endif
