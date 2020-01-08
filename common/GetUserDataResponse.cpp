//
// $Id: GetUserDataResponse.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "GameMessageProducer.h"
#include "XmlIterator.h"
#include "GetUserDataResponse.h"

std::string GetUserDataResponse::getTag()
{
   return "GUD";
}

GetUserDataResponse::GetUserDataResponse(XmlIterator& xmlResponse)
: ServiceBrokerResponse(xmlResponse)
{
   if (isSuccess())
   {
      m_data = xmlResponse.extractString("userData");
   }
}

GetUserDataResponse::GetUserDataResponse(const std::string& messageId,  
                              const std::string& userData)
: ServiceBrokerResponse(messageId)
, m_data(userData)
, serialisedUserData(wrapString("userData", m_data))
{
}

GetUserDataResponse::GetUserDataResponse(const std::string& messageId,               
                                 const enum SglGameAPI::SglServerExceptionType& errorCode,
                                 const std::string& errorString)
: ServiceBrokerResponse(messageId, errorCode, errorString)
, serialisedUserData("")
{
}

void GetUserDataResponse::send(GameMessageProducer& producer) const
{
   // serialise
   producer.sendMessage(serialiseResponse(serialisedUserData), "");
}

std::string GetUserDataResponse::getTla() const
{
   return getTag();
}
