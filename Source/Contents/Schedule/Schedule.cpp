#include "Contents/Schedule/Schedule.h"

// GLOBAL: gl_24850
std::uniform_int_distribution<int> g_scheduleIntervalDistribution;

// GLOBAL: gl_24851
std::mt19937 g_scheduleRandomEngine;

// OFFSET: 0x00BA8930
// FUNCTION: fn_19725
CSchedule::CSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType, int serial, int unknown) {
  this->serial = serial;
  this->timestamp = timestamp;
  this->interval = interval;
  this->scheduleType = scheduleType;
  this->unknown_002C = unknown;
  this->active = true;
  this->unknown_0025 = 0;
  this->unknown_0028 = 0;
}

// OFFSET: 0x00BA8980
// FUNCTION: fn_19726
CSchedulerIDAlocator::~CSchedulerIDAlocator() = default;

// OFFSET: 0x00BA8990
// FUNCTION: fn_19727
bool CSchedule::CheckTime(const std::uint64_t& now) {
  const std::uint32_t intervalLow = static_cast<std::uint32_t>(this->interval);
  const std::int32_t intervalHigh = static_cast<std::int32_t>(this->interval >> 32);

  if ((intervalLow == 0 && intervalHigh == 0) || intervalHigh < 0 || (intervalHigh == 0 && intervalLow == 0)) {
    return false;
  }

  const std::uint32_t nowLow = *reinterpret_cast<const std::uint32_t*>(&now);
  const std::int32_t nowHigh = *(reinterpret_cast<const std::int32_t*>(&now) + 1);

  const std::uint64_t target = this->timestamp + static_cast<std::uint64_t>(this->interval) * 10000;
  const std::uint32_t targetLow = static_cast<std::uint32_t>(target);
  const std::int32_t targetHigh = static_cast<std::int32_t>(target >> 32);

  if (nowLow == targetLow && nowHigh == targetHigh) {
    signed char c = 0;
    return c > 0;
  }
  if (nowHigh > targetHigh) {
    signed char c = 1;
    return c > 0;
  }
  if (nowHigh < targetHigh) {
    signed char c = -1;
    return c > 0;
  }
  if (nowLow >= targetLow) {
    signed char c = 1;
    return c > 0;
  }
  signed char c = -1;
  return c > 0;
}

// OFFSET: 0x00BA8A20
// FUNCTION: fn_19728
void CSchedule::Process(const std::uint64_t& now) {
  if (this->scheduleType == 0) {
    this->active = false;
    return;
  }

  if (this->scheduleType == 2) {
    this->interval = static_cast<std::int64_t>(g_scheduleIntervalDistribution(g_scheduleRandomEngine)) * 1000;
  }

  this->timestamp = now;
}
