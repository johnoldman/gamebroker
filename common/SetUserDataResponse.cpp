//
// $Id: SetUserDataResponse.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "SetUserDataResponse.h"
#include "XmlIterator.h"

SetUserDataResponse::SetUserDataResponse(XmlIterator& xmlResponse)
: ResponseWithNoData(xmlResponse)
{
}

SetUserDataResponse::SetUserDataResponse(const std::string& messageId)
: ResponseWithNoData(messageId)
{
}

SetUserDataResponse::SetUserDataResponse(const std::string& messageId,               
                                const enum SglGameAPI::SglServerExceptionType& errorCode,
                                const std::string& errorString)
: ResponseWithNoData(messageId, errorCode, errorString)
{
}

std::string SetUserDataResponse::getTag()
{
   return "SUD";
}

std::string SetUserDataResponse::getTla() const
{
   return getTag();
}
