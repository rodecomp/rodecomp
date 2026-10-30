#include "Contents/Schedule/EmotionSchedule.h"

#include "Contents/ShowRankingScheduler.h"

// OFFSET: 0x00636B60
// FUNCTION: fn_04715
CEmotionPurchaseExpireSchedule::CEmotionPurchaseExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 5) {
}

// OFFSET: 0x00636D20
// FUNCTION: fn_04718
CEmotionPurchaseUseableTimeUISchedule::CEmotionPurchaseUseableTimeUISchedule(
  const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType
)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 7) {
}

// OFFSET: 0x00636E80
// FUNCTION: fn_04721
CEmotionSaleExpireSchedule::CEmotionSaleExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 6) {
}
