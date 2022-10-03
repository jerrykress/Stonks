#pragma once

#include <vector>
#include "constant.h"
#include "data.h"

/**
 * @brief Save data into a vector with regex expression
 *
 * @tparam T Vector type
 * @param v Dest vector
 * @param s Source string
 * @param e Regex expression
 * @param f Adapter, convert string to dest vector type
 */
template <typename T>
void regex_save(std::vector<T> &v, const std::string &s, std::regex &e, const std::function<T(const std::string &)> &f)
{
    std::smatch sm;
    std::string::const_iterator searchStart(s.cbegin());

    while (std::regex_search(searchStart, s.cend(), sm, e))
    {
        v.emplace_back(f(sm[0]));
        searchStart = sm.suffix().first;
    }

    // reverse the array for chrono order
    v = {v.rbegin(), v.rend()};
}

/**
 * @brief Parse all data separatly into a DataSet object
 *
 * @param d Dest dataset
 * @param s Origin string
 */
void parse_dataset(DataSet &ds, const std::string &s)
{
    regex_save<Date>(ds.dates, s, e_date, [](const std::string &s) -> Date
                     { return Date(std::stoi(s.substr(0, 4)),
                                   std::stoi(s.substr(5, 2)),
                                   std::stoi(s.substr(8, 2))); });
    regex_save<Time>(ds.times, s, e_time, [](const std::string &s) -> Time
                     { return Time(std::stoi(s.substr(0, 2)),
                                   std::stoi(s.substr(3, 2)),
                                   std::stoi(s.substr(6, 2))); });
    regex_save<float>(ds.open, s, e_open, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(ds.high, s, e_high, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(ds.low, s, e_low, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(ds.close, s, e_close, [](const std::string &s) -> float
                      { return std::stof(s); });
    regex_save<float>(ds.volume, s, e_volume, [](const std::string &s) -> float
                      { return std::stof(s); });

    if (ds.dates.size() != ds.times.size() || ds.times.size() != ds.open.size() || ds.open.size() != ds.high.size() || ds.high.size() != ds.low.size() || ds.low.size() != ds.close.size() || ds.close.size() != ds.volume.size())
    {
        std::cout << "DataPoint parameter size mismatch!\n"
                  << "["
                  << "dates:" << ds.dates.size()
                  << "times:" << ds.times.size()
                  << "open:" << ds.open.size()
                  << "high:" << ds.high.size()
                  << "low:" << ds.low.size()
                  << "close:" << ds.close.size()
                  << "volume:" << ds.volume.size()
                  << "]\n";
    }
}

/**
 * @brief Build a DataSet into a vector of DataPoints
 *
 * @param ds Origin DataSet
 * @param v Dest vector
 */
void build_dataset(DataSet &ds, std::vector<DataPoint> &v)
{
    const int _size = ds.dates.size();

    for (int i = 0; i < _size; i++)
    {
        v.emplace_back(DataPoint(
            ds.dates[i], ds.times[i], ds.open[i], ds.high[i], ds.low[i], ds.close[i], ds.volume[i]));
    }
}