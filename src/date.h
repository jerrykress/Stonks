#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <numeric>

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

    std::string to_string()
    {
        return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
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