#include "Contents/Schedule/Schedule.h"

// GLOBAL: gl_24850
std::uniform_int_distribution<int> g_scheduleIntervalDistribution;

// GLOBAL: gl_24851
std::mt19937 g_scheduleRandomEngine;

// OFFSET: 0x00BA8930
// FUNCTION: fn_19725
CSchedule::CSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType, int serial, int userValue) {
  this->serial = serial;
  this->timestamp = timestamp;
  this->interval = interval;
  this->scheduleType = scheduleType;
  this->userValue = userValue;
  this->active = true;
  this->pending = 0;
  this->processCount = 0;
}

// OFFSET: 0x00BA8980
// FUNCTION: fn_19726
CSchedulerIDAlocator::~CSchedulerIDAlocator() = default;

// OFFSET: 0x00BA8990
// FUNCTION: fn_19727
bool CSchedule::CheckTime(const std::uint64_t& now) {
  if (this->interval == 0 || this->interval <= 0) {
    return false;
  }
  const std::int64_t target = this->timestamp + static_cast<std::uint64_t>(this->interval) * 10000;
  return (static_cast<std::int64_t>(now) <=> target) > 0;
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
