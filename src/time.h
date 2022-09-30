#pragma once

#include <iostream>

class Time
{
public:
    int hour, min, sec;

    Time() : hour(0), min(0), sec(0) {}

    Time(int hour, int min, int sec) : hour(hour), min(min), sec(sec) {}

    bool operator<(const Time &that) const
    {
        return (hour * 3600 + min * 60 + sec) < (that.hour * 3600 + that.min * 60 + that.sec);
    }

    bool operator>(const Time &that) const
    {
        return (hour * 3600 + min * 60 + sec) > (that.hour * 3600 + that.min * 60 + that.sec);
    }

    bool operator==(const Time &that) const
    {
        return hour == that.hour && min == that.hour && sec == that.sec;
    }
};

std::ostream &operator<<(std::ostream &os, const Time &that)
{
    os << "Time: " << that.hour << ":" << that.min << ":" << that.sec << "\n";
    return os;
}