//----------------------------------------------------------------------------------------
/**
* \file       Semaphore.h
* \author     Filip Srajer
* \date       November, 2015
* \brief      Semaphore using mutex and condition variable.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>

/// Semaphore using mutex and condition variable.
class Semaphore
{
public:
  /// Constructor.
  /**
  \param[in] count Count for counting semaphore.
  */
  Semaphore(int count = 0);

  /// Set new count.
  /**
  \param[in] count Count for counting semaphore.
  */
  void setCount(int count);

  /// Notify waiting threads.
  void notify();

  /// Returned by wait().
  enum WaitSignal
  {
    OK, ///< Everything OK.
    SHUTDOWN ///< All work done. (count is 0 and shutdown() was called.)
  };

  /// Wait for notify or shutdown signal.
  /**
  Returns if there is work or if all work is done and shutdown signal was
  passed using shutdown().
  \return Shutdown signal.
  */
  WaitSignal wait();

  /// Shutdown when all work will get done.
  void shutdown();

private:
  std::atomic<bool> shutdownWhenDone_;
  std::mutex mtx_;
  std::condition_variable cv_;
  int count_;
};

