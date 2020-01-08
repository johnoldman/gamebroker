//
// $Id: DuplicateUserException.h 3484 2007-12-12 11:57:57Z stephend $
//
#include <stdexcept>

class UserSessionId;

class DuplicateUserException : public std::logic_error
{
public:
   DuplicateUserException(const std::string& context, long userId);
};
