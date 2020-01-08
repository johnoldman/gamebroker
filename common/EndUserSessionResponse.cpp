//
// $Id: EndUserSessionResponse.cpp 2791 2007-11-14 18:24:18Z stephend $
//
#include "EndUserSessionResponse.h"
#include "GameMessageProducer.h"
#include "XmlIterator.h"

EndUserSessionResponse::EndUserSessionResponse(XmlIterator& xmlResponse)
: ResponseWithNoData(xmlResponse)
{
}

std::string EndUserSessionResponse::getTag()
{
   return "EUS";
}

EndUserSessionResponse::EndUserSessionResponse(const std::string& messageId)
: ResponseWithNoData(messageId)
{
}

EndUserSessionResponse::EndUserSessionResponse(const std::string& messageId,               
                                  const enum SglGameAPI::SglServerExceptionType& errorCode,
                                  const std::string& errorString)
: ResponseWithNoData(messageId, errorCode, errorString)
{
}

std::string EndUserSessionResponse::getTla() const
{
   return getTag();
}
