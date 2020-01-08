//
// $Id: UserSessionId.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef USER_SESSION_ID_H_
#define USER_SESSION_ID_H_
//
// Type-safe SessionId class, used for incoming SessionID strings.
// Implemented all inline for now, so just as performant as not using it.
//
class UserSessionId
{
public:
   explicit UserSessionId(const std::string& id) : m_id(id)
   {
   }
   bool operator<(const UserSessionId& other) const
   {
      return m_id < other.m_id;
   }
   std::string toString() const
   {
      return m_id;
   }
private:
   std::string m_id;
};

#endif
