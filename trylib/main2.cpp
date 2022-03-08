#include "webServer/HttpServer.h"

#include <bits/stdc++.h>
void ignoreLine()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
int main()
{
    int c = 0;
    HttpServer *httpserver = HttpServer::getInstance();
    while (1)
    {
        std::cout << "Enter the key: ";
        std::cin >> c;
        if (std::cin.fail())
        {
            std::cin.clear(); // put us back in 'normal' operation mode
            ignoreLine();     // and remove the bad input
        }
        else
        {
            if (c == 1)
            {
                httpserver->start_server(true);
            }
            else if (c == 2)
            {
                httpserver->start_server(false);
            }
            else if (c == 3)
            {
                httpserver->stop_server();
            }
            ignoreLine();
        }
        sleep(2);
    }

    return 0;
}

// g++  -Wall -g  -std=c++11 main2.cpp webServer/HttpServer.h webServer/HttpServer.cpp webServer/mongoose.c webServer/mongoose.h -o server.o -pthread