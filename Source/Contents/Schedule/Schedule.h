#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_SCHEDULE_H

#include <cstddef>
#include <cstdint>

#pragma pack(push, 4)
class CSchedule {
public:
  CSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType, int serial, int unknown);

  virtual void Process(const std::uint64_t& now);
  virtual ~CSchedule();
  virtual void Slot02() = 0;
  virtual void Slot03();

  /* 0004 */ std::uint8_t unknown_0004[0xC];
  /* 0010 */ std::uint64_t timestamp;
  /* 0018 */ std::int64_t interval;
  /* 0020 */ int scheduleType;
  /* 0024 */ bool active;
  /* 0025 */ std::uint8_t unknown_0025[0xB];
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSchedule) == 0x30);
static_assert(offsetof(CSchedule, timestamp) == 0x10);
static_assert(offsetof(CSchedule, interval) == 0x18);
static_assert(offsetof(CSchedule, scheduleType) == 0x20);
static_assert(offsetof(CSchedule, active) == 0x24);
#endif

#endif
