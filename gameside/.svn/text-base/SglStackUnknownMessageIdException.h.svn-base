//
// $Id: SglStackUnknownMessageIdException.h 3262 2007-12-03 10:40:54Z stephend $
//
#include "SglGameApiException.h"

class SglStackUnknownMessageIdException : public SglGameApiException
{
public:
   SglStackUnknownMessageIdException(const std::string& key)
   : SglGameApiException(std::string("Unexpected message id ") + key), m_key(key)
   {
   }
   virtual ~SglStackUnknownMessageIdException() throw() {}
   std::string getKey() const
   {
      return m_key;
   }
private:
   const std::string m_key;
};
