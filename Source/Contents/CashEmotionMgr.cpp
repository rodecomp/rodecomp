#include "Contents/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UISpriteButton.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICashEmotionSprBtn.h"
#include "UI/CashEmotion/UICashEmotionPack.h"
#include "UI/CashEmotion/UICashEmotionPiece.h"
#include "UI/Chat/ChatSuggestions.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/UILapineScrollBar.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <array>
#include <climits>
#include <cstring>
#include <ctime>
#include <iterator>
#include <map>
#include <new>
#include <windows.h>

// STATIC: 0x013896C4
// GLOBAL: gl_24350
CCashEmotionMgr* g_cashEmotionMgr = nullptr;

// STATIC: 0x01457848
// GLOBAL: gl_24484
ChatSuggestions* g_chatSuggestions = nullptr;

class ICashEmotionAction {
public:
  virtual ~ICashEmotionAction();
  virtual bool Execute(int packId, int argument) = 0;

  bool ExecuteNamed(const char* name);
};

class UICashEmotionPack_Purchase : public UICashEmotionPack {
public:
  ~UICashEmotionPack_Purchase() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00E0 */ ICashEmotionAction* m_selectAction;
  /* 00E4 */ ICashEmotionAction* m_purchaseAction;
};

class UICashEmotionPack_Search : public UICashEmotionPack {
public:
  void Paint() override;
};

class UICashEmotionPack_Search_Normal : public UICashEmotionPack_Search {
public:
  ~UICashEmotionPack_Search_Normal() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00E0 */ ICashEmotionAction* m_action;
};

class UICashEmotionPack_Search_UnPurchased : public UICashEmotionPack_Search {
public:
  ~UICashEmotionPack_Search_UnPurchased() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00E0 */ ICashEmotionAction* m_action;
};

class UICashEmotionPack_Unpurchase : public UICashEmotionPack {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void SetPressed(bool pressed, bool force);

protected:
  /* 00E0 */ bool m_pressed;
  /* 00E4 */ std::string unknown_00E4;
};

class UICashEmotionScrollBar : public UIScrollBar {
public:
  void Paint() override;

  void SetDisabled(bool disabled);

protected:
  /* 00B4 */ bool m_disabled;
};

class UICashEmotionScrollWnd : public UIScrollWnd {
public:
  void Slot15(int width, int height) override;
};

class UICashEmotionListChildWnd_Abstract : public UICashEmotionScrollWnd {
public:
  void Slot35(int delta) override;
};

class UICashEmotionListChildWnd_Basic : public UICashEmotionListChildWnd_Abstract {
public:
  void Slot19() override;
  void Paint() override;

protected:
  /* 00B0 */ std::vector<UIWindow*> m_items;
  /* 00BC */ ICashEmotionAction* m_selectAction;
  /* 00C0 */ ICashEmotionAction* m_playAction;
};

class UICashEmotionListChildWnd_Purchase : public UICashEmotionListChildWnd_Abstract {
public:
  void Paint() override;
};

class UICashEmotionListChildWnd_Search : public UICashEmotionListChildWnd_Abstract {
public:
  void Paint() override;
};

class UICashEmotionListChildWnd_UnPurchased : public UICashEmotionListChildWnd_Abstract {
public:
  void Paint() override;
};

struct CashEmotionListPlacement {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
};

class UICashEmotionListWnd : public UIFrameWnd {
public:
  UICashEmotionListWnd(UIWindowContext& windows, UIFrameContext& frame, UIButtonContext& controls);
  void Slot11() override;
  void Slot19() override;
  void Paint() override;

  UIBitmapButton* CreateButton(int x, int y, int width, int height, int id, const char* format, bool hasDisabled);

protected:
  /* 00B4 */ CashEmotionListPlacement* m_placement;
  /* 00B8 */ UICashEmotionListChildWnd_Abstract* m_basicList;
  /* 00BC */ UICashEmotionListChildWnd_Abstract* m_purchaseList;
  /* 00C0 */ UICashEmotionListChildWnd_Abstract* m_searchList;
  /* 00C4 */ UICashEmotionListChildWnd_Abstract* m_unpurchasedList;
  /* 00C8 */ std::map<char, UIWindow*> m_tabs;
  /* 00D0 */ char m_tab;
  /* 00D4 */ UIWindow* unknown_00D4;
  /* 00D8 */ UIEditCtrl* m_searchEdit;
  /* 00DC */ UIWindow* unknown_00DC;
  /* 00E0 */ UIWindow* unknown_00E0;
  /* 00E4 */ UIWindow* unknown_00E4;
  /* 00E8 */ UICashEmotionListChildWnd_Abstract* m_currentList;
  /* 00EC */ UIWindow* unknown_00EC;
  UIButtonContext* m_controls;
};

