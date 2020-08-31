#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "lock.h"
#include "sql_connection_pool.h"

template <typename T>
class threadpool
{
public:
    threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_requests = 10000);
    ~threadpool();
    bool apend(T* request, int state);
    bool append_p(T* request);

private:
    int m_thread_number;
    int m_max_requests;
    pthread_t *m_threads;
    std::list<T*> m_workqueue;
    locker m_queuelocker;
    sem m_queuestat;
    connection_pool *m_connPool;
    int m_actor_model;
    
    static void *worker(void *arg);
    void run();
};

#endif // THREADPOOL_H

