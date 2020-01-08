//
// $Id: TagNotFoundException.h 2053 2007-10-25 10:09:46Z stephend $
//
#include <string>
#include <stdexcept>

class TagNotFoundException : public std::runtime_error
{
public:
   TagNotFoundException(const std::string& tagname, const std::string& context);
   virtual ~TagNotFoundException() throw();
private:
   const std::string m_tagName;
   const std::string m_context;
};
