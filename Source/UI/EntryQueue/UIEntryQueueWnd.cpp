#include "UI/EntryQueue/UIEntryQueueWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Resource/MSGSTRINGID.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <windows.h>

// OFFSET: 0x00BA3920
// FUNCTION: fn_19677
UIEntryQueueHelpWnd::UIEntryQueueHelpWnd() : UIFrameWnd(0), m_viewer(nullptr), m_close(nullptr) {
}

// OFFSET: 0x00BA4260
// FUNCTION: fn_19680
void UIEntryQueueHelpWnd::OnDestroy() {
}

// OFFSET: 0x00BA4270
// FUNCTION: fn_19681
void UIEntryQueueHelpWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_BATTLEFIELD_WARNINGMSG1), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00BA42A0
// FUNCTION: fn_19682
void UIEntryQueueHelpWnd::OnProcess() {
}

// OFFSET: 0x00BA42B0
// FUNCTION: fn_19683
int UIEntryQueueHelpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND && a == 0) {
    g_windowMgr.CloseWindow(WID_ENTRYQUEUEHELPWND);
  }

  return 0;
}

// OFFSET: 0x00BA42E0
// FUNCTION: fn_19684
__declspec(noinline) UIEntryQueueListBox::UIEntryQueueListBox() : UIListBox() {
}

// INLINED
UIEntryQueueListBox::~UIEntryQueueListBox() {
}


// OFFSET: 0x00BA4490
// FUNCTION: fn_19686
void UIEntryQueueListBox::AddEntry(const char* name, int value) {
  if (this->m_entries.insert(std::map<std::string, int>::value_type(name, value)).second) {
    this->AddLine(name);
  }
}

// OFFSET: 0x00BA46A0
// FUNCTION: fn_19687
int UIEntryQueueListBox::EntryValue(int index) {
  const std::string name = this->lines[index];
  const auto it = this->m_entries.find(name);
  if (it != this->m_entries.end()) {
    return it->second;
  }

  return -1;
}

// OFFSET: 0x00BA4770
// FUNCTION: fn_19688
void UIEntryQueueListBox::OnLBtnUp(int x, int y) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, UM_LIST_SELECTION, this->geometry.id, this->selected, 0, 0);
  }
}

// OFFSET: 0x00BA47A0
// FUNCTION: fn_19689
UIEntryQueueRequestWnd::UIEntryQueueRequestWnd()
    : UIFrameWnd(0), m_interval(1000), m_elapsed(0), m_delta(0), m_lastTick(0) {
}

// INLINED
UIEntryQueueRequestWnd::~UIEntryQueueRequestWnd() {
}


// OFFSET: 0x00BA48E0
// FUNCTION: fn_19691
bool UIEntryQueueRequestWnd::Create() {
  this->Resize(300, 250);
  this->AddBaseButton();
  this->AddCloseButton("", 0);
  this->SetSize(300, 250);
  this->OnCreate(300, 250);
  this->OnSize(300, 250);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);

  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 22, 280, 305, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  char message[0x104];
  std::memset(message, 0, sizeof(message));
  if (g_entryQueueMgr->current) {
    sprintf_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_WARNINGMSG2), g_entryQueueMgr->current->displayName);
  }

  this->m_viewer->Clear();
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_WARNINGMSG3)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(""), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_WARNINGMSG4)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_WAIT_PLAYER)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_REQUEST_JOIN)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_WAITTIME_D)), 38, 0, 0);

  const std::string path = "유저인터페이스\\";

  this->m_ok = new UIBitmapButton();
  this->m_ok->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
  this->m_ok->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  this->m_ok->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  this->m_ok->SetSize(this->m_ok->BitmapWidth(), this->m_ok->BitmapHeight());
  this->m_ok->Move(209, 226);
  this->m_ok->SetId(0);
  this->AddChild(this->m_ok);

  this->m_cancel = new UIBitmapButton();
  this->m_cancel->SetBitmap((path + "btn_cancel" + ".bmp").c_str(), 0, 0);
  this->m_cancel->SetBitmap((path + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
  this->m_cancel->SetBitmap((path + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
  this->m_cancel->SetSize(this->m_cancel->BitmapWidth(), this->m_cancel->BitmapHeight());
  this->m_cancel->Move(254, 226);
  this->m_cancel->SetId(1);
  this->AddChild(this->m_cancel);

  this->m_countdown = new UIStaticText();
  this->m_countdown->SetSize(150, 20);
  this->m_countdown->Move(10, this->geometry.height - 20);
  this->AddChild(this->m_countdown);
  return true;
}

// OFFSET: 0x00BA5380
// FUNCTION: fn_19692
void UIEntryQueueRequestWnd::OnDestroy() {
}

// OFFSET: 0x00BA5390
// FUNCTION: fn_19693
void UIEntryQueueRequestWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_BATTLEFIELD_NAME_S), 0);
  this->DrawPanelBackground(17, 0, "");

  char text[100];
  std::memset(text, 0, sizeof(text));
  const int remaining = 20 - static_cast<int>(this->m_elapsed / 1000 % 60);
  if (remaining <= 0) {
    this->SendMsg(nullptr, UM_COMMAND, 1, 0, 0, 0);
  }

  std::sprintf(text, LookupMessage(MSI_BATTLEFIELD_MEMBER_D), remaining);
  this->m_countdown->SetText(text, 0);
}

