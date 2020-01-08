//
// $Id: MessageIdGenerator.h 3580 2007-12-13 17:01:54Z stephend $
//
#ifndef MESSAGE_ID_GENERATOR_H_
#define MESSAGE_ID_GENERATOR_H_

//#include <ctime>
#include <uuid/uuid.h>
#include <string>

// simple little test class for dishing out unique message ids

class MessageIdGenerator
{
public:
   MessageIdGenerator();
   operator std::string() const;
   MessageIdGenerator& operator++();
private:
   int m_id;
   std::string m_uuidStartTime;
private:
   static std::string getUuid();
};

#endif // _MESSAGE_ID_GENERATOR_HPP_