class UICashEmotionMiniWnd : public UIWindow {
public:
  ~UICashEmotionMiniWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  unsigned int Select(int actionId);
  void Select(CashEmotionCommand* command);
  void Previous();
  void Refresh(int first);
  void Next();
  void SetIndex(int index);

protected:
  /* 007C */ CashEmotionCommand* m_selected;
  /* 0080 */ std::array<UICashEmotionPiece*, 3> m_pieces;
  /* 008C */ UIBitmapButton* m_previous;
  /* 0090 */ UIBitmapButton* m_next;
  /* 0094 */ ICashEmotionAction* m_action;
  /* 0098 */ int m_page;
  /* 009C */ int m_selectedIndex;
  UICashEmotionPieceContext* m_controls;
};

// OFFSET: 0x00591D80
// FUNCTION: fn_02841
ICashEmotionAction::~ICashEmotionAction() {
}

// OFFSET: 0x00591E60
// FUNCTION: fn_02844
bool ICashEmotionAction::ExecuteNamed(const char* name) {
  if (!name || std::strlen(name) == 0) {
    return false;
  }

  return this->Execute(g_cashEmotionMgr->BasePackId(), g_cashEmotionMgr->FindEmotionId(name));
}

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

// OFFSET: 0x0059C4F0
// FUNCTION: fn_02976
bool CCashEmotionMgr::PlaySearchedEmotion(const std::string& name) {
  if (this->m_action) {
    auto found = this->m_searchIndex.find(name);
    const std::pair<int, int>* emotion = found == this->m_searchIndex.end() ? nullptr : &found->second;
    if (emotion) {
      return this->m_action->Execute(emotion->first, emotion->second);
    }
  }

  return false;
}

// OFFSET: 0x0059C0A0
// FUNCTION: fn_02972
void CCashEmotionMgr::SetOwnedPack(int packId, unsigned int expiry) {
  if (packId <= 0) {
    return;
  }

  if (!this->IsOwnedPack(packId)) {
    this->m_ownedPacks.push_back(packId);

    auto it = std::find(this->m_unownedPacks.begin(), this->m_unownedPacks.end(), packId);
    if (it != this->m_unownedPacks.end()) {
      this->m_unownedPacks.erase(it);
    }
  }

  for (auto it = this->m_expiry.begin(); it != this->m_expiry.end(); ++it) {
    if (it->first == packId) {
      if (expiry) {
        it->second.Set(expiry);
      } else {
        this->m_expiry.erase(it);
      }
      return;
    }
  }

  if (expiry > 0) {
    this->m_expiry.emplace_back(packId, expiry);
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

// OFFSET: 0x0059CDE0
// FUNCTION: fn_02981
void CCashEmotionMgr::sub_59CDE0(int param) {
  this->unknown_01D0 = _time32(nullptr) - param;
}

// OFFSET: 0x0059CE00
// FUNCTION: fn_02982
void CCashEmotionMgr::SetServerTimeZone(short serverBias) {
  TIME_ZONE_INFORMATION info;
  short localBias;
  if (GetTimeZoneInformation(&info) == TIME_ZONE_ID_INVALID) {
    localBias = 0;
  } else {
    localBias = static_cast<short>(-info.Bias);
  }
  const int offset = (serverBias - localBias) * 60;
  this->unknown_01D4 = offset;
  for (auto& entry : this->unknown_0010) {
    CashEmotionPack& pack = entry.second;
    if (pack.originalStartTime) {
      pack.startTime = pack.originalStartTime - offset;
    }
    if (pack.originalEndTime) {
      pack.endTime = pack.originalEndTime - offset;
    }
  }
}

// OFFSET: 0x0059FC50
// FUNCTION: fn_03031
int CCashEmotionMgr::sub_59FC50() {
  return _time32(nullptr) - this->unknown_01D0 - this->unknown_01CC;
}

// OFFSET: 0x007C3AD0
// FUNCTION: fn_09344
void UICashEmotionListChildWnd_Abstract::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
  }
}

// OFFSET: 0x007C4450
// FUNCTION: fn_09354
void UICashEmotionListChildWnd_Basic::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C4460
// FUNCTION: fn_09355
void UICashEmotionListChildWnd_Basic::Slot19() {
  for (UIWindow* item : this->m_items) {
    if (item->geometry.visible) {
      item->Slot19();
    }
  }
}

