#ifndef __SOCKET_MESSAGE_H__
#define __SOCKET_MESSAGE_H__

#include <bits/stdc++.h>
#include <pthread.h>

const int MAX_BUFF_SK = 2048;

class SocketMessage
{
private:
    static SocketMessage *mInstancePtr;
    static std::mutex mLocker_;
    SocketMessage();

    char *buffer_rev_;

    uint16_t port_;
    int socket_fd_;
    pthread_t socket_receive_tid_;

    int socket_open(void);
    int socket_close(void);
    int socket_send_buffer(uint16_t port, char *buff, int len);
    static void *socket_receive_thread(void *arg);

public:
    static SocketMessage *getInstance()
    {
        mLocker_.lock();
        if (nullptr == mInstancePtr)
        {
            mInstancePtr = new SocketMessage();
        }
        mLocker_.unlock();
        return mInstancePtr;
    }

    void create_socket(uint16_t port);
    ~SocketMessage();
};

#endif //__SOCKET_MESSAGE_H__