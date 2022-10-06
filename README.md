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

**Boost Library**

```
# install Boost
https://www.boost.org/
# install via apt
sudo apt-get install boost
# install via pacman
sudo pacman -S boost
```

**Xcurse**

```
# Clone from my other repo
https://github.com/jerrykress/Xcurse
```

## How to use

Refer to AlphaVantage [API documentation](https://www.alphavantage.co/documentation/) page for more information.

**Syntax:**

```
# general syntax
./Stonks <name> <function> <interval>
# example
./Stonks TSLA TIME_SERIES_INTRADAY 1min
```
