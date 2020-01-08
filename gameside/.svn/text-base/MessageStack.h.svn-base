//
// $Id: MessageStack.h 1989 2007-10-23 16:06:18Z stephend $
//
// (badly named) Message Stack class for GameAPI purse functionality
//
// Thread-safe version of map essentially with Add() and Remove() methods
// so that message consumer can read replies from broker and 
// deliver them to the async client API.
//
#ifndef MESSAGE_STACK_H_
#define MESSAGE_STACK_H_

#include <string>

#include <iostream>

#include <map>

#include <boost/thread/mutex.hpp>

template <class Value>
class MessageStack
{
public:
   void addMessage(const std::string& key, const Value& value);
   Value removeMessage(const std::string& key);
private: 
   boost::mutex m_mutex;
   typedef std::map<std::string, Value> MapType;
   //typedef MapType::iterator Iterator;
   MapType m_map;
};

//#include "MessageStack.hpp"
#include "SglStackDuplicateMessageIdException.h"
#include "SglStackUnknownMessageIdException.h"

template<class Value>
void MessageStack<Value>::addMessage(const std::string& key, const Value& value)
{
   boost::mutex::scoped_lock lock(m_mutex);

   bool worked = m_map.insert(std::pair<std::string, Value>(key, value)).second;
   if (!worked)
   {
      // Oh dear throw exception - item already present.
      throw SglStackDuplicateMessageIdException(key);
   }
}

template <class Value>
Value MessageStack<Value>::removeMessage(const std::string& key)
{
   boost::mutex::scoped_lock lock(m_mutex);

   //Iterator it = m_map.find();
   if (m_map.find(key) != m_map.end())
   {
      Value value = m_map.find(key)->second;
      m_map.erase(key);
      return value;
   }
   else
   {
      // oh dear throw, we didn't find it.
      throw SglStackUnknownMessageIdException(key);
   }
}

#endif
