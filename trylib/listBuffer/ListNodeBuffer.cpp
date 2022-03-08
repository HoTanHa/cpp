#include "ListNodeBuffer.h"

/********************* Buffer **************************/
Buffer::Buffer(char *buff, int length)
{
    this->length_ = length;
    buffer_ = new char[this->length_];
    memcpy(this->buffer_, buff, this->length_);
}

Buffer::Buffer(Buffer &buffer)
{
    this->length_ = buffer.length_;
    buffer_ = new char[this->length_];
    memcpy(this->buffer_, buffer.buffer_, this->length_);
}

Buffer::Buffer(Buffer *buffer)
{
    this->length_ = buffer->length_;
    buffer_ = new char[this->length_];
    memcpy(this->buffer_, buffer->buffer_, this->length_);
}

Buffer::~Buffer()
{
    delete[] buffer_;
    buffer_ = nullptr;
}

int Buffer::get_length()
{
    return this->length_;
}

char *Buffer::get_buffer()
{
    return this->buffer_;
}

/********************* ListNodeBuffer **************************/
ListNodeBuffer::ListNodeBuffer(/* args */)
{
    head = nullptr;
}

ListNodeBuffer::~ListNodeBuffer()
{
    mutex_.lock();
    NodeBuffer *current = head;
    NodeBuffer *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        delete tmp->buffer;
        delete tmp;
    }
    mutex_.unlock();
}

void ListNodeBuffer::add_buffer(char *buff, int length)
{
    Buffer *buffer = new Buffer(buff, length);
    mutex_.lock();
    if (head == nullptr)
    {
        head = new NodeBuffer();
        head->buffer = buffer;
        head->next = NULL;
    }
    else
    {
        NodeBuffer *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new NodeBuffer();
        current->next->buffer = buffer;
        current->next->next = NULL;
    }
    mutex_.unlock();
}

void ListNodeBuffer::add_buffer(Buffer &rbuffer)
{
    Buffer *buffer = new Buffer(rbuffer);
    mutex_.lock();
    if (head == nullptr)
    {
        head = new NodeBuffer();
        head->buffer = buffer;
        head->next = NULL;
    }
    else
    {
        NodeBuffer *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new NodeBuffer();
        current->next->buffer = buffer;
        current->next->next = NULL;
    }
    mutex_.unlock();
}

void ListNodeBuffer::add_buffer(Buffer *pbuffer)
{
    Buffer *buffer = new Buffer(pbuffer);
    if (buffer == nullptr)
    {
        return;
    }

    mutex_.lock();
    if (head == nullptr)
    {
        head = new NodeBuffer();
        head->buffer = buffer;
        head->next = NULL;
    }
    else
    {
        NodeBuffer *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new NodeBuffer();
        current->next->buffer = buffer;
        current->next->next = NULL;
    }
    mutex_.unlock();
}

void ListNodeBuffer::delete_head_buffer()
{
    mutex_.lock();
    if (head == NULL)
    {
        return;
    }
    NodeBuffer *temp_buffer = head;
    head = head->next;
    delete temp_buffer->buffer;
    delete temp_buffer;
    mutex_.unlock();
}

void ListNodeBuffer::delete_all_buffer()
{
    mutex_.lock();
    NodeBuffer *current = head;
    NodeBuffer *tmp;
    while (current != NULL)
    {
        tmp = current;
        current = current->next;
        delete tmp->buffer;
        delete tmp;
    }
    head = NULL;
    mutex_.unlock();
}

Buffer *ListNodeBuffer::getBuffer()
{
    if (head == NULL)
    {
        return NULL;
    }
    return head->buffer;
}