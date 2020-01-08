#include "TagNotFoundException.h"

TagNotFoundException::TagNotFoundException(const std::string& tagname, const std::string& context)
: std::runtime_error(std::string("Element ") + tagname + " not found " + context)
, m_tagName(tagname)
, m_context(context)
{
}

TagNotFoundException::~TagNotFoundException() throw()
{
}
