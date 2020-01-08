//
// $Id: DuplicateSessionException.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "DuplicateSessionException.h"
#include "UserSessionId.h"

DuplicateSessionException::DuplicateSessionException(const std::string& context, const UserSessionId& sessionid)
: std::logic_error(context + " Duplicate Session " + sessionid.toString())
{
}
