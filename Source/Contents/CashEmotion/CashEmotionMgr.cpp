#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Window/UIEnums.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UISpriteButton.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Network/TextEncoding.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/CashEmotion/UICashEmotionSprBtn.h"
#include "UI/CashEmotion/UICashEmotionPack.h"
#include "UI/CashEmotion/UICashEmotionPiece.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Items/UILapineScrollBar.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UINormalMB.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/EmotionSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "UI/Emotion/UIEmotionWnd.h"
#include "Client/Mode/CMode.h"

#include "Resource/MSGSTRINGID.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

#include <algorithm>
#include <array>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <format>
#include <functional>
#include <iterator>
#include <map>
#include <new>
#include <windows.h>

// STATIC: 0x013896C4
// GLOBAL: gl_24350
CCashEmotionMgr* g_cashEmotionMgr = nullptr;

extern "C" long long _Xtime_get_ticks();

template __declspec(noinline) std::unordered_map<std::string, CashEmotionCommand*>::iterator
std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::find<void>(const std::string&);

class ICashEmotionAction {
public:
  virtual ~ICashEmotionAction();
  virtual bool Execute(int packId, int argument) = 0;

  bool ExecuteNamed(const char* name);
};

class CCashEmotionAction_Play : public ICashEmotionAction {
public:
  CCashEmotionAction_Play();
  ~CCashEmotionAction_Play() override;
  bool Execute(int packId, int emotionId) override;

protected:
  /* 0004 */ int m_diceEmotion;
  /* 0008 */ int m_yutEmotion;
};

class CCashEmotionAction_Register : public ICashEmotionAction {
public:
  // INLINED
  CCashEmotionAction_Register() {
  }
  bool Execute(int packId, int emotionId) override;
};

class CCashEmotionAction_SelectedUnPurchasedPack : public ICashEmotionAction {
public:
  bool Execute(int packId, int argument) override;
};

class UICashEmotionPack_Purchase : public UICashEmotionPack {
public:
  explicit __declspec(noinline) UICashEmotionPack_Purchase();
  ~UICashEmotionPack_Purchase() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void UpdateExpiry(Timestamp* expiry) override;
  void ShowExpiry(__time32_t expiry) override;
  int SetPack(int packId, bool showTooltip, bool hoverable) override;

protected:
  /* 00E0 */ ICashEmotionAction* m_selectAction;
  /* 00E4 */ ICashEmotionAction* m_purchaseAction;
};

class UICashEmotionPack_Search : public UICashEmotionPack {
public:
  // INLINED
  explicit UICashEmotionPack_Search() : UICashEmotionPack() {
  }
  // INLINED
  ~UICashEmotionPack_Search() override {
  }
  void Paint() override;

  int SetResults(const char* title, const std::vector<std::pair<int, int>>& results);
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
  explicit UICashEmotionPack_Search_UnPurchased();
  ~UICashEmotionPack_Search_UnPurchased() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00E0 */ ICashEmotionAction* m_action;
};

class UICashEmotionPack_Unpurchase : public UICashEmotionPack {
  friend class UICashEmotionListChildWnd_UnPurchased;

public:
  explicit __declspec(noinline) UICashEmotionPack_Unpurchase();
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  int SetPack(int packId, bool showTooltip, bool hoverable) override;

  __declspec(noinline) void SetPressed(bool pressed, bool force);

protected:
  /* 00E0 */ bool m_pressed;
  /* 00E4 */ std::string priceText;
};

class UICashEmotionScrollBar : public UIScrollBar {
public:
  __declspec(noinline) explicit UICashEmotionScrollBar();

  void Paint() override;

  void SetDisabled(bool disabled);

protected:
  /* 00B4 */ bool m_disabled;
};

class UICashEmotionScrollWnd : public UIScrollWnd {
public:
  __declspec(noinline) UICashEmotionScrollWnd(int width, int height, unsigned char cached);
  // INLINED
  ~UICashEmotionScrollWnd() override {
  }
  void OnCreate(int width, int height) override;
  void UpdateScrollRange() override;

  void AddChild(UIWindow* child);
  void UpdateScrollBars();
};

class UICashEmotionListChildWnd_Abstract : public UICashEmotionScrollWnd {
public:
  __declspec(noinline) UICashEmotionListChildWnd_Abstract(int width, int height, unsigned char cached);
  // INLINED
  ~UICashEmotionListChildWnd_Abstract() override {
  }
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual void Refresh(const char* filter) = 0;
  virtual const char* Filter() = 0;
};

class UICashEmotionListChildWnd_Basic : public UICashEmotionListChildWnd_Abstract {
public:
  __declspec(noinline) UICashEmotionListChildWnd_Basic(int width, int height, unsigned char cached);
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh(const char* filter) override;
  const char* Filter() override;

protected:
  /* 00B0 */ std::vector<UIWindow*> m_items;
  /* 00BC */ ICashEmotionAction* m_selectAction;
  /* 00C0 */ ICashEmotionAction* m_playAction;
};

class UICashEmotionListChildWnd_Purchase : public UICashEmotionListChildWnd_Abstract {
public:
  __declspec(noinline) UICashEmotionListChildWnd_Purchase(int width, int height);
  ~UICashEmotionListChildWnd_Purchase() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh(const char* filter) override;
  const char* Filter() override;

  __declspec(noinline) void UpdateExpiry(int packId, Timestamp* expiry);

protected:
  /* 00B0 */ std::vector<UICashEmotionPack*> m_items;
  /* 00BC */ ICashEmotionAction* playAction;
  /* 00C0 */ std::string m_filter;
};

class UICashEmotionListChildWnd_Search : public UICashEmotionListChildWnd_Abstract {
public:
  __declspec(noinline) UICashEmotionListChildWnd_Search(int width, int height);
  ~UICashEmotionListChildWnd_Search() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh(const char* filter) override;
  const char* Filter() override;

  void AddResults(char type, const std::vector<std::pair<int, int>>& results, int& y);

protected:
  /* 00B0 */ std::vector<UICashEmotionPack*> m_items;
  /* 00BC */ ICashEmotionAction* m_action;
  /* 00C0 */ std::string m_filter;
};

class UICashEmotionListChildWnd_UnPurchased : public UICashEmotionListChildWnd_Abstract {
public:
  __declspec(noinline) UICashEmotionListChildWnd_UnPurchased(int width, int height);
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Refresh(const char* filter) override;
  const char* Filter() override;

protected:
  /* 00B0 */ std::vector<UICashEmotionPack*> m_items;
  /* 00BC */ std::string m_filter;
};

#pragma pack(push, 1)
struct CashEmotionPurchasePacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint16_t packId;
  /* 0004 */ std::uint16_t itemId;
  /* 0006 */ std::uint8_t price;
};
#pragma pack(pop)

static_assert(sizeof(CashEmotionPurchasePacket) == 7);

class UICashEmotionListWnd : public UIFrameWnd {
public:
  UICashEmotionListWnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  UIBitmapButton* CreateButton(int x, int y, int width, int height, int id, const char* format, bool hasDisabled);
  void RefreshCurrentList();
  __declspec(noinline) void ShowPack(int packId);
  void RefreshPackLists();
  __declspec(noinline) void ResetLists();
  int SetPeriod(int packId);
  void SetTab(char tab);
  void StartExpirySchedule();
  __declspec(noinline) void UpdateExpiry(int packId, Timestamp* expiry);
  __declspec(noinline) void RefreshSelection();
  void RestartExpirySchedule();

protected:
  /* 00B4 */ CashEmotionListWndInfo* m_placement;
  /* 00B8 */ UICashEmotionListChildWnd_Basic* m_basicList;
  /* 00BC */ UICashEmotionListChildWnd_Purchase* m_purchaseList;
  /* 00C0 */ UICashEmotionListChildWnd_UnPurchased* m_unpurchasedList;
  /* 00C4 */ UICashEmotionListChildWnd_Search* m_searchList;
  /* 00C8 */ std::map<char, UIWindow*> m_tabs;
  /* 00D0 */ char m_tab;
  /* 00D4 */ UIBitmapButton* m_backButton;
  /* 00D8 */ UIEditCtrl* m_searchEdit;
  /* 00DC */ UIBitmapButton* m_searchButton;
  /* 00E0 */ UIBitmapButton* m_shortcutButton;
  /* 00E4 */ UIBitmapButton* m_buyButton;
  /* 00E8 */ UICashEmotionListChildWnd_Abstract* m_currentList;
  /* 00EC */ UIStaticText* m_period;
};

class UICashEmotionMiniWnd : public UIWindow {
public:
  UICashEmotionMiniWnd();
  ~UICashEmotionMiniWnd() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  __declspec(noinline) void Select(CashEmotionCommand* command);
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
};

// OFFSET: 0x00591CA0
// FUNCTION: fn_02839
CCashEmotionAction_Play::CCashEmotionAction_Play() : m_diceEmotion(0), m_yutEmotion(0) {
  this->m_diceEmotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Dice);
  this->m_yutEmotion = g_cashEmotionMgr->GetSpecialEmotion(CashEmotionSpecial::Yut);
}

// OFFSET: 0x00591D30
// FUNCTION: fn_02840
CCashEmotionAction_Play::~CCashEmotionAction_Play() {
}

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

  const int emotionId = g_cashEmotionMgr->FindEmotionId(name);
  return this->Execute(g_cashEmotionMgr->BasePackId(), emotionId);
}

// OFFSET: 0x00591EB0
// FUNCTION: fn_02845
bool CCashEmotionAction_Play::Execute(int packId, int emotionId) {
  if (packId < 0 || emotionId < 0) {
    return false;
  }

  CMode* mode = g_modeMgr.gameMode;
  if (!mode) {
    return false;
  }

  if (this->m_diceEmotion == emotionId) {
    emotionId = std::ranges::max(rand() % 6, 0) + this->m_diceEmotion;
  } else if (this->m_yutEmotion == emotionId) {
    emotionId = std::ranges::max(rand() % 7, 0) + this->m_yutEmotion;
  }

  mode->SendMsg(MM_REQ_EMOTION, emotionId, packId, 0, 0);
  return true;
}

// OFFSET: 0x00591F30
// FUNCTION: fn_02846
bool CCashEmotionAction_Register::Execute(int packId, int emotionId) {
  if (packId < 0 || emotionId < 0) {
    return false;
  }

  const char* name = g_cashEmotionMgr->GetEmotionName(packId, emotionId);
  if (!name) {
    return false;
  }

  if (auto* emotionWnd = reinterpret_cast<UIEmotionWnd*>(g_windowMgr.emotionWnd)) {
    emotionWnd->SetMacroText(name, -1);
  } else {
    UINewChatWnd* suggestions = ChatWindow();
    if (suggestions && suggestions->m_chatEdit) {
      suggestions->m_chatEdit->SetText(name);
      suggestions->m_chatEdit->Invalidate();
    }
  }

  return true;
}

// OFFSET: 0x00591FB0
// FUNCTION: fn_02847
bool CCashEmotionAction_SelectedUnPurchasedPack::Execute(int packId, int) {
  if (g_windowMgr.inputSuggestionsEnabled == 1 && ChatWindow()) {
    UINewChatWnd* suggestions = ChatWindow();
    suggestions->ShowSuggestions(0);
    if (suggestions->m_chatEdit) {
      suggestions->m_chatEdit->SetText("");
    }
    suggestions->ToggleInput();
  }

  UICashEmotionListWnd* listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd);
  if (!listWnd) {
    listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.EnsureWindow(WID_CASHEMOTIONLISTWND));
  }

  const unsigned int now = g_cashEmotionMgr->GetServerTime();
  if (g_cashEmotionMgr->IsPackAvailable(now, packId)) {
    if (listWnd) {
      listWnd->ShowPack(packId);
    }
    return true;
  }

  const unsigned int endTime = g_cashEmotionMgr->GetPackEndTime(packId);
  if (endTime) {
    const char* key = now < endTime ? "EMSG_NOT_YET_SALE_START_TIME" : "EMSG_EMOTION_EXPANTION_FAIL_DATE";
    const char* message = g_simpleStringMgr->GetString(key);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(message), 255, 0, 0, 0);
  } else if (g_cashEmotionMgr->GetPackStartTime(packId) > now) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_NOT_YET_SALE_START_TIME")), 255, 0, 0, 0);
  }

  return true;
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
__declspec(noinline) std::pair<int, int> CashEmotionCommand::Get(int index) const {
  if (index < 0 || static_cast<std::size_t>(index) >= this->entries.size()) {
    return std::pair<int, int>(-1, -1);
  }

  return this->entries[index];
}

