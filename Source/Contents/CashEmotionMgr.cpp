#include "Contents/CashEmotionMgr.h"
#include "UI/Chat/ChatSuggestions.h"

#include <algorithm>
#include <climits>
#include <cstring>
#include <iterator>
#include <new>

// STATIC: 0x013896C4
// GLOBAL: gl_24350
CCashEmotionMgr* g_cashEmotionMgr = nullptr;

// STATIC: 0x01457848
// GLOBAL: gl_24484
ChatSuggestions* g_chatSuggestions = nullptr;

class UICashEmotionMiniWnd {
public:
  // OFFSET: 0x007C9200
  unsigned int Select(int actionId);
};

// OFFSET: 0x00592290
// FUNCTION: fn_02849
CashEmotionCommand::CashEmotionCommand(const std::string& name, int packId, int emotionId) : name(name) {
  this->Add(packId, emotionId);
}

// OFFSET: 0x005923D0
// FUNCTION: fn_02850
void CashEmotionCommand::Add(int packId, int emotionId) {
  if (packId < 0 || emotionId < 0) {
    return;
  }

  this->entries.emplace_back(packId, emotionId);
}

// OFFSET: 0x00592420
// FUNCTION: fn_02851
std::pair<int, int> CashEmotionCommand::Get(int index) const {
  if (index < 0 || static_cast<std::size_t>(index) >= this->entries.size()) {
    return std::pair<int, int>(-1, -1);
  }

  return this->entries[index];
}

// OFFSET: 0x00592520
// FUNCTION: fn_02856
CashEmotionData::CashEmotionData(int id, int subId, const char* name, int unknown20, int unknown24, int actId, int sprId)
    : id(id), subId(subId), name(name), unknown_0020(unknown20), unknown_0024(unknown24), actId(actId), sprId(sprId) {
}

// OFFSET: 0x00592820
// FUNCTION: fn_02860
bool CashEmotionPackOrder::operator()(const int& left, const int& right) const {
  const char* leftTitle = this->mgr->GetPackTitle(left);
  if (!leftTitle) {
    return false;
  }

  const char* rightTitle = this->mgr->GetPackTitle(right);
  if (!rightTitle) {
    return true;
  }

  int result = _stricmp(leftTitle, rightTitle);
  return this->ascending ? result < 0 : result > 0;
}

// OFFSET: 0x00596430
// FUNCTION: fn_02915
CCashEmotionMgr::CCashEmotionMgr()
    : m_enabled(true), unknown_0005{}, m_action(nullptr), unknown_000C(0), unknown_0010{}, m_emotionsByPack{}, unknown_0050{}, m_packNames{}, unknown_00B0{},
      m_commands{}, unknown_0198{} {
}

// OFFSET: 0x00598520
// FUNCTION: fn_02928
CCashEmotionMgr::~CCashEmotionMgr() {
  if (this->m_action) {
    this->m_action = nullptr;
  }
}

// OFFSET: 0x00598C70
// FUNCTION: fn_02934
void CCashEmotionMgr::Reset() {
  this->m_emotionsByPack.clear();
  this->m_packNames.clear();
  this->m_commands.clear();
}

// OFFSET: 0x00598F00
// FUNCTION: fn_02935
void CCashEmotionMgr::ClearOwnedPacks() {
  this->m_unownedPacks.clear();
  this->m_ownedPacks.clear();
  this->m_expiry.clear();
}

// OFFSET: 0x00598F40
// FUNCTION: fn_02936
void CCashEmotionMgr::ClearCommands() {
  for (auto& entry : this->m_commands) {
    delete reinterpret_cast<CashEmotionCommand*>(entry.second);
  }

  this->m_commands.erase(this->m_commands.begin(), this->m_commands.end());
}

// OFFSET: 0x005994B0
// FUNCTION: fn_02939
int CCashEmotionMgr::FindEmotionId(const char* name) {
  if (!name || std::strlen(name) == 0) {
    return -1;
  }

  std::string key(name);
  auto it = this->m_emotionIds.find(key);
  if (it == this->m_emotionIds.end()) {
    return -1;
  }

  return it->second;
}

