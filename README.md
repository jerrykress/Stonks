# Stonks

Stonks is a terminal based stock price monitor. It pulls real time data using AlphaVantage APIs and display history prices and trading volume.

![alt text](https://github.com/jerrykress/Stonks/blob/main/img/Screenshot.png?raw=true)

## Requirement

**OpenSSL**

```
# Download from
https://www.openssl.org/source/
# install via apt
sudo apt-get install openssl
# install via pacman
sudo pacman -S openssl
```

**Boost Asio**

```
# install Boost
https://www.boost.org/
# install via apt
sudo apt-get install boost
# install via pacman
sudo pacman -S boost
```

**Xcurse Library**

```
# Clone from my other repo
git clone https://github.com/jerrykress/Xcurse
```

## How to use

Refer to AlphaVantage [API documentation](https://www.alphavantage.co/documentation/) page for more information.

**Configuration**

```
<constant.h>

# max requests per minate, limit to not overload free api
const int requests_pm = 4;

# your api key here
const std::string API_KEY = "xxxxxx";
```

**Syntax:**

```
# general syntax
./Stonks <stoke name>
# example
./Stonks TSLA
```

**Control**

```
# show/hide price
key 'p'
# show/hide volume
key 'v'
# change to wider view, for example intraday to daily
key '-'
# change to narrower view, for example daily to intraday
key '+'
# quit program
key 'x'
```

## Known Issue

- Restricted by free API key. Causing program not being able to process more than 5 requests per minute. This includes changing views which results in making new API calls.
- Bad API calls such as bad stock symbols will result in program crash. This will be fixed in an update in Xcurse.
- Only tested on Linux. Not tested on Windows or MacOS.

## Coming Soon

- Ability to view more than one stock at once. (This will require a paid API key as it makes more requests).
