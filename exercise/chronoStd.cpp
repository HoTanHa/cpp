#include <iostream>
#include <chrono>
#include <ctime>

long fibonacci(unsigned n)
{
    if (n < 2)
        return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
    auto start = std::chrono::steady_clock::now();
    std::cout << "f(30) = " << fibonacci(30) << '\n';
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    /// std::time_t

    std::time_t timeUnix = std::time(nullptr);
    std::cout << "Time unix: " << timeUnix << std::endl;

    std::tm tm = *std::localtime(&timeUnix);
    std::cout << (tm.tm_year + 1900) << "/" << (tm.tm_mon + 1) << "/" << tm.tm_mday << std::endl;
    return 0;
}