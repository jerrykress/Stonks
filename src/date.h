#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <numeric>

/**
 * @brief Class that stores a date object parsed from API response
 *
 */
class Date
{
public:
    int year, month, day;

    /**
     * @brief Construct a new Date object
     *
     */
    Date() : year(1970), month(12), day(31) {}

    /**
     * @brief Construct a new Date object
     *
     * @param year
     * @param month
     * @param day
     */
    Date(int year, int month, int day) : year(year), month(month), day(day) {}

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator<(const Date &that) const
    {
        return abs_val() < that.abs_val();
    }

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator>(const Date &that) const
    {
        return abs_val() > that.abs_val();
    }

    /**
     * @brief Operator overload
     *
     * @param that
     * @return true
     * @return false
     */
    bool operator==(const Date &that) const
    {
        return year == that.year && month == that.month && day == that.day;
    }

    /**
     * @brief Output Date object as string
     *
     * @return std::string
     */
    std::string to_string()
    {
        return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
    }

private:
    /**
     * @brief Represent Date object in nth day of the year
     *
     * @return int
     */
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