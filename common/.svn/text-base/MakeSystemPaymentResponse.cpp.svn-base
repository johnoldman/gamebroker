//
// $Id: MakeSystemPaymentResponse.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "MakeSystemPaymentResponse.h"
#include "XmlIterator.h"

MakeSystemPaymentResponse::MakeSystemPaymentResponse(XmlIterator& xmlResponse)
: ResponseWithNoData(xmlResponse)
{
}

MakeSystemPaymentResponse::MakeSystemPaymentResponse(const std::string& messageId)
: ResponseWithNoData(messageId)
{
}

MakeSystemPaymentResponse::MakeSystemPaymentResponse(const std::string& messageId,               
                         const enum SglGameAPI::SglServerExceptionType& errorCode,
                         const std::string& errorString)
: ResponseWithNoData(messageId, errorCode, errorString)
{
}

std::string MakeSystemPaymentResponse::getTag()
{
   return "MSP";
}

std::string MakeSystemPaymentResponse::getTla() const
{
   return getTag();
}
