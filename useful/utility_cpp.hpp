#ifndef __UTILITY_CPP_H__
#define __UTILITY_CPP_H__

#include <chrono> // for std::chrono functions

namespace utilityCpp
{
    /** **************************************************
     * @brief  A simple Timer class to measure elapsed time
     */
    class Timer
    {
    private:
        // Type aliases to make accessing nested type easier
        using Clock = std::chrono::steady_clock;
        using Second = std::chrono::duration<double, std::ratio<1>>;
        using miliSecond = std::chrono::duration<double, std::ratio<1, 1000>>;
        std::chrono::time_point<Clock> m_beg{Clock::now()};

    public:
        void reset()
        {
            m_beg = Clock::now();
        }

        double elapsed_miliseconds() const
        {
            auto end = Clock::now();
            // return std::chrono::duration_cast<Second>(end - m_beg).count();
            return std::chrono::duration_cast<miliSecond>(end - m_beg).count();
        }
    };
}

#endif //__UTILITY_CPP_H__