#include "locker.h"
#include "lst_timer.h"
#include "threadpool.h"
#include "sql_connection_pool.h"
#include <iostream>

using namespace std;

int main() {
    
    cond cond_;
    locker locker_;
    sem sem_;
    Utils utils_;
//    util_timer util_timer_;
//    sort_timer_lst sort_timer_lst;
    connection_pool connection_pool_;
    threadpool<double> threadpool_(1, &connection_pool_);




    cout << "hello " << endl;

    return 0;
}
