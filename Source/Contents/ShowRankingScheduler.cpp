#include "Contents/ShowRankingScheduler.h"

#include <cerrno>
#include <cstring>
#include <crtdbg.h>

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
