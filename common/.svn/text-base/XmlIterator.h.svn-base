//
// $Id: XmlIterator.h 3249 2007-11-30 11:23:49Z stephend $
//
#ifndef XMLITERATOR_H_
#define XMLITERATOR_H_

#include <string>

class XmlIterator
{
public:
   explicit XmlIterator(const std::string& xmlData);
   std::string extractString(const std::string& elementName);
   long extractLong(const std::string& elementName);
   std::string extractXml(const std::string& elementName);
private:
   std::string extractStringData(const std::string& elementName);
private:
   std::string m_data;
   std::string::size_type m_pos;
};

#endif
