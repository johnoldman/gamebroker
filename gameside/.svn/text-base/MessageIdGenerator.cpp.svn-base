//
// $Id: MessageIdGenerator.cpp 3580 2007-12-13 17:01:54Z stephend $
//
#include "MessageIdGenerator.h"

#include <string>
#include <boost/lexical_cast.hpp>

MessageIdGenerator::MessageIdGenerator()
: m_id(0)
, m_uuidStartTime(getUuid())
{}
//
// Prefix increment operator
//
MessageIdGenerator& MessageIdGenerator::operator++()
{
   const int oldId = m_id;
   // Wraparound time - generate a new UUID.
   if (++m_id <= oldId)
   {
      m_uuidStartTime = getUuid();
   }
   return *this;
}

MessageIdGenerator::operator std::string() const
{
   return std::string("S-") + m_uuidStartTime + ":" + boost::lexical_cast<std::string>(m_id);
}

std::string MessageIdGenerator::getUuid()
{
   uuid_t uuid;
   char uuidChar[37];
   uuid_generate_time(uuid);
   uuid_unparse(uuid, uuidChar);
   return std::string(uuidChar);
}
