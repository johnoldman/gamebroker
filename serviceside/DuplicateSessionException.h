//
// $Id: DuplicateSessionException.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <stdexcept>

class UserSessionId;

class DuplicateSessionException : public std::logic_error
{
public:
   DuplicateSessionException(const std::string& context, const UserSessionId& sessionId);
};
