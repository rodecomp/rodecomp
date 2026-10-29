#include "Contents/ShowRankingScheduler.h"

#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "UI/Window/UIWindowMgr.h"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <crtdbg.h>
#include <string>

// GLOBAL: gl_24719
int g_schedulerSerial;

// GLOBAL: gl_24731
std::string g_namelessName;

// OFFSET: 0x00637550
// FUNCTION: fn_04735
CShowRankingScheduler::CShowRankingScheduler(const std::uint64_t& start, const std::uint64_t& end, int extra) {
  ++g_schedulerSerial;
  *reinterpret_cast<int*>(this->schedule + 4) = g_schedulerSerial;
  *reinterpret_cast<std::uint64_t*>(this->schedule + 12) = start;
  *reinterpret_cast<std::uint64_t*>(this->schedule + 20) = end;
  *reinterpret_cast<int*>(this->schedule + 28) = extra;
  *reinterpret_cast<std::uint16_t*>(this->schedule + 32) = 1;
  *reinterpret_cast<int*>(this->schedule + 36) = 0;
  *reinterpret_cast<int*>(this->schedule + 40) = 0;
  this->unknown_0088 = 0;
  this->unknown_008C = 0;
  std::memset(this->rankingData, 0, sizeof(this->rankingData));
  if (this->rankingList.capacity() < 16) {
    this->rankingList.reserve(16);
  }
}

// OFFSET: 0x00637C60
// FUNCTION: fn_04739
void CShowRankingScheduler::Process(const std::uint64_t& now) {
  if (*reinterpret_cast<const std::int16_t*>(this->rankingData) != 0) {
    const std::int64_t count =
        static_cast<std::int64_t>((static_cast<std::uint64_t>(this->unknown_008C) << 32) | this->unknown_0088);
    if (count > 0) {
      const std::uint64_t due =
          static_cast<std::uint64_t>(count) * 10000ull + *reinterpret_cast<const std::uint64_t*>(this->schedule + 12);
      if (static_cast<std::int64_t>(due) <= static_cast<std::int64_t>(now)) {
        this->Show();
      } else {
        bool complete = true;
        const auto* ids = reinterpret_cast<const std::uint32_t*>(this->rankingData + 4);
        for (int index = 0; index < 10; ++index) {
          const std::uint32_t id = ids[index];
          if (id == 0) {
            continue;
          }
          std::string name;
          for (std::uint32_t listed : this->rankingList) {
            if (listed == id) {
              name = g_namelessName;
              break;
            }
          }
          if (name.find(g_namelessName) == std::string::npos) {
            name = LookupPlayerName(id);
            if (name.empty()) {
              complete = false;
            }
          }
        }
        if (!complete) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3481)), 0xffff, 0, 0, 0);
          return;
        }
        this->Show();
      }
    }
  }
  this->schedule[32] = 0;
}

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
const void* CShowRankingScheduler::SetData(const void* data) {
  if (data) {
    std::memcpy(this->rankingData, data, sizeof(this->rankingData));
    return data;
  }

  std::memset(this->rankingData, 0, sizeof(this->rankingData));
  errno = EINVAL;
  _invalid_parameter_noinfo();

  return nullptr;
}
