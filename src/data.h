#pragma once

#include <iostream>
#include "date.h"
#include "time.h"

class DataPoint
{
public:
    Date date;
    Time time;
    float open, high, low, close, volume;

    DataPoint() : open(0), high(0), low(0), close(0), volume(0)
    {
        date = Date();
        time = Time();
    }

    DataPoint(int year, int month, int day, int hour, int min, int sec, float open, float high, float low, float close, float volume) : open(open), high(high), low(low), close(close), volume(volume)
    {
        date = Date(year, month, day);
        time = Time(hour, min, sec);
    }

    DataPoint(Date d, Time t, float open, float high, float low, float close, float volume) : open(open), high(high), low(low), close(close), volume(volume), date(d), time(t) {}

    bool operator<(const DataPoint &that)
    {
        return (date < that.date)   ? true
               : (date > that.date) ? false
                                    : (time < that.time);
    }

    bool operator>(const DataPoint &that)
    {
        return (date > that.date)   ? true
               : (date < that.date) ? false
                                    : (time > that.time);
    }

    bool operator==(const DataPoint &that)
    {
        return date == that.date && time == that.time;
    }
};

std::ostream &operator<<(std::ostream &os, const DataPoint &that)
{
    os << that.date << that.time
       << "    Open: " << that.open << "\n"
       << "    High: " << that.high << "\n"
       << "    Low: " << that.low << "\n"
       << "    Close: " << that.close << "\n"
       << "    Volume: " << that.volume << "\n\n";
    return os;
}

class DataSet
{
public:
    float max_price, min_price;
    int size;
    std::vector<DataPoint> data;
};

std::ostream &operator<<(std::ostream &os, const DataSet &that)
{
    os << "\n";
    os << "Max Price: " << that.max_price << "\n";
    os << "Min Price: " << that.min_price << "\n";
    os << "\n";

    for (auto &d : that.data)
    {
        os << d << "\n";
    }

    return os;
}
