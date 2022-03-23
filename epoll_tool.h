#ifndef _EPOLL_TOOL_H
#define _EPOLL_TOOL_H

#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>

namespace epoll_tool{
    //对文件描述符设置非阻塞
    static int setnonblocking(int fd)
    {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
        return old_option;
    }

    //将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
    static void addfd(int epollfd, int fd, bool one_shot, int TRIGMode)
    {
        epoll_event event;
        event.data.fd = fd;

        if (TRIGMode == 1)
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        else
            event.events = EPOLLIN | EPOLLRDHUP;

        if (one_shot)
            event.events |= EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
        setnonblocking(fd);
    }

    //从内核时间表删除描述符
    static void removefd(int epollfd, int fd)
    {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
        close(fd);
    }

    //将事件重置为EPOLLONESHOT
    static void modfd(int epollfd, int fd, int ev, int TRIGMode)
    {
        epoll_event event;
        event.data.fd = fd;

        if (TRIGMode == 1)
            event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
        else
            event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
    }
}

#endif