// OFFSET: 0x00599650
// FUNCTION: fn_02940
bool CCashEmotionMgr::Initialize() {
  if (g_cashEmotionMgr) {
    return false;
  }

  g_cashEmotionMgr = new (std::nothrow) CCashEmotionMgr;
  if (!g_cashEmotionMgr) {
    return false;
  }

  if (!g_cashEmotionMgr->m_enabled) {
    delete g_cashEmotionMgr;
    g_cashEmotionMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00599700
// FUNCTION: fn_02941
void CCashEmotionMgr::Shutdown() {
  if (g_cashEmotionMgr) {
    g_cashEmotionMgr->Reset();
    delete g_cashEmotionMgr;
    g_cashEmotionMgr = nullptr;
  }
}

// OFFSET: 0x00599020
// FUNCTION: fn_02937
void CCashEmotionMgr::HideSuggestions() {
  if (g_chatSuggestions) {
    g_chatSuggestions->Show(0);
  }
}

// OFFSET: 0x00599730
// FUNCTION: fn_02942
int CCashEmotionMgr::ProcessChatCommand(const char* command) {
  if (!g_chatSuggestions || !g_chatSuggestions->window) {
    return 0;
  }

  if (!command || std::strlen(command) < 1 || command[0] != '/') {
    g_chatSuggestions->Show(0);
    return 0;
  }

  const char* key = command + 1;
  auto it = this->m_commands.find(key);
  int actionId = 0;

  if (it != this->m_commands.end()) {
    actionId = it->second;
  }

  if (actionId != 0) {
    g_chatSuggestions->Show(1);
    auto* miniWnd = reinterpret_cast<UICashEmotionMiniWnd*>(g_chatSuggestions->window);
    return static_cast<int>(miniWnd->Select(actionId));
  } else {
    this->HideSuggestions();
    return 0;
  }
}

// OFFSET: 0x005998C0
// FUNCTION: fn_02943
const char* CCashEmotionMgr::GetPackName(int packId) {
  auto it = this->m_packNames.find(packId);
  if (it == this->m_packNames.end()) {
    return nullptr;
  }

  return it->second.c_str();
}

// OFFSET: 0x00599930
// FUNCTION: fn_02944
std::vector<int> CCashEmotionMgr::FindBaseEmotions(const char* filter) {
  std::vector<int> result;

  auto it = this->unknown_0050.find(this->unknown_000C);
  if (it == this->unknown_0050.end()) {
    return result;
  }

  result.reserve(it->second.size());
  if (filter) {
    std::vector<int> others;
    others.reserve(it->second.size());

    for (int emotionId : it->second) {
      const char* name = this->GetEmotionName(this->unknown_000C, emotionId);
      if (name) {
        if (std::strstr(name, filter)) {
          result.push_back(emotionId);
        } else {
          others.push_back(emotionId);
        }
      }
    }

    std::sort(result.begin(), result.end(), CashEmotionPackOrder{this, true});
    for (int emotionId : others) {
      result.push_back(emotionId);
    }
  } else {
    std::copy(it->second.begin(), it->second.end(), std::back_inserter(result));
  }

  return result;
}

// OFFSET: 0x00599C60
// FUNCTION: fn_02945
CashEmotionTable* CCashEmotionMgr::GetBasePackEmotions() {
  auto it = this->m_emotionsByPack.find(this->unknown_000C);
  return it != this->m_emotionsByPack.end() ? &it->second : nullptr;
}

// OFFSET: 0x00599CA0
// FUNCTION: fn_02946
int CCashEmotionMgr::GetSpecialEmotion(CashEmotionSpecial type) {
  auto it = this->m_specials.find(type);
  if (it != this->m_specials.end()) {
    return it->second;
  }

  return this->LookupSpecialEmotion(type);
}

// OFFSET: 0x00599D00
// FUNCTION: fn_02947
CashEmotionData* CCashEmotionMgr::GetEmotionData(int packId, int emotionId) {
  if (packId < 0 || emotionId < 0) {
    return nullptr;
  }

  auto packIt = this->m_emotionsByPack.find(packId);
  if (packIt == this->m_emotionsByPack.end()) {
    return nullptr;
  }

  auto emoIt = packIt->second.find(emotionId);
  if (emoIt == packIt->second.end()) {
    return nullptr;
  }

  return &emoIt->second;
}

// OFFSET: 0x00599D70
// FUNCTION: fn_02948
const char* CCashEmotionMgr::GetEmotionName(int packId, int emotionId) {
  auto packIt = this->m_emotionsByPack.find(packId);
  if (packIt == this->m_emotionsByPack.end()) {
    return nullptr;
  }

  auto emoIt = packIt->second.find(emotionId);
  if (emoIt == packIt->second.end()) {
    return nullptr;
  }

  return emoIt->second.name.c_str();
}

// OFFSET: 0x00599DE0
// FUNCTION: fn_02949
std::vector<int> CCashEmotionMgr::GetPackEmotions(int packId) {
  std::vector<int> result;

  auto it = this->unknown_0050.find(packId);
  if (it == this->unknown_0050.end()) {
    return result;
  }

  result.reserve(it->second.size());
  for (int emotionId : it->second) {
    result.push_back(emotionId);
  }

  return result;
}

// OFFSET: 0x0059A010
// FUNCTION: fn_02950
int CCashEmotionMgr::GetEarliestExpiry() {
  int earliest = INT_MAX;
  for (const auto& expiry : this->m_expiry) {
    if (expiry.second.seconds < earliest) {
      earliest = expiry.second.seconds;
    }
  }

  return earliest;
}

// OFFSET: 0x0059A040
// FUNCTION: fn_02951
CashEmotionTable* CCashEmotionMgr::GetPackEmotionTable(int packId) {
  auto it = this->m_emotionsByPack.find(packId);
  if (it == this->m_emotionsByPack.end()) {
    return nullptr;
  }

  return &it->second;
}

// OFFSET: 0x0059A080
// FUNCTION: fn_02952
CashEmotionPack* CCashEmotionMgr::GetPack(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return nullptr;
  }

  return &it->second;
}

// OFFSET: 0x0059A0C0
// FUNCTION: fn_02953
const char* CCashEmotionMgr::GetPackTitle(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return nullptr;
  }

  return it->second.name.c_str();
}

