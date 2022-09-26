#include <functional>
#include <vector>
#include <numeric>
#include <string>
#include <iostream>
#include <regex>

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

class Date
{
public:
    int year, month, day;

    Date() : year(1970), month(12), day(31) {}

    Date(int year, int month, int day) : year(year), month(month), day(day) {}

    bool operator<(const Date &that) const
    {
        return abs_val() < that.abs_val();
    }

    bool operator>(const Date &that) const
    {
        return abs_val() > that.abs_val();
    }

    bool operator==(const Date &that) const
    {
        return year == that.year && month == that.month && day == that.day;
    }

private:
    int abs_val() const
    {
        int y = year;
        std::vector<int> months = {31, leapYear(y) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        int total = 0;
        while (--y > 1970)
        {
            total += leapYear(y) ? 366 : 365;
        }
        // total days of this year
        total += std::accumulate(months.begin(), months.begin() + month - 1, 0, [](int a, int b)
                                 { return a + b; }) +
                 day;
        return total;
    }

    std::function<bool(int)> leapYear = [](int y)
    { return (y % 4 == 0) && (y % 100 != 0 || y % 400 == 0); };
};

std::ostream &operator<<(std::ostream &os, const Date &that)
{
    os << "Date: " << that.year << ":" << that.month << ":" << that.day << "\n";
    return os;
}

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

std::vector<std::string> &regex_save(const std::string &s, std::regex &e)
{
    std::vector<std::string> matches;
    std::smatch res;
    std::string::const_iterator searchStart(s.cbegin());

    while (std::regex_search(searchStart, s.cend(), res, e))
    {
        matches.emplace_back(res[0]);
        std::cout << res[0] << "\n";
        searchStart = res.suffix().first;
    }
    return matches;
}

void parse_data(std::string &s)
{
    std::vector<Date> dates;
    std::vector<Time> times;

    std::regex e_date_time("([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2})");
    std::regex e_open("([0-9]+.[0-9]+)(?=\",\\n\\s+\"2)");
    std::regex e_high("([0-9]+.[0-9]+)(?=\",\\n\\s+\"3)");
    std::regex e_low("([0-9]+.[0-9]+)(?=\",\\n\\s+\"4)");
    std::regex e_close("([0-9]+.[0-9]+)(?=\",\\n\\s+\"5)");
    std::regex e_volume("([0-9]+.[0-9]*)(?=\"\\n\\s+})");
}