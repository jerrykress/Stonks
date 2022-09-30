#pragma once

#include <iostream>
#include "date.h"
#include "time.h"

/**
 * @brief Single data point object
 *
 */
class DataPoint
{
public:
    Date date;
    Time time;
    float open, high, low, close, volume;

    /**
     * @brief Construct a new Data Point object
     *
     */
    DataPoint() : open(0), high(0), low(0), close(0), volume(0)
    {
        date = Date();
        time = Time();
    }

    /**
     * @brief Construct a new Data Point object
     *
     * @param year
     * @param month
     * @param day
     * @param hour
     * @param min
     * @param sec
     * @param open
     * @param high
     * @param low
     * @param close
     * @param volume
     */
    DataPoint(int year, int month, int day, int hour, int min, int sec, float open, float high, float low, float close, float volume) : open(open), high(high), low(low), close(close), volume(volume)
    {
        date = Date(year, month, day);
        time = Time(hour, min, sec);
    }

    /**
     * @brief Construct a new Data Point object
     *
     * @param d Date
     * @param t Time
     * @param open
     * @param high
     * @param low
     * @param close
     * @param volume
     */
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

/**
 * @brief Collection of data point objects
 *
 */
class DataSet
{
public:
    std::vector<Date> dates;
    std::vector<Time> times;
    std::vector<float> open;
    std::vector<float> high;
    std::vector<float> low;
    std::vector<float> close;
    std::vector<float> volume;
};