// OFFSET: 0x007C52A0
// FUNCTION: fn_09369
void UICashEmotionListChildWnd_Purchase::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C5D90
// FUNCTION: fn_09384
void UICashEmotionListChildWnd_Search::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C6AB0
// FUNCTION: fn_09395
void UICashEmotionListChildWnd_UnPurchased::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C6CB0
// FUNCTION: fn_09401
UICashEmotionListWnd::UICashEmotionListWnd(UIWindowContext& windows, UIFrameContext& frame, UIButtonContext& controls)
    : UIFrameWnd(windows, frame, 0), m_placement(nullptr), m_basicList(nullptr), m_purchaseList(nullptr), m_searchList(nullptr), m_unpurchasedList(nullptr),
      m_tab(0), unknown_00D4(nullptr), m_searchEdit(nullptr), unknown_00DC(nullptr), unknown_00E0(nullptr), unknown_00E4(nullptr), m_currentList(nullptr),
      unknown_00EC(nullptr), m_controls(&controls) {
}

// OFFSET: 0x007C7240
// FUNCTION: fn_09405
UIBitmapButton* UICashEmotionListWnd::CreateButton(int x, int y, int width, int height, int id, const char* format, bool hasDisabled) {
  UIBitmapButton* button = new (std::nothrow) UIBitmapButton(*this->m_context, *this->m_controls);
  if (!button) {
    return nullptr;
  }
  button->SetBitmap(FormatOwnedString(format, "normal").c_str(), 0, 0);
  button->SetBitmap(FormatOwnedString(format, "over").c_str(), 1, 0);
  button->SetBitmap(FormatOwnedString(format, "press").c_str(), 2, 0);
  if (hasDisabled == true) {
    std::string name = Resolve(FormatOwnedString(format, "disable").c_str());
    if (ResourceMgr::Instance().Load(name)) {
      button->SetBitmap(name.c_str(), 3, 0);
    }
  }
  button->SetSize(width, height);
  button->Move(x, y);
  button->SetId(id);
  this->AddChild(button);
  return button;
}

// OFFSET: 0x007C7C20
// FUNCTION: fn_09407
void UICashEmotionListWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const int width = this->geometry.width - 30;
  const int top = this->DrawTitleBarAt(this->m_controls->fonts, this->m_controls->locale, 29, width, false, LookupMessage(542));
  this->DrawPanelBackgroundAt(29, top, width, nullptr);
  DrawRectangleBorder(*this, 60, 22, 161, 18, 0xB5B5B5);
  this->FillRegion(29, 46, width, 1, 0xCECECE);
  this->DrawButtonBarAt(2, 29, width, 1);
}

// OFFSET: 0x007C7CC0
// FUNCTION: fn_09410
void UICashEmotionListWnd::Slot19() {
  if (this->m_currentList) {
    this->m_currentList->Slot19();
  }
}

// OFFSET: 0x007C8AF0
// FUNCTION: fn_09420
void UICashEmotionListWnd::Slot11() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x007C90B0
// FUNCTION: fn_09427
UICashEmotionMiniWnd::~UICashEmotionMiniWnd() {
  delete this->m_action;
  this->m_action = nullptr;
}

// OFFSET: 0x007C9200
// FUNCTION: fn_09428
void UICashEmotionMiniWnd::Select(CashEmotionCommand* command) {
  this->m_selected = command;
  this->m_page = 0;
  this->m_selectedIndex = 0;
  this->Refresh(0);

  if (this->m_previous) {
    this->m_previous->Enable(0);
  }

  if (this->m_selected && this->m_selected->Count() > 3) {
    if (this->m_next) {
      this->m_next->Enable(1);
    }
  } else if (this->m_next) {
    this->m_next->Enable(0);
  }
}

// OFFSET: 0x007C9290
// FUNCTION: fn_09429
void UICashEmotionMiniWnd::Previous() {
  if (this->m_selectedIndex != 0) {
    this->SetIndex(this->m_selectedIndex - 1);
  }
}

