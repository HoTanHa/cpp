#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include <bits/stdc++.h>

#include "mongoose.h"
#include <pthread.h>

class HttpServer
{
private:
    static HttpServer *mInstancePtr;
    static std::mutex mLocker_;
    HttpServer();

    int port;
    struct mg_mgr mgr;
    struct mg_connection *connection;
    static struct mg_serve_http_opts s_http_server_opts;
    int is_running_;
    pthread_t http_server_tid_;
    static void *http_server_thread(void *arg);
    static void event_request_handler(struct mg_connection *nc, int ev, void *p);
    static void handle_request_hello(struct mg_connection *nc, int ev, void *p);
    static void handle_request_api(struct mg_connection *nc, int ev, void *p);
    static void handle_request_setup(struct mg_connection *nc, int ev, void *p);

    pthread_t file_server_tid_;
    static void *file_server_thread(void *arg);
    struct mg_mgr file_mgr;
    struct mg_connection *file_connection;
    static struct mg_serve_http_opts file_serve_opts;
    static void file_event_handle(struct mg_connection *nc, int ev, void *p);


public:
    static HttpServer *getInstance()
    {
        mLocker_.lock();
        if (nullptr == mInstancePtr)
        {
            mInstancePtr = new HttpServer();
        }
        mLocker_.unlock();
        return mInstancePtr;
    }

    ~HttpServer();

    void start_server(bool isAdsun);
    void stop_server();
};

#endif // __HTTPSERVER_H__