// OFFSET: 0x0059A110
// FUNCTION: fn_02954
int CCashEmotionMgr::GetPackUnknown18(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return -1;
  }

  return it->second.unknown_0018;
}

// OFFSET: 0x0059A150
// FUNCTION: fn_02955
Timestamp* CCashEmotionMgr::FindExpiry(int packId) {
  for (auto& expiry : this->m_expiry) {
    if (expiry.first == packId) {
      return &expiry.second;
    }
  }

  return nullptr;
}

// OFFSET: 0x0059A1C0
// FUNCTION: fn_02956
std::vector<int> CCashEmotionMgr::SortOwnedPacks(const char* filter) {
  return this->SortPacks(this->m_ownedPacks, filter);
}

// OFFSET: 0x0059A5B0
// FUNCTION: fn_02958
unsigned int CCashEmotionMgr::GetPackEndTime(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return 0;
  }

  return it->second.endTime;
}

// OFFSET: 0x0059A5F0
// FUNCTION: fn_02959
unsigned int CCashEmotionMgr::GetPackStartTime(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return 0;
  }

  return it->second.startTime;
}

// OFFSET: 0x0059A630
// FUNCTION: fn_02960
void CCashEmotionMgr::Search(
  const char* text, std::vector<std::pair<int, int>>& basePack, std::vector<std::pair<int, int>>& ownedPacks, std::vector<std::pair<int, int>>& otherPacks
) {
  if (!text || std::strlen(text) < 1) {
    return;
  }

  for (auto& entry : this->m_searchIndex) {
    if (entry.first.find(text) == std::string::npos) {
      continue;
    }

    if (entry.second.first == this->unknown_000C) {
      this->AddSearchResult(basePack, entry.second);
    } else if (this->IsOwnedPack(entry.second.first)) {
      this->AddSearchResult(ownedPacks, entry.second);
    } else {
      this->AddSearchResult(otherPacks, entry.second);
    }
  }
}

// OFFSET: 0x0059A7C0
// FUNCTION: fn_02961
void CCashEmotionMgr::AddSearchResult(std::vector<std::pair<int, int>>& results, const std::pair<int, int>& entry) {
  if (std::find(results.begin(), results.end(), entry) == results.end()) {
    results.push_back(entry);
  }
}

// OFFSET: 0x0059A820
// FUNCTION: fn_02962
std::vector<int> CCashEmotionMgr::SortPacks(const std::vector<int>& packs, const char* filter) {
  if (!filter || std::strlen(filter) == 0) {
    std::vector<int> result(packs);
    std::sort(result.begin(), result.end(), CashEmotionPackOrder{this, true});
    return result;
  }

  std::vector<int> matched;
  matched.reserve(packs.size());
  std::vector<int> others;
  others.reserve(packs.size());

  for (int packId : packs) {
    const char* title = this->GetPackTitle(packId);
    if (title && std::strstr(title, filter)) {
      matched.push_back(packId);
    } else {
      others.push_back(packId);
    }
  }

  std::sort(matched.begin(), matched.end(), CashEmotionPackOrder{this, true});
  std::sort(others.begin(), others.end(), CashEmotionPackOrder{this, true});
  for (int packId : others) {
    matched.push_back(packId);
  }

  return matched;
}

