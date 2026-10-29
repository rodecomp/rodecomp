#ifndef RAGNAROK_SOURCE_CONTENTS_SERVERTIMEMANAGER_H
#define RAGNAROK_SOURCE_CONTENTS_SERVERTIMEMANAGER_H

#include "Utility/Singleton.h"

#include <chrono>
#include <cstddef>

// VTABLE: vt_00057
class ServerTimeManager : public Singleton<ServerTimeManager> {
public:
  /* 0008 */ std::chrono::system_clock::time_point now;
  /* 0010 */ std::chrono::seconds offset;
  /* 0018 */ int year;
  /* 001C */ int month;
  /* 0020 */ int day;
  /* 0024 */ int hour;
  /* 0028 */ int minute;
  /* 002C */ int second;

  ~ServerTimeManager() override;

  void Initialize();
  int GetDay() const;
  int GetHour() const;
  int GetMinute() const;
  int GetMonth() const;
  int GetSecond() const;
  int GetYear() const;
  void Reset();
  void Update();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(ServerTimeManager, now) == 0x8);
static_assert(offsetof(ServerTimeManager, offset) == 0x10);
static_assert(offsetof(ServerTimeManager, second) == 0x2C);
static_assert(sizeof(ServerTimeManager) == 0x30);
#endif

#endif
