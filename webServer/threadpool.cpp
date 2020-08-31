#include "threadpool.h"

template <typename T>
bool threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests)
    : m_actor_mode(actor_model), m_thread_number(thread_number), m_max_requests(max_requests), m_threads(NULL), m_connPool(connPool)
{
    

}
