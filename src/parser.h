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
}

/**
 * @brief Parse all data separatly into a DataSet object
 *
 * @param d Dest dataset
 * @param s Origin string
 */
void parse_dataset(DataSet &d, const std::string &s)
{
    std::vector<Date> dates;
    std::vector<Time> times;
    std::vector<float> open;
    std::vector<float> high;
    std::vector<float> low;
    std::vector<float> close;
    std::vector<float> volume;

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

    if (dates.size() != times.size() || times.size() != open.size() || open.size() != high.size() || high.size() != low.size() || low.size() != close.size() || close.size() != volume.size())
    {
        std::cout << "DataPoint parameter size mismatch!\n"
                  << "["
                  << "dates:" << dates.size()
                  << "times:" << times.size()
                  << "open:" << open.size()
                  << "high:" << high.size()
                  << "low:" << low.size()
                  << "close:" << close.size()
                  << "volume:" << volume.size()
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