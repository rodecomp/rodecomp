#include "Contents/Schedule/EmotionSchedule.h"

#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Events/ShowRankingScheduler.h"
#include "UI/CashEmotion/UICashEmotionListWnd.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00636B60
// FUNCTION: fn_04715
CEmotionPurchaseExpireSchedule::CEmotionPurchaseExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 5) {
}

// OFFSET: 0x00636C90
// FUNCTION: fn_04717
void CEmotionPurchaseExpireSchedule::Process(const std::uint64_t& now) {
  CSchedule::Process(now);
  if (g_cashEmotionMgr->RemoveExpiredPacks()) {
    if (static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)) {
      static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)->RefreshPackLists();
    }
  }
  int earliest = g_cashEmotionMgr->GetEarliestExpiry();
  if (earliest != 0x7FFFFFFF) {
    int current = g_cashEmotionMgr->GetServerTime();
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
  if (!g_cashEmotionMgr->UpdatePurchasedPackExpiry()) {
    this->active = false;
  }
}

// OFFSET: 0x00636E80
// FUNCTION: fn_04721
CEmotionSaleExpireSchedule::CEmotionSaleExpireSchedule(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 6) {
}

// OFFSET: 0x00636FB0
// FUNCTION: fn_04723
void CEmotionSaleExpireSchedule::Process(const std::uint64_t& now) {
  CSchedule::Process(now);
  CashEmotionPackRefresh refresh = g_cashEmotionMgr->RefreshUnownedPacks();
  if (refresh.changed && static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)) {
    static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)->RefreshPackLists();
    static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)->RefreshSelection();
  }
  if (refresh.nextTime != 0x7FFFFFFF) {
    int current = g_cashEmotionMgr->GetServerTime();
    if (current < refresh.nextTime) {
      this->interval = (static_cast<std::int64_t>(refresh.nextTime) * 10000000i64 - static_cast<std::int64_t>(current) * 10000000i64) / 10000i64;
      return;
    }
  }
  this->active = false;
}
