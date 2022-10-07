#include <string>
#include <unordered_map>
#include "constant.h"

/**
 * @brief Class for constructing API request strings
 *
 */
class API_Request
{
public:
    /**
     * @brief Construct a new api request object
     *
     */
    API_Request();

    /**
     * @brief Construct a new api request object
     *
     * @param argc argc from main
     * @param argv argv from main
     */
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

    /**
     * @brief Get the symbol of the current stock
     *
     * @return std::string
     */
    std::string get_name()
    {
        return m_params["symbol"];
    }

    /**
     * @brief Get the function currently used in the api
     *
     * @return std::string
     */
    std::string get_func()
    {
        return m_func_str[m_func_select];
    }

    /**
     * @brief Add a parameter to the API request
     *
     * @param param
     * @param value
     */
    void add_param(const std::string &param, const std::string &value)
    {
        m_params[param] = value;
    }

    /**
     * @brief Output this API to request to string format
     *
     * @return std::string
     */
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

    /**
     * @brief Switch to the previous function
     *
     */
    void prev_func()
    {
        m_func_select = (m_func_select + m_total_func - 1) % m_total_func;
        update_func();
    }

    /**
     * @brief Switch to the next function
     *
     */
    void next_func()
    {
        m_func_select = (m_func_select + 1) % m_total_func;
        update_func();
    }

private:
    /**
     * @brief Update the API content after a function selection change
     *
     */
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