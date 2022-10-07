#include <string>
#include <unordered_map>
#include "constant.h"

class API_Request
{
public:
    API_Request();

    explicit API_Request(int argc, char *argv[])
    {
        if (argc < 2)
        {
            throw std::invalid_argument("Too few arguments, please provide stock name\n");
        }
        else
        {
            // add stock name param
            add_param("symbol", argv[1]);
            // add fixed apikey param
            add_param("apikey", API_KEY);
            // add default func selection
            update_func();
        }
    }

    std::string get_name()
    {
        return m_params["symbol"];
    }

    std::string get_func()
    {
        return m_func_str[m_func_select];
    }

    void add_param(const std::string &param, const std::string &value)
    {
        m_params[param] = value;
    }

    std::string to_string()
    {
        std::string request = "GET /query?";

        for (auto &pair : m_params)
        {
            request.append(pair.first + "=" + pair.second + "&");
        }

        request.pop_back();
        request += " HTTP/1.1\r\n";
        request += "Host: ";
        request += DOMAIN;
        request += "\r\nConnection: close\r\n\r\n";

        return request;
    }

    void prev_func()
    {
        m_func_select = (m_func_select + m_total_func - 1) % m_total_func;
        update_func();
    }

    void next_func()
    {
        m_func_select = (m_func_select + 1) % m_total_func;
        update_func();
    }

private:
    void update_func()
    {
        m_params["function"] = m_funcs[m_func_select];

        if (m_func_select == 0)
        {
            m_params["interval"] = "5min";
        }
        else
        {
            m_params.erase("interval");
        }
    }

    const std::vector<std::string> m_funcs = {"TIME_SERIES_INTRADAY", "TIME_SERIES_DAILY", "TIME_SERIES_WEEKLY", "TIME_SERIES_MONTHLY"};
    const std::vector<std::string> m_func_str = {"Intra-day", "Daily", "Weekly", "Monthly"};
    std::unordered_map<std::string, std::string> m_params;
    const int m_total_func = 4;
    int m_func_select = 0;
};