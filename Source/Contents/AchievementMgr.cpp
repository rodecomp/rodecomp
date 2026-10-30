#include "Contents/AchievementMgr.h"
#include "Client/Session.h"
#include "Script/NLuaState.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>
#include <windows.h>

// GLOBAL: gl_24856
CAchievementMgr* g_achievementMgr;

// OFFSET: 0x0066D3A0
// FUNCTION: fn_05311
bool CAchievementMgr::CreateInstance() {
  if (g_achievementMgr != nullptr) {
    return false;
  }

  g_achievementMgr = new (std::nothrow) CAchievementMgr();
  if (g_achievementMgr == nullptr) {
    return false;
  }

  if (!g_achievementMgr->unknown_0004) {
    delete g_achievementMgr;
    g_achievementMgr = nullptr;
    return false;
  }

  if (!g_achievementMgr->sub_66EAA0()) {
    if (g_achievementMgr != nullptr) {
      delete g_achievementMgr;
    }
    g_achievementMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x0066D500
// FUNCTION: fn_05312
void CAchievementMgr::DestroyInstance() {
  if (g_achievementMgr) {
    delete g_achievementMgr;
    g_achievementMgr = nullptr;
  }
}

// OFFSET: 0x0066D540
// FUNCTION: fn_05313
bool CAchievementMgr::GetAchievement(AchievementInfo& out, int id) {
  if (this->achievements.find(id) == this->achievements.end()) {
    return false;
  }

  out = this->achievements[id];
  return true;
}

// OFFSET: 0x0066D720
// FUNCTION: fn_05314
std::string CAchievementMgr::GetUnknown0068(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].unknown_0068;
  }
  return "";
}

// OFFSET: 0x0066D800
// FUNCTION: fn_05315
bool CAchievementMgr::GetGroupAchievements(std::map<int, AchievementInfo>& out, int group, int index) {
  out.clear();

  if (index == -1) {
    for (int i = 0; i < this->groupWidth; ++i) {
      int key = this->groupWidth * group + i;
      for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
        if (!out.insert(std::make_pair(*it, this->achievements[*it])).second) {
          return false;
        }
      }
    }
    return true;
  }

  int key = this->groupWidth * group + index;
  for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
    if (!out.insert(std::make_pair(*it, this->achievements[*it])).second) {
      return false;
    }
  }
  return true;
}

// OFFSET: 0x0066E250
// FUNCTION: fn_05316
int CAchievementMgr::GetUnknown0048(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].unknown_0048;
  }
  return -1;
}

// OFFSET: 0x0066E2A0
// FUNCTION: fn_05317
std::string CAchievementMgr::GetUnknown0018(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].unknown_0018;
  }
  return "";
}

// OFFSET: 0x0066E460
// FUNCTION: fn_05318
int CAchievementMgr::GetUnknown0050(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].unknown_0050;
  }
  return -1;
}

// OFFSET: 0x0066E4B0
// FUNCTION: fn_05319
int CAchievementMgr::GetAchievementCount(int group, int index) {
  int count = 0;
  if (group == 0) {
    count = static_cast<int>(this->achievements.size());
  } else if (index == -1) {
    for (int i = 0; i < this->groupWidth; ++i) {
      count += static_cast<int>(this->groups[this->groupWidth * group + i].size());
    }
  } else {
    count = static_cast<int>(this->groups[this->groupWidth * group + index].size());
  }
  return count;
}

namespace {

// INLINED
bool IsAchievementCompleted(int id) {
  const SessionRecord6144 record = g_session.FindRecord6144(id);
  return static_cast<unsigned char>(record.unknown_0000 >> 8) == 1;
}

}

// OFFSET: 0x0066E630
// FUNCTION: fn_05320
int CAchievementMgr::GetCompletedCount(int group, int index) {
  int count = 0;
  if (group == 0) {
    for (auto it = this->achievements.begin(); it != this->achievements.end(); ++it) {
      if (IsAchievementCompleted(it->first)) {
        ++count;
      }
    }
  } else if (index == -1) {
    for (int i = 0; i < this->groupWidth; ++i) {
      int key = this->groupWidth * group + i;
      for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
        if (IsAchievementCompleted(*it)) {
          ++count;
        }
      }
    }
  } else {
    int key = this->groupWidth * group + index;
    for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
      if (IsAchievementCompleted(*it)) {
        ++count;
      }
    }
  }
  return count;
}

// OFFSET: 0x0066EA00
// FUNCTION: fn_05321
bool CAchievementMgr::FindTab(int id, int& group, int& index) {
  for (auto it = this->groups.begin(); it != this->groups.end(); ++it) {
    for (std::vector<int>::iterator value = it->second.begin(); value != it->second.end(); ++value) {
      if (*value == id) {
        group = it->first / this->groupWidth;
        index = it->first % this->groupWidth;
        return true;
      }
    }
  }
  return false;
}

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

}

// OFFSET: 0x0066EAA0

// OFFSET: 0x0066ED90

// OFFSET: 0x0066F170

// OFFSET: 0x0066F450
// FUNCTION: fn_05325
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardBuff(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    return std::pair<bool, std::string>(false, FormatOwnedString("achievement info [%d] cannot be find", id));
  }

  found->second.unknown_0054 = value;
  return std::pair<bool, std::string>(true, "good");
}

// OFFSET: 0x0066F570
// FUNCTION: fn_05326
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardItem(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    return std::pair<bool, std::string>(false, FormatOwnedString("achievement info [%d] cannot be find", id));
  }

  found->second.unknown_004C = value;
  return std::pair<bool, std::string>(true, "good");
}

// OFFSET: 0x0066F690

// OFFSET: 0x0066F7B0
// FUNCTION: fn_05328
std::pair<bool, std::string> CAchievementMgr::InsertAchieveUIType(int id, int type, std::string text) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    return std::pair<bool, std::string>(false, FormatOwnedString("achievement info [%d] cannot be find", id));
  }

  found->second.unknown_0064 = type;
  found->second.unknown_0068 = text;
  return std::pair<bool, std::string>(true, "good");
}

// OFFSET: 0x0066F900
// FUNCTION: fn_05329
bool CAchievementMgr::HasReward(int id) {
  if (this->achievements.find(id) == this->achievements.end()) {
    return false;
  }

  const AchievementInfo& info = this->achievements[id];
  bool result = info.unknown_0054 > 0 || info.unknown_0050 > 0;
  if (info.unknown_004C > 0) {
    result = true;
  }
  return result;
}

// OFFSET: 0x0066F970

// OFFSET: 0x0066FED0

// OFFSET: 0x00670140

// OFFSET: 0x00670320

// OFFSET: 0x00670490

// OFFSET: 0x00670600

// OFFSET: 0x00670770

// OFFSET: 0x006709C0