// OFFSET: 0x00592520
// FUNCTION: fn_02856
CashEmotionData::CashEmotionData(int id, int subId, const char* name, int actionIndex, int frameIndex, int actId, int sprId)
    : id(id), subId(subId), name(name), actionIndex(actionIndex), frameIndex(frameIndex), actId(actId), sprId(sprId) {
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
    : m_enabled(true), m_action(nullptr), basePackId(0), packs{}, m_emotionsByPack{}, emotionOrder{}, m_packNames{}, actIds{},
      m_commands{}, packOrder{} {
}

// OFFSET: 0x00598520
// FUNCTION: fn_02928
CCashEmotionMgr::~CCashEmotionMgr() {
  if (this->m_action) {
    delete this->m_action;
    this->m_action = nullptr;
  }

  g_timeScheduler->RemoveSchedulesByKey(5);
  g_timeScheduler->RemoveSchedulesByKey(6);
  g_timeScheduler->RemoveSchedulesByKey(7);
}

// OFFSET: 0x00598C70
// FUNCTION: fn_02934
void CCashEmotionMgr::Reset() {
  this->packs.clear();
  this->m_emotionsByPack.clear();
  this->emotionOrder.clear();
  this->m_searchIndex.clear();
  this->m_packNames.clear();
  this->actIds.clear();
  this->m_lastActId = 0;
  this->m_sprNames.clear();
  this->m_sprIds.clear();
  this->m_lastSprId = 0;

  for (auto& shortcut : this->m_shortcuts) {
    delete shortcut.second;
  }
  this->m_shortcuts.clear();

  this->ClearCommands();
  this->packOrder.clear();
  this->m_ownedPacks.clear();
  this->m_unownedPacks.clear();
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

  this->m_commands.clear();
}

// OFFSET: 0x005994B0
// FUNCTION: fn_02939
int CCashEmotionMgr::FindEmotionId(const char* name) {
  if (!name || std::strlen(name) == 0) {
    return -1;
  }

  std::string key(name);
  auto it = this->m_emotionIds.find(key);
  if (it != this->m_emotionIds.end()) {
    return it->second;
  }

  return -1;
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
  if (ChatWindow()) {
    ChatWindow()->ShowSuggestions(0);
  }
}

// OFFSET: 0x00599040
// FUNCTION: fn_02938
void CCashEmotionMgr::RefreshCommands() {
  this->ClearCommands();

  const unsigned int now = _time32(nullptr) - this->serverTimeOffset - this->intendedDelay;
  for (auto& shortcut : this->m_shortcuts) {
    for (const std::pair<int, int>& entry : shortcut.second->entries) {
      const int packId = entry.first;
      const int emotionId = entry.second;
      if (packId != this->basePackId && !this->IsOwnedPack(packId) && !this->IsPackAvailable(now, packId)) {
        continue;
      }

      auto it = this->m_commands.find(shortcut.first);
      if (it == this->m_commands.end()) {
        CashEmotionCommand* command = new CashEmotionCommand(shortcut.first, packId, emotionId);
        this->m_commands.try_emplace(shortcut.first, reinterpret_cast<int>(command));
      } else {
        reinterpret_cast<CashEmotionCommand*>(it->second)->Add(packId, emotionId);
      }
    }
  }
}

// OFFSET: 0x00599730
// FUNCTION: fn_02942
void CCashEmotionMgr::ProcessChatCommand(const char* command) {
  UINewChatWnd* chatWnd = ChatWindow();
  if (!chatWnd) {
    return;
  }

  UIWindow* suggestions = chatWnd->m_suggestions;
  if (!suggestions) {
    return;
  }

  if (!command || std::strlen(command) < 1 || command[0] != '/') {
    chatWnd->ShowSuggestions(0);
    return;
  }

  int actionId;
  {
    ++command;
    std::string key(command);
    auto it = this->m_commands.find(key);
    actionId = this->m_commands.end() != it ? it->second : 0;
  }

  if (actionId == 0) {
    this->HideSuggestions();
  } else {
    chatWnd->ShowSuggestions(1);
    reinterpret_cast<UICashEmotionMiniWnd*>(suggestions)->Select(reinterpret_cast<CashEmotionCommand*>(actionId));
  }
}

// OFFSET: 0x00599840
// FUNCTION: fn_45457
int CCashEmotionMgr::GetActFilePath(const char* name, bool add) {
  return this->RegisterFileName(this->actIds, this->m_packNames, [this]() { return ++this->m_lastActId; }, name, add);
}

// INLINED
static const char* FindFileName(CashEmotionFileNames& names, int id) {
  auto it = names.find(id);
  if (names.end() == it) {
    return nullptr;
  }

  return it->second.c_str();
}

// OFFSET: 0x005998C0
// FUNCTION: fn_02943
const char* CCashEmotionMgr::GetPackName(int packId) {
  return FindFileName(this->m_packNames, packId);
}

// OFFSET: 0x00599930
// FUNCTION: fn_02944
std::vector<int> CCashEmotionMgr::FindBaseEmotions(const char* filter) {
  std::vector<int> result;

  auto it = this->emotionOrder.find(this->basePackId);
  if (it == this->emotionOrder.end()) {
    return result;
  }

  result.reserve(it->second.size());
  if (filter) {
    std::vector<int> others;
    others.reserve(it->second.size());

    for (int emotionId : it->second) {
      auto pack = this->m_emotionsByPack.find(this->basePackId);
      if (pack == this->m_emotionsByPack.end()) {
        continue;
      }

      auto emotion = pack->second.find(emotionId);
      if (emotion == pack->second.end()) {
        continue;
      }

      const char* name = emotion->second.name.c_str();
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
  const int packId = this->basePackId;
  auto it = this->m_emotionsByPack.find(packId);
  return this->m_emotionsByPack.end() != it ? &it->second : nullptr;
}

// OFFSET: 0x00599CA0
// FUNCTION: fn_02946
int CCashEmotionMgr::GetSpecialEmotion(CashEmotionSpecial type) {
  auto it = this->m_specials.find(type);
  if (this->m_specials.end() != it) {
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
  if (this->m_emotionsByPack.end() == packIt) {
    return nullptr;
  }

  auto emoIt = packIt->second.find(emotionId);
  if (packIt->second.end() == emoIt) {
    return nullptr;
  }

  return &emoIt->second;
}

// OFFSET: 0x00599D70
// FUNCTION: fn_02948
const char* CCashEmotionMgr::GetEmotionName(int packId, int emotionId) {
  auto packIt = this->m_emotionsByPack.find(packId);
  if (this->m_emotionsByPack.end() == packIt) {
    return nullptr;
  }

  auto emoIt = packIt->second.find(emotionId);
  if (packIt->second.end() == emoIt) {
    return nullptr;
  }

  return emoIt->second.name.c_str();
}

// OFFSET: 0x00599DE0
// FUNCTION: fn_02949
std::vector<int> CCashEmotionMgr::GetPackEmotions(int packId) {
  std::vector<int> result;

  auto it = this->emotionOrder.find(packId);
  if (this->emotionOrder.end() == it) {
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
    earliest = std::min<int>(earliest, expiry.second.seconds);
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
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
    return nullptr;
  }

  return &it->second;
}

// OFFSET: 0x0059A0C0
// FUNCTION: fn_02953
const char* CCashEmotionMgr::GetPackTitle(int packId) {
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
    return nullptr;
  }

  return it->second.name.c_str();
}

// OFFSET: 0x0059A110
// FUNCTION: fn_02954
int CCashEmotionMgr::GetPackPrice(int packId) {
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
    return -1;
  }

  return it->second.price;
}

// OFFSET: 0x0059A150
// FUNCTION: fn_02955
Timestamp* CCashEmotionMgr::FindExpiry(int packId) {
  auto it = std::find_if(this->m_expiry.begin(), this->m_expiry.end(), [packId](const auto& entry) { return entry.first == packId; });

  if (this->m_expiry.end() != it) {
    return &it->second;
  }

  return nullptr;
}

// OFFSET: 0x0059A1C0
// FUNCTION: fn_02956
std::vector<int> CCashEmotionMgr::SortOwnedPacks(const char* filter) {
  return this->SortPacks(this->m_ownedPacks, filter);
}

// OFFSET: 0x0059A220
// FUNCTION: fn_02957
int CCashEmotionMgr::RegisterFileName(
  std::unordered_map<std::string, int>& ids, CashEmotionFileNames& names, std::function<int()> nextId, const char* name, bool add
) {
  std::string lowered(name);
  std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) { return static_cast<char>(tolower(c)); });

  auto found = ids.find(name);
  if (found != ids.end()) {
    return found->second;
  }

  if (add == true) {
    std::function<int()> generator = nextId;
    int id = generator();
    if (names.find(id) == names.end()) {
      auto inserted = ids.emplace(lowered, id);
      if (inserted.second == true) {
        names.emplace(id, inserted.first->first.c_str());
        return id;
      }
    }
  }

  return -1;
}

// OFFSET: 0x0059A5B0
// FUNCTION: fn_02958
unsigned int CCashEmotionMgr::GetPackEndTime(int packId) {
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
    return 0;
  }

  return it->second.endTime;
}