// OFFSET: 0x007C92B0
// FUNCTION: fn_09430
void UICashEmotionMiniWnd::Refresh(int first) {
  if (this->m_pieces[0]) {
    if (this->m_selected) {
      std::pair<int, int> entry = this->m_selected->Get(first);
      bool owned = entry.first == g_cashEmotionMgr->BasePackId() || g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[0]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[0]->Set(-1, -1, false, false, false);
    }
  }

  if (this->m_pieces[1]) {
    if (this->m_selected) {
      std::pair<int, int> entry = this->m_selected->Get(first + 1);
      bool owned = entry.first == g_cashEmotionMgr->BasePackId() || g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[1]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[1]->Set(-1, -1, false, false, false);
    }
  }

  if (this->m_pieces[2]) {
    if (this->m_selected) {
      std::pair<int, int> entry = this->m_selected->Get(first + 2);
      bool owned = entry.first == g_cashEmotionMgr->BasePackId() || g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[2]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[2]->Set(-1, -1, false, false, false);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007C94A0
// FUNCTION: fn_09431
void UICashEmotionMiniWnd::Slot15(int width, int height) {
  int x = 24;
  for (UICashEmotionPiece*& piece : this->m_pieces) {
    piece = new (std::nothrow) UICashEmotionPiece(*this->m_context, *this->m_controls);
    if (piece) {
      piece->SetSize(42, 42);
      piece->Move(x, 1);
      this->AddChild(piece);
      x += 49;
    }
  }

  this->m_previous = new (std::nothrow) UIBitmapButton(*this->m_context, this->m_controls->buttons);
  if (this->m_previous) {
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_normal.bmp", 0, 0);
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_over.bmp", 1, 0);
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_press.bmp", 2, 0);
    this->m_previous->SetSize(this->m_previous->BitmapWidth(), this->m_previous->BitmapHeight());
    this->m_previous->Move(0, 0);
    this->m_previous->SetId(1);
    this->AddChild(this->m_previous);
  }

  this->m_next = new (std::nothrow) UIBitmapButton(*this->m_context, this->m_controls->buttons);
  if (this->m_next) {
    this->m_next->SetBitmap("유저인터페이스\\cashemotion\\bth_arrow_r_normal.bmp", 0, 0);
    this->m_next->SetBitmap("유저인터페이스\\cashemotion\\bth_arrow_r_over.bmp", 1, 0);
    this->m_next->SetBitmap("유저인터페이스\\cashemotion\\bth_arrow_r_press.bmp", 2, 0);
    int buttonWidth = this->m_next->BitmapWidth();
    this->m_next->SetSize(buttonWidth, this->m_next->BitmapHeight());
    this->m_next->Move(this->geometry.width - buttonWidth, 0);
    this->m_next->SetId(2);
    this->AddChild(this->m_next);
  }
}

// OFFSET: 0x007C96E0
// FUNCTION: fn_09432
void UICashEmotionMiniWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);

  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\bg_preview.bmp")));
  if (background) {
    this->DrawBitmap(0, 0, &background->image, false);
  }

  if (this->m_selectedIndex == -1) {
    return;
  }

  int slot = this->m_selectedIndex - this->m_page;
  if (slot <= -1) {
    return;
  }

  UICashEmotionPiece* piece = this->m_pieces.at(slot);
  if (!piece) {
    return;
  }

  auto* highlight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\bg_preview_select.bmp")));
  if (highlight) {
    this->DrawBitmap(piece->geometry.x - 3, 0, &highlight->image, false);
  }
}

// OFFSET: 0x007C9790
// FUNCTION: fn_09433
void UICashEmotionMiniWnd::Next() {
  if (this->m_selected && this->m_selected->Count() > this->m_selectedIndex + 1) {
    this->SetIndex(this->m_selectedIndex + 1);
  }
}

// OFFSET: 0x007C97C0
// FUNCTION: fn_09434
void UICashEmotionMiniWnd::SetIndex(int index) {
  if (index == this->m_selectedIndex) {
    return;
  }

  this->m_selectedIndex = index;
  if (this->m_selected) {
    bool enable = index > 0 && this->m_selected->Count() > 1;
    if (this->m_previous) {
      this->m_previous->Enable(enable);
    }

    enable = this->m_selectedIndex + 1 < this->m_selected->Count() && this->m_selected->Count() > 1;
    if (this->m_next) {
      this->m_next->Enable(enable);
    }
  }

  if (this->m_selectedIndex >= this->m_page + 3) {
    this->m_page = this->m_selectedIndex - 2;
    this->Refresh(this->m_page);
  } else if (this->m_selectedIndex < this->m_page) {
    this->m_page = this->m_selectedIndex;
    this->Refresh(this->m_page);
  }

  this->Invalidate();
}

// OFFSET: 0x007C98A0
// FUNCTION: fn_09435
int UICashEmotionMiniWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  std::pair<int, int> entry;

  switch (message) {
  case 6:
    if (argument1 == 1) {
      this->Previous();
    } else if (argument1 == 2) {
      this->Next();
    }
    return 0;
  case 60:
    this->Invalidate();
    return 0;
  case 174:
    entry.first = static_cast<int>(argument1);
    entry.second = static_cast<int>(argument2);
    break;
  case 176:
    if (argument1 == 13) {
      if (!this->m_selected) {
        return 0;
      }
      entry = this->m_selected->Get(this->m_selectedIndex);
      break;
    }
    if (argument1 == 37) {
      this->Previous();
    } else if (argument1 == 39) {
      this->Next();
    }
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  CashEmotionData* data = g_cashEmotionMgr->GetEmotionData(entry.first, entry.second);
  if (!data) {
    return 0;
  }

  int packId = data->id;
  if (packId == g_cashEmotionMgr->BasePackId() || g_cashEmotionMgr->IsOwnedPack(packId)) {
    if (g_chatSuggestions) {
      if (g_chatSuggestions->input) {
        g_chatSuggestions->input->SetText(data->name.c_str());
      }
      g_chatSuggestions->Show(0);
    }
  } else if (this->m_action) {
    this->m_action->Execute(packId, 0);
  }

  return 0;
}

// OFFSET: 0x007C9D60
// FUNCTION: fn_09439
UICashEmotionPack::~UICashEmotionPack() {
  this->m_pieces.clear();
}

// OFFSET: 0x007CA690
// FUNCTION: fn_09446
void UICashEmotionPack::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->m_background.empty()) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(this->m_background.c_str()));
    if (bitmap) {
      this->DrawBitmapScaled(0, 0, this->geometry.width, this->geometry.height, &bitmap->image, 0);
    }
  }

  if (!this->m_saleBadge.empty()) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(this->m_saleBadge.c_str()));
    if (bitmap) {
      this->DrawBitmap(171, 12, &bitmap->image, true);
    }
  }

  if (this->m_limited) {
    auto* bitmap =
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\icon_limitemotion.bmp")));
    if (bitmap) {
      this->DrawBitmap(0, 0, &bitmap->image, true);
    }
  }
}

