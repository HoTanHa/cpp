#ifndef __LISTNODEBUFFER_H__
#define __LISTNODEBUFFER_H__

#include <bits/stdc++.h>

class Buffer
{
private:
    char *buffer_;
    int length_;

public:
    Buffer(char *buff, int length);
    Buffer(Buffer &buffer);
    Buffer(Buffer *buffer);
    ~Buffer();

    int get_length();
    char *get_buffer();
};

typedef struct node_buffer_
{
    Buffer *buffer;
    struct node_buffer_ *next;
} NodeBuffer;

class ListNodeBuffer
{
private:
    NodeBuffer *head;
    std::mutex mutex_;

public:
    ListNodeBuffer(/* args */);
    ~ListNodeBuffer();

    void add_buffer(char *buffer, int length);
    void add_buffer(Buffer &buffer);
    void add_buffer(Buffer *buffer);
    void delete_head_buffer();
    void delete_all_buffer();

    Buffer *getBuffer();
};

#endif //__LISTNODEBUFFER_H__