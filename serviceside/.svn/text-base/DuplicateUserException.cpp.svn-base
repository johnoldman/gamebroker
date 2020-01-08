//
// $Id: DuplicateUserException.cpp 3484 2007-12-12 11:57:57Z stephend $
//
#include "DuplicateUserException.h"

#include <boost/lexical_cast.hpp>

DuplicateUserException::DuplicateUserException(const std::string& context, long userId)
: std::logic_error(context + " Duplicate User " + boost::lexical_cast<std::string>(userId))
{
}
