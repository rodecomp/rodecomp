#include "Contents/Achievement/AchievementMgr.h"
#include "Client/Session/Session.h"
#include "Script/NLuaState.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

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

  if (!g_achievementMgr->enabled) {
    delete g_achievementMgr;
    g_achievementMgr = nullptr;
    return false;
  }

  if (!g_achievementMgr->LoadAchievementInfo()) {
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
std::string CAchievementMgr::GetIconName(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].iconName;
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
int CAchievementMgr::GetPoints(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].points;
  }
  return -1;
}

// OFFSET: 0x0066E2A0
// FUNCTION: fn_05317
std::string CAchievementMgr::GetSummaryText(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].summaryText;
  }
  return "";
}

// OFFSET: 0x0066E460
// FUNCTION: fn_05318
int CAchievementMgr::GetRewardTitleId(int id) {
  if (this->achievements.find(id) != this->achievements.end()) {
    return this->achievements[id].rewardTitleId;
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
  return g_session.FindAchievementRecord(id).completed == 1;
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
      for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end();) {
        const int id = *it;
        ++it;
        if (IsAchievementCompleted(id)) {
          ++count;
        }
      }
    }
  } else {
    int key = this->groupWidth * group + index;
    for (std::vector<int>::iterator it = this->groups[key].begin(); it != this->groups[key].end();) {
      const int id = *it;
      ++it;
      if (IsAchievementCompleted(id)) {
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
    std::vector<int>& values = it->second;
    const std::vector<int>::iterator valuesEnd = values.end();
    for (std::vector<int>::iterator value = values.begin(); value != valuesEnd; ++value) {
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

// OFFSET: 0x0066EAA0
// FUNCTION: fn_05322
bool CAchievementMgr::LoadAchievementInfo() {
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
    if (luaL_loadfile(lua.GetState(), OBFUSCATE_TEXT_NAMED("system\\Achievement_list.lub", 0x490FFB43A12F0DB9ULL, path)) != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
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
std::pair<bool, std::string> CAchievementMgr::InsertAchieveInfo(int id, std::string title, std::string summaryText, std::string description, int points) {
  if (this->achievements.find(id) != this->achievements.end()) {
    const std::string message = FormatOwnedString("duplicate achievement info [%d]", id);
    return std::make_pair(false, message);
  }

  if (this->achievements.insert(std::pair<int, AchievementInfo>(id, AchievementInfo(title, summaryText, description, points))).second) {
    return std::make_pair(true, "good");
  }

  const std::string message = FormatOwnedString("achievement info [%d] insert fail", id);
  return std::make_pair(false, message);
}

// OFFSET: 0x0066F170
// FUNCTION: fn_05324
std::pair<bool, std::string> CAchievementMgr::InsertAchieveResource(int id, std::string text, int targetCount, int conditionId) {
  const auto found = this->achievements.find(id);
  if (found == this->achievements.end()) {
    const std::string message = FormatOwnedString("achievement info [%d] cannot be find", id);
    return std::make_pair(false, message);
  }

  std::vector<AchievementReward>& rewards = found->second.rewards;
  const auto end = rewards.end();
  if (std::find_if(rewards.begin(), end, [text](const AchievementReward& reward) { return text.compare(reward.text) == 0; }) == end) {
    rewards.push_back(AchievementReward(text, targetCount, conditionId));
    return std::make_pair(true, "good");
  }

  const std::string message = FormatOwnedString("achievement info [%d] - resource info already exist", id);
  return std::make_pair(false, message);
}

// OFFSET: 0x0066F450
// FUNCTION: fn_05325
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardBuff(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found != this->achievements.end()) {
    found->second.rewardBuffId = value;
    return std::make_pair(true, "good");
  } else {
    return std::make_pair(false, static_cast<const std::string&>(FormatOwnedString("achievement info [%d] cannot be find", id)));
  }
}

// OFFSET: 0x0066F570
// FUNCTION: fn_05326
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardItem(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found != this->achievements.end()) {
    found->second.rewardItemId = value;
    return std::make_pair(true, "good");
  } else {
    return std::make_pair(false, static_cast<const std::string&>(FormatOwnedString("achievement info [%d] cannot be find", id)));
  }
}

// OFFSET: 0x0066F690
// FUNCTION: fn_05327
std::pair<bool, std::string> CAchievementMgr::InsertAchieveRewardTitle(int id, int value) {
  const auto found = this->achievements.find(id);
  if (found != this->achievements.end()) {
    found->second.rewardTitleId = value;
    return std::make_pair(true, "good");
  } else {
    return std::make_pair(false, static_cast<const std::string&>(FormatOwnedString("achievement info [%d] cannot be find", id)));
  }
}

// OFFSET: 0x0066F7B0
// FUNCTION: fn_05328
std::pair<bool, std::string> CAchievementMgr::InsertAchieveUIType(int id, int type, std::string text) {
  const auto found = this->achievements.find(id);
  if (found != this->achievements.end()) {
    found->second.progressMode = type;
    found->second.iconName = text;
    return std::make_pair(true, "good");
  } else {
    return std::make_pair(false, static_cast<const std::string&>(FormatOwnedString("achievement info [%d] cannot be find", id)));
  }
}

// OFFSET: 0x0066F900
// FUNCTION: fn_05329
bool CAchievementMgr::HasReward(int id) {
  if (this->achievements.find(id) == this->achievements.end()) {
    return false;
  }

  const AchievementInfo& info = this->achievements[id];
  bool result = info.rewardBuffId > 0;
  result |= info.rewardTitleId > 0;
  if (info.rewardItemId > 0) {
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

  return std::make_pair(false, static_cast<const std::string&>(FormatOwnedString("duplicate achievement tab number info [%d]", id)));
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
    const std::string summaryText = lua_tostring(state, -3);
    const std::string description = lua_tostring(state, -2);
    const double value = lua_tonumber(state, -1);
    CAchievementMgr* const manager = g_achievementMgr;
    const std::pair<bool, std::string> result = manager->InsertAchieveInfo(id, title, summaryText, description, static_cast<int>(value));
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
    const int targetCount = static_cast<int>(lua_tonumber(state, -2));
    CAchievementMgr* const manager = g_achievementMgr;
    const std::pair<bool, std::string> result =
      manager->InsertAchieveResource(id, text, targetCount, static_cast<int>(lua_tonumber(state, -1)));
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
    CAchievementMgr* const manager = g_achievementMgr;
    const std::pair<bool, std::string> result = manager->InsertAchieveUIType(id, static_cast<int>(type), text);
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
