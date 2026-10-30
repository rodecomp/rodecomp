#include "Contents/Schedule/EmotionSchedule.h"

#include "Contents/CashEmotionMgr.h"
#include "Contents/ShowRankingScheduler.h"

class UICashEmotionListWnd {
public:
  void sub_7C7E50();
};

// GLOBAL: gl_24915
UICashEmotionListWnd* g_cashEmotionListWnd;

// OFFSET: 0x00636B60
// FUNCTION: fn_04715
CEmotionPurchaseExpireSchedule::CEmotionPurchaseExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 5) {
}

// OFFSET: 0x00636C90
// FUNCTION: fn_04717
void CEmotionPurchaseExpireSchedule::Process(const std::uint64_t& now) {
  CSchedule::Process(now);
  if (g_cashEmotionMgr->sub_59C5A0()) {
    if (g_cashEmotionListWnd) {
      g_cashEmotionListWnd->sub_7C7E50();
    }
  }
  int earliest = g_cashEmotionMgr->GetEarliestExpiry();
  if (earliest != 0x7FFFFFFF) {
    int current = g_cashEmotionMgr->sub_59FC50();
    if (current < earliest) {
      this->interval = (static_cast<std::int64_t>(earliest) * 10000000i64 - static_cast<std::int64_t>(current) * 10000000i64) / 10000i64;
      return;
    }
  }
  this->active = false;
}

// OFFSET: 0x00636D20
// FUNCTION: fn_04718
CEmotionPurchaseUseableTimeUISchedule::CEmotionPurchaseUseableTimeUISchedule(
  const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType
)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 7) {
}

// OFFSET: 0x00636E50
// FUNCTION: fn_04720
void CEmotionPurchaseUseableTimeUISchedule::Process(const std::uint64_t& now) {
  CSchedule::Process(now);
  if (!g_cashEmotionMgr->sub_59D2C0()) {
    this->active = false;
  }
}

// OFFSET: 0x00636E80
// FUNCTION: fn_04721
CEmotionSaleExpireSchedule::CEmotionSaleExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 6) {
}
