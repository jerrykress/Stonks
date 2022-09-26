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

template <typename T>
void regex_save(std::vector<T> &v, const std::string &s, std::regex &e, const std::function<T(const std::string &)> &f)
{
    std::smatch res;
    std::string::const_iterator searchStart(s.cbegin());

    while (std::regex_search(searchStart, s.cend(), res, e))
    {
        v.emplace_back(f(res[0]));
        searchStart = res.suffix().first;
    }
}

void parse_data(const std::string &s)
{
    std::vector<Date> dates;
    std::vector<Time> times;
    std::vector<float> open;
    std::vector<float> high;
    std::vector<float> low;
    std::vector<float> close;
    std::vector<float> volume;

    std::regex e_date("([0-9]{4}-[0-9]{2}-[0-9]{2})(?= [0-9]{2}:[0-9]{2}:[0-9]{2}\":)");
    std::regex e_time("([0-9]{2}:[0-9]{2}:[0-9]{2})(?=\":)");
    std::regex e_open("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"2)");
    std::regex e_high("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"3)");
    std::regex e_low("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"4)");
    std::regex e_close("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"5)");
    std::regex e_volume("([0-9]+\\.*[0-9]*)(?=\"[\\w\\-\\s]+})");

    regex_save<Date>(dates, s, e_date, [](const std::string &s) -> Date
                     { return Date(std::stoi(s.substr(0, 4)),
                                   std::stoi(s.substr(5, 2)),
                                   std::stoi(s.substr(8, 2))); });
    regex_save<Time>(times, s, e_time, [](const std::string &s) -> Time
                     { return Time(std::stoi(s.substr(0, 2)),
                                   std::stoi(s.substr(3, 2)),
                                   std::stoi(s.substr(6, 2))); });
    regex_save<float>(open, s, e_open, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(high, s, e_high, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(low, s, e_low, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(close, s, e_close, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(volume, s, e_volume, [](const std::string &s) -> float
                      { return std::stof(s); });

    std::cout << dates.size() << times.size() << open.size() << high.size() << low.size() << close.size() << volume.size() << std::endl;
}