// OFFSET: 0x0059A5F0
// FUNCTION: fn_02959
unsigned int CCashEmotionMgr::GetPackStartTime(int packId) {
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
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

    if (this->basePackId == entry.second.first) {
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

// OFFSET: 0x0059ABC0
// FUNCTION: fn_45458
int CCashEmotionMgr::GetSprFilePath(const char* name, bool add) {
  return this->RegisterFileName(this->m_sprIds, this->m_sprNames, [this]() { return ++this->m_lastSprId; }, name, add);
}

// OFFSET: 0x0059AC40
// FUNCTION: fn_02963
const char* CCashEmotionMgr::GetSprFileName(int sprId) {
  return FindFileName(this->m_sprNames, sprId);
}

// OFFSET: 0x0059ACE0
// FUNCTION: fn_02964
std::vector<int> CCashEmotionMgr::SortUnownedPacks(const char* filter) {
  return this->SortPacks(this->m_unownedPacks, filter);
}

// OFFSET: 0x0059AD40
// FUNCTION: fn_02965
int CCashEmotionMgr::GetPackPeriod(int packId) {
  auto it = this->packs.find(packId);
  if (this->packs.end() == it) {
    return 0;
  }

  return it->second.period;
}

// OFFSET: 0x0059AD80
// FUNCTION: fn_02966
void CCashEmotionMgr::LoadScripts() {
  g_session.m_hotkeyScript->Register("c_InsertPackInfo", InsertPackInfo);
  g_session.m_hotkeyScript->Register("c_InsertEmotionEnum", InsertEmotionEnum);
  g_session.m_hotkeyScript->Register("c_InsertEmotionListTable", InsertEmotionListTable);
  g_session.m_hotkeyScript->Register("c_InsertEmotionMsgListTable", InsertEmotionMsgListTable);
  g_session.m_hotkeyScript->Register("c_InsertEmotionMsgListTableForCustom", InsertEmotionMsgListTableForCustom);
  g_session.m_hotkeyScript->Register("c_InsertEmotionVarientListTable", InsertEmotionVarientListTable);
  g_session.m_hotkeyScript->Register("c_InsertEmotionGroup", InsertEmotionGroup);
  g_session.m_hotkeyScript->Register("c_InsertOrderList", InsertOrderList);

  g_session.m_hotkeyScript->Load("Lua Files\\CashEmotion\\CashEmotionClientSetting", true, false);
  g_session.m_hotkeyScript->Call("getEmotionClientSetting", "s>d", "IntendedDelay", &this->intendedDelay);

  g_session.m_hotkeyScript->Load("Lua Files\\CashEmotion\\CashEmotionListDefine_F", true, false);
  g_session.m_hotkeyScript->Load("Lua Files\\CashEmotion\\CashEmotionListDefine", true, false);
  g_session.m_hotkeyScript->Load("Lua Files\\CashEmotion\\CashEmotionList_F", true, false);
  g_session.m_hotkeyScript->Load("Lua Files\\CashEmotion\\CashEmotionList", true, false);
  g_session.m_hotkeyScript->Call("main_emotion", "");

  this->m_action = new (std::nothrow) CCashEmotionAction_Play;
}

// OFFSET: 0x0059B320
// FUNCTION: fn_02967
int CCashEmotionMgr::LookupSpecialEmotion(CashEmotionSpecial type) {
  switch (type) {
  case CashEmotionSpecial::ChatProhibit: {
    int emotionId;
    {
      std::string key("ET_CHAT_PROHIBIT");
      auto it = this->m_emotionIds.find(key);
      emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    }
    this->m_specials.emplace(type, emotionId);
    return emotionId;
  }
  case CashEmotionSpecial::Yut: {
    int emotionId;
    {
      std::string key("ET_YUT1");
      auto it = this->m_emotionIds.find(key);
      emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    }
    this->m_specials.emplace(type, emotionId);
    return emotionId;
  }
  case CashEmotionSpecial::Dice: {
    int emotionId;
    {
      std::string key("ET_DICE1");
      auto it = this->m_emotionIds.find(key);
      emotionId = it != this->m_emotionIds.end() ? it->second : -1;
    }
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
  if (this->m_shortcuts.end() != it) {
    it->second->Add(packId, emotionId);
    return;
  }

  CashEmotionCommand* shortcut = new (std::nothrow) CashEmotionCommand(name, packId, emotionId);
  if (shortcut) {
    this->m_shortcuts.try_emplace(name, shortcut);
  }
}

// OFFSET: 0x0059B8F0
// FUNCTION: fn_02970
bool CCashEmotionMgr::AddEmotion(int packId, int emotionId, const char* name, int actionIndex, int frameIndex, const char* actFile, const char* sprFile) {
  const int actId = this->GetActFilePath(actFile, true);
  const int sprId = this->GetSprFilePath(sprFile, true);

  bool inserted = false;
  auto it = this->m_emotionsByPack.find(packId);
  if (this->m_emotionsByPack.end() != it) {
    auto result = it->second.try_emplace(emotionId, packId, emotionId, name, actionIndex, frameIndex, actId, sprId);
    return result.second;
  }

  CashEmotionTable table;
  auto result = table.try_emplace(emotionId, packId, emotionId, name, actionIndex, frameIndex, actId, sprId);
  if (result.second == true) {
    inserted = this->m_emotionsByPack.emplace(packId, table).second;
  }

  return inserted;
}

// INLINED
static void AddSearchKey(std::unordered_map<std::string, std::pair<int, int>>& index, const char* key, int packId, int emotionId) {
  if (!key || std::strlen(key) == 0) {
    return;
  }

  index.try_emplace(key, packId, emotionId);
}

// OFFSET: 0x0059BC30
// FUNCTION: fn_02971
void CCashEmotionMgr::InsertEmotion(const int packId, const int emotionId, int actionIndex, int frameIndex, const char* actFile, const char* sprFile) {
  const char* packName = this->GetPackTitle(packId);
  if (!packName) {
    return;
  }

  const char* baseName = this->GetEmotionName(this->basePackId, emotionId);
  if (!baseName) {
    return;
  }

  std::string name = FormatOwnedString("%s_%s", baseName, packName);
  this->AddEmotion(packId, emotionId, name.c_str(), actionIndex, frameIndex, actFile, sprFile);

  AddSearchKey(this->m_searchIndex, name.c_str(), packId, emotionId);

  if (std::strlen(baseName) > 1) {
    this->AddShortcut(baseName + 1, packId, emotionId);
  }
}

// OFFSET: 0x0059C4F0
// FUNCTION: fn_02976
bool CCashEmotionMgr::PlaySearchedEmotion(const std::string& name) {
  if (this->m_action) {
    auto found = this->m_searchIndex.find(name);
    const std::pair<int, int>* emotion = found != this->m_searchIndex.end() ? &found->second : nullptr;
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

  std::vector<int>& owned = this->m_ownedPacks;
  if (owned.end() == std::find(owned.begin(), owned.end(), packId)) {
    owned.push_back(packId);

    auto unowned = std::find(this->m_unownedPacks.begin(), this->m_unownedPacks.end(), packId);

    if (this->m_unownedPacks.end() != unowned) {
      this->m_unownedPacks.erase(unowned);
    }
  }

  auto it = this->m_expiry.begin();
  for (; this->m_expiry.end() != it; ++it) {
    if (it->first == packId) {
      break;
    }
  }

  if (this->m_expiry.end() != it) {
    if (expiry) {
      it->second.Set(expiry);
    } else {
      this->m_expiry.erase(it);
    }
    return;
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
  return this->m_ownedPacks.end() != std::find(this->m_ownedPacks.begin(), this->m_ownedPacks.end(), packId);
}

// OFFSET: 0x0059C5A0
// FUNCTION: fn_02977
bool CCashEmotionMgr::RemoveExpiredPacks() {
  auto* listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd);
  bool expired = false;
  const __time32_t now = _time32(nullptr) - this->serverTimeOffset - this->intendedDelay;

  for (auto it = this->m_expiry.begin(); it != this->m_expiry.end();) {
    if (it->second.IsBefore(now + 1)) {
      this->RemoveOwnedPack(it->first, false, false);
      it = this->m_expiry.erase(it);
      expired = true;
    } else {
      if (listWnd) {
        listWnd->UpdateExpiry(it->first, &it->second);
      }
      ++it;
    }
  }

  return expired;
}

// OFFSET: 0x0059C730
// FUNCTION: fn_02978
void CCashEmotionMgr::RemoveOwnedPack(int packId, bool refresh, bool clearExpiry) {
  auto owned = std::find(this->m_ownedPacks.begin(), this->m_ownedPacks.end(), packId);

  if (this->m_ownedPacks.end() != owned) {
    this->m_ownedPacks.erase(owned);
  }

  auto unowned = std::find(this->m_unownedPacks.begin(), this->m_unownedPacks.end(), packId);

  if (this->m_unownedPacks.end() == unowned) {
    this->m_unownedPacks.push_back(packId);
  }

  if (refresh) {
    if (auto* listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd)) {
      listWnd->RefreshPackLists();
    }
  }

  if (clearExpiry) {
    auto it = std::find_if(this->m_expiry.begin(), this->m_expiry.end(), [packId](const auto& entry) { return packId == entry.first; });

    if (this->m_expiry.end() != it) {
      this->m_expiry.erase(it);
    }
  }
}

// OFFSET: 0x0059C860
// FUNCTION: fn_02979
CashEmotionPackRefresh CCashEmotionMgr::RefreshUnownedPacks() {
  bool changed = false;
  const unsigned int now = _time32(nullptr) - this->serverTimeOffset - this->intendedDelay;

  std::unordered_map<int, int> owned;
  for (int packId : this->m_ownedPacks) {
    owned.emplace(packId, 1);
  }

  std::unordered_map<int, int> unowned;
  for (int packId : this->m_unownedPacks) {
    unowned.emplace(packId, 1);
  }

  std::vector<int> packs;
  const int total = static_cast<int>(this->packOrder.size());
  const int ownedCount = static_cast<int>(this->m_ownedPacks.size());
  packs.reserve(total > ownedCount ? total - ownedCount : 0);

  int nextTime = INT_MAX;
  for (int packId : this->packOrder) {
    CashEmotionPack* pack = this->GetPack(packId);
    if (!pack) {
      continue;
    }

    const unsigned int startTime = pack->startTime;
    const unsigned int endTime = pack->endTime;
    if (pack->IsAvailable(now) && owned.find(packId) == owned.end()) {
      packs.push_back(packId);
    }

    if (startTime > now && static_cast<int>(startTime) < nextTime) {
      nextTime = startTime;
    }
    if (endTime > now && static_cast<int>(endTime) < nextTime) {
      nextTime = endTime;
    }
  }

  for (int packId : packs) {
    if (unowned.find(packId) == unowned.end()) {
      changed = true;
    }
  }

  if (packs != this->m_unownedPacks) {
    changed = true;
  }
  if (changed) {
    this->m_unownedPacks.clear();
    this->m_unownedPacks.reserve(packs.size());
    this->m_unownedPacks = packs;
  }

  return {changed, nextTime};
}

// OFFSET: 0x0059CD60
// FUNCTION: fn_02980
void LuaScript::Register(std::string name, int (*function)(lua_State*)) {
  lua_register(this->m_state, name.c_str(), function);
}

// OFFSET: 0x0059CDE0
// FUNCTION: fn_02981
void CCashEmotionMgr::SetServerTime(int param) {
  this->serverTimeOffset = _time32(nullptr) - param;
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
  this->serverTimezoneOffset = offset;
  for (auto& entry : this->packs) {
    CashEmotionPack& pack = entry.second;
    if (pack.originalStartTime) {
      pack.startTime = pack.originalStartTime - offset;
    }
    if (pack.originalEndTime) {
      pack.endTime = pack.originalEndTime - offset;
    }
  }
}

// OFFSET: 0x0059CEC0
// FUNCTION: fn_02983
void CCashEmotionMgr::ShowErrorMessage(const char* key, int value) {
  const char* format = g_simpleStringMgr->GetString(key);
  if (!format) {
    return;
  }

  std::string text = std::vformat(format, std::make_format_args(value));
  MessageBoxA(nullptr, text.c_str(), std::string().c_str(), MB_OK);
}

// OFFSET: 0x0059D040
// FUNCTION: fn_02984
void CCashEmotionMgr::EmptyHandler59D040(int, int) {
}

// OFFSET: 0x0059D090
// FUNCTION: fn_02985
void CCashEmotionMgr::AcquirePack(int packId, unsigned int expiry) {
  if (packId <= 0) {
    return;
  }

  this->SetOwnedPack(packId, expiry);
  this->selectedPackId = -1;

  UICashEmotionListWnd* listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd);
  if (listWnd) {
    listWnd->ResetLists();
    listWnd->RefreshSelection();
    listWnd->RestartExpirySchedule();
  }
}

// OFFSET: 0x0059D2C0
// FUNCTION: fn_02986
bool CCashEmotionMgr::UpdatePurchasedPackExpiry() {
  auto* listWnd = static_cast<UICashEmotionListWnd*>(g_windowMgr.cashEmotionListWnd);
  if (listWnd) {
    for (auto it = this->m_expiry.begin(); it != this->m_expiry.end();) {
      if (it->second.IsBeforeNow()) {
        this->RemoveOwnedPack(it->first, false, false);
        it = this->m_expiry.erase(it);
      } else {
        listWnd->UpdateExpiry(it->first, &it->second);
        ++it;
      }
    }

    listWnd->RefreshPackLists();
    return this->m_expiry.size() != 0;
  }

  return false;
}

// OFFSET: 0x0059FC50
// FUNCTION: fn_03031
int CCashEmotionMgr::GetServerTime() {
  return _time32(nullptr) - this->serverTimeOffset - this->intendedDelay;
}

// OFFSET: 0x0059FC70
// FUNCTION: fn_03032
int CCashEmotionMgr::InsertEmotionEnum(lua_State* state) {
  std::string name = FromUtf8(luaL_checkstring(state, 1), g_textCodePage);
  int id = luaL_checkinteger(state, 2);
  g_cashEmotionMgr->m_emotionIds.emplace(name, id);
  return 1;
}

// OFFSET: 0x0059FDC0
// FUNCTION: fn_03033
int CCashEmotionMgr::InsertEmotionGroup(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);

  CCashEmotionMgr* mgr = g_cashEmotionMgr;
  const char* title = mgr->GetPackTitle(packId);
  if (title) {
    mgr->AddShortcut(title, packId, emotionId);
  }

  return 1;
}

// OFFSET: 0x0059FF10
// FUNCTION: fn_03034
int CCashEmotionMgr::InsertEmotionListTable(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);
  const std::string key = FromUtf8(luaL_checkstring(state, 3), g_textCodePage);
  const int actionIndex = luaL_checkinteger(state, 4);
  const int frameIndex = luaL_checkinteger(state, 5);
  const std::string actFile = FromUtf8(luaL_checkstring(state, 6), g_textCodePage);
  const std::string sprFile = FromUtf8(luaL_checkstring(state, 7), g_textCodePage);

  const char* result;
  if (key.empty()) {
    g_cashEmotionMgr->AddEmotion(packId, emotionId, "", actionIndex, frameIndex, actFile.c_str(), sprFile.c_str());
    result = nullptr;
  } else {
    const char* name = g_simpleStringMgr->GetString(key.c_str());
    if (!name) {
      name = LookupMessage(FindMessageId(key.c_str()));
    }

    result = g_cashEmotionMgr->AddEmotion(packId, emotionId, name, actionIndex, frameIndex, actFile.c_str(), sprFile.c_str()) ? name : nullptr;
  }

  lua_pushstring(state, result);
  return 1;
}

// OFFSET: 0x005A0290
// FUNCTION: fn_03035
int CCashEmotionMgr::InsertEmotionMsgListTable(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);
  const bool raw = luaL_checkinteger(state, 3) != 0;
  const int count = luaL_checkinteger(state, 4);

  for (int index = 5; index < count + 5; ++index) {
    std::string name = raw ? std::string(luaL_checkstring(state, index)) : FromUtf8(luaL_checkstring(state, index), g_textCodePage);

    const char* key = name.c_str();
    if (key && std::strlen(key) != 0) {
      g_cashEmotionMgr->m_searchIndex.try_emplace(key, packId, emotionId);
    }

    if (name.find('/') == 0) {
      name.erase(0, 1);
    }

    g_cashEmotionMgr->AddShortcut(name, packId, emotionId);
  }

  return 1;
}

// OFFSET: 0x005A07F0
// FUNCTION: fn_03036
int CCashEmotionMgr::InsertEmotionMsgListTableForCustom(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);
  const int baseEmotionId = luaL_checkinteger(state, 3);

  const char* name = g_cashEmotionMgr->GetEmotionName(g_cashEmotionMgr->basePackId, baseEmotionId);
  if (name && std::strlen(name) > 1) {
    g_cashEmotionMgr->AddShortcut(name + 1, packId, emotionId);
  }

  return 1;
}

// OFFSET: 0x005A0940
// FUNCTION: fn_03037
int CCashEmotionMgr::InsertEmotionVarientListTable(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);
  const int actionIndex = luaL_checkinteger(state, 3);
  const int frameIndex = luaL_checkinteger(state, 4);
  const std::string actFile = FromUtf8(luaL_checkstring(state, 5), g_textCodePage);
  const std::string sprFile = FromUtf8(luaL_checkstring(state, 6), g_textCodePage);

  CCashEmotionMgr* manager = g_cashEmotionMgr;
  manager->InsertEmotion(packId, emotionId, actionIndex, frameIndex, actFile.c_str(), sprFile.c_str());
  return 1;
}

// OFFSET: 0x005A0B80
// FUNCTION: fn_03038
int CCashEmotionMgr::InsertOrderList(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const int emotionId = luaL_checkinteger(state, 2);

  CCashEmotionMgr* mgr = g_cashEmotionMgr;
  auto it = mgr->emotionOrder.find(packId);
  if (mgr->emotionOrder.end() != it) {
    std::vector<int>& order = it->second;
    auto found = order.begin();
    for (; order.end() != found; ++found) {
      if (*found == emotionId) {
        break;
      }
    }

    if (order.end() == found) {
      order.push_back(emotionId);
    }
  } else {
    std::vector<int> order;
    order.push_back(emotionId);
    mgr->emotionOrder.emplace(packId, std::move(order));
  }

  return 1;
}

