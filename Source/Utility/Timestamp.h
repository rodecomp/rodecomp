#ifndef RAGNAROK_SOURCE_UTILITY_TIMESTAMP_H
#define RAGNAROK_SOURCE_UTILITY_TIMESTAMP_H

#include <ctime>
#include <windows.h>
#include <sqltypes.h>

class Timestamp {
public:
  Timestamp(__time32_t value);
  ~Timestamp();
  void Set(__time32_t value);
  TIMESTAMP_STRUCT ToSqlTimestamp();
  int IsBefore(__time32_t other) const;
  int IsBeforeNow() const;
  int Day() const;
  int Hour() const;
  int Minute() const;
  int Month() const;
  int Second() const;
  int Year() const;
  __time32_t SetDate(short year, short month, short day, short hour);

  /* 0000 */ __time32_t seconds;
};

static_assert(sizeof(Timestamp) == 4);
static_assert(sizeof(TIMESTAMP_STRUCT) == 16);

#endif
