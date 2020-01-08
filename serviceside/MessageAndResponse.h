//
// $Id: MessageAndResponse.h 3484 2007-12-12 11:57:57Z stephend $
//
#ifndef _MESSAGE_AND_RESPONSE_
#define _MESSAGE_AND_RESPONSE_

#include <string>

#include <iostream>

class MessageAndResponse
{
public:
   MessageAndResponse(const std::string& message, const std::string& response)
   : m_message(message), m_response(response)
   {
   }
   std::string getMessage() const { return m_message; }
   std::string getResponse() const { return m_response; }
   bool operator==(const MessageAndResponse& other) const
   {
      return m_message == other.m_message && m_response == other.m_response;
   }
   std::ostream& printOn(std::ostream& os) const
   {
      os << m_message << "->" << m_response;
      return os;
   }
private:
   const std::string m_message;
   const std::string m_response;
};

inline std::ostream& operator<<(std::ostream& os, const MessageAndResponse& mar)
{
   return mar.printOn(os);
}

#endif