// OFFSET: 0x005A0CE0
// FUNCTION: fn_03039
int CCashEmotionMgr::InsertPackInfo(lua_State* state) {
  const int packId = luaL_checkinteger(state, 1);
  const std::string title = FromUtf8(luaL_checkstring(state, 2), g_textCodePage);
  const char* name = g_simpleStringMgr->GetString(title.c_str());

  const int saleType = luaL_checkinteger(state, 3);
  const int price = luaL_checkinteger(state, 4);
  bool valid = true;
  if (saleType == 0) {
    if (price != 0) {
      valid = false;
    }
  } else if (saleType == 1) {
    if (price == 0) {
      valid = false;
    }
  } else {
    valid = false;
  }

  if (!valid) {
    g_cashEmotionMgr->ShowErrorMessage("EMSG_ERR_SALE_TYPE", packId);
    return 1;
  }

  const int dateType = luaL_checkinteger(state, 5);
  unsigned int startTime = static_cast<unsigned int>(luaL_checknumber(state, 6));
  unsigned int endTime = static_cast<unsigned int>(luaL_checknumber(state, 7));
  if (dateType == 0) {
    if (endTime != 0) {
      valid = false;
    }
  } else if (dateType == 1) {
    if (endTime == 0) {
      valid = false;
    }
  } else {
    valid = false;
  }

  if (!valid) {
    g_cashEmotionMgr->ShowErrorMessage("EMSG_ERR_DATE_TYPE", packId);
    return 1;
  }

  if (startTime != 0) {
    std::tm date = {0, 0, 0, static_cast<int>(startTime % 100), static_cast<int>(startTime % 10000 / 100 - 1), static_cast<int>(startTime / 10000 - 1900)};
    startTime = _mktime32(&date);
  }

  if (endTime != 0) {
    std::tm date = {0, 0, 0, static_cast<int>(endTime % 100), static_cast<int>(endTime % 10000 / 100 - 1), static_cast<int>(endTime / 10000 - 1900)};
    endTime = _mktime32(&date);
  }

  const int useType = luaL_checkinteger(state, 8);
  const int period = luaL_checkinteger(state, 9);
  if (!(useType == 0 && period == 0) && !(useType == 1 && period != 0)) {
    g_cashEmotionMgr->ShowErrorMessage("EMSG_ERR_USE_TIME", packId);
    return 1;
  }

  const char limited = static_cast<char>(luaL_checkinteger(state, 10));
  CCashEmotionMgr* mgr = g_cashEmotionMgr;
  if (mgr->packs.try_emplace(packId, name, price, startTime, endTime, period, limited).second == true) {
    mgr->packOrder.push_back(packId);
  }

  return 1;
}

// OFFSET: 0x007C37A0
// FUNCTION: fn_09340
UICashEmotionListChildWnd_Abstract::UICashEmotionListChildWnd_Abstract(int width, int height, unsigned char cached)
    : UICashEmotionScrollWnd(width, height, cached) {
}

// OFFSET: 0x007C3990
// FUNCTION: fn_09343
__declspec(noinline) void UICashEmotionScrollWnd::UpdateScrollBars() {
  if (!this->horizontalBar) {
    this->horizontalBar = new (std::nothrow) UICashEmotionScrollBar();
    if (this->horizontalBar) {
      this->horizontalBar->Configure(0, this->geometry.height - this->scrollBarHeight - 2, 0, this->geometry.width);
    }
  } else {
    this->RemoveChild(this->horizontalBar, 0);
  }

  if (this->horizontalBar) {
    UIWindow::AddChild(this->horizontalBar);
  }

  if (!this->scrollBar) {
    this->scrollBar = new (std::nothrow) UICashEmotionScrollBar();
    if (this->scrollBar) {
      this->scrollBar->Configure(this->geometry.width - this->scrollBarWidth - 2, 0, 1, this->geometry.height);
    }
  } else {
    this->RemoveChild(this->scrollBar, 0);
  }

  if (this->scrollBar) {
    UIWindow::AddChild(this->scrollBar);
  }

  this->UpdateScrollRange();
  this->Invalidate();
}

// OFFSET: 0x007C3AD0
// FUNCTION: fn_09344
void UICashEmotionListChildWnd_Abstract::OnWheel(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, 15, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, UM_SCROLLVERT, -15, 0, 0, 0);
  }
}

// OFFSET: 0x007C3B10
// FUNCTION: fn_09345
int UICashEmotionListChildWnd_Abstract::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SCROLLVERT:
    this->scrollPosition += argument1;
    if (this->scrollPosition < 0) {
      this->scrollPosition = 0;
    }
    if (this->scrollPosition > this->contentHeight - this->viewHeight) {
      this->scrollPosition = this->contentHeight - this->viewHeight;
    }
    this->scrollBar->SetPosition(this->scrollPosition);
    this->scrollBar->Invalidate();
    this->SendMsg(sender, UM_REFRESH, 0, 0, 0, 0);
    break;
  case UM_REFRESH:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(sender, UM_REFRESH, 0, 0, 0, 0);
    }
    break;
  default:
    return UIScrollWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  return 0;
}

// OFFSET: 0x007C3D60
// FUNCTION: fn_09347
UICashEmotionListChildWnd_Basic::UICashEmotionListChildWnd_Basic(int width, int height, unsigned char cached)
    : UICashEmotionListChildWnd_Abstract(width, height, cached) {
  this->m_selectAction = new (std::nothrow) CCashEmotionAction_Play();
  this->m_playAction = new (std::nothrow) CCashEmotionAction_Register();
  this->OnCreate(width, height);
}

// OFFSET: 0x007C4100
// FUNCTION: fn_09351
const char* UICashEmotionListChildWnd_Basic::Filter() {
  return nullptr;
}

// OFFSET: 0x007C4110
// FUNCTION: fn_09352
__declspec(noinline) void UICashEmotionListChildWnd_Basic::Refresh(const char* filter) {
  for (UIWindow* item : this->m_items) {
    this->RemoveChild(item, 0);
  }
  this->scrollPosition = 0;
  this->horizontalScrollPosition = 0;

  const int packId = g_cashEmotionMgr->BasePackId();
  std::vector<int> emotions = g_cashEmotionMgr->FindBaseEmotions(filter);
  int x = 15;
  int y = 4;
  std::vector<UIWindow*> recycled(this->m_items);
  this->m_items.clear();

  CashEmotionTable* table = g_cashEmotionMgr->GetBasePackEmotions();
  int index = 0;
  for (int emotionId : emotions) {
    if (table->find(emotionId) == table->end()) {
      continue;
    }

    UICashEmotionPiece* piece;
    if (!recycled.empty()) {
      piece = static_cast<UICashEmotionPiece*>(recycled.back());
      recycled.pop_back();
    } else {
      piece = new (std::nothrow) UICashEmotionPiece();
      if (!piece) {
        continue;
      }
      piece->SetSize(40, 40);
    }

    piece->Set(packId, emotionId, false, true, false);
    piece->Move(x, y);
    this->AddChild(piece);
    if (index % 5 == 4) {
      x = 15;
      y += 47;
    } else {
      x += 47;
    }
    ++index;
    this->m_items.push_back(piece);
  }

  for (UIWindow* item : recycled) {
    delete item;
  }
  this->UpdateScrollBars();
}

// OFFSET: 0x007C4440
// FUNCTION: fn_09353
void UICashEmotionListChildWnd_Basic::OnCreate(int, int) {
  this->Refresh(nullptr);
}

// OFFSET: 0x007C4450
// FUNCTION: fn_09354
void UICashEmotionListChildWnd_Basic::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C4460
// FUNCTION: fn_09355
void UICashEmotionListChildWnd_Basic::OnProcess() {
  for (UIWindow* item : this->m_items) {
    if (item->geometry.visible) {
      item->OnProcess();
    }
  }
}

// OFFSET: 0x007C4490
// FUNCTION: fn_09356
int UICashEmotionListChildWnd_Basic::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  ICashEmotionAction* action;
  switch (message) {
  case UM_CASH_EMOTION_ACTIVATE:
    action = this->m_playAction;
    break;
  case UM_CASH_EMOTION_SELECT:
    action = this->m_selectAction;
    break;
  default:
    return UICashEmotionListChildWnd_Abstract::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (action) {
    action->Execute(argument1, argument2);
  }
  return 0;
}

// OFFSET: 0x007C4810
// FUNCTION: fn_09362
UICashEmotionListChildWnd_Purchase::UICashEmotionListChildWnd_Purchase(int width, int height)
    : UICashEmotionListChildWnd_Abstract(width, height, 0) {
  this->playAction = new (std::nothrow) CCashEmotionAction_Play();
  this->OnCreate(width, height);
}

// OFFSET: 0x007C4AD0
// FUNCTION: fn_09364
UICashEmotionListChildWnd_Purchase::~UICashEmotionListChildWnd_Purchase() {
  delete this->playAction;
  this->playAction = nullptr;
  this->m_items.clear();
}

// OFFSET: 0x007C4D10
// FUNCTION: fn_09366
const char* UICashEmotionListChildWnd_Purchase::Filter() {
  return this->m_filter.c_str();
}

// OFFSET: 0x007C4D30
// FUNCTION: fn_09367
__declspec(noinline) void UICashEmotionListChildWnd_Purchase::Refresh(const char* filter) {
  this->m_filter = filter ? filter : "";
  for (UICashEmotionPack* item : this->m_items) {
    this->RemoveChild(item, 0);
  }
  this->scrollPosition = 0;
  this->horizontalScrollPosition = 0;

  std::list<UICashEmotionPack*> unused(this->m_items.begin(), this->m_items.end());
  this->m_items.clear();

  std::vector<int> packs = g_cashEmotionMgr->SortOwnedPacks(filter);
  std::string expiryFormat = g_simpleStringMgr->GetString("EMSG_EMOTION_USEABLE_EXPIRE_TIME");
  int y = 0;
  for (int packId : packs) {
    CashEmotionPack* info = g_cashEmotionMgr->GetPack(packId);
    if (!info || !g_cashEmotionMgr->GetPackEmotionTable(packId)) {
      continue;
    }

    UICashEmotionPack* pack;
    int height;
    auto it = std::find_if(unused.begin(), unused.end(), [packId](UICashEmotionPack* item) { return item->packId == packId; });
    if (it != unused.end() && (pack = *it) != nullptr) {
      unused.remove(pack);
      height = pack->geometry.height;
    } else {
      pack = new (std::nothrow) UICashEmotionPack_Purchase();
      if (!pack) {
        continue;
      }
      pack->SetSize(243, 159);
      height = pack->SetPack(packId, true, true);
    }

    pack->SetSaleBadge(info->saleBadge);
    Timestamp* expiry = g_cashEmotionMgr->FindExpiry(packId);
    if (expiry) {
      pack->SetTitle(expiryFormat.c_str());
      pack->UpdateExpiry(expiry);
    } else {
      pack->SetTitle(nullptr);
    }
    pack->m_limited = g_cashEmotionMgr->GetPackPeriod(packId) > 0;
    pack->Move(9, y);
    y += height;
    this->AddChild(pack);
    this->m_items.push_back(pack);
  }

  for (UICashEmotionPack* item : unused) {
    delete item;
  }
  unused.clear();
  this->UpdateScrollBars();
}

// OFFSET: 0x007C5290
// FUNCTION: fn_09368
void UICashEmotionListChildWnd_Purchase::OnCreate(int, int) {
  this->Refresh(nullptr);
}

// OFFSET: 0x007C52A0
// FUNCTION: fn_09369
void UICashEmotionListChildWnd_Purchase::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C52B0
// FUNCTION: fn_09370
int UICashEmotionListChildWnd_Purchase::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_CASH_EMOTION_ACTIVATE) {
    return UICashEmotionListChildWnd_Abstract::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (this->playAction) {
    this->playAction->Execute(argument1, argument2);
  }
  return 0;
}

// OFFSET: 0x007C52F0
// FUNCTION: fn_09371
void UICashEmotionListChildWnd_Purchase::UpdateExpiry(int packId, Timestamp* expiry) {
  auto it = this->m_items.begin();
  for (; it != this->m_items.end(); ++it) {
    if (packId == (*it)->packId) {
      break;
    }
  }

  if (this->m_items.end() != it) {
    (*it)->UpdateExpiry(expiry);
  }
}

// OFFSET: 0x007C5590
// FUNCTION: fn_09375
UICashEmotionListChildWnd_Search::UICashEmotionListChildWnd_Search(int width, int height)
    : UICashEmotionListChildWnd_Abstract(width, height, 0) {
  this->m_action = new (std::nothrow) CCashEmotionAction_Play();
  this->OnCreate(width, height);
}

// OFFSET: 0x007C5720
// FUNCTION: fn_09377
UICashEmotionListChildWnd_Search::~UICashEmotionListChildWnd_Search() {
  delete this->m_action;
  this->m_action = nullptr;
  this->m_items.clear();
}

// OFFSET: 0x007C5980
// FUNCTION: fn_09380
const char* UICashEmotionListChildWnd_Search::Filter() {
  return this->m_filter.c_str();
}

// OFFSET: 0x007C59A0
// FUNCTION: fn_09381
__declspec(noinline) void UICashEmotionListChildWnd_Search::AddResults(char type, const std::vector<std::pair<int, int>>& results, int& y) {
  UICashEmotionPack_Search* pack;
  if (type == 3) {
    pack = new (std::nothrow) UICashEmotionPack_Search_UnPurchased();
  } else {
    pack = new (std::nothrow) UICashEmotionPack_Search();
  }
  if (!pack) {
    return;
  }

  pack->SetSize(243, 135);
  const char* title = nullptr;
  if (type == 1) {
    title = g_simpleStringMgr->GetString("EMSG_SEARCH_WND_BASIC_TITLE");
  } else if (type == 2) {
    title = g_simpleStringMgr->GetString("EMSG_SEARCH_WND_PURCHASE_TITLE");
  } else if (type == 3) {
    title = g_simpleStringMgr->GetString("EMSG_SEARCH_WND_UNPURCHASED_TITLE");
  }
  const int height = pack->SetResults(title, results);
  pack->Resize(243, height);
  pack->Move(9, y);
  y += height;

  this->AddChild(pack);
  this->m_items.push_back(pack);
}

