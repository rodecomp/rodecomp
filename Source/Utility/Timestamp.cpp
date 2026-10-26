#include "Utility/Timestamp.h"

// OFFSET: 0x00AE81A0
// FUNCTION: fn_18084
Timestamp::~Timestamp() {
}

// OFFSET: 0x00AE81B0
// FUNCTION: fn_18085
TIMESTAMP_STRUCT Timestamp::ToSqlTimestamp() {
  TIMESTAMP_STRUCT result{};
  if (this->seconds < 0) {
    this->seconds = 0;
  }

  const std::tm* local = _localtime32(&this->seconds);
  result.fraction = 0;
  result.year = static_cast<SQLSMALLINT>(local->tm_year + 1900);
  result.month = static_cast<SQLUSMALLINT>(local->tm_mon + 1);
  result.day = static_cast<SQLUSMALLINT>(local->tm_mday);
  result.hour = static_cast<SQLUSMALLINT>(local->tm_hour);
  result.minute = static_cast<SQLUSMALLINT>(local->tm_min);
  result.second = static_cast<SQLUSMALLINT>(local->tm_sec);
  return result;
}

// OFFSET: 0x00AE8280
// FUNCTION: fn_18086
int Timestamp::IsBefore(__time32_t other) const {
  return _difftime32(this->seconds, other) < 0.0;
}

// OFFSET: 0x00AE82B0
// FUNCTION: fn_18087
int Timestamp::IsBeforeNow() const {
  __time32_t now;
  _time32(&now);
  return _difftime32(this->seconds, now) < 0.0;
}

// OFFSET: 0x00AE82F0
// FUNCTION: fn_18088
int Timestamp::Day() const {
  return _localtime32(&this->seconds)->tm_mday;
}

// OFFSET: 0x00AE8300
// FUNCTION: fn_18089
int Timestamp::Hour() const {
  return _localtime32(&this->seconds)->tm_hour;
}

// OFFSET: 0x00AE8310
// FUNCTION: fn_18090
int Timestamp::Minute() const {
  return _localtime32(&this->seconds)->tm_min;
}

// OFFSET: 0x00AE8320
// FUNCTION: fn_18091
int Timestamp::Month() const {
  return _localtime32(&this->seconds)->tm_mon + 1;
}

// OFFSET: 0x00AE8350
// FUNCTION: fn_18092
int Timestamp::Second() const {
  return _localtime32(&this->seconds)->tm_sec;
}

// OFFSET: 0x00AE83B0
// FUNCTION: fn_18093
int Timestamp::Year() const {
  return _localtime32(&this->seconds)->tm_year + 1900;
}

// OFFSET: 0x00AE83D0
// FUNCTION: fn_18094
__time32_t Timestamp::SetDate(short year, short month, short day, short hour) {
  std::tm local{};
  local.tm_hour = hour;
  local.tm_mday = day;
  local.tm_mon = month - 1;
  local.tm_year = year - 1900;
  this->seconds = _mktime32(&local);
  return this->seconds;
}
