#include <string>
#include <unordered_map>
#include "constant.h"

class API_Request
{
public:
    API_Request();

    explicit API_Request(std::string t)
    {
        m_type = t;
    }

    void add_param(const std::string &param, const std::string &value)
    {
        m_params[param] = value;
    }

    std::string to_string()
    {
        std::string request = m_type + " /query?";

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

    std::string m_type; // GET
    std::unordered_map<std::string, std::string> m_params;
};