// OFFSET: 0x007C5B10
// FUNCTION: fn_09382
__declspec(noinline) void UICashEmotionListChildWnd_Search::Refresh(const char* filter) {
  g_windowMgr.ClearPreviousMouseWnd();
  this->m_items.clear();
  this->Clear();
  this->horizontalBar = nullptr;
  this->scrollBar = nullptr;
  if (filter && std::strlen(filter) >= 1) {
    this->m_filter.assign(filter, std::strlen(filter));
  } else {
    this->m_filter.assign("", 0);
  }

  int y = 0;
  std::vector<std::pair<int, int>> basePack;
  std::vector<std::pair<int, int>> ownedPacks;
  std::vector<std::pair<int, int>> otherPacks;
  g_cashEmotionMgr->Search(filter, basePack, ownedPacks, otherPacks);
  if (!basePack.empty() || !ownedPacks.empty() || !otherPacks.empty()) {
    this->AddResults(1, basePack, y);
    this->AddResults(2, ownedPacks, y);
    this->AddResults(3, otherPacks, y);
  }
  this->UpdateScrollBars();
}

// OFFSET: 0x007C5D80
// FUNCTION: fn_09383
void UICashEmotionListChildWnd_Search::OnCreate(int, int) {
  this->Refresh(nullptr);
}

// OFFSET: 0x007C5D90
// FUNCTION: fn_09384
void UICashEmotionListChildWnd_Search::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C5DA0
// FUNCTION: fn_09385
int UICashEmotionListChildWnd_Search::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_CASH_EMOTION_ACTIVATE) {
    return UICashEmotionListChildWnd_Abstract::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (this->m_action) {
    this->m_action->Execute(argument1, argument2);
  }
  return 0;
}

// OFFSET: 0x007C6030
// FUNCTION: fn_09389
UICashEmotionListChildWnd_UnPurchased::UICashEmotionListChildWnd_UnPurchased(int width, int height)
    : UICashEmotionListChildWnd_Abstract(width, height, 0) {
  this->OnCreate(width, height);
}

// OFFSET: 0x007C6520
// FUNCTION: fn_09392
const char* UICashEmotionListChildWnd_UnPurchased::Filter() {
  return this->m_filter.c_str();
}

// OFFSET: 0x007C6540
// FUNCTION: fn_09393
__declspec(noinline) void UICashEmotionListChildWnd_UnPurchased::Refresh(const char* filter) {
  this->m_filter = filter ? filter : "";
  for (UICashEmotionPack* item : this->m_items) {
    this->RemoveChild(item, 0);
  }
  this->scrollPosition = 0;
  this->horizontalScrollPosition = 0;

  std::list<UICashEmotionPack*> unused(this->m_items.begin(), this->m_items.end());
  this->m_items.clear();

  std::string endFormat = g_simpleStringMgr->GetString("EMSG_EMOTION_SALE_END_TIME");
  std::vector<int> packs = g_cashEmotionMgr->SortUnownedPacks(filter);
  int y = 0;
  for (int packId : packs) {
    CashEmotionPack* info = g_cashEmotionMgr->GetPack(packId);
    if (!info || !g_cashEmotionMgr->GetPackEmotionTable(packId)) {
      continue;
    }

    UICashEmotionPack* pack;
    int height;
    auto it = std::find_if(unused.begin(), unused.end(), [packId](UICashEmotionPack* item) { return item->packId == packId; });
    if (it != unused.end() && (pack = *it) != nullptr) {
      unused.remove(pack);
      height = pack->geometry.height;
    } else {
      pack = new (std::nothrow) UICashEmotionPack_Unpurchase();
      if (!pack) {
        continue;
      }
      pack->SetSize(243, 159);
      height = pack->SetPack(packId, false, false);
    }

    pack->SetSaleBadge(info->saleBadge);
    const unsigned int endTime = g_cashEmotionMgr->GetPackEndTime(packId);
    if (endTime) {
      pack->SetTitle(endFormat.c_str());
      pack->ShowExpiry(endTime);
    } else {
      pack->SetTitle(nullptr);
    }
    pack->m_limited = g_cashEmotionMgr->GetPackPeriod(packId) > 0;
    pack->Move(9, y);
    y += height;
    this->AddChild(pack);
    this->m_items.push_back(pack);
  }

  for (UICashEmotionPack* item : unused) {
    delete item;
  }
  unused.clear();
  this->UpdateScrollBars();
}

// OFFSET: 0x007C6AA0
// FUNCTION: fn_09394
void UICashEmotionListChildWnd_UnPurchased::OnCreate(int, int) {
  this->Refresh(nullptr);
}

// OFFSET: 0x007C6AB0
// FUNCTION: fn_09395
void UICashEmotionListChildWnd_UnPurchased::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x007C6AC0
// FUNCTION: fn_09396
int UICashEmotionListChildWnd_UnPurchased::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_CASH_EMOTION_SELECT_PACK) {
    return UICashEmotionListChildWnd_Abstract::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (this->parent) {
    this->parent->SendMsg(sender, UM_TOGGLEPUSH, argument1, 0, 0, 0);
    for (UICashEmotionPack* pack : this->m_items) {
      UICashEmotionPack_Unpurchase* item = static_cast<UICashEmotionPack_Unpurchase*>(pack);
      item->SetPressed(item->packId == argument1, false);
    }
  }
  return 0;
}

// OFFSET: 0x007C6CB0
// FUNCTION: fn_09401
UICashEmotionListWnd::UICashEmotionListWnd()
    : UIFrameWnd(0), m_placement(nullptr), m_basicList(nullptr), m_purchaseList(nullptr), m_unpurchasedList(nullptr), m_searchList(nullptr),
      m_tab(0), m_backButton(nullptr), m_searchEdit(nullptr), m_searchButton(nullptr), m_shortcutButton(nullptr), m_buyButton(nullptr), m_currentList(nullptr),
      m_period(nullptr) {
}

// OFFSET: 0x007C7240
// FUNCTION: fn_09405
UIBitmapButton* UICashEmotionListWnd::CreateButton(int x, int y, int width, int height, int id, const char* format, bool hasDisabled) {
  UIBitmapButton* button = new (std::nothrow) UIBitmapButton();
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

// OFFSET: 0x007C74D0
// FUNCTION: fn_09406
void UICashEmotionListWnd::OnCreate(int width, int height) {
  this->AddCloseButton("Alt+L", 0);
  if (UIBitmapButton* base = this->AddBaseButton()) {
    base->Move(32, 3);
  }

  const char* const tabs[] = {
    "유저인터페이스\\cashemotion\\tap_basic_%s.bmp",
    "유저인터페이스\\cashemotion\\tap_bought_%s.bmp",
    "유저인터페이스\\cashemotion\\tap_unbought_%s.bmp",
    "유저인터페이스\\cashemotion\\tap_search_%s.bmp",
  };
  int x = 46;
  for (int index = 0; index < 4; ++index) {
    UIBmpRadioBtn2* tab = new (std::nothrow) UIBmpRadioBtn2();
    if (!tab) {
      continue;
    }

    const char* format = tabs[index];
    std::string normal = FormatOwnedString(format, "normal");
    tab->SetBitmap(normal.c_str(), 0);
    tab->SetBitmap(FormatOwnedString(format, "normal").c_str(), 1);
    tab->SetBitmap(FormatOwnedString(format, "press").c_str(), 2);
    tab->SetBitmap(FormatOwnedString(format, "disable").c_str(), 3);
    tab->Move(0, x);

    const char id = static_cast<char>(index + 1);
    tab->SendMsg(this, UM_SETSTATE, id == 1, 0, 0, 0);
    this->AddChild(tab);
    this->m_tabs.insert(std::make_pair(id, static_cast<UIWindow*>(tab)));

    if (CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(normal.c_str())))) {
      x += bitmap->image.height;
    }
  }

  this->m_backButton = this->CreateButton(36, 22, 20, 18, 189, "유저인터페이스\\cashemotion\\btn_arrowback_%s.bmp", false);
  this->m_searchButton = this->CreateButton(221, 22, 26, 18, 190, "유저인터페이스\\cashemotion\\btn_search_%s.bmp", true);
  this->m_shortcutButton = this->CreateButton(252, 22, 37, 18, 191, "유저인터페이스\\cashemotion\\btn_shortui_%s.bmp", false);
  this->m_buyButton = this->CreateButton(247, 355, 42, 20, 526, "유저인터페이스\\cashemotion\\btn_buy_%s.bmp", true);

  this->m_searchEdit = new (std::nothrow) UIEditCtrl2();
  if (this->m_searchEdit) {
    this->m_searchEdit->SetSize(160, 16);
    this->m_searchEdit->Move(61, 23);
    this->m_searchEdit->maximum = 20;
    this->AddChild(this->m_searchEdit);
  }

  this->m_basicList = new (std::nothrow) UICashEmotionListChildWnd_Basic(271, 303, 0);
  if (this->m_basicList) {
    this->m_basicList->Move(28, 47);
    this->AddChild(this->m_basicList);
  }

  this->m_purchaseList = new (std::nothrow) UICashEmotionListChildWnd_Purchase(271, 303);
  if (this->m_purchaseList) {
    this->m_purchaseList->Move(28, 47);
    this->AddChild(this->m_purchaseList);
  }

  this->m_unpurchasedList = new (std::nothrow) UICashEmotionListChildWnd_UnPurchased(271, 303);
  if (this->m_unpurchasedList) {
    this->m_unpurchasedList->Move(28, 47);
    this->AddChild(this->m_unpurchasedList);
  }

  this->m_searchList = new (std::nothrow) UICashEmotionListChildWnd_Search(271, 303);
  if (this->m_searchList) {
    this->m_searchList->Move(28, 47);
    this->AddChild(this->m_searchList);
  }

  auto first = this->m_tabs.find(1);
  if (this->m_tabs.end() != first) {
    this->SendMsg(first->second, UM_COMMAND, ID_RADIOBTN, 0, 0, 0);
  }

  this->m_period = new UIStaticText();
  this->m_period->SetSize(200, 14);
  this->m_period->Move(39, 359);
  this->m_period->SetColor(214, 24, 16);
  this->m_period->Show(0);
  this->AddChild(this->m_period);

  this->RestartExpirySchedule();
}

// OFFSET: 0x007C7C20
// FUNCTION: fn_09407
void UICashEmotionListWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const int width = this->geometry.width - 30;
  const int top = this->DrawTitleBarAt(29, width, false, LookupMessage(MSI_EMOTION_LIST));
  this->DrawPanelBackgroundAt(29, top, width, nullptr);
  DrawRectangleBorder(*this, 60, 22, 161, 18, 0xB5B5B5);
  this->FillRegion(29, 46, width, 1, 0xCECECE);
  this->DrawButtonBarAt(2, 29, width, 1);
}

// OFFSET: 0x007C7CC0
// FUNCTION: fn_09410
void UICashEmotionListWnd::OnProcess() {
  if (this->m_currentList) {
    this->m_currentList->OnProcess();
  }
}

// OFFSET: 0x007C7CD0
// FUNCTION: fn_09411
void UICashEmotionListWnd::RefreshCurrentList() {
  if (this->m_currentList) {
    this->m_currentList->Refresh(!this->m_searchEdit ? nullptr : this->m_searchEdit->Text());
  }
}

// OFFSET: 0x007C7D10
// FUNCTION: fn_09412
void UICashEmotionListWnd::ShowPack(int packId) {
  const char* title = g_cashEmotionMgr->GetPackTitle(packId);
  if (!title) {
    return;
  }

  auto tab = this->m_tabs.find(3);
  if (tab != this->m_tabs.end()) {
    this->SendMsg(tab->second, UM_COMMAND, ID_RADIOBTN, 0, 0, 0);
  }

  if (this->m_searchEdit) {
    this->m_searchEdit->SetText(title);
  }

  this->RefreshCurrentList();

  if (this->m_searchEdit) {
    this->m_searchEdit->SetText("");
  }

  if (this->m_currentList) {
    this->m_currentList->SendMsg(this, UM_CASH_EMOTION_SELECT_PACK, packId, 0, 0, 0);
  }
}

// OFFSET: 0x007C7E50
// FUNCTION: fn_09413
void UICashEmotionListWnd::RefreshPackLists() {
  if (this->m_purchaseList) {
    this->m_purchaseList->Refresh(this->m_tab == 2 && this->m_searchEdit ? this->m_searchEdit->Text() : nullptr);
  }

  if (this->m_unpurchasedList) {
    this->m_unpurchasedList->Refresh(this->m_tab == 3 && this->m_searchEdit ? this->m_searchEdit->Text() : nullptr);
  }

  if (this->m_searchList) {
    this->m_searchList->Refresh(this->m_tab == 4 && this->m_searchEdit ? this->m_searchEdit->Text() : nullptr);
  }
}

// OFFSET: 0x007C7F00
// FUNCTION: fn_09414
void UICashEmotionListWnd::ResetLists() {
  if (this->m_searchEdit) {
    this->m_searchEdit->SetText("");
  }

  if (this->m_purchaseList) {
    this->m_purchaseList->Refresh(nullptr);
  }

  if (this->m_unpurchasedList) {
    this->m_unpurchasedList->Refresh(nullptr);
  }

  if (this->m_searchList) {
    this->m_searchList->Refresh(nullptr);
  }
}

