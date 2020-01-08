//
// $Id: StubConsumer.cpp 3226 2007-11-29 17:16:16Z johno $
//
#include "StubConsumer.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

StubConsumer::StubConsumer()
: m_bThrow(false)
, m_bThrowInt(false)
{
}

void StubConsumer::setThrow(bool bThrow)
{
   m_bThrow = bThrow;
}

void StubConsumer::setThrowInt()
{
   m_bThrowInt = true;
}

void StubConsumer::receive(const std::string& message, const time_t& eventTime)
{
   if (m_bThrowInt)
   {
      m_bThrowInt = false;
      throw 27;
   } else if (m_bThrow)
   {
      throw std::runtime_error("StubConsumer::receive throwing");
   }
   else
   {
      boost::mutex::scoped_lock lock(m_mutex);
      m_messages.push_back(message);
   }
}

void StubConsumer::error(const std::string& message)
{
   std::cout << "error " << message << std::endl;
}

std::string StubConsumer::getMessage(size_t index) const
{
   boost::mutex::scoped_lock lock(m_mutex);
   if (m_messages.size() > index)
   {
      return m_messages[index];
   }
   else
   {
      throw std::runtime_error(std::string("Request for index ") + boost::lexical_cast<std::string>(index) + " when size is " + boost::lexical_cast<std::string>(m_messages.size()));
   }
}

size_t StubConsumer::getMessageCount() const
{
   boost::mutex::scoped_lock lock(m_mutex);
   return m_messages.size();
}

void StubConsumer::clear()
{
   m_messages.clear();
}
