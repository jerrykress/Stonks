#pragma once

#include <functional>
#include <vector>
#include <numeric>
#include <string>
#include <iostream>
#include <regex>
#include <algorithm>
#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>
#include <sstream>
#include <stdexcept>
#include "../Xcurse/src/Window/DataWindow.h"

std::regex e_date("([0-9]{4}-[0-9]{2}-[0-9]{2})(?= [0-9]{2}:[0-9]{2}:[0-9]{2}\":)");
std::regex e_time("([0-9]{2}:[0-9]{2}:[0-9]{2})(?=\":)");
std::regex e_open("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"2)");
std::regex e_high("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"3)");
std::regex e_low("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"4)");
std::regex e_close("([0-9]+\\.[0-9]+)(?=\",[\\w\\-\\s]+\"5)");
std::regex e_volume("([0-9]+\\.*[0-9]*)(?=\"[\\w\\-\\s]+})");

const std::string DOMAIN = "www.alphavantage.co";

const std::string API_KEY = "02PX8JZ63RHDIAUN";