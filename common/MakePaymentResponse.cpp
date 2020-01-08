//
// $Id: MakePaymentResponse.cpp 2791 2007-11-14 18:24:18Z stephend $
//
#include "MakePaymentResponse.h"
#include "XmlIterator.h"

MakePaymentResponse::MakePaymentResponse(XmlIterator& xmlResponse)
: ResponseWithNoData(xmlResponse)
{
}

MakePaymentResponse::MakePaymentResponse(const std::string& messageId)
: ResponseWithNoData(messageId)
{
}

MakePaymentResponse::MakePaymentResponse(const std::string& messageId,               
                         const enum SglGameAPI::SglServerExceptionType& errorCode,
                         const std::string& errorString)
: ResponseWithNoData(messageId, errorCode, errorString)
{
}

std::string MakePaymentResponse::getTag()
{
   return "MPY";
}

std::string MakePaymentResponse::getTla() const
{
   return getTag();
}