// OFFSET: 0x00BA5460
// FUNCTION: fn_19694
void UIEntryQueueRequestWnd::OnProcess() {
  const DWORD now = GetTickCount();
  const unsigned delta = now - this->m_lastTick;
  this->m_delta = delta;
  if (delta > this->m_interval) {
    this->m_elapsed += this->m_interval;
    this->m_lastTick = now;
    this->Invalidate();
  }
}

// OFFSET: 0x00BA54A0
// FUNCTION: fn_19695
int UIEntryQueueRequestWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND) {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case 0:
      g_entryQueueMgr->SendAdmissionReply(1);
      g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND);
      g_windowMgr.CloseWindow(WID_ENTRYQUEUEREQUESTWND);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BATTLEFIELD_STATE_JOINWAIT)), 0x64FFFF, 0, 0, 0);
      break;
    case 1:
      g_entryQueueMgr->SendAdmissionReply(2);
      g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND);
      g_windowMgr.CloseWindow(WID_ENTRYQUEUEREQUESTWND);
      return 0;
    }
  }

  return 0;
}

// OFFSET: 0x00BA5560
// FUNCTION: fn_19696
UIEntryQueueStandByWnd::UIEntryQueueStandByWnd()
    : UIFrameWnd(0), m_waiting(0), m_ranking(0), m_viewer(nullptr), m_queueText(), m_waitingText(),
      m_rankingText() {
}

// INLINED
UIEntryQueueStandByWnd::~UIEntryQueueStandByWnd() {
}


// OFFSET: 0x00BA5700
// FUNCTION: fn_19698
bool UIEntryQueueStandByWnd::Create() {
  this->Resize(280, 120);
  this->AddBaseButton();
  this->SetSize(280, 120);
  this->OnCreate(280, 120);
  this->OnSize(280, 120);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);

  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 22, 260, 65, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  const std::string path = "유저인터페이스\\";

  this->m_cancel = new UIBitmapButton();
  this->m_cancel->SetBitmap((path + "cancel_waiting" + ".bmp").c_str(), 0, 0);
  this->m_cancel->SetBitmap((path + "cancel_waiting_a" + ".bmp").c_str(), 1, 0);
  this->m_cancel->SetBitmap((path + "cancel_waiting_b" + ".bmp").c_str(), 2, 0);
  this->m_cancel->SetSize(this->m_cancel->BitmapWidth(), this->m_cancel->BitmapHeight());
  this->m_cancel->Move(159, 96);
  this->m_cancel->SetId(0);
  this->AddChild(this->m_cancel);

  this->m_close = new UIBitmapButton();
  this->m_close->SetBitmap((path + "btn_close" + ".bmp").c_str(), 0, 0);
  this->m_close->SetBitmap((path + "btn_close_a" + ".bmp").c_str(), 1, 0);
  this->m_close->SetBitmap((path + "btn_close_b" + ".bmp").c_str(), 2, 0);
  this->m_close->SetSize(this->m_close->BitmapWidth(), this->m_close->BitmapHeight());
  this->m_close->Move(234, 96);
  this->m_close->SetId(1);
  this->AddChild(this->m_close);
  return true;
}

// OFFSET: 0x00BA5E50
// FUNCTION: fn_19699
void UIEntryQueueStandByWnd::OnDestroy() {
}

// OFFSET: 0x00BA5E60
// FUNCTION: fn_19700
void UIEntryQueueStandByWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_BATTLEFIELD_MYRANK_D), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00BA5E90
// FUNCTION: fn_19701
void UIEntryQueueStandByWnd::OnProcess() {
}

// OFFSET: 0x00BA5EA0
// FUNCTION: fn_19702
int UIEntryQueueStandByWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND) {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case 0:
      g_entryQueueMgr->SendCancel();
      break;
    case 1:
      g_windowMgr.CloseWindow(WID_ENTRYQUEUESTANDBYWND);
      return 0;
    }
  }

  return 0;
}