// OFFSET: 0x007C7F60
// FUNCTION: fn_09415
int UICashEmotionListWnd::SetPeriod(int packId) {
  if (!this->m_period) {
    return 0;
  }

  const int period = g_cashEmotionMgr->GetPackPeriod(packId);
  if (period < 1) {
    this->m_period->Show(0);
  } else {
    const char* format = g_simpleStringMgr->GetString("EMSG_PERIOD_OF_USE");
    std::string text = std::vformat(format, std::make_format_args(period / 24));
    this->m_period->SetTextFitted(text.c_str());
    this->m_period->Show(1);
  }

  return this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x007C80C0
// FUNCTION: fn_09416
void UICashEmotionListWnd::RestartExpirySchedule() {
  g_timeScheduler->RemoveSchedulesByKey(7);
  if (g_cashEmotionMgr->m_expiry.size() != 0) {
    const std::uint64_t now = _Xtime_get_ticks();
    g_timeScheduler->AddSchedule(new CEmotionPurchaseUseableTimeUISchedule(now, 30000, 1));
  }
}

// OFFSET: 0x007C8170
// FUNCTION: fn_09417
int UICashEmotionListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_DEFPUSH:
    this->RefreshCurrentList();
    return 0;

  case UM_SETINFO:
    this->m_placement = reinterpret_cast<CashEmotionListWndInfo*>(argument1);
    if (this->m_placement) {
      if (!this->m_placement->IsValid()) {
        this->m_placement->Reset();
      }
      this->Move(this->m_placement->x, this->m_placement->y);
    }
    return 0;

  case UM_COMMAND:
    switch (argument1) {
    case ID_BUTTON1:
      if (this->m_searchEdit) {
        this->m_searchEdit->SetText("");
      }
      if (this->m_currentList) {
        this->m_currentList->Refresh(nullptr);
      }
      return 0;

    case ID_BUTTON2:
      this->RefreshCurrentList();
      return 0;

    case 191:
      if (!g_windowMgr.LookupWindow(WID_EMOTIONWND)) {
        g_windowMgr.EnsureWindow(WID_EMOTIONWND);
      } else {
        g_windowMgr.CloseWindow(WID_EMOTIONWND);
      }
      return 0;

    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_CASHEMOTIONLISTWND);
      return 0;

    case ID_RADIOBTN: {
      if (this->m_tab != 0) {
        auto current = std::as_const(this->m_tabs).find(this->m_tab);
        if (current == this->m_tabs.end() || sender == current->second) {
          return 0;
        }
      }

      char selected = 0;
      for (auto& tab : this->m_tabs) {
        if (sender == tab.second) {
          tab.second->SendMsg(nullptr, UM_SETSTATE, 1, 0, 0, 0);
          selected = tab.first;
        } else {
          tab.second->SendMsg(nullptr, UM_SETSTATE, 0, 0, 0, 0);
        }
      }
      this->SetTab(selected);
      return 0;
    }

    case 526: {
      const int packId = g_cashEmotionMgr->selectedPackId;
      if (packId <= 0) {
        return 0;
      }

      CSkillInfo* skill;
      if (g_session.IsDoramJob(g_session.GetDisplayJob())) {
        skill = g_session.m_skillBundle.Find(5018);
        if (!skill || skill->level <= 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_BASICSKILL_LEVEL")), 255, 0, 0, 0);
          return 0;
        }
      } else {
        skill = g_session.m_skillBundle.Find(1);
        if (!skill || skill->level <= 1) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_BASICSKILL_LEVEL")), 255, 0, 0, 0);
          return 0;
        }
      }

      const int price = g_cashEmotionMgr->GetPackPrice(packId);
      if (price < 0) {
        return 0;
      }

      const bool free = price == 0;
      if (!free && g_session.GetInventoryItemCount(g_cashEmotionMgr->purchaseItemId) < price) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(g_simpleStringMgr->GetString("EMSG_EMOTION_EXPANTION_NOT_ENOUGH_NYANGVINE")), 255, 0, 0, 0);
        return 0;
      }

      const char* title = g_cashEmotionMgr->GetPackTitle(packId);
      if (!title) {
        return 0;
      }

      const char* format = g_simpleStringMgr->GetString(free ? "EMSG_CASH_EMOTION_FREE_PURCHASE_QUESTION" : "EMSG_CASH_EMOTION_PURCHASE_QUESTION");
      if (!format || std::strlen(format) == 0) {
        return 0;
      }

      std::string text;
      text = free ? std::vformat(format, std::make_format_args(title)) : std::vformat(format, std::make_format_args(title, price));

      UINormalMB* box = new UINormalMB();
      box->Create(nullptr, text.c_str(), 2, true, nullptr, 280, 128, nullptr);
      box->m_keepOpen = 1;
      g_windowMgr.AddRendered(box);
      if (g_windowMgr.RunModal(box) == 187) {
        CashEmotionPurchasePacket packet;
        packet.id = 0xBEC;
        packet.packId = g_cashEmotionMgr->selectedPackId;
        packet.itemId = g_cashEmotionMgr->purchaseItemId;
        packet.price = g_cashEmotionMgr->GetPackPrice(packet.packId);
        GetRagConnection().Send(sizeof(packet), &packet);
      }
      return 0;
    }
    }
    return 0;

  case UM_TOGGLEPUSH:
    if (this->m_buyButton) {
      this->m_buyButton->Enable(1);
    }
    g_cashEmotionMgr->selectedPackId = argument1;
    return this->SetPeriod(argument1);

  case UM_REFRESH:
    this->Invalidate();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007C8800
// FUNCTION: fn_09418
void UICashEmotionListWnd::SetTab(char tab) {
  if (tab == this->m_tab || tab == 0) {
    return;
  }

  UICashEmotionListChildWnd_Abstract* list;
  if (tab == 1) {
    list = this->m_basicList;
  } else if (tab == 2) {
    list = this->m_purchaseList;
  } else if (tab == 3) {
    list = this->m_unpurchasedList;
  } else if (tab == 4) {
    list = this->m_searchList;
  } else {
    list = nullptr;
  }
  this->m_currentList = list;

  if (this->m_basicList) {
    this->m_basicList->Show(tab == 1);
  }
  if (this->m_purchaseList) {
    this->m_purchaseList->Show(tab == 2);
  }
  if (this->m_unpurchasedList) {
    this->m_unpurchasedList->Show(tab == 3);
  }
  if (this->m_searchList) {
    this->m_searchList->Show(tab == 4);
  }

  this->m_tab = tab;
  if (this->m_tab == 3) {
    const int packId = g_cashEmotionMgr->selectedPackId;
    if (this->m_buyButton) {
      this->m_buyButton->Enable(packId > 0);
    }
    this->SetPeriod(packId);
  } else {
    if (this->m_buyButton) {
      this->m_buyButton->Enable(0);
    }
    this->SetPeriod(0);
  }

  const bool basic = this->m_currentList == this->m_basicList;
  if (this->m_searchEdit) {
    if (g_windowMgr.GetHoverWindow() == this->m_searchEdit) {
      g_windowMgr.SetFocus(nullptr);
    }

    this->m_searchEdit->OnFinishEdit(0);
    this->m_searchEdit->disabled = basic;
    this->m_searchEdit->Invalidate();
    if (basic) {
      this->m_searchEdit->SetText("");
    }
  }

  if (this->m_backButton) {
    this->m_backButton->SetDisabled(basic);
    this->m_backButton->Invalidate();
  }

  if (this->m_searchButton) {
    this->m_searchButton->SetDisabled(basic);
    this->m_searchButton->Invalidate();
  }

  if (this->m_currentList) {
    const char* filter = this->m_currentList->Filter();
    if (this->m_searchEdit) {
      this->m_searchEdit->SetText(filter ? filter : "");
    }
  }
}

// OFFSET: 0x007C8A40
// FUNCTION: fn_09419
void UICashEmotionListWnd::StartExpirySchedule() {
  if (g_cashEmotionMgr->m_expiry.size() != 0) {
    const std::uint64_t now = _Xtime_get_ticks();
    g_timeScheduler->AddSchedule(new CEmotionPurchaseUseableTimeUISchedule(now, 30000, 1));
  }
}

// OFFSET: 0x007C8AF0
// FUNCTION: fn_09420
void UICashEmotionListWnd::StoreInfo() {
  if (this->m_placement) {
    this->m_placement->x = this->geometry.x;
    this->m_placement->y = this->geometry.y;
    this->m_placement->width = this->geometry.width;
    this->m_placement->height = this->geometry.height;
  }
}

// OFFSET: 0x007C8B30
// FUNCTION: fn_09421
void UICashEmotionListWnd::UpdateExpiry(int packId, Timestamp* expiry) {
  if (this->m_purchaseList) {
    this->m_purchaseList->UpdateExpiry(packId, expiry);
  }
}

// OFFSET: 0x007C8B50
// FUNCTION: fn_09422
void UICashEmotionListWnd::RefreshSelection() {
  if (this->m_tab == 3) {
    const int packId = g_cashEmotionMgr->selectedPackId;
    if (this->m_buyButton) {
      this->m_buyButton->Enable(packId > 0);
    }
    this->SetPeriod(packId);
  } else {
    if (this->m_buyButton) {
      this->m_buyButton->Enable(0);
    }
    this->SetPeriod(0);
  }
}

// OFFSET: 0x007C8E70
// FUNCTION: fn_09425
UICashEmotionMiniWnd::UICashEmotionMiniWnd()
    : UIWindow(0), m_selected(nullptr), m_previous(nullptr), m_next(nullptr), m_page(0), m_selectedIndex(0) {
  this->m_pieces.fill(nullptr);
  this->m_action = new (std::nothrow) CCashEmotionAction_SelectedUnPurchasedPack;
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
      const bool owned = g_cashEmotionMgr->BasePackId() == entry.first ? true : g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[0]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[0]->Set(-1, -1, false, false, false);
    }
  }

  if (this->m_pieces[1]) {
    if (this->m_selected) {
      std::pair<int, int> entry = this->m_selected->Get(first + 1);
      const bool owned = g_cashEmotionMgr->BasePackId() == entry.first ? true : g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[1]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[1]->Set(-1, -1, false, false, false);
    }
  }

  if (this->m_pieces[2]) {
    if (this->m_selected) {
      std::pair<int, int> entry = this->m_selected->Get(first + 2);
      const bool owned = g_cashEmotionMgr->BasePackId() == entry.first ? true : g_cashEmotionMgr->IsOwnedPack(entry.first);
      this->m_pieces[2]->Set(entry.first, entry.second, false, false, !owned);
    } else {
      this->m_pieces[2]->Set(-1, -1, false, false, false);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007C94A0
// FUNCTION: fn_09431
void UICashEmotionMiniWnd::OnCreate(int width, int height) {
  int x = 24;
  for (UICashEmotionPiece*& piece : this->m_pieces) {
    piece = new (std::nothrow) UICashEmotionPiece();
    if (piece) {
      piece->SetSize(42, 42);
      piece->Move(x, 1);
      this->AddChild(piece);
      x += 49;
    }
  }

  this->m_previous = new (std::nothrow) UIBitmapButton();
  if (this->m_previous) {
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_normal.bmp", 0, 0);
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_over.bmp", 1, 0);
    this->m_previous->SetBitmap("유저인터페이스\\cashemotion\\btn_arrow_l_press.bmp", 2, 0);
    this->m_previous->SetSize(this->m_previous->BitmapWidth(), this->m_previous->BitmapHeight());
    this->m_previous->Move(0, 0);
    this->m_previous->SetId(1);
    this->AddChild(this->m_previous);
  }

  this->m_next = new (std::nothrow) UIBitmapButton();
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
    this->DrawBitmap(0, 0, background, false);
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
    this->DrawBitmap(piece->geometry.x - 3, 0, highlight, false);
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
  case UM_REFRESH:
    this->Invalidate();
    return 0;

  case UM_CASH_EMOTION_ACTIVATE:
    entry.first = static_cast<int>(argument1);
    entry.second = static_cast<int>(argument2);
    break;

  case UM_COMMAND:
    if (argument1 == 1) {
      this->Previous();
    } else if (argument1 == 2) {
      this->Next();
    }
    return 0;

  case UM_CASH_EMOTION_KEY:
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
    if (ChatWindow()) {
      if (ChatWindow()->m_chatEdit) {
        ChatWindow()->m_chatEdit->SetText(data->name.c_str());
      }
      ChatWindow()->ShowSuggestions(0);
    }
  } else if (this->m_action) {
    this->m_action->Execute(packId, 0);
  }

  return 0;
}

// OFFSET: 0x007C9C40
// FUNCTION: fn_09438
UICashEmotionPack::UICashEmotionPack()
    : UIWindow(0), packTitleText(nullptr), m_titleText(nullptr), packId(-1), m_limited(false) {
}

// OFFSET: 0x007C9D60
// FUNCTION: fn_09439
UICashEmotionPack::~UICashEmotionPack() {
  this->m_pieces.clear();
}

// OFFSET: 0x007C9EB0
// FUNCTION: fn_09441
void UICashEmotionPack::ShowExpiry(__time32_t value) {
  if (!this->m_titleText || this->m_title.empty()) {
    return;
  }

  Timestamp expiry(value);
  int minute = expiry.Minute();
  int hour = expiry.Hour();
  int day = expiry.Day();
  int month = expiry.Month();
  int year = expiry.Year();
  std::string text = std::vformat(this->m_title, std::make_format_args(year, month, day, hour, minute));
  this->m_titleText->SetTextFitted(text.c_str());
}

// OFFSET: 0x007C9FE0
// FUNCTION: fn_09442
void UICashEmotionPack::UpdateExpiry(Timestamp* expiry) {
  if (!expiry || !this->m_titleText || this->m_title.empty()) {
    return;
  }

  int minute = expiry->Minute();
  int hour = expiry->Hour();
  int day = expiry->Day();
  int month = expiry->Month();
  int year = expiry->Year();
  std::string text = std::vformat(this->m_title, std::make_format_args(year, month, day, hour, minute));
  this->m_titleText->SetTextFitted(text.c_str());
}

// OFFSET: 0x007CA100
// FUNCTION: fn_09443
std::string UICashEmotionPack::GetPriceText(int packId) {
  int price = g_cashEmotionMgr->GetPackPrice(packId);
  if (price > 0) {
    const char* format = g_simpleStringMgr->GetString("EMSG_PURCHASE_PRICE");
    if (format) {
      return std::vformat(format, std::make_format_args(price));
    }

    return std::to_string(price);
  }

  if (price == 0) {
    const char* text = g_simpleStringMgr->GetString("EMSG_PURCHASE_FREE_PRICE");
    if (text) {
      return text;
    }
  }

  return "";
}

// OFFSET: 0x007CA2C0
// FUNCTION: fn_09444
int UICashEmotionPack::SetPack(int packId, bool showTooltip, bool hoverable) {
  this->packId = packId;
  if (this->packTitleText) {
    this->packTitleText->SetTextFitted(g_cashEmotionMgr->GetPackTitle(packId));
  }

  for (UICashEmotionPiece* piece : this->m_pieces) {
    this->RemoveChild(piece, 1);
  }
  this->m_pieces.clear();

  std::vector<int> emotions = g_cashEmotionMgr->GetPackEmotions(packId);
  if (!emotions.empty()) {
    CashEmotionTable* table = g_cashEmotionMgr->GetPackEmotionTable(packId);
    if (table) {
      this->m_pieces.reserve(table->size());
      int x = 8;
      int y = 63;
      int index = 0;
      for (int emotionId : emotions) {
        auto it = table->find(emotionId);
        if (it == table->end()) {
          continue;
        }

        UICashEmotionPiece* piece = new (std::nothrow) UICashEmotionPiece();
        if (!piece) {
          continue;
        }

        piece->SetSize(40, 40);
        piece->Set(it->second.id, it->second.subId, showTooltip, hoverable, false);
        piece->Move(x, y);
        this->AddChild(piece);
        this->m_pieces.push_back(piece);
        if (index % 5 == 4) {
          x = 8;
          y += 47;
        } else {
          x += 47;
        }
        ++index;
      }
    }
  }

  this->Invalidate();
  return 159;
}

// OFFSET: 0x007CA550
// FUNCTION: fn_09445
void UICashEmotionPack::OnCreate(int, int) {
  this->packTitleText = new (std::nothrow) UIStaticText();
  if (this->packTitleText) {
    this->packTitleText->SetSize(145, 14);
    this->packTitleText->Move(33, 14);
    this->packTitleText->shadow = 1;
    this->AddChild(this->packTitleText);
  }

  this->m_titleText = new UIStaticText();
  this->m_titleText->SetSize(145, 14);
  this->m_titleText->Move(10, 42);
  this->m_titleText->SetColor(214, 24, 16);
  this->m_titleText->SetFont(0, 11, 0, 0);
  this->AddChild(this->m_titleText);
}

// OFFSET: 0x007CA690
// FUNCTION: fn_09446
void UICashEmotionPack::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (!this->m_background.empty()) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(this->m_background.c_str()));
    if (bitmap) {
      this->DrawBitmapScaled(0, 0, this->geometry.width, this->geometry.height, bitmap, 0);
    }
  }

  if (!this->m_saleBadge.empty()) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(this->m_saleBadge.c_str()));
    if (bitmap) {
      this->DrawBitmap(171, 12, bitmap, true);
    }
  }

  if (this->m_limited) {
    auto* bitmap =
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\icon_limitemotion.bmp")));
    if (bitmap) {
      this->DrawBitmap(0, 0, bitmap, true);
    }
  }
}

