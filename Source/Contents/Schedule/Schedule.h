#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SCHEDULE_H

#include <cstddef>
#include <cstdint>
#include <random>

extern std::uniform_int_distribution<int> g_scheduleIntervalDistribution;
extern std::mt19937 g_scheduleRandomEngine;

#pragma pack(push, 4)
class CSchedulerIDAlocator {
public:
  virtual ~CSchedulerIDAlocator();

  /* 0004 */ int unknown_0004;
};

class CSchedule {
public:
  CSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType, int serial, int unknown);

  virtual void Process(const std::uint64_t& now);
  virtual ~CSchedule();

  bool CheckTime(const std::uint64_t& now);

  /* 0004 */ std::uint8_t unknown_0004[0x4];
  /* 0008 */ int serial;
  /* 000C */ std::uint8_t unknown_000C[0x4];
  /* 0010 */ std::uint64_t timestamp;
  /* 0018 */ std::int64_t interval;
  /* 0020 */ int scheduleType;
  /* 0024 */ bool active;
  /* 0025 */ std::uint8_t unknown_0025;
  /* 0026 */ std::uint8_t unknown_0026[2];
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSchedulerIDAlocator) == 0x8);
static_assert(sizeof(CSchedule) == 0x30);
static_assert(offsetof(CSchedule, serial) == 0x8);
static_assert(offsetof(CSchedule, timestamp) == 0x10);
static_assert(offsetof(CSchedule, interval) == 0x18);
static_assert(offsetof(CSchedule, scheduleType) == 0x20);
static_assert(offsetof(CSchedule, active) == 0x24);
static_assert(offsetof(CSchedule, unknown_002C) == 0x2C);
#endif

#endif
