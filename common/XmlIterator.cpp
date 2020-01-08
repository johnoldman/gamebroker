//
// $Id: XmlIterator.cpp 3314 2007-12-05 11:19:17Z stephend $
//
// Noddy XML parser. Used so that it it much faster
// than a standard XML parser - we pretty much expect our XML to
// be very simple (no attributes even!)
//
#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "XmlIterator.h"
#include "TagNotFoundException.h"

XmlIterator::XmlIterator(const std::string& data)
: m_data(data)
, m_pos(0)
{
}
//
// extractString - remove a single value from <a>data</a>
// process XML escapes so that string turns out correct.
//
std::string XmlIterator::extractString(const std::string& elementName)
{
   const std::string data = extractStringData(elementName);
   std::string::size_type pos = 0; 
   std::string retdata;
   while (pos < data.length())
   {
      static char* pats[] = {"&amp;", "&lt;", "&gt;"};
      static char* chars = "&<>";
      bool matched = false;
      for (size_t i = 0; i < sizeof(pats)/sizeof(pats[0]); ++i)
      {
         if (data.substr(pos, strlen(pats[i])) == pats[i])
         {
            retdata += chars[i];
            pos += strlen(pats[i]);
            matched = true;
            break;
         }
      }
      if (!matched)
      {
         retdata += data[pos++];
      }
   }
   return retdata;
}
//
// extractStringData - private method called by extractString and extractXML
// Just search for data within <x> and </x>
//
std::string XmlIterator::extractStringData(const std::string& elementName)
{
   // Skip white space between tags - otherwise sensibly-formatted XML doesn't parse in our noddy parser.
   while (m_pos <= m_data.length() && std::string("\r\n\t ").find(m_data[m_pos]) != std::string::npos)
   {
      ++m_pos;
   }
   if (m_data.substr(m_pos, elementName.length() + 2) == std::string("<") + elementName + ">")
   {
      m_pos += elementName.length() + 2;
      const std::string::size_type end = m_data.find("</" + elementName + ">", m_pos); 
      if (end != std::string::npos)
      {
         const std::string::size_type pos = m_pos;
         m_pos = end + elementName.length() + 3;
         return m_data.substr(pos, end - pos);
      }
      else
      {
         throw TagNotFoundException(elementName, std::string("End tag ") + m_data.substr(m_pos));
      }
   }
   else
   {
      throw TagNotFoundException(elementName, m_data.substr(m_pos));
   }
}

long XmlIterator::extractLong(const std::string& elementName)
{
   return boost::lexical_cast<long>(extractString(elementName));
}

std::string XmlIterator::extractXml(const std::string& elementName)
{
   return extractStringData(elementName);
}
