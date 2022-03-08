#include "SocketMessage.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

std::mutex SocketMessage::mLocker_;
SocketMessage *SocketMessage::mInstancePtr = nullptr;

SocketMessage::SocketMessage(/* args */)
{
}

SocketMessage::~SocketMessage()
{
}

int SocketMessage::socket_open(void)
{

    if ((socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in socketAddr;
    while (1)
    {
        memset(&socketAddr, 0, sizeof(socketAddr));
        socketAddr.sin_family = AF_INET;
        socketAddr.sin_port = htons(port_);
        socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (bind(socket_fd_, (const struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
        {
            perror("bind socket fail.\r\n");
            port_ += 100;
        }
        else
        {
            break;
        }
    }

    pthread_create(&this->socket_receive_tid_, NULL, socket_receive_thread, this);
    return 0;
}

int SocketMessage::socket_close(void)
{
    pthread_join(this->socket_receive_tid_, NULL);
    close(this->socket_fd_);
    return 0;
}

int SocketMessage::socket_send_buffer(uint16_t port, char *buff, int len)
{
    if (socket_fd <= 0)
        return -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int n = 0;
    n = sendto(this->socket_fd_, buff, len, MSG_CONFIRM, (struct sockaddr *)&addr, sizeof(addr));
    if (len != n)
    {
        error_msg("send socket: error\n");
    }
    return n;
}

void *SocketMessage::socket_receive_thread(void *arg)
{
    SocketMessage *socketMsg = (SocketMessage *)arg;
    socketMsg->buffer_rev_ = (char *)malloc(MAX_BUFF_SK);
    int len;
    uint16_t n = 0;
    char *buffer = socketMsg->buffer_rev_;
    struct sockaddr_in clientsk;
    memset(buffer, 0, MAX_BUFF_SK);
    memset(&clientsk, 0, sizeof(clientsk));

    len = sizeof(clientsk); // len is value/resuslt
    int i;
    while (1)
    {
        /* code */
        n = recvfrom(socketMsg->socket_fd_, (char *)buffer, MAX_BUFF_SK, MSG_WAITALL, (struct sockaddr *)&clientsk, &len);
        if ((n == 0)) //|| (n > MAX_BUFF_SK))
        {
            continue;
        }
        union data32 add;
        add.data32 = clientsk.sin_addr.s_addr;
        uint16_t port_cli = ntohs(clientsk.sin_port);
        debug_msg("Socket revc ip: %d.%d.%d.%d port: %d..%d...\r\n", add.data8[0], add.data8[1], add.data8[2], add.data8[3], port_cli, n);
        // for (i = 0; i < n; i++)
        // {
        // 	printf("%02X ", buffer[i]);
        // }
        // printf("\r\n");

        memset(buffer, 0, MAX_BUFF_SK);
    }
    free(socketMsg->buffer_rev);
    pthread_exit(0);
}

void SocketMessage::create_socket(int port)
{
    this->port_ = port;
}