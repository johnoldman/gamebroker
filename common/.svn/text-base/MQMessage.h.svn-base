//
// message base class
//
// $Id: MQMessage.h 3682 2007-12-17 14:08:53Z stephend $
//
#ifndef _MQMESSAGE_H_
#define _MQMESSAGE_H_

#include <string>

class GameMessageProducer;

class MQMessage
{
public:
   MQMessage(const std::string &messageId, const time_t& eventTime);
   explicit MQMessage(const std::string &messageId);
   virtual ~MQMessage();
   std::string getMessageId() const;
   virtual void send(GameMessageProducer& towhere) const = 0;
   // Return the serialised version of the message.
   std::string getSerialised() const;
   // Tla (3-letter anacronym) for command
   virtual std::string getTla() const = 0;
   time_t getEventTime() const;
   static std::string getEnvelopeContents(const std::string& message);
protected:
   static std::string wrapXml(const std::string &wrapperString, const std::string &insertString);
   static std::string wrapString(const std::string &wrapperString, const std::string &insertString);
   static std::string wrapString(const std::string &wrapperString, long insertValue);
   std::string wrapEnvelope(const std::string& messageType, const std::string &contents) const;
private:
   const std::string m_messageId;
   const time_t m_eventTime;
};

#endif
