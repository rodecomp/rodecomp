#ifndef RAGNAROK_SOURCE_CONTENTS_SCHEDULE_EMOTIONSCHEDULE_H
#define RAGNAROK_SOURCE_CONTENTS_SCHEDULE_EMOTIONSCHEDULE_H

#include "Contents/Schedule/Schedule.h"

class CEmotionPurchaseExpireSchedule : public CSchedule {
public:
  CEmotionPurchaseExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  ~CEmotionPurchaseExpireSchedule() override = default;
  void Process(const std::uint64_t& now) override;
};

class CEmotionPurchaseUseableTimeUISchedule : public CSchedule {
public:
  CEmotionPurchaseUseableTimeUISchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  ~CEmotionPurchaseUseableTimeUISchedule() override = default;
  void Process(const std::uint64_t& now) override;
};

class CEmotionSaleExpireSchedule : public CSchedule {
public:
  CEmotionSaleExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType);
  ~CEmotionSaleExpireSchedule() override = default;
  void Process(const std::uint64_t& now) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CEmotionPurchaseExpireSchedule) == 0x30);
static_assert(sizeof(CEmotionPurchaseUseableTimeUISchedule) == 0x30);
static_assert(sizeof(CEmotionSaleExpireSchedule) == 0x30);
#endif

#endif