// OFFSET: 0x00BA5EF0
// FUNCTION: fn_19703
void UIEntryQueueStandByWnd::SetInfo(const char* queueName, int waiting, int ranking) {
  this->m_queueName = queueName;
  this->m_waiting = waiting;
  this->m_ranking = ranking;
  sprintf_s(this->m_queueText, sizeof(this->m_queueText), LookupMessage(MSI_BATTLEFIELD_DP_TITLE), queueName);
  sprintf_s(this->m_waitingText, sizeof(this->m_waitingText), LookupMessage(MSI_BATTLEFIELD_DP_AIM), waiting);
  sprintf_s(this->m_rankingText, sizeof(this->m_rankingText), LookupMessage(MSI_BATTLEFIELD_DP_MEMBER), ranking);

  if (this->m_viewer) {
    this->m_viewer->Clear();
    this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->m_queueText), 38, 0, 0);
    this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->m_waitingText), 38, 0, 0);
    this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(this->m_rankingText), 38, 0, 0);
  }
}

// OFFSET: 0x00BA6000
// FUNCTION: fn_19704
UIEntryQueueWnd::UIEntryQueueWnd() : UIFrameWnd(0) {
  this->m_list = nullptr;
  this->saved = nullptr;
  this->m_helpButton = nullptr;
  std::memset(this->m_applyButtons, 0, sizeof(this->m_applyButtons));
  std::memset(this->m_labels, 0, sizeof(this->m_labels));
  std::memset(this->m_values, 0, sizeof(this->m_values));
}

// INLINED
UIEntryQueueWnd::~UIEntryQueueWnd() {
}


// OFFSET: 0x00BA6410
// FUNCTION: fn_19706
int UIEntryQueueWnd::SelectedIndex() {
  return this->m_list->selected;
}

// OFFSET: 0x00BA6420
// FUNCTION: fn_19707
void UIEntryQueueWnd::OnCreate(int width, int height) {

  this->Resize(350, 350);
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_list = new UIEntryQueueListBox();
  this->m_list->SetSize(130, 270);
  this->m_list->Move(10, 40);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->AddChild(this->m_list);

  const std::map<int, EntryQueueInfo*>::iterator end = g_entryQueueList.end();
  for (std::map<int, EntryQueueInfo*>::iterator it = g_entryQueueList.begin(); it != end; ++it) {
    this->m_list->AddEntry(it->second->displayName, it->second->id);
  }

  std::map<int, EntryQueueInfo*>::iterator first = g_entryQueueList.begin();
  g_entryQueueMgr->current = first->second;

  const char* labels[7] = {
    LookupMessage(MSI_BATTLEFIELD_DP_REQUIRED), LookupMessage(MSI_BATTLEFIELD_DP_WIN), LookupMessage(MSI_BATTLEFIELD_DP_DRAW), LookupMessage(MSI_BATTLEFIELD_DP_LOSS),
    LookupMessage(MSI_BATTLEFIELD_DOYOU_SINGLE), LookupMessage(MSI_BATTLEFIELD_DOYOU_PALTY), LookupMessage(MSI_BATTLEFIELD_DOYOU_GUILD),
  };
  int index = 0;
  for (int y = 165; y < 305; ++index, y += 20) {
    this->m_labels[index] = new UIStaticText();
    if (this->m_labels[index]) {
      const char* label = labels[index];
      const int labelWidth = this->TextWidth(label, 0, 0, 12, 0, 0);
      this->m_labels[index]->SetSize(labelWidth, 25);
      this->m_labels[index]->SetText(label, 0);
      this->m_labels[index]->Move(157, y);
      this->AddChild(this->m_labels[index]);
    }
  }

  for (int value = 0; value < 7; ++value) {
    this->m_values[value] = new UIStaticText();
    if (this->m_labels[value]) {
      this->m_values[value]->SetSize(157, 25);
      this->AddChild(this->m_values[value]);
    }
  }

  this->SelectQueue(first->second);

  const std::string path = "유저인터페이스\\battle_field\\";
  const char* names[3][3] = {
    {"apply_individual", "apply_individual_a", "apply_individual_b"},
    {"apply_party", "apply_party_a", "apply_party_b"},
    {"apply_guild", "apply_guild_a", "apply_guild_b"},
  };
  int id = 0;
  for (int x = 8; x < 287; x += 93) {
    this->m_applyButtons[id] = new UIBitmapButton();
    this->m_applyButtons[id]->SetBitmap((path + names[id][0] + ".bmp").c_str(), 0, 0);
    this->m_applyButtons[id]->SetBitmap((path + names[id][1] + ".bmp").c_str(), 1, 0);
    this->m_applyButtons[id]->SetBitmap((path + names[id][2] + ".bmp").c_str(), 2, 0);
    this->m_applyButtons[id]->SetSize(this->m_applyButtons[id]->BitmapWidth(), this->m_applyButtons[id]->BitmapHeight());
    this->m_applyButtons[id]->Move(x, height - 24);
    this->m_applyButtons[id]->SetId(id);
    this->AddChild(this->m_applyButtons[id]);
    ++id;
  }

  this->m_helpButton = new UIBitmapButton();
  this->m_helpButton->SetBitmap((path + "btn_help" + ".bmp").c_str(), 0, 0);
  this->m_helpButton->SetBitmap((path + "btn_help" + "_a.bmp").c_str(), 1, 0);
  this->m_helpButton->SetBitmap((path + "btn_help" + "_b.bmp").c_str(), 2, 0);
  this->m_helpButton->SetSize(this->m_helpButton->BitmapWidth(), this->m_helpButton->BitmapHeight());
  this->m_helpButton->Move(width - 62, height - 24);
  this->m_helpButton->SetId(3);
  this->AddChild(this->m_helpButton);
}