// OFFSET: 0x007CA760
// FUNCTION: fn_09447
void UICashEmotionPack::OnLBtnDblClk(int, int) {
}

// OFFSET: 0x007CA770
// FUNCTION: fn_09448
void UICashEmotionPack::OnLBtnDown(int, int) {
}

// OFFSET: 0x007CA780
// FUNCTION: fn_09449
void UICashEmotionPack::OnMouseMove(int, int) {
}

// OFFSET: 0x007CA790
// FUNCTION: fn_09450
void UICashEmotionPack::OnProcess() {
  for (UICashEmotionPiece* piece : this->m_pieces) {
    if (piece) {
      piece->OnProcess();
    }
  }
}

// OFFSET: 0x007CA7C0
// FUNCTION: fn_09451
void UICashEmotionPack::OnWheel(int argument) {
  if (this->parent) {
    this->parent->OnWheel(argument);
  }
}

// OFFSET: 0x007CA7E0
// FUNCTION: fn_09452
int UICashEmotionPack::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_REFRESH:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(sender, UM_REFRESH, 0, 0, 0, 0);
    }
    return 0;
  case UM_CASH_EMOTION_ACTIVATE:
    if (this->parent) {
      this->parent->SendMsg(sender, UM_CASH_EMOTION_ACTIVATE, argument1, argument2, argument3, argument4);
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
__declspec(noinline) void UICashEmotionPack::SetTitle(const char* title) {
  if (title) {
    this->m_title = title;
  } else {
    this->m_title.clear();
  }

  if (this->m_titleText) {
    this->m_titleText->Show(!this->m_title.empty());
  }
}

// OFFSET: 0x007CAAC0
// FUNCTION: fn_09457
UICashEmotionPack_Purchase::UICashEmotionPack_Purchase() : UICashEmotionPack() {
  this->m_selectAction = new (std::nothrow) CCashEmotionAction_Play;
  this->m_purchaseAction = new (std::nothrow) CCashEmotionAction_Register;
  this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_bought.bmp");
}

// OFFSET: 0x007CAC40
// FUNCTION: fn_09458
UICashEmotionPack_Purchase::~UICashEmotionPack_Purchase() {
  delete this->m_selectAction;
  this->m_selectAction = nullptr;
  delete this->m_purchaseAction;
  this->m_purchaseAction = nullptr;
}

// OFFSET: 0x007CACE0
// FUNCTION: fn_09459
void UICashEmotionPack_Purchase::ShowExpiry(__time32_t expiry) {
  if (!this->m_titleText || this->m_title.empty()) {
    return;
  }

  unsigned int remaining = expiry - g_cashEmotionMgr->GetServerTime();
  if (remaining > 60) {
    unsigned int days = remaining / 86400;
    remaining -= days * 86400;
    unsigned int hours = remaining / 3600;
    remaining -= hours * 3600;
    unsigned int minutes = remaining / 60;
    std::string text = std::vformat(this->m_title, std::make_format_args(days, hours, minutes));
    this->m_titleText->SetTextFitted(text.c_str());
  } else {
    this->m_titleText->SetTextFitted(g_simpleStringMgr->GetString("EMSG_PERIOD_OF_LAST_TIME"));
  }

  this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
}

// OFFSET: 0x007CAE70
// FUNCTION: fn_09460
void UICashEmotionPack_Purchase::UpdateExpiry(Timestamp* expiry) {
  if (expiry && this->m_titleText && !this->m_title.empty()) {
    this->ShowExpiry(expiry->seconds);
  }
}

// OFFSET: 0x007CAEA0
// FUNCTION: fn_09461
int UICashEmotionPack_Purchase::SetPack(int packId, bool showTooltip, bool hoverable) {
  return UICashEmotionPack::SetPack(packId, showTooltip, hoverable);
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
  case UM_REFRESH:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(sender, UM_REFRESH, 0, 0, 0, 0);
    }
    return 0;
  case UM_CASH_EMOTION_ACTIVATE:
    this->m_purchaseAction->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
    return 0;
  case UM_CASH_EMOTION_SELECT:
    this->m_selectAction->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CAF60
// FUNCTION: fn_09464
int UICashEmotionPack_Search::SetResults(const char* title, const std::vector<std::pair<int, int>>& results) {
  if (this->packTitleText) {
    if (title) {
      this->packTitleText->SetTextFitted(title);
    } else {
      this->packTitleText->SetText("", 0);
    }
  }

  for (UICashEmotionPiece* piece : this->m_pieces) {
    this->RemoveChild(piece, 1);
  }
  this->m_pieces.clear();

  const int count = static_cast<int>(results.size());
  this->m_pieces.reserve(results.size());
  int y = 39;
  int x = 8;
  int index = 0;
  for (const std::pair<int, int>& result : results) {
    UICashEmotionPiece* piece = new (std::nothrow) UICashEmotionPiece();
    if (!piece) {
      continue;
    }

    piece->SetSize(40, 40);
    piece->Set(result.first, result.second, false, true, false);
    piece->Move(x, y);
    this->AddChild(piece);
    this->m_pieces.push_back(piece);
    if (index % 5 == 4 && index + 1 < count) {
      y += 47;
      x = 8;
    } else {
      x += 47;
    }
    ++index;
  }

  this->Invalidate();
  return y + (count > 0 ? 50 : 10);
}

// OFFSET: 0x007CB160
// FUNCTION: fn_09465
void UICashEmotionPack_Search::Paint() {
  this->ClearSurface(0xFFFF00FF);

  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\bg_list_search.bmp")));
  if (bitmap) {
    this->DrawBitmap(0, 0, bitmap, false);
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
  if (message != UM_CASH_EMOTION_SELECT) {
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->m_action) {
    this->m_action->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
  }

  return 0;
}

// OFFSET: 0x007CB3A0
// FUNCTION: fn_09468
UICashEmotionPack_Search_UnPurchased::UICashEmotionPack_Search_UnPurchased() : UICashEmotionPack_Search() {
  this->m_action = new (std::nothrow) CCashEmotionAction_SelectedUnPurchasedPack;
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
  case UM_REFRESH:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
    }
    return 0;
  case UM_CASH_EMOTION_ACTIVATE:
    if (this->m_action) {
      this->m_action->Execute(static_cast<int>(argument1), static_cast<int>(argument2));
      return 0;
    }
    return 0;
  default:
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CB5E0
// FUNCTION: fn_09471
UICashEmotionPack_Unpurchase::UICashEmotionPack_Unpurchase() : UICashEmotionPack(), m_pressed(false) {
  this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_normal.bmp");
  this->m_pressed = false;
  this->Invalidate();
}

// OFFSET: 0x007CB790
// FUNCTION: fn_09473
int UICashEmotionPack_Unpurchase::SetPack(int packId, bool showTooltip, bool hoverable) {
  const int height = UICashEmotionPack::SetPack(packId, showTooltip, hoverable);
  this->priceText = this->GetPriceText(packId);
  return height;
}

// OFFSET: 0x007CB850
// FUNCTION: fn_09474
void UICashEmotionPack_Unpurchase::Paint() {
  UICashEmotionPack::Paint();
  if (!this->priceText.empty()) {
    this->DrawRightAlignedText(233, 42, this->priceText.c_str(), 0, 0, 12, 0xE68C5A, 1);
  }
}

// OFFSET: 0x007CB890
// FUNCTION: fn_09475
void UICashEmotionPack_Unpurchase::OnLBtnDblClk(int, int) {
  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, UM_CASH_EMOTION_SELECT_PACK, this->packId, 0, 0, 0);
  }
}

// OFFSET: 0x007CB8C0
// FUNCTION: fn_09476
void UICashEmotionPack_Unpurchase::OnLBtnDown(int, int) {
  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, UM_CASH_EMOTION_SELECT_PACK, this->packId, 0, 0, 0);
  }
}

