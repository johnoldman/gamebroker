//
// $Id: SglStackDuplicateMessageIdException.h 3262 2007-12-03 10:40:54Z stephend $
//
#include "SglGameApiException.h"

class SglStackDuplicateMessageIdException : public SglGameApiException
{
public:
   SglStackDuplicateMessageIdException(const std::string& key)
   : SglGameApiException(std::string("Duplicate message id") + key)
   , m_key(key)
   {
   }
   virtual ~SglStackDuplicateMessageIdException() throw() {}
   std::string getKey() const
   {
      return m_key;
   }
private:
   const std::string m_key;
};