// OFFSET: 0x0059AC40
// FUNCTION: fn_02963
const char* CCashEmotionMgr::GetSprFileName(int sprId) {
  auto it = this->m_sprNames.find(sprId);
  if (it == this->m_sprNames.end()) {
    return nullptr;
  }

  return it->second.c_str();
}

// OFFSET: 0x0059ACE0
// FUNCTION: fn_02964
std::vector<int> CCashEmotionMgr::SortUnownedPacks(const char* filter) {
  return this->SortPacks(this->m_unownedPacks, filter);
}

// OFFSET: 0x0059AD40
// FUNCTION: fn_02965
int CCashEmotionMgr::GetPackUnknown24(int packId) {
  auto it = this->unknown_0010.find(packId);
  if (it == this->unknown_0010.end()) {
    return 0;
  }

  return it->second.unknown_0024;
}

// OFFSET: 0x0059B320
// FUNCTION: fn_02967
int CCashEmotionMgr::LookupSpecialEmotion(CashEmotionSpecial type) {
  switch (type) {
  case CashEmotionSpecial::ChatProhibit: {
    auto it = this->m_emotionIds.find("ET_CHAT_PROHIBIT");
    int emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    this->m_specials.emplace(type, emotionId);
    return emotionId;
  }
  case CashEmotionSpecial::Yut: {
    auto it = this->m_emotionIds.find("ET_YUT1");
    int emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    this->m_specials.emplace(type, emotionId);
    return emotionId;
  }
  case CashEmotionSpecial::Dice: {
    auto it = this->m_emotionIds.find("ET_DICE1");
    int emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    this->m_specials.emplace(type, emotionId);
    return emotionId;
  }
  case CashEmotionSpecial::Sweat:
    return this->LookupSpecialEmotion(type, "ET_SWEAT");
  case CashEmotionSpecial::Chup:
    return this->LookupSpecialEmotion(type, "ET_CHUP");
  default:
    return -1;
  }
}

// OFFSET: 0x0059B660
// FUNCTION: fn_02968
int CCashEmotionMgr::LookupSpecialEmotion(CashEmotionSpecial type, const char* name) {
  if (!name || std::strlen(name) == 0) {
    return -1;
  }

  int emotionId;
  {
    std::string key(name);
    auto it = this->m_emotionIds.find(key);
    emotionId = it != this->m_emotionIds.end() ? it->second : -1;
  }

  this->m_specials.emplace(type, emotionId);
  return emotionId;
}

// OFFSET: 0x0059B820
// FUNCTION: fn_02969
void CCashEmotionMgr::AddShortcut(const std::string& name, int packId, int emotionId) {
  auto it = this->m_shortcuts.find(name);
  if (it != this->m_shortcuts.end()) {
    it->second->Add(packId, emotionId);
    return;
  }

  CashEmotionCommand* shortcut = new (std::nothrow) CashEmotionCommand(name, packId, emotionId);
  if (shortcut) {
    this->m_shortcuts.try_emplace(name, shortcut);
  }
}

// OFFSET: 0x0059C3A0
// FUNCTION: fn_02973
bool CCashEmotionMgr::IsChatProhibitEmotion(int emotionId) {
  if (emotionId < 0) {
    return false;
  }

  return this->GetSpecialEmotion(CashEmotionSpecial::ChatProhibit) == emotionId;
}

// OFFSET: 0x0059C410
// FUNCTION: fn_02974
bool CCashEmotionMgr::IsPackAvailable(unsigned int now, int packId) {
  CashEmotionPack* pack = this->GetPack(packId);
  if (!pack) {
    return false;
  }

  return pack->IsAvailable(now);
}

// OFFSET: 0x0059C490
// FUNCTION: fn_02975
bool CCashEmotionMgr::IsOwnedPack(int packId) {
  return std::find(this->m_ownedPacks.begin(), this->m_ownedPacks.end(), packId) != this->m_ownedPacks.end();
}
