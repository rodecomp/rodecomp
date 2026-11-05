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

  if (index != -1) {
    int key = this->groupWidth * group + index;
    for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
      if (!out.insert(std::pair<int, AchievementInfo>(*it, this->achievements[*it])).second) {
        return false;
      }
    }
    return true;
  }

  for (int i = 0; i < this->groupWidth; ++i) {
    int key = this->groupWidth * group + i;
    for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end(); ++it) {
      if (!out.insert(std::pair<int, AchievementInfo>(*it, this->achievements[*it])).second) {
        return false;
      }
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
        const int key = it->first;
        group = key / this->groupWidth;
        index = key % this->groupWidth;
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
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
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
// FUNCTION: fn_05322
bool CAchievementMgr::sub_66EAA0() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "InsertAchieveInfo", CAchievementMgr::L_InsertAchieveInfo);
    lua_register(lua.GetState(), "InsertAchieveResource", CAchievementMgr::L_InsertAchieveResource);
    lua_register(lua.GetState(), "InsertAchieveRewardItem", CAchievementMgr::L_InsertAchieveRewardItem);
    lua_register(lua.GetState(), "InsertAchieveRewardTitle", CAchievementMgr::L_InsertAchieveRewardTitle);
    lua_register(lua.GetState(), "InsertAchieveRewardBuff", CAchievementMgr::L_InsertAchieveRewardBuff);
    lua_register(lua.GetState(), "InsertAchieveUIType", CAchievementMgr::L_InsertAchieveUIType);
    lua_register(lua.GetState(), "SetAchieveIDByTab", CAchievementMgr::L_SetAchieveIDByTab);
    const char source[] = {'\xCA', '\x74', '\x5C', '\xD5', '\x26', '\x96', '\x53', '\x08', '\xDA', '\x65', '\x46', '\xC4', '\x35', '\x9E',
                           '\x62', '\x2C', '\xD7', '\x79', '\x70', '\xCD', '\x2A', '\x88', '\x7B', '\x67', '\xD5', '\x78', '\x4D', '\xA1'};
    thread_local EncryptedName<sizeof(source), 0x490FFB43A12F0DB9ULL> path(source);
    if (luaL_loadfile(lua.GetState(), path.Text()) != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CAchievementMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CAchievementMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "CAchievementMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        MessageBoxA(nullptr, FormatString("error : %s", lua_tostring(lua.GetState(), -1)), "CAchievementMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x0066ED90
// FUNCTION: fn_05323
std::pair<bool, std::string> CAchievementMgr::InsertAchieveInfo(int id, std::string title, std::string unknown0018, std::string unknown0030, int unknown0048) {
  if (this->achievements.find(id) != this->achievements.end()) {
    const std::string message = FormatOwnedString("duplicate achievement info [%d]", id);
    return std::make_pair(false, message);
  }

  if (this->achievements.insert(std::pair<int, AchievementInfo>(id, AchievementInfo(title, unknown0018, unknown0030, unknown0048))).second) {
    return std::make_pair(true, "good");
  }

  const std::string message = FormatOwnedString("achievement info [%d] insert fail", id);
  return std::make_pair(false, message);
}

// OFFSET: 0x0066F170
// FUNCTION: fn_05324
std::pair<bool, std::string> CAchievementMgr::InsertAchieveResource(int id, std::string text, int unknown0018, int unknown001C) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  std::vector<AchievementReward>& rewards = found->second.rewards;
  if (std::find_if(rewards.begin(), rewards.end(), [text](const AchievementReward& reward) { return reward.text == text; }) == rewards.end()) {
    const AchievementReward reward(text, unknown0018, unknown001C);
    rewards.push_back(reward);
    return std::make_pair(true, "good");
  }

  const std::string message = FormatOwnedString("achievement info [%d] - resource info already exist", id);
  return std::make_pair(false, message);
}

// OFFSET: 0x0066F450
// FUNCTION: fn_05325
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardBuff(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  found->second.unknown_0054 = value;
  return std::make_pair(true, "good");
}

// OFFSET: 0x0066F570
// FUNCTION: fn_05326
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardItem(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  found->second.unknown_004C = value;
  return std::make_pair(true, "good");
}

// OFFSET: 0x0066F690
// FUNCTION: fn_05327
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardTitle(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  found->second.unknown_0050 = value;
  return std::make_pair(true, "good");
}

