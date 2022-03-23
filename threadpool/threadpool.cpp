#include "threadpool.h"

//线程池的创建
threadpool::threadpool( int actor_model, connection_pool *connPool, int thread_number, int max_requests) 
  : m_actor_model(actor_model),m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL),m_connPool(connPool)
{
    if (thread_number <= 0 || max_requests <= 0)
        throw std::exception();
    m_threads = new pthread_t[m_thread_number];
    if (!m_threads)
        throw std::exception();
    for (int i = 0; i < thread_number; ++i)
    {
        if (pthread_create(m_threads + i, NULL, worker, this) != 0)
        {
            delete[] m_threads;
            throw std::exception();
        }
        if (pthread_detach(m_threads[i]))//自动回收
        {
            delete[] m_threads;
            throw std::exception();
        }
    }
}

threadpool::~threadpool()
{
    delete[] m_threads;
}

//添加到线程池中（队列）
bool threadpool::append(http_conn *request, int state)
{
    m_queuelocker.lock();
    if (m_workqueue.size() >= m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    request->m_state = state;
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();//提示有请求
    return true;
}

//回调函数
void *threadpool::worker(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    pool->run();
    return pool;
}

//被回调函数调用执行任务
void threadpool::run()
{
    //线程池线程循环处理任务
    while (true)
    {
      m_queuestat.wait(); //信号量等待
      m_queuelocker.lock();

      if (m_workqueue.empty())
      {
        m_queuelocker.unlock();
        continue;
      }

      //提取http请求
      http_conn *request = m_workqueue.front();
      m_workqueue.pop_front();
      m_queuelocker.unlock();
      if (!request)
        continue;

      if (m_actor_model == 1)
      {
        if (request->m_state == 0)
        {
          if (request->read_once())
          {
            request->improv = 1;
            connectionRAII mysqlcon(&request->mysql, m_connPool);
            request->process();
          }
          else
          {
            request->improv = 1;
            request->timer_flag = 1;
          }
        }
        else
        {
          if (request->write())
          {
            request->improv = 1;
          }
          else
          {
            request->improv = 1;
            request->timer_flag = 1;
          }
        }
      }
      else
      {
        connectionRAII mysqlcon(&request->mysql, m_connPool);
        request->process();
      }
    }
}