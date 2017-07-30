#include "Semaphore.h"

Semaphore::Semaphore(int count)
  : count_(count),shutdownWhenDone_(false)
{
}

void Semaphore::setCount(int count)
{
  count_ = count;
}

void Semaphore::notify()
{
  std::unique_lock<std::mutex> lck(mtx_);
  ++count_;
  cv_.notify_one();
}

Semaphore::WaitSignal Semaphore::wait()
{
  std::unique_lock<std::mutex> lck(mtx_);
  while(count_ == 0 && !shutdownWhenDone_)
  {
    cv_.wait(lck);
  }
  if(count_ != 0)
  {
    --count_;
    return WaitSignal::OK;
  } else
  {
    return WaitSignal::SHUTDOWN;
  }
}

void Semaphore::shutdown()
{
  shutdownWhenDone_ = true;
  cv_.notify_all();
}