#include "lst_timer.h"
#include "http_conn.h"

sort_timer_lst::sort_timer_lst() {
    head = tail = NULL;
}

sort_timer_lst::~sort_timer_lst() {
    while (head) {
        util_timer *tmp = head->next;
        delete head;
        head = tmp;
    }
}

void sort_timer_lst::add_timer(util_timer *timer) {
    if (!timer) return;
    if (!head) {
        head = tail = timer;
        return;
    }
    if (timer->expire <= head->expire) {
        head->prev = timer;
        timer->next = head;
        head = timer;
        return;
    }
    add_timer(timer, head);
}

void sort_timer_lst::adjust_timer(util_timer *timer) {
    if (!timer || !timer->next) return;
    if (timer->next && timer->next->expire >= timer->expire) return;
    if (timer == head) {
        head = head->next;
        head->prev = NULL;
        timer->next = NULL;
        add_timer(timer, head);
    }else {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        add_timer(timer, timer->next);
    }
}

void sort_timer_lst::del_timer(util_timer *timer) {
    if (!timer) return;
    if(timer == head && timer == tail) {
        delete timer;
        head = tail = NULL;
        return;
    }
    if (timer == head) {
        head = head->next;
        head->prev = NULL;
        delete timer;
    }else if (timer == tail) {
        tail = tail->prev;
        tail->next = NULL;
        delete timer;
    }else {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }
}

void sort_timer_lst::tick() {
    if (!head) return;
    util_timer *now = head;
    time_t cur = time(NULL);

    while (now && cur >= now->expire) {
        now->cb_func(now->user_data);
        head = now->next;
        delete now;
        now = head;
    }
    if (head) head->prev = NULL;
}

void sort_timer_lst::add_timer(util_timer *timer, util_timer *head) {
    util_timer* prev = head->prev;
    while (head) {
        if (timer->expire > head->expire) {
            prev = head;
            head = head->prev;
        }else break;
    }
    if  (!head) {
        tail = timer;
        tail->prev = prev;
        tail->next = NULL;
        prev->next = timer;
    }else {
        prev->next = timer;
        timer->prev = prev;
        timer->next = head;
        head->prev = timer;
    }
}

int Utils::setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void Utils::addfd(int epollfd, int fd, bool one_shot, int TRIGMode) {
    setnonblocking(fd);
    epoll_event event;
    event.data.fd = fd;
    if (TRIGMode == 1) event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    else event.events = EPOLLIN | EPOLLRDHUP;
    if (one_shot) event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void Utils::addsig(int sig, void(handler)(int), bool restart) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
    if (restart) sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void Utils::timer_handler() {
    m_timer_lst.tick();
    alarm(m_TIMESLOT);
}

void Utils::show_error(int connfd, const char *info) {
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int *Utils::u_pipefd = 0;
int Utils::u_epollfd = 0;

class Utils;
void cb_func(client_data *user_data) {
    epoll_ctl(Utils::u_epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
}