// OFFSET: 0x00BA7250
// FUNCTION: fn_19708
void UIEntryQueueWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_BATTLEFIELD_LIST), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);

  EntryQueueInfo* info = g_entryQueueMgr->current;
  if (info) {
    const std::string path = "유저인터페이스\\battle_field\\";
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + info->bitmapName).c_str())));
    if (bitmap) {
      this->DrawBitmap(157, 25, bitmap, false);
    }
  }

  this->DrawShadowedText(10, 23, LookupMessage(MSI_BATTLEFIELD_LEVELUP_D), static_cast<int>(std::strlen(LookupMessage(MSI_BATTLEFIELD_LEVELUP_D))), 0, 0xC0C0C0, 0, 12, 0);
}

// OFFSET: 0x00BA7440
// FUNCTION: fn_19709
void __stdcall Nullsub429() {
}

// OFFSET: 0x00BA7450
// FUNCTION: fn_19710
int UIEntryQueueWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  EntryQueueInfo* info = g_entryQueueMgr->current;
  switch (message) {
  case UM_COMMAND: {
    int error;
    switch (a) {
    case 0:
      if (g_session.GetMapProperty13() == 1) {
        error = 0x86B;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsActiveSkillEffectId(0x29C)) {
        error = 0x86C;
      } else {
        if (this->ConfirmApply(LookupMessage(MSI_BATTLEFIELD_LIST2))) {
          g_entryQueueMgr->SendApply(1, info->name);
        }
        return 0;
      }
      break;
    case 1:
      if (g_session.GetMapProperty13() == 1) {
        error = 0x86B;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsActiveSkillEffectId(0x29C)) {
        error = 0x86C;
      } else if (g_session.PartyCount() == 0) {
        error = 0x86D;
      } else if (g_session.partyLeader == 0) {
        error = 0x86E;
      } else if (static_cast<unsigned>(g_entryQueueMgr->current->minLevel) <
                 static_cast<unsigned>(g_session.OnlinePartyCount())) {
        error = 0x86F;
      } else {
        if (this->ConfirmApply(LookupMessage(MSI_BATTLEFIELD_D_VS_D))) {
          g_entryQueueMgr->SendApply(2, info->name);
        }
        return 0;
      }
      break;
    case 2: {
      GuildInfo* guild = &g_guildInfo;
      if (g_session.GetMapProperty13() == 1) {
        error = 0x86B;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsActiveSkillEffectId(0x29C)) {
        error = 0x86C;
      } else if (guild->m_guildId < 1) {
        error = 0x870;
      } else if (guild->isMaster == 0) {
        error = 0x871;
      } else if (static_cast<unsigned>(g_entryQueueMgr->current->minLevel) < static_cast<unsigned>(guild->onlineMemberCount)) {
        error = 0x872;
      } else {
        if (this->ConfirmApply(LookupMessage(MSI_BATTLEFIELD_LEVELDOWN_D))) {
          g_entryQueueMgr->SendApply(4, info->name);
        }
        return 0;
      }
      break;
    }
    case 3:
      if (!g_windowMgr.CloseWindow(WID_ENTRYQUEUEHELPWND)) {
        g_windowMgr.EnsureWindow(WID_ENTRYQUEUEHELPWND);
      }
      return 0;
    case 4:
    case ID_CLOSE:
      this->RecordResponse(static_cast<int>(a), b, c, d);
      g_windowMgr.CloseWindow(WID_JOINTO_BATTLEFIELDWND);
      return 0;
    default:
      return 0;
    }
    g_windowMgr.Confirm(LookupMessage(error), 0, false, false, 0, "Message", 280, 120, 0);
    return 0;
  }
  case UM_SETINFO:
    this->saved = reinterpret_cast<EntryQueueWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_LIST_SELECTION:
    if (this->m_list) {
      const int value = this->m_list->EntryValue(static_cast<int>(b));
      std::map<int, EntryQueueInfo*>::iterator it = g_entryQueueList.find(value);
      if (it != g_entryQueueList.end() && it->second) {
        this->SelectQueue(it->second);
      }
    }
    break;
  }

  return 0;
}