// OFFSET: 0x007CA760
// FUNCTION: fn_09447
void UICashEmotionPack::Slot26(int, int) {
}

// OFFSET: 0x007CA770
// FUNCTION: fn_09448
void UICashEmotionPack::Slot25(int, int) {
}

// OFFSET: 0x007CA780
// FUNCTION: fn_09449
void UICashEmotionPack::Slot27(int, int) {
}

// OFFSET: 0x007CA790
// FUNCTION: fn_09450
void UICashEmotionPack::Slot19() {
  for (UICashEmotionPiece* piece : this->m_pieces) {
    if (piece) {
      piece->Slot19();
    }
  }
}

// OFFSET: 0x007CA7C0
// FUNCTION: fn_09451
void UICashEmotionPack::Slot35(int argument) {
  if (this->parent) {
    this->parent->Slot35(argument);
  }
}

// OFFSET: 0x007CA7E0
// FUNCTION: fn_09452
int UICashEmotionPack::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 60:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(sender, 60, 0, 0, 0, 0);
    }
    return 0;
  case 174:
    if (this->parent) {
      this->parent->SendMsg(sender, 174, argument1, argument2, argument3, argument4);
    }
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CA870
// FUNCTION: fn_09453
void UICashEmotionPack::SetSaleBadge(char type) {
  if (type == 1) {
    this->m_saleBadge = Resolve("유저인터페이스\\cashemotion\\sale_account.bmp");
  } else if (type == 2) {
    this->m_saleBadge = Resolve("유저인터페이스\\cashemotion\\sale_char.bmp");
  } else {
    this->m_saleBadge = "";
  }
}

// OFFSET: 0x007CA8F0
// FUNCTION: fn_09454
void UICashEmotionPack::SetTitle(const char* title) {
  if (title) {
    this->m_title = title;
  } else {
    this->m_title.clear();
  }

  if (this->m_titleText) {
    this->m_titleText->Show(!this->m_title.empty());
  }
}

// OFFSET: 0x007CAC40
// FUNCTION: fn_09458
UICashEmotionPack_Purchase::~UICashEmotionPack_Purchase() {
  delete this->m_selectAction;
  this->m_selectAction = nullptr;
  delete this->m_purchaseAction;
  this->m_purchaseAction = nullptr;
}

// OFFSET: 0x007CAEB0
// FUNCTION: fn_09462
void UICashEmotionPack_Purchase::Paint() {
  UICashEmotionPack::Paint();
}

// OFFSET: 0x007CAEC0
// FUNCTION: fn_09463
int UICashEmotionPack_Purchase::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 60:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(sender, 60, 0, 0, 0, 0);
    }
    return 0;
  case 174:
    this->m_purchaseAction->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
    return 0;
  case 175:
    this->m_selectAction->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CB160
