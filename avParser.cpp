#include <functional>
#include <vector>
#include <numeric>
#include <string>

struct Time
{
    int hour, min, sec;

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

struct Date
{
    int year, month, day;

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

struct DataPoint
{
    Date date;
    Time time;
    float open, high, low, close, volume;

    bool operator<(const DataPoint &that)
    {
    }

    bool operator>(const DataPoint &that)
    {
    }

    bool operator==(const DataPoint &that)
    {
        return date == that.date && time == that.time;
    }
};

std::vector<DataPoint> parse_data(std::string &s)
{
}
