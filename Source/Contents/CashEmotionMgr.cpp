#include "Contents/CashEmotionMgr.h"
#include "UI/Chat/ChatSuggestions.h"

#include <algorithm>
#include <cstring>
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

// OFFSET: 0x00598F40
// FUNCTION: fn_02936
void CCashEmotionMgr::ClearCommands() {
  for (auto& entry : this->m_commands) {
    delete reinterpret_cast<CashEmotionCommand*>(entry.second);
  }

  this->m_commands.erase(this->m_commands.begin(), this->m_commands.end());
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

// OFFSET: 0x0059AC40
// FUNCTION: fn_02963
const char* CCashEmotionMgr::GetSprFileName(int sprId) {
  auto it = this->m_sprNames.find(sprId);
  if (it == this->m_sprNames.end()) {
    return nullptr;
  }

  return it->second.c_str();
}

// OFFSET: 0x0059C490
// FUNCTION: fn_02975
bool CCashEmotionMgr::IsOwnedPack(int packId) {
  return std::find(this->m_ownedPacks.begin(), this->m_ownedPacks.end(), packId) != this->m_ownedPacks.end();
}