// FUNCTION: fn_09465
void UICashEmotionPack_Search::Paint() {
  this->ClearSurface(0xFFFF00FF);

  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\bg_list_search.bmp")));
  if (bitmap) {
    this->DrawBitmap(0, 0, &bitmap->image, false);
  }
}

// OFFSET: 0x007CB2C0
// FUNCTION: fn_09466
UICashEmotionPack_Search_Normal::~UICashEmotionPack_Search_Normal() {
  delete this->m_action;
  this->m_action = nullptr;
}

// OFFSET: 0x007CB360
// FUNCTION: fn_09467
int UICashEmotionPack_Search_Normal::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message != 175) {
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->m_action) {
    this->m_action->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
  }

  return 0;
}

// OFFSET: 0x007CB4C0
// FUNCTION: fn_09469
UICashEmotionPack_Search_UnPurchased::~UICashEmotionPack_Search_UnPurchased() {
  delete this->m_action;
  this->m_action = nullptr;
}

// OFFSET: 0x007CB560
// FUNCTION: fn_09470
int UICashEmotionPack_Search_UnPurchased::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 60:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(this, 60, 0, 0, 0, 0);
    }
    return 0;
  case 174:
    if (this->m_action) {
      this->m_action->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
      return 0;
    }
    return 0;
  default:
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CB850
// FUNCTION: fn_09474
void UICashEmotionPack_Unpurchase::Paint() {
  UICashEmotionPack::Paint();
  if (!this->unknown_00E4.empty()) {
    this->DrawRightAlignedText(233, 42, this->unknown_00E4.c_str(), 0, 0, 12, 0xE68C5A, 1);
  }
}

// OFFSET: 0x007CB890
// FUNCTION: fn_09475
void UICashEmotionPack_Unpurchase::Slot26(int, int) {
  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, 171, this->unknown_00A8, 0, 0, 0);
  }
}

// OFFSET: 0x007CB8C0
// FUNCTION: fn_09476
void UICashEmotionPack_Unpurchase::Slot25(int, int) {
  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, 171, this->unknown_00A8, 0, 0, 0);
  }
}

// OFFSET: 0x007CB8F0
// FUNCTION: fn_09477
void UICashEmotionPack_Unpurchase::SetPressed(bool pressed, bool force) {
  if (force || pressed != this->m_pressed) {
    if (!pressed) {
      this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_normal.bmp");
    } else {
      this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_press.bmp");
    }

    this->m_pressed = pressed;
    this->Invalidate();
  }
}

// OFFSET: 0x007CB970
// FUNCTION: fn_09478
int UICashEmotionPack_Unpurchase::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message != 174) {
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, 171, this->unknown_00A8, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x007CB9F0
// FUNCTION: fn_09479
UICashEmotionPiece::UICashEmotionPiece(UIWindowContext& windows, UICashEmotionPieceContext& controls)
    : UIWindow(windows, 0), m_packId(0), m_emotionId(0), m_button(nullptr), m_hoverable(false), m_hovered(false), m_controls(&controls) {
}

// OFFSET: 0x007CBB30
// FUNCTION: fn_09481
void UICashEmotionPiece::Set(int packId, int emotionId, bool showTooltip, bool hoverable, bool locked) {
  this->m_packId = packId;
  this->m_emotionId = emotionId;
  this->m_hoverable = hoverable;

  CashEmotionData* data = g_cashEmotionMgr->GetEmotionData(packId, emotionId);
  if (!data) {
    if (this->m_button) {
      this->m_button->Enable(0);
    }
    this->Invalidate();
    return;
  }

  if (this->m_button) {
    this->m_button->SetSprite(g_cashEmotionMgr->GetSprFileName(data->actId));
    this->m_button->SetActName(g_cashEmotionMgr->GetPackName(data->sprId));
    this->m_button->unknown_00C0 = std::max(data->unknown_0024, 0);
    this->m_button->unknown_00BC = std::max(data->unknown_0020, 0);
    if (showTooltip) {
      this->m_button->SetTooltip(data->name.c_str());
    } else {
      this->m_button->m_tooltip.clear();
    }
    this->m_button->Enable(1);
    this->m_button->SetSelected(locked);
    this->m_button->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x007CBC70
// FUNCTION: fn_09482
void UICashEmotionPiece::Slot15(int width, int height) {
  this->m_button = new (std::nothrow) UICashEmotionSprBtn(*this->m_context, this->m_controls->buttons, this);
  if (this->m_button) {
    this->m_button->SetSize(width, height);
    this->m_button->Move(0, 0);
    this->m_button->SetId(189);
    this->AddChild(this->m_button);
  }
}

// OFFSET: 0x007CBD30
// FUNCTION: fn_09483
void UICashEmotionPiece::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_hoverable && this->m_hovered) {
    unsigned red;
    unsigned green;
    unsigned blue;
    SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
    DrawRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, RGB(red, green, blue));
  }
}

