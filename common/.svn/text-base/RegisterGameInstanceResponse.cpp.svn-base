//
// $Id: RegisterGameInstanceResponse.cpp 2791 2007-11-14 18:24:18Z stephend $
//
#include "RegisterGameInstanceResponse.h"
#include "GameMessageProducer.h"

std::string RegisterGameInstanceResponse::getTag()
{
   return "RGI";
}

RegisterGameInstanceResponse::RegisterGameInstanceResponse(XmlIterator& xmlResponse)
: ResponseWithNoData(xmlResponse)
{
}

RegisterGameInstanceResponse::RegisterGameInstanceResponse(const std::string& messageId)
: ResponseWithNoData(messageId)
{
}

RegisterGameInstanceResponse::RegisterGameInstanceResponse(const std::string& messageId,               
                              const enum SglGameAPI::SglServerExceptionType& errorCode,
                              const std::string& errorString)
: ResponseWithNoData(messageId, errorCode, errorString)
{
}

std::string RegisterGameInstanceResponse::getTla() const
{
   return getTag();
}
