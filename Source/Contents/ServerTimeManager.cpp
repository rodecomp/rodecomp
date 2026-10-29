#include "Contents/ServerTimeManager.h"

#include <ctime>

// OFFSET: 0x0053E4F0
// FUNCTION: fn_01685
void ServerTimeManager::Initialize() {
}

// OFFSET: 0x0053E500
// FUNCTION: fn_01686
ServerTimeManager::~ServerTimeManager() {
}

// OFFSET: 0x0053E6A0
// FUNCTION: fn_01689
int ServerTimeManager::GetDay() const {
  return this->day;
}

// OFFSET: 0x0053E6B0
// FUNCTION: fn_01690
int ServerTimeManager::GetHour() const {
  return this->hour;
}

// OFFSET: 0x0053E6C0
// FUNCTION: fn_01691
int ServerTimeManager::GetMinute() const {
  return this->minute;
}

// OFFSET: 0x0053E6D0
// FUNCTION: fn_01692
int ServerTimeManager::GetMonth() const {
  return this->month;
}

// OFFSET: 0x0053E6E0
// FUNCTION: fn_01693
int ServerTimeManager::GetSecond() const {
  return this->second;
}

// OFFSET: 0x0053E840
// FUNCTION: fn_01694
int ServerTimeManager::GetYear() const {
  return this->year;
}

// OFFSET: 0x0053E910
// FUNCTION: fn_01695
void ServerTimeManager::Reset() {
  this->offset = std::chrono::seconds::zero();
  this->Update();
}

// OFFSET: 0x0053E930
// FUNCTION: fn_01696
void ServerTimeManager::Update() {
  this->now = std::chrono::system_clock::now();
  this->now += this->offset;

  const std::time_t seconds = std::chrono::system_clock::to_time_t(this->now);
  std::tm local;
  if (localtime_s(&local, &seconds) == 0) {
    this->year = local.tm_year + 1900;
    this->month = local.tm_mon + 1;
    this->day = local.tm_mday;
    this->hour = local.tm_hour;
    this->minute = local.tm_min;
    this->second = local.tm_sec;
  }
}