// OFFSET: 0x007CBDA0
// FUNCTION: fn_09484
void UICashEmotionPiece::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 175, this->m_packId, this->m_emotionId, 0, 0);
  }
}

// OFFSET: 0x007CBDD0
// FUNCTION: fn_09485
void UICashEmotionPiece::Slot47() {
  this->m_hovered = true;
  if (this->m_hoverable) {
    this->SendMsg(this, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007CBE00
// FUNCTION: fn_09486
void UICashEmotionPiece::Slot48() {
  this->m_hovered = false;
  if (this->m_hoverable) {
    this->SendMsg(this, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007CBE30
// FUNCTION: fn_09487
void UICashEmotionPiece::Slot19() {
  if (this->m_button && this->m_button->unknown_00C4 != 0) {
    this->m_button->Invalidate();
  }
}

// OFFSET: 0x007CBE50
// FUNCTION: fn_09488
void UICashEmotionPiece::Slot35(int argument) {
  if (this->parent) {
    this->parent->Slot35(argument);
  }
}

// OFFSET: 0x007CBE70
// FUNCTION: fn_09489
int UICashEmotionPiece::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 6:
    if (argument1 == 189 && this->parent) {
      this->parent->SendMsg(this, 174, this->m_packId, this->m_emotionId, 0, 0);
    }
    return 0;
  case 60:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(this, 60, 0, 0, 0, 0);
    }
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CC060
// FUNCTION: fn_09492
void UICashEmotionScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const int height = this->geometry.height;
  DrawRectangleBorder(*this, 0, 0, this->geometry.width, height, 0xDECEBD);

  bool disabled;
  const char* name;
  if (!this->m_disabled && this->maximum != 0) {
    disabled = false;
    if (this->state == 1) {
      name = "유저인터페이스\\cashemotion\\scroll_btn_up_over.bmp";
    } else {
      name = this->state != 4 ? "유저인터페이스\\cashemotion\\scroll_btn_up_normal.bmp" : "유저인터페이스\\cashemotion\\scroll_btn_up_press.bmp";
    }
  } else {
    disabled = true;
    name = "유저인터페이스\\cashemotion\\scroll_btn_up_diasble.bmp";
  }

  auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (up) {
    this->DrawBitmap(0, -1, &up->image, false);
  }

  if (disabled) {
    name = "유저인터페이스\\cashemotion\\scroll_btn_down_disable.bmp";
  } else if (this->state == 2) {
    name = "유저인터페이스\\cashemotion\\scroll_btn_down_over.bmp";
  } else {
    name = this->state != 5 ? "유저인터페이스\\cashemotion\\scroll_btn_down_normal.bmp" : "유저인터페이스\\cashemotion\\scroll_btn_down_press.bmp";
  }

  auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (down) {
    this->DrawBitmap(0, height - this->arrowLength + 1, &down->image, false);
  }

  if (disabled) {
    return;
  }

  const int top = this->ThumbOffset() + this->arrowLength - 2;
  const int length = this->ThumbLength();
  int topHeight = 0;
  auto* topBar = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\scrollbar_top.bmp")));
  if (topBar) {
    topHeight = topBar->image.height;
    this->DrawBitmap(0, top, &topBar->image, true);
  }

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\scrollbar_mid.bmp")));
  auto* bottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\scrollbar_bottom.bmp")));
  const int bottomHeight = bottom ? bottom->image.height : 0;

  int end = top + (length + 4);
  if (middle) {
    const int step = middle->image.height;
    end = (length + 4) + (top - bottomHeight);
    for (int y = top + topHeight; y < end; y += step) {
      this->DrawBitmap(0, y, &middle->image, true);
    }
  }

  if (bottom) {
    this->DrawBitmap(0, end, &bottom->image, true);
  }
}

// OFFSET: 0x007CC290
// FUNCTION: fn_09493
void UICashEmotionScrollBar::SetDisabled(bool disabled) {
  if (disabled != this->m_disabled) {
    this->m_disabled = disabled;
    this->Invalidate();
  }
}

// OFFSET: 0x007CC5D0
// FUNCTION: fn_09496
void UICashEmotionScrollWnd::Slot15(int, int) {
  this->RemoveChild(this->horizontalBar, 1);
  this->RemoveChild(this->scrollBar, 1);
  this->horizontalBar = nullptr;
  this->scrollBar = nullptr;
  this->scrollBarWidth = 11;
}

// OFFSET: 0x007CC870
// FUNCTION: fn_09499
UICashEmotionSprBtn::UICashEmotionSprBtn(UIWindowContext& windowContext, UIButtonContext& buttonContext, UIWindow* target)
    : UISpriteButton2(windowContext, buttonContext, target), m_selected(false) {
}

// OFFSET: 0x007CCE10
// FUNCTION: fn_09503
void UICashEmotionSprBtn::SetSelected(bool selected) {
  if (selected != this->m_selected) {
    this->m_selected = selected;
    this->Invalidate();
  }
}

// OFFSET: 0x008502C0
// FUNCTION: fn_10674
UISpriteButton2::UISpriteButton2(UIWindowContext& windowContext, UIButtonContext& buttonContext, UIWindow* target)
    : UISpriteButton(windowContext, buttonContext), m_target(target) {
}

// OFFSET: 0x00850320
// FUNCTION: fn_10675
UISpriteButton::UISpriteButton(UIWindowContext& windowContext, UIButtonContext& buttonContext) : UIButton(windowContext, buttonContext) {
  this->unknown_00BC = 0;
  this->unknown_00C0 = 0;
  this->unknown_00B4 = 0;
  this->unknown_00B8 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00C8 = 1;
}

// OFFSET: 0x00851C10
// FUNCTION: fn_10697
UISpriteButton2::~UISpriteButton2() {
  this->m_target = nullptr;
}

// OFFSET: 0x00851C70
// FUNCTION: fn_10698
UISpriteButton::~UISpriteButton() = default;

// OFFSET: 0x0085CF90

// OFFSET: 0x0085D6C0

// OFFSET: 0x0085E9B0
// FUNCTION: fn_10834
void UISpriteButton2::Slot26(int x, int y) {
  this->Slot25(x, y);
  if (this->m_target) {
    this->m_target->Slot26(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085E9F0
// FUNCTION: fn_10835
void UISpriteButton::Slot26(int x, int y) {
  this->Slot25(x, y);
  this->Root()->Slot26(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085ED70
// FUNCTION: fn_10847
void UISpriteButton2::Slot25(int x, int y) {
  UIButton::Slot25(x, y);
  if (this->m_target) {
    this->m_target->Slot25(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085EDC0
// FUNCTION: fn_10848
void UISpriteButton::Slot25(int x, int y) {
  UIButton::Slot25(x, y);
  this->Root()->Slot25(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085F160
// FUNCTION: fn_10857
void UISpriteButton2::Slot47() {
  if (this->m_target) {
    this->m_target->Slot47();
  }
}

// OFFSET: 0x0085F480
// FUNCTION: fn_10863
void UISpriteButton2::Slot28(int x, int y) {
  UIButton::Slot28(x, y);
  if (this->m_target) {
    this->m_target->Slot28(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085F4C0
// FUNCTION: fn_10864
void UISpriteButton::Slot28(int x, int y) {
  UIButton::Slot28(x, y);
  this->Root()->Slot28(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085F5E0
// FUNCTION: fn_10867
void UISpriteButton2::Slot48() {
  if (this->m_target) {
    this->m_target->Slot48();
  }
}

// OFFSET: 0x0085F7A0
// FUNCTION: fn_10870
void UISpriteButton2::Slot27(int x, int y) {
  UIButton::Slot27(x, y);
  if (this->m_target) {
    this->m_target->Slot27(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085F870
// FUNCTION: fn_10871
void UISpriteButton::Slot27(int x, int y) {
  this->Root()->Slot27(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085FFF0
// FUNCTION: fn_10895
void UISpriteButton2::Slot35(int argument) {
  if (this->m_target) {
    this->m_target->Slot35(argument);
  }
}

// OFFSET: 0x00865380
// FUNCTION: fn_10924
void UISpriteButton::SetActName(const char* name) {
  this->actionName = name;
}

// OFFSET: 0x00867AD0
// FUNCTION: fn_10946
void UISpriteButton::SetSprite(const char* name) {
  char baseName[256];
  std::size_t length = std::strlen(name);
  std::strcpy(baseName, name);
  baseName[length - 3] = '\0';
  this->spriteName = baseName;
  this->spriteName += "spr";
  this->actionName = baseName;
  this->actionName += "act";
}