// OFFSET: 0x00BA78A0
// FUNCTION: fn_19711
void UIEntryQueueWnd::SelectQueue(EntryQueueInfo* info) {
  g_entryQueueMgr->current = info;

  char text[0x104];
  std::memset(text, 0, sizeof(text));
  for (int index = 0; index < 7; ++index) {
    if (!this->m_values[index]) {
      continue;
    }

    switch (index) {
    case 0:
      strcpy_s(text, sizeof(text), info->displayName);
      break;
    case 2:
      sprintf_s(text, sizeof(text), LookupMessage(MSI_BATTLEFIELD_LEVEL_D_D), info->minLevel, info->maxLevel);
      break;
    case 3:
      if (info->limitType == 1) {
        sprintf_s(text, sizeof(text), LookupMessage(MSI_BATTLEFIELD_LIMITLESS), info->limitValue);
      } else if (info->limitType == 2) {
        sprintf_s(text, sizeof(text), LookupMessage(MSI_BATTLEFIELD_MSG_NOT_JOINMAP), info->limitValue);
      } else if (info->limitType == 3) {
        sprintf_s(text, sizeof(text), LookupMessage(MSI_BATTLEFIELD_MSG_WAIT_1MM), info->limitValue, info->limitValueEnd);
      } else {
        sprintf_s(text, sizeof(text), LookupMessage(MSI_BATTLEFIELD_MSG_PLZ_PALTYJOIN));
      }
      break;
    case 4:
      strcpy_s(text, sizeof(text), info->description);
      break;
    case 5:
      strcpy_s(text, sizeof(text), info->rulesText);
      break;
    case 6:
      strcpy_s(text, sizeof(text), info->rewardText);
      break;
    case 1:
      strcpy_s(text, sizeof(text), info->extraText);
      break;
    }

    UIWindow* label = this->m_labels[index];
    const int x = label->geometry.x + label->geometry.width + 5;
    const int y = label->geometry.y;
    const int textWidth = this->TextWidth(text, 0, 0, 12, 0, 0);
    this->m_values[index]->Resize(textWidth, 25);
    this->m_values[index]->SetText(text, 0);
    this->m_values[index]->Move(x, y);
    this->m_values[index]->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x00BA7AE0
// FUNCTION: fn_19712
bool UIEntryQueueWnd::ConfirmApply(const char* type) {
  EntryQueueInfo* info = g_entryQueueMgr->current;
  if (info) {
    char text[0x104];
    std::memset(text, 0, sizeof(text));
    sprintf_s(text, sizeof(text), "%s %s", info->displayName, type);
    return g_windowMgr.Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 187;
  }

  return false;
}

// OFFSET: 0x00BA7B90
// FUNCTION: fn_19713
void UIEntryQueueWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00BA3A50
// FUNCTION: fn_19679
void UIEntryQueueHelpWnd::OnCreate(int width, int height) {
  this->Resize(280, 280);

  this->m_viewer = new UITextViewer();
  this->m_viewer->Configure(10, 22, 270, 225, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  char message[0x104];
  std::memset(message, 0, sizeof(message));
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP4));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP5));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP6));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP7));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP8));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP9));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_HELP10));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_START));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(MSI_BATTLEFIELD_DOYOU_MOVE));
  this->m_viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);

  const std::string path = "유저인터페이스\\";

  this->m_close = new UIBitmapButton();
  this->m_close->SetBitmap((path + "btn_close" + ".bmp").c_str(), 0, 0);
  this->m_close->SetBitmap((path + "btn_close_a" + ".bmp").c_str(), 1, 0);
  this->m_close->SetBitmap((path + "btn_close_b" + ".bmp").c_str(), 2, 0);
  this->m_close->SetSize(this->m_close->BitmapWidth(), this->m_close->BitmapHeight());
  this->m_close->Move(234, 256);
  this->m_close->SetId(0);
  this->AddChild(this->m_close);
}
