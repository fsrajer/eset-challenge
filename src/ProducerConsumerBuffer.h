//----------------------------------------------------------------------------------------
/**
* \file       producer_consumer_buffer.h
* \author     Filip Srajer
* \date       November, 2015
* \brief      Templated buffer for producer-consumer pattern.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include <mutex>

#include "semaphore.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////     Declarations     ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/// For signalling state.
enum ProducerConsumerBufferSignal
{
  OK, ///< Everything OK.
  SHUTDOWN ///< All work done. Shutdown.
};

/// Templated buffer for producer-consumer pattern.
/**
Uses std::vector. For insert and remove uses assignment operator.
*/
template<typename T>
class ProducerConsumerBuffer
{
public:
  /// Constructor.
  /**
  Initializes buffer and sets empty Semaphore to bufferSize and 
  full semaphore to 0.
  \param[in] bufferSize Buffer maximum size.
  */
  ProducerConsumerBuffer(int bufferSize);

  /// Insert item to the buffer. Wait if the buffer is full.
  /**
  \param[in] item  Item to be added by copy constructor.
  */
  void insertItem(const T& item);

  /// Remove item from the buffer. Wait if the buffer is empty and shutdown was not
  /// triggered.
  /**
  \param[in] removedItem  Item that was removed (returned using copy constructor).
  \return Shutdown signal.
  */
  ProducerConsumerBufferSignal removeItem(T *removedItem);

  /// Shutdown says the buffer to send Signal::SHUTDOWN when 
  /// all the work will get done.
  void shutdown();

private:
  int counter_;
  std::vector<T> data_;
  std::mutex mtx_;
  Semaphore full_;
  Semaphore empty_;
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////     Definitions      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ProducerConsumerBuffer<T>::ProducerConsumerBuffer(int bufferSize)
  : counter_(0),data_(bufferSize),full_(0),empty_(bufferSize)
{
}

template<typename T>
void ProducerConsumerBuffer<T>::insertItem(const T& item)
{
  empty_.wait();
  mtx_.lock();
  data_[counter_++] = item;
  mtx_.unlock();
  full_.notify();
}

template<typename T>
ProducerConsumerBufferSignal ProducerConsumerBuffer<T>::removeItem(T *removedItem)
{
  Semaphore::WaitSignal signal = full_.wait();
  if(signal == Semaphore::WaitSignal::OK)
  {
    mtx_.lock();
    *removedItem = data_[--counter_];
    mtx_.unlock();
    empty_.notify();
    return ProducerConsumerBufferSignal::OK;
  } else
  {
    return ProducerConsumerBufferSignal::SHUTDOWN;
  }
}

template<typename T>
void ProducerConsumerBuffer<T>::shutdown()
{
  // It should suffice to shutdown only full but shutting down empty
  // does no harm.
  empty_.shutdown();
  full_.shutdown();
}
