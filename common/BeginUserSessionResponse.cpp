//
// $Id: BeginUserSessionResponse.cpp 2791 2007-11-14 18:24:18Z stephend $
//
#include "BeginUserSessionResponse.h"

#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include "XmlIterator.h"

#define PURSE_TOTAL_TAG "purseTotal"

BeginUserSessionResponse::BeginUserSessionResponse(XmlIterator& xmlResponse)
: ServiceBrokerResponse(xmlResponse)
, m_purseTotal(0)
{
   if (isSuccess())
   {
      m_purseTotal = xmlResponse.extractLong(PURSE_TOTAL_TAG);
   }
}

std::string BeginUserSessionResponse::getTla() const
{
   return getTag();
}

BeginUserSessionResponse::BeginUserSessionResponse(const std::string& messageId,               
                          long purseTotal)
: ServiceBrokerResponse(messageId)
, m_purseTotal(purseTotal)
, serialPurseTotal(wrapString(PURSE_TOTAL_TAG, boost::lexical_cast<std::string>(m_purseTotal)))
{
}

BeginUserSessionResponse::BeginUserSessionResponse(const std::string& messageId,               
                          const enum SglGameAPI::SglServerExceptionType& errorCode,
                          const std::string& errorString)
: ServiceBrokerResponse(messageId, errorCode, errorString)
, serialPurseTotal("")
{
}

long BeginUserSessionResponse::getPurseTotal() const
{
   if (isSuccess())
   {
      return m_purseTotal;
   }
   else
   {
      throw std::logic_error("BeginUserSessionResponse::getPurseTotal - no total available");
   }
}

std::string BeginUserSessionResponse::getTag()
{
   return "BUS";
}

void BeginUserSessionResponse::send(GameMessageProducer& producer) const
{
   // serialise
   producer.sendMessage(serialiseResponse(serialPurseTotal), "");
}