// OFFSET: 0x0066F7B0
// FUNCTION: fn_05328
std::pair<bool, std::string> CAchievementMgr::InsertAchieveUIType(int id, int type, std::string text) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  found->second.unknown_0064 = type;
  found->second.unknown_0068 = text;
  return std::make_pair(true, "good");
}

// OFFSET: 0x0066F900
// FUNCTION: fn_05329
bool CAchievementMgr::HasReward(int id) {
  if (this->achievements.find(id) == this->achievements.end()) {
    return false;
  }

  const AchievementInfo& info = this->achievements[id];
  bool result = info.unknown_0054 > 0;
  result |= info.unknown_0050 > 0;
  if (info.unknown_004C > 0) {
    result = true;
  }
  return result;
}

// OFFSET: 0x0066F970
// FUNCTION: fn_05330
std::pair<bool, std::string> CAchievementMgr::SetAchieveTab(int id, int group, int index) {
  const int key = this->groupWidth * group + index;
  if (std::find(this->groups[key].begin(), this->groups[key].end(), id) == this->groups[key].end()) {
    this->groups[key].push_back(id);
    return std::make_pair(true, "good");
  }

  const std::string message = FormatOwnedString("duplicate achievement tab number info [%d]", id);
  return std::make_pair(false, message);
}

// OFFSET: 0x0066FED0
// FUNCTION: fn_05337
int CAchievementMgr::L_InsertAchieveInfo(lua_State* state) {
  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1nd is must number");
  } else if (!lua_isstring(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must string");
  } else if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
  } else if (!lua_isstring(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must string");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 5th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -5));
    const std::string title = lua_tostring(state, -4);
    const std::string unknown0018 = lua_tostring(state, -3);
    const std::string unknown0030 = lua_tostring(state, -2);
    const std::pair<bool, std::string> result =
      g_achievementMgr->InsertAchieveInfo(id, title, unknown0018, unknown0030, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveInfo : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x00670140
// FUNCTION: fn_05338
int CAchievementMgr::L_InsertAchieveResource(lua_State* state) {
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1th is must number");
  } else if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must string");
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 4th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -4));
    const std::string text = lua_tostring(state, -3);
    const int unknown0018 = static_cast<int>(lua_tonumber(state, -2));
    const std::pair<bool, std::string> result =
      g_achievementMgr->InsertAchieveResource(id, text, unknown0018, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveResource : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x00670320
// FUNCTION: fn_05339
int CAchievementMgr::L_InsertAchieveRewardBuff(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1th is must number");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -2));
    const std::pair<bool, std::string> result = g_achievementMgr->InsertAchieveRewardBuff(id, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveRewardBuff : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x00670490
// FUNCTION: fn_05340
int CAchievementMgr::L_InsertAchieveRewardItem(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1th is must number");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -2));
    const std::pair<bool, std::string> result = g_achievementMgr->InsertAchieveRewardItem(id, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveRewardItem : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x00670600
// FUNCTION: fn_05341
int CAchievementMgr::L_InsertAchieveRewardTitle(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1th is must number");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -2));
    const std::pair<bool, std::string> result = g_achievementMgr->InsertAchieveRewardTitle(id, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveRewardTitle : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x00670770
// FUNCTION: fn_05342
int CAchievementMgr::L_InsertAchieveUIType(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1th is must number");
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must number");
  } else if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must string");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -3));
    const lua_Number type = lua_tonumber(state, -2);
    const std::string text = lua_tostring(state, -1);
    const std::pair<bool, std::string> result = g_achievementMgr->InsertAchieveUIType(id, static_cast<int>(type), text);
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("InsertAchieveUIType : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}

// OFFSET: 0x006709C0
// FUNCTION: fn_05343
int CAchievementMgr::L_SetAchieveIDByTab(lua_State* state) {
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 1nd is must number");
  } else if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 2th is must number");
  } else if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "argment 3th is must number");
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -3));
    const int group = static_cast<int>(lua_tonumber(state, -2));
    const std::pair<bool, std::string> result = g_achievementMgr->SetAchieveTab(id, group, static_cast<int>(lua_tonumber(state, -1)));
    if (!result.first) {
      lua_pushboolean(state, 0);
      lua_pushstring(state, FormatString("SetAchieveTab : %s", result.second.c_str()));
    } else {
      lua_pushboolean(state, 1);
      lua_pushstring(state, "good");
    }
  }
  return 2;
}
