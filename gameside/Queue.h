//
//
// $Id:
//
//
#ifndef QUEUE_H_
#define QUEUE_H_

#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>
#include <map>

// queue stores a mutex, whilst blocking a sync command

template <class T>
class Queue
{
public:
   T getItem(const std::string& key)
   {
      boost::mutex::scoped_lock lock(mutex_);     
      while (!doesKeyExist(key))
      {
         condition_.wait(lock);
      }
      typename std::map<std::string, T>::iterator posn;
      posn = data_.find(key);
      T ret = posn->second;      
      data_.erase(key);
      return ret;
   }
   void putItem(const std::string& key, const T& value)
   {
      boost::mutex::scoped_lock lock(mutex_);
      data_.insert(make_pair(key,value));
      condition_.notify_all();
   }
private:
   bool doesKeyExist(const std::string& key) const
   { 
      return( data_.find(key) != data_.end() );  
   }
private:
   boost::mutex mutex_;
   boost::condition condition_;
   std::map<std::string, T> data_;
};

#endif

