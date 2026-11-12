#include "UI/Window/UIEnums.h"
#include "Contents/Events/ShowRankingScheduler.h"
#include "Client/Session/Session.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/TimeScheduler.h"

#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"
#include <cerrno>
#include <compare>
#include <cstdint>
#include <cstring>
#include <crtdbg.h>
#include <string>
#include <xtimec.h>

// GLOBAL: gl_24719
int g_schedulerSerial;

// GLOBAL: gl_24731
std::string g_namelessName;

// OFFSET: 0x00637550
// FUNCTION: fn_04735
CShowRankingScheduler::CShowRankingScheduler(const std::uint64_t& timestamp, const std::int64_t& interval, int scheduleType)
    : CSchedule(timestamp, interval, scheduleType, ++g_schedulerSerial, 0) {
  std::memset(this->rankingData, 0, sizeof(this->rankingData));
  if (this->rankingList.capacity() < 16) {
    this->rankingList.reserve(16);
  }
}

// OFFSET: 0x00637C60
// FUNCTION: fn_04739
void CShowRankingScheduler::Process(const std::uint64_t& now) {
  if (0 != *reinterpret_cast<const std::int16_t*>(this->rankingData)) {
    const std::int64_t count = this->delay;
    if (count != 0 && count > 0) {
      const std::int64_t due = static_cast<std::int64_t>(this->timestamp) + count * 10000;
      if ((due <=> static_cast<std::int64_t>(now)) <= 0) {
        this->Show();
      } else {
        bool complete = true;
        const auto* ids = reinterpret_cast<const std::uint32_t*>(this->rankingData + 4);
        for (int index = 0; index < 10; ++index) {
          std::string name("");
          if (ids[index] == 0) {
            continue;
          }
          for (std::size_t listed = 0; listed < this->rankingList.size(); ++listed) {
            if (this->rankingList[listed] == ids[index]) {
              name = g_namelessName;
              break;
            }
          }
          if (name.find(g_namelessName) == std::string::npos) {
            name = LookupPlayerName(ids[index]);
            if (name.empty()) {
              complete = false;
            }
          }
        }
        if (!complete) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STORE_ASSISTANT_TRADE_FORCED_REMOVAL_TITLE)), 0xffff, 0, 0, 0);
          return;
        }
        this->Show();
      }
    }
  }
  this->active = false;
}

__declspec(noinline) std::map<int, RankingBoardStyle> RankingBoardStyles();

// OFFSET: 0x006378D0
// FUNCTION: fn_04738
std::map<int, RankingBoardStyle> RankingBoardStyles() {
  std::map<int, RankingBoardStyle> styles;
  styles.insert({0, {"========== BLACKSMITH RANK ============", 0x9B9B9B, 0xCDCDCD}});
  styles.insert({1, {"=========== ALCHEMIST RANK =============", 0x9BCD9B, 0xCDFFCD}});
  styles.insert({2, {"============ TAEKWON RANK =============", 0xCD9B9B, 0xFFCDCD}});
  styles.insert({3, {"=========== KILLER RANK ==============", 0x9B9B9B, 0xCDCDCD}});
  return styles;
}

// OFFSET: 0x006380A0
// FUNCTION: fn_04740
void CShowRankingScheduler::SetData(const void* data) {
  memcpy_s(this->rankingData, sizeof(this->rankingData), data, sizeof(this->rankingData));
}

// OFFSET: 0x00638110
// FUNCTION: fn_04741
void CShowRankingScheduler::Show() {
  const int kind = *reinterpret_cast<const std::int16_t*>(this->rankingData + 2);
  // GLOBAL: gl_25055
  static const std::map<int, RankingBoardStyle> styles = RankingBoardStyles();

  const char* title;
  std::uint32_t color;
  std::uint32_t background;
  auto style = styles.find(kind);
  if (style != styles.end()) {
    title = style->second.title;
    color = style->second.color;
    background = style->second.background;
  } else {
    title = "===========================================";
    background = 0;
    color = 0;
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(title), color, 0, 0, 0);

  const auto* ids = reinterpret_cast<const std::uint32_t*>(this->rankingData + 4);
  const auto* points = reinterpret_cast<const int*>(this->rankingData + 44);
  for (int index = 0; index < 10; ++index) {
    std::string name;
    if (ids[index] != 0) {
      for (std::size_t listed = 0; listed < this->rankingList.size(); ++listed) {
        if (this->rankingList[listed] == ids[index]) {
          name = g_namelessName;
          break;
        }
      }
      if (name.find(g_namelessName) == std::string::npos) {
        name = LookupPlayerName(ids[index]);
      }
    }
    if (name.empty() || name.find(g_namelessName) != std::string::npos) {
      name = LookupMessage(MSI_NAMELESS_USER);
    }

    g_windowMgr.SendNotice(
      UIM_PUSHINTOCHATHISTORY,
      reinterpret_cast<std::intptr_t>(FormatOwnedString("[ %2d ] %s  :  %4d POINT", index + 1, name.c_str(), points[index]).c_str()), background, 0, 0, 0
    );
  }

  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("==========================================="), color, 0, 0, 0);
  g_windowMgr.SendNotice(
    UIM_PUSHINTOCHATHISTORY,
    reinterpret_cast<std::intptr_t>(FormatOwnedString("MY POINT    :  %4d POINT ", *reinterpret_cast<const int*>(this->rankingData + 84)).c_str()), background, 0, 0, 0
  );
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("==========================================="), color, 0, 0, 0);
}

// OFFSET: 0x00E493E0
// FUNCTION: fn_24235
void CSession::SetShowRankingSchedule(bool show, const void* data) {
  CSchedule* existing = g_timeScheduler->FindSchedule(this->m_showRankingSchedule);
  if (show == true) {
    if (existing == nullptr) {
      std::uint64_t now = _Xtime_get_ticks();
      CShowRankingScheduler* ranking = new CShowRankingScheduler(now, 1000, 1);
      if (ranking != nullptr) {
        ranking->delay = 3000;
        ranking->SetData(data);
        ranking->Process(now);
        this->m_showRankingSchedule = ranking->serial;
        g_timeScheduler->AddSchedule(ranking);
      }
    }
  } else if (existing != nullptr) {
    existing->active = false;
  }
}

// OFFSET: 0x00D18AB0
// FUNCTION: fn_21599
void CSession::SetTraitStat(int type, int value) {
  std::map<int, int>& traitStats = this->m_traits.stats;
  traitStats[type] = value;
}
