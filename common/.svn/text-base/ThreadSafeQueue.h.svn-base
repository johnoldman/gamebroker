//
// $Id: ThreadSafeQueue.h 2594 2007-11-09 10:46:53Z stephend $
//
// This class is a simple templated queue with an optional maximum size.
//
#ifndef THREAD_SAFE_QUEUE_H_
#define THREAD_SAFE_QUEUE_H_

#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/bind.hpp>
#include <map>

template <class T>
class ThreadSafeQueue
{
public:
   class QueueKickedException : public std::runtime_error
   {
   public:
      QueueKickedException() : std::runtime_error("Queue kicked, blocking read unblocked and throws")
      {
      }
   };
   ThreadSafeQueue(unsigned int limit = UINT_MAX)
   : m_limit(limit)
   , m_bKicked(false)
   {
      if (limit == 0)
      {
         throw std::logic_error("Doesn't make sense to create a ThreadSafeQueue with size 0");
      }
   }
   // This blocks until either an item arrives or another thread calls 'kick'
   // allowing a blocking thread to wake w/o reading a datum.
   T blockingRead()
   {
      boost::mutex::scoped_lock lock(mutex_);
      
      while (data_.empty() && !m_bKicked)
      {
         condition_.wait(lock);
      }
      if (data_.empty())
      {
         m_bKicked = false;
         throw QueueKickedException();
      } 
      else
      {
         T ret = data_.front();
         data_.pop_front();
         // Notify any potential writers that an item has been removed.
         toobigcondition_.notify_all();
         return ret; 
      }
   }
   // Add an item to the queue.
   void write(const T& value)
   {
      boost::mutex::scoped_lock lock(mutex_);
      while (data_.size() == m_limit && !m_bKicked)
      {
         toobigcondition_.wait(lock);
      }
      if (data_.size() < m_limit)
      {
         data_.push_back(value);
         condition_.notify_all();
      }
      else
      {
         m_bKicked = false;
         throw QueueKickedException();
      }
   }
   // Free a thread waiting on blockingRead() to run again.
   void kick()
   {
      boost::mutex::scoped_lock lock(mutex_);
      m_bKicked = true;
      condition_.notify_all();
   }
private:
   boost::mutex mutex_;
   boost::condition condition_;
   // condition used to stall write when too much data written.
   boost::condition toobigcondition_;
   std::list<T> data_;
   unsigned int m_limit;
   bool m_bKicked;
};

#endif

