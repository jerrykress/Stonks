#pragma once

#include <iostream>

class Time
{
public:
    int hour, min, sec;

    /**
     * @brief Construct a new Time object
     *
     */
    Time() : hour(0), min(0), sec(0) {}

    /**
     * @brief Construct a new Time object
     *
     * @param hour
     * @param min
     * @param sec
     */
    Time(int hour, int min, int sec) : hour(hour), min(min), sec(sec) {}

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator<(const Time &that) const
    {
        return (hour * 3600 + min * 60 + sec) < (that.hour * 3600 + that.min * 60 + that.sec);
    }

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator>(const Time &that) const
    {
        return (hour * 3600 + min * 60 + sec) > (that.hour * 3600 + that.min * 60 + that.sec);
    }

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator==(const Time &that) const
    {
        return hour == that.hour && min == that.hour && sec == that.sec;
    }

    /**
     * @brief Output Time object as string
     *
     * @return std::string
     */
    std::string to_string()
    {
        return std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec);
    }
};

std::ostream &operator<<(std::ostream &os, const Time &that)
{
    os << "Time: " << that.hour << ":" << that.min << ":" << that.sec << "\n";
    return os;
}