// OFFSET: 0x007CB8F0
// FUNCTION: fn_09477
void UICashEmotionPack_Unpurchase::SetPressed(bool pressed, bool force) {
  if (force || pressed != this->m_pressed) {
    if (pressed) {
      this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_press.bmp");
    } else {
      this->m_background = Resolve("유저인터페이스\\cashemotion\\bg_list_normal.bmp");
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
  if (message != UM_CASH_EMOTION_ACTIVATE) {
    return UICashEmotionPack::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (!this->m_pressed && this->parent) {
    this->parent->SendMsg(this, UM_CASH_EMOTION_SELECT_PACK, this->packId, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x007CB9F0
// FUNCTION: fn_09479
UICashEmotionPiece::UICashEmotionPiece()
    : UIWindow(0), m_packId(0), m_emotionId(0), m_button(nullptr), m_hoverable(false), m_hovered(false) {
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
    this->m_button->frameIndex = std::max(data->frameIndex, 0);
    this->m_button->animationIndex = std::max(data->actionIndex, 0);
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
void UICashEmotionPiece::OnCreate(int width, int height) {
  this->m_button = new (std::nothrow) UICashEmotionSprBtn(this);
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
void UICashEmotionPiece::OnLBtnDblClk(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_CASH_EMOTION_SELECT, this->m_packId, this->m_emotionId, 0, 0);
  }
}

// OFFSET: 0x007CBDD0
// FUNCTION: fn_09485
void UICashEmotionPiece::OnMouseEnter() {
  this->m_hovered = true;
  if (this->m_hoverable) {
    this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007CBE00
// FUNCTION: fn_09486
void UICashEmotionPiece::OnMouseLeave() {
  this->m_hovered = false;
  if (this->m_hoverable) {
    this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007CBE30
// FUNCTION: fn_09487
void UICashEmotionPiece::OnProcess() {
  if (this->m_button && this->m_button->animate != 0) {
    this->m_button->Invalidate();
  }
}

// OFFSET: 0x007CBE50
// FUNCTION: fn_09488
void UICashEmotionPiece::OnWheel(int argument) {
  if (this->parent) {
    this->parent->OnWheel(argument);
  }
}

// OFFSET: 0x007CBE70
// FUNCTION: fn_09489
int UICashEmotionPiece::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case UM_COMMAND:
    if (argument1 == 189 && this->parent) {
      this->parent->SendMsg(this, UM_CASH_EMOTION_ACTIVATE, this->m_packId, this->m_emotionId, 0, 0);
    }
    return 0;
  case UM_REFRESH:
    this->Invalidate();
    if (this->parent) {
      this->parent->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
    }
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x007CBF20
// FUNCTION: fn_09490
UICashEmotionScrollBar::UICashEmotionScrollBar() : UIScrollBar() {
  this->m_disabled = false;
  this->horizontalStep = 135;
  this->verticalStep = 135;
  this->thickness = 11;
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
    this->DrawBitmap(0, -1, up, false);
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
    this->DrawBitmap(0, height - this->arrowLength + 1, down, false);
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
    this->DrawBitmap(0, top, topBar, true);
  }

  auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\scrollbar_mid.bmp")));
  auto* bottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\cashemotion\\scrollbar_bottom.bmp")));
  const int bottomHeight = bottom ? bottom->image.height : 0;

  int end = top + (length + 4);
  if (middle) {
    const int step = middle->image.height;
    end = (length + 4) + (top - bottomHeight);
    for (int y = top + topHeight; y < end; y += step) {
      this->DrawBitmap(0, y, middle, true);
    }
  }

  if (bottom) {
    this->DrawBitmap(0, end, bottom, true);
  }
}

// OFFSET: 0x007CC290
// FUNCTION: fn_09493
__declspec(noinline) void UICashEmotionScrollBar::SetDisabled(bool disabled) {
  if (disabled != this->m_disabled) {
    this->m_disabled = disabled;
    this->Invalidate();
  }
}

// OFFSET: 0x007CC2B0
// FUNCTION: fn_09494
UICashEmotionScrollWnd::UICashEmotionScrollWnd(int width, int height, unsigned char cached)
    : UIScrollWnd(width, height, cached) {
  this->RemoveChild(this->horizontalBar, 1);
  this->RemoveChild(this->scrollBar, 1);
  this->horizontalBar = nullptr;
  this->scrollBar = nullptr;
  this->scrollBarWidth = 11;
}

// OFFSET: 0x007CC440
// FUNCTION: fn_09495
void UICashEmotionScrollWnd::AddChild(UIWindow* child) {
  UIWindow::AddChild(child);

  for (UIWindow* window : this->m_children) {
    if (window == this->horizontalBar || window == this->scrollBar) {
      continue;
    }

    this->childOrigins[window] = std::make_pair(window->geometry.x, window->geometry.y);
  }
}

// OFFSET: 0x007CC5D0
// FUNCTION: fn_09496
void UICashEmotionScrollWnd::OnCreate(int, int) {
  this->RemoveChild(this->horizontalBar, 1);
  this->RemoveChild(this->scrollBar, 1);
  this->horizontalBar = nullptr;
  this->scrollBar = nullptr;
  this->scrollBarWidth = 11;
}

// OFFSET: 0x007CC620
// FUNCTION: fn_09497
void UICashEmotionScrollWnd::UpdateScrollRange() {
  UIScrollWnd::UpdateScrollRange();
  UIScrollBar* bar = this->scrollBar;
  if (bar) {
    bar->Show(1);
    static_cast<UICashEmotionScrollBar*>(bar)->SetDisabled(this->wheelScroll == 0);
  }
}

// OFFSET: 0x007CC870
// FUNCTION: fn_09499
__declspec(noinline) UICashEmotionSprBtn::UICashEmotionSprBtn(UIWindow* target)
    : UISpriteButton2(target), m_selected(false) {
}

// OFFSET: 0x007CC990
// FUNCTION: fn_09501
void UICashEmotionSprBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (!this->m_selected) {
    UISpriteButton::Paint();
    return;
  }

  CDC* surface = this->geometry.surface;
  if (!surface) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->actionName.c_str()));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->spriteName.c_str()));
  if (!action || !sprite) {
    return;
  }

  CPaletteRes* paletteRes = nullptr;
  if (this->paletteName.size() != 0) {
    paletteRes = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->paletteName.c_str()));
  }

  const ActionFrame* frame = &action->Frame(this->animationIndex, this->frameIndex);
  if (!frame) {
    return;
  }

  const int width = this->geometry.width;
  const int height = this->geometry.height;
  SpriteAtlasRecord record{};
  int halfWidth = 0;
  int halfHeight = 0;
  int baseX = 0;
  int baseY = 0;
  const std::uint32_t* palette = paletteRes ? paletteRes->palette : sprite->palette;
  for (unsigned index = 0; index < frame->cells.size(); ++index) {
    record = SpriteAtlasRecord{};
    const SpriteFrameCell* cell = &frame->Cell(index);
    if (!cell || cell->image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
    if (!image) {
      continue;
    }

    CTexture* texture;
    if (cell->imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    if (!texture) {
      continue;
    }

    int x;
    int y;
    if (index == 0) {
      halfWidth = image->width / 2;
      x = width / 2 - halfWidth;
      halfHeight = image->height / 2;
      y = height / 2 - halfHeight;
      baseX = cell->x;
      baseY = cell->y;
    } else {
      x = width / 2 + (cell->x - baseX - halfWidth);
      y = height / 2 + (cell->y - baseY - halfHeight);
    }

    const int clipX = x < 0 ? -x : 0;
    const int clipY = y < 0 ? -y : 0;
    int drawWidth = image->width;
    if (drawWidth + x > width) {
      drawWidth = width - x;
    }
    drawWidth -= clipX;

    int drawHeight = image->height;
    if (drawHeight + y > height) {
      drawHeight = height - y;
    }
    drawHeight -= clipY;

    const int sourceX = static_cast<int>(record.u0 * 256.0f + clipX);
    const int sourceY = static_cast<int>(record.v0 * 256.0f + clipY);
    surface->DrawSurface(
      x < 0 ? 0 : x, y < 0 ? 0 : y, texture, sourceX < 0 ? 0 : sourceX, sourceY < 0 ? 0 : sourceY, drawWidth < 0 ? 0 : drawWidth,
      drawHeight < 0 ? 0 : drawHeight, cell->mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 1
    );
  }

  if (this->animate) {
    ++this->frameIndex;
    if (this->animationIndex >= static_cast<int>(action->animations.size())) {
      this->animationIndex = 0;
      this->frameIndex = 0;
    }

    if (this->frameIndex >= static_cast<int>(action->FrameCount(this->animationIndex))) {
      this->frameIndex = 0;
    }
  }
}

// OFFSET: 0x007CCE10
// FUNCTION: fn_09503
__declspec(noinline) void UICashEmotionSprBtn::SetSelected(bool selected) {
  if (selected != this->m_selected) {
    this->m_selected = selected;
    this->Invalidate();
  }
}

// OFFSET: 0x007CCD90
// FUNCTION: fn_09502
void UICashEmotionSprBtn::OnMouseHover(int x, int y) {
  if (!this->m_tooltip.empty()) {
    int screenX = 0;
    int screenY = -20;
    this->ToScreen(screenX, screenY);

    if (CMode* mode = g_modeMgr.gameMode) {
      mode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, false);
    }
  }

  if (!this->m_disabled) {
    this->OnMouseMove(x, y);
  }
}

// OFFSET: 0x008502C0
// FUNCTION: fn_10674
UISpriteButton2::UISpriteButton2(UIWindow* target)
    : UISpriteButton(), m_target(target) {
}

// OFFSET: 0x00850320
// FUNCTION: fn_10675
UISpriteButton::UISpriteButton() : UIButton() {
  this->animationIndex = 0;
  this->frameIndex = 0;
  this->spriteStateA = 0;
  this->spriteStateB = 0;
  this->animate = 0;
  this->spriteMode = 1;
}

// OFFSET: 0x00851C10
// FUNCTION: fn_10697
UISpriteButton2::~UISpriteButton2() {
  this->m_target = nullptr;
}

// OFFSET: 0x0085CF90
// FUNCTION: fn_10810
void UIHeadsprButton::Paint() {
  if (!this->geometry.surface) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->actionName.c_str()));
  if (!action) {
    return;
  }

  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->spriteName.c_str()));
  if (!sprite) {
    return;
  }

  const std::uint32_t* palette;
  if (this->paletteName.size() != 0) {
    palette = dynamic_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->paletteName.c_str()))->palette;
  } else {
    palette = sprite->palette;
  }

  const SpriteFrameCell& cell = action->Frame(this->animationIndex, 0).Cell(1);
  const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
  if (!image) {
    return;
  }

  SpriteAtlasRecord record{};
  CTexture* texture;
  if (cell.imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
  }

  if (!texture) {
    return;
  }

  const int sourceY = static_cast<int>(record.v0 * 256.0f);
  const int sourceX = static_cast<int>(record.u0 * 256.0f);
  this->geometry.surface->DrawSurface(
    0, 0, texture, sourceX < 0 ? 0 : sourceX, sourceY < 0 ? 0 : sourceY, image->width, image->height, cell.mirrored != 0, image->doubleWidth + 1,
    image->doubleHeight + 1, 0
  );
}

// OFFSET: 0x0085D6C0
// FUNCTION: fn_10816
void UISpriteButton::Paint() {
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->actionName.c_str()));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->spriteName.c_str()));
  if (!action || !sprite) {
    return;
  }

  CPaletteRes* paletteRes = nullptr;
  if (this->paletteName.size() != 0) {
    paletteRes = dynamic_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->paletteName.c_str()));
  }

  const ActionFrame& frame = action->Frame(this->animationIndex, this->frameIndex);
  int halfWidth = 0;
  int halfHeight = 0;
  int baseX = 0;
  int baseY = 0;
  const std::uint32_t* palette = paletteRes ? paletteRes->palette : sprite->palette;
  for (unsigned index = 0; index < frame.cells.size(); ++index) {
    SpriteAtlasRecord record{};
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }

    const int width = this->geometry.width;
    const int height = this->geometry.height;
    int x;
    int y;
    if (index == 0) {
      halfWidth = image->width / 2;
      x = width / 2 - halfWidth;
      halfHeight = image->height / 2;
      y = height / 2 - halfHeight;
      baseX = cell.x;
      baseY = cell.y;
    } else {
      x = width / 2 - baseX - halfWidth + cell.x;
      y = height / 2 - baseY - halfHeight + cell.y;
    }

    int drawWidth = image->width;
    int drawHeight = image->height;
    if (drawWidth + x > width) {
      drawWidth = width - x;
    }

    const int clipX = x < 0 ? -x : 0;
    const int left = x < 0 ? 0 : x;
    if (drawHeight + y > height) {
      drawHeight = height - y;
    }

    const int clipY = y < 0 ? -y : 0;
    drawWidth -= clipX;
    drawHeight -= clipY;
    const int sourceX = static_cast<int>(record.u0 * 256.0f + clipX);
    const float sourceY = record.v0 * 256.0f + clipY;
    if (this->geometry.surface) {
      const int top = static_cast<int>(sourceY);
      this->geometry.surface->DrawSurface(
        left, y < 0 ? 0 : y, texture, sourceX < 0 ? 0 : sourceX, top < 0 ? 0 : top, drawWidth < 0 ? 0 : drawWidth, drawHeight < 0 ? 0 : drawHeight,
        cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0
      );
    }
  }

  if (this->animate) {
    ++this->frameIndex;
    if (this->animationIndex >= static_cast<int>(action->animations.size())) {
      this->animationIndex = 0;
      this->frameIndex = 0;
    }

    if (this->frameIndex >= static_cast<int>(action->FrameCount(this->animationIndex))) {
      this->frameIndex = 0;
    }
  }
}

// OFFSET: 0x0085E9B0
// FUNCTION: fn_10834
void UISpriteButton2::OnLBtnDblClk(int x, int y) {
  this->OnLBtnDown(x, y);
  if (this->m_target) {
    this->m_target->OnLBtnDblClk(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085E9F0
// FUNCTION: fn_10835
void UISpriteButton::OnLBtnDblClk(int x, int y) {
  this->OnLBtnDown(x, y);
  this->Root()->OnLBtnDblClk(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085F160
// FUNCTION: fn_10857
void UISpriteButton2::OnMouseEnter() {
  if (this->m_target) {
    this->m_target->OnMouseEnter();
  }
}

// OFFSET: 0x0085F480
// FUNCTION: fn_10863
void UISpriteButton2::OnMouseHover(int x, int y) {
  UIButton::OnMouseHover(x, y);
  if (this->m_target) {
    this->m_target->OnMouseHover(this->geometry.x + x, this->geometry.y + y);
  }
}

// OFFSET: 0x0085F4C0
// FUNCTION: fn_10864
void UISpriteButton::OnMouseHover(int x, int y) {
  UIButton::OnMouseHover(x, y);
  this->Root()->OnMouseHover(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085F5E0
// FUNCTION: fn_10867
void UISpriteButton2::OnMouseLeave() {
  if (this->m_target) {
    this->m_target->OnMouseLeave();
  }
}

// OFFSET: 0x0085F870
// FUNCTION: fn_10871
void UISpriteButton::OnMouseMove(int x, int y) {
  this->Root()->OnMouseMove(this->geometry.x + x, this->geometry.y + y);
}

// OFFSET: 0x0085FFF0
// FUNCTION: fn_10895
void UISpriteButton2::OnWheel(int argument) {
  if (this->m_target) {
    this->m_target->OnWheel(argument);
  }
}

// OFFSET: 0x00865380
// FUNCTION: fn_10924
__declspec(noinline) void UISpriteButton::SetActName(const char* name) {
  this->actionName = name;
}

// OFFSET: 0x00867AA0
// FUNCTION: fn_10945
void UISpriteButton::SetPalette(const char* name) {
  this->paletteName = name;
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

// OFFSET: 0x0084FE00
// FUNCTION: fn_10666
UIHeadsprButton::UIHeadsprButton() : UISpriteButton() {
}
