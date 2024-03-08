#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

class timer
{
public:
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count());
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

    std::chrono::time_point<std::chrono::system_clock> getStartTime()
    {
        return m_StartTime;
    }

    std::chrono::time_point<std::chrono::system_clock> getEndTime()
    {
        return m_EndTime;
    }


    string format_duration(double dms)
    {
        std::chrono::duration<double, std::milli> ms { dms };

        auto secs = duration_cast<std::chrono::seconds>(ms);
        auto mins = duration_cast<std::chrono::minutes>(secs);
        secs -= duration_cast<std::chrono::seconds>(mins);
        auto hour = duration_cast<std::chrono::hours>(mins);
        mins -= duration_cast<std::chrono::minutes>(hour);

        std::stringstream ss;
        ss << hour.count() << "h " << mins.count() << "mn " << secs.count() << "s";
        return ss.str();
    }

    void displayTime()
    {
        string www = format_duration(elapsedMilliseconds());
        std::clog << "\nExecution Time: " << www.c_str();
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool m_bRunning = false;
};

#endif
