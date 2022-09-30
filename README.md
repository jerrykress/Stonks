# Stonks

Stonks is a terminal based stock price monitor.

## Requirements

- **OpenSSL**
  > **Download:** https://www.openssl.org/source/ > **Arch:** sudo pacman -S openssl
- **Xcurse**
  > **Download:** https://github.com/jerrykress/Xcurse > **Clone:** git clone git@github.com:jerrykress/Xcurse.git

## Usage

**Syntax:** \<name> \<function> \<interval>
|Function |Interval  
|----------------|-------------------------------|
|TIME_SERIES_INTRADAY|1min, 5min |

Example:

> ./Stonks TSLA TIME_SERIES_INTRADAY 1min
