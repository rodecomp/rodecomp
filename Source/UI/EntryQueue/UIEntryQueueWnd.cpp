#include "UI/EntryQueue/UIEntryQueueWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/Messenger/ClientMessenger.h"
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
#include "UI/Window/WindowDialogs.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <windows.h>

// OFFSET: 0x00BA3920
// FUNCTION: fn_19677
UIEntryQueueHelpWnd::UIEntryQueueHelpWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0), m_viewer(nullptr), m_close(nullptr) {
}

// OFFSET: 0x00BA4260
// FUNCTION: fn_19680
void UIEntryQueueHelpWnd::Slot18() {
}

// OFFSET: 0x00BA4270
// FUNCTION: fn_19681
void UIEntryQueueHelpWnd::Paint() {
  auto& controls = this->m_entryQueue->text.buttons;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2125), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00BA42A0
// FUNCTION: fn_19682
void UIEntryQueueHelpWnd::Slot19() {
}

// OFFSET: 0x00BA42B0
// FUNCTION: fn_19683
int UIEntryQueueHelpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6 && a == 0) {
    this->m_frameContext->windows.CloseWindow(211);
  }

  return 0;
}

// OFFSET: 0x00BA42E0
// FUNCTION: fn_19684
UIEntryQueueListBox::UIEntryQueueListBox(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list) {
}

// OFFSET: 0x00BA43F0

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
void UIEntryQueueListBox::Slot31(int x, int y) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, 135, this->geometry.id, this->selected, 0, 0);
  }
}

// OFFSET: 0x00BA47A0
// FUNCTION: fn_19689
UIEntryQueueRequestWnd::UIEntryQueueRequestWnd(UIWindowContext& windows, UIFrameContext& frames, UIEntryQueueContext& entryQueue)
    : UIFrameWnd(windows, frames, 0), m_entryQueue(&entryQueue), m_interval(1000), m_elapsed(0), m_delta(0), m_lastTick(0) {
}

// OFFSET: 0x00BA4870

// OFFSET: 0x00BA48E0
// FUNCTION: fn_19691
bool UIEntryQueueRequestWnd::Create() {
  auto& text = this->m_entryQueue->text;
  auto& controls = text.buttons;

  this->Resize(300, 250);
  this->AddBaseButton(controls);
  this->AddCloseButton(controls, "", 0);
  this->SetSize(300, 250);
  this->Slot15(300, 250);
  this->Slot22(300, 250);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);

  this->m_viewer = new UITextViewer(*this->m_context, text);
  this->m_viewer->Configure(10, 22, 280, 305, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  char message[0x104];
  std::memset(message, 0, sizeof(message));
  if (g_entryQueueMgr->current) {
    sprintf_s(message, sizeof(message), LookupMessage(2126), g_entryQueueMgr->current->displayName);
  }

  this->m_viewer->Clear();
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2127)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(""), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2128)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2129)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2130)), 38, 0, 0);
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2131)), 38, 0, 0);

  const std::string path = "유저인터페이스\\";

  this->m_ok = new UIBitmapButton(*this->m_context, controls);
  this->m_ok->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
  this->m_ok->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  this->m_ok->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  this->m_ok->SetSize(this->m_ok->BitmapWidth(), this->m_ok->BitmapHeight());
  this->m_ok->Move(209, 226);
  this->m_ok->SetId(0);
  this->AddChild(this->m_ok);

  this->m_cancel = new UIBitmapButton(*this->m_context, controls);
  this->m_cancel->SetBitmap((path + "btn_cancel" + ".bmp").c_str(), 0, 0);
  this->m_cancel->SetBitmap((path + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
  this->m_cancel->SetBitmap((path + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
  this->m_cancel->SetSize(this->m_cancel->BitmapWidth(), this->m_cancel->BitmapHeight());
  this->m_cancel->Move(254, 226);
  this->m_cancel->SetId(1);
  this->AddChild(this->m_cancel);

  this->m_countdown = new UIStaticText(*this->m_context, controls);
  this->m_countdown->SetSize(150, 20);
  this->m_countdown->Move(10, this->geometry.height - 20);
  this->AddChild(this->m_countdown);
  return true;
}

// OFFSET: 0x00BA5380
// FUNCTION: fn_19692
void UIEntryQueueRequestWnd::Slot18() {
}

// OFFSET: 0x00BA5390
// FUNCTION: fn_19693
void UIEntryQueueRequestWnd::Paint() {
  auto& controls = this->m_entryQueue->text.buttons;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2133), 0);
  this->DrawPanelBackground(17, 0, "");

  char text[100];
  std::memset(text, 0, sizeof(text));
  const int remaining = 20 - static_cast<int>(this->m_elapsed / 1000 % 60);
  if (remaining < 1) {
    this->SendMsg(nullptr, 6, 1, 0, 0, 0);
  }

  std::sprintf(text, LookupMessage(2134), remaining);
  this->m_countdown->SetText(text, 0);
}

// OFFSET: 0x00BA5460
// FUNCTION: fn_19694
void UIEntryQueueRequestWnd::Slot19() {
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
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->m_entryQueue->recorder.Record(this->geometry.id, response, b, c, d);

    if (response == 0) {
      g_entryQueueMgr->SendAdmissionReply(1);
      this->m_frameContext->windows.CloseWindow(209);
      this->m_frameContext->windows.CloseWindow(210);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2132)), 0x64FFFF, 0, 0, 0);
    } else if (response == 1) {
      g_entryQueueMgr->SendAdmissionReply(2);
      this->m_frameContext->windows.CloseWindow(209);
      this->m_frameContext->windows.CloseWindow(210);
      return 0;
    }
  }

  return 0;
}

// OFFSET: 0x00BA5560
// FUNCTION: fn_19696
UIEntryQueueStandByWnd::UIEntryQueueStandByWnd(UIWindowContext& windows, UIFrameContext& frames, UIEntryQueueContext& entryQueue)
    : UIFrameWnd(windows, frames, 0), m_entryQueue(&entryQueue), m_waiting(0), m_ranking(0), m_viewer(nullptr), m_queueText(), m_waitingText(),
      m_rankingText() {
}

// OFFSET: 0x00BA5680

// OFFSET: 0x00BA5700
// FUNCTION: fn_19698
bool UIEntryQueueStandByWnd::Create() {
  auto& text = this->m_entryQueue->text;
  auto& controls = text.buttons;

  this->Resize(280, 120);
  this->AddBaseButton(controls);
  this->SetSize(280, 120);
  this->Slot15(280, 120);
  this->Slot22(280, 120);
  this->Move((g_renderer->viewport.width - 640) / 2 + 185, (g_renderer->viewport.height - 480) / 2 + 180);

  this->m_viewer = new UITextViewer(*this->m_context, text);
  this->m_viewer->Configure(10, 22, 260, 65, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  const std::string path = "유저인터페이스\\";

  this->m_cancel = new UIBitmapButton(*this->m_context, controls);
  this->m_cancel->SetBitmap((path + "cancel_waiting" + ".bmp").c_str(), 0, 0);
  this->m_cancel->SetBitmap((path + "cancel_waiting_a" + ".bmp").c_str(), 1, 0);
  this->m_cancel->SetBitmap((path + "cancel_waiting_b" + ".bmp").c_str(), 2, 0);
  this->m_cancel->SetSize(this->m_cancel->BitmapWidth(), this->m_cancel->BitmapHeight());
  this->m_cancel->Move(159, 96);
  this->m_cancel->SetId(0);
  this->AddChild(this->m_cancel);

  this->m_close = new UIBitmapButton(*this->m_context, controls);
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
void UIEntryQueueStandByWnd::Slot18() {
}

// OFFSET: 0x00BA5E60
// FUNCTION: fn_19700
void UIEntryQueueStandByWnd::Paint() {
  auto& controls = this->m_entryQueue->text.buttons;
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(2135), 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x00BA5E90
// FUNCTION: fn_19701
void UIEntryQueueStandByWnd::Slot19() {
}

// OFFSET: 0x00BA5EA0
// FUNCTION: fn_19702
int UIEntryQueueStandByWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->m_entryQueue->recorder.Record(this->geometry.id, response, b, c, d);

    if (response == 0) {
      g_entryQueueMgr->SendCancel();
    } else if (response == 1) {
      this->m_frameContext->windows.CloseWindow(209);
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
  sprintf_s(this->m_queueText, sizeof(this->m_queueText), LookupMessage(2136), queueName);
  sprintf_s(this->m_waitingText, sizeof(this->m_waitingText), LookupMessage(2137), waiting);
  sprintf_s(this->m_rankingText, sizeof(this->m_rankingText), LookupMessage(2138), ranking);

  if (this->m_viewer) {
    this->m_viewer->Clear();
    this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_queueText), 38, 0, 0);
    this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_waitingText), 38, 0, 0);
    this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(this->m_rankingText), 38, 0, 0);
  }
}

// OFFSET: 0x00BA6000
// FUNCTION: fn_19704
UIEntryQueueWnd::UIEntryQueueWnd(UIWindowContext& windows, UIFrameContext& frames, UIEntryQueueContext& entryQueue, UIListBoxContext& list)
    : UIFrameWnd(windows, frames, 0), unknown_00BC(), m_labels(), m_values(), m_list(nullptr), saved(nullptr), m_entryQueue(&entryQueue),
      m_listContext(&list), m_applyButtons(), m_helpButton(nullptr) {
}

// OFFSET: 0x00BA6110

// OFFSET: 0x00BA6410
// FUNCTION: fn_19706
int UIEntryQueueWnd::SelectedIndex() {
  return this->m_list->selected;
}

// OFFSET: 0x00BA6420
// FUNCTION: fn_19707
void UIEntryQueueWnd::Slot15(int width, int height) {
  auto& controls = this->m_entryQueue->text.buttons;

  this->Resize(350, 350);
  this->AddBaseButton(controls);
  this->AddCloseButton(controls, "", 0);

  this->m_list = new UIEntryQueueListBox(*this->m_context, *this->m_listContext);
  this->m_list->SetSize(130, 270);
  this->m_list->Move(10, 40);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->AddChild(this->m_list);

  for (std::map<int, EntryQueueInfo*>::iterator it = g_entryQueueList.begin(); it != g_entryQueueList.end(); ++it) {
    this->m_list->AddEntry(it->second->displayName, it->second->id);
  }

  std::map<int, EntryQueueInfo*>::iterator first = g_entryQueueList.begin();
  g_entryQueueMgr->current = first->second;

  const char* labels[7] = {
    LookupMessage(0x85B), LookupMessage(0x85C), LookupMessage(0x85D), LookupMessage(0x85E),
    LookupMessage(0x85F), LookupMessage(0x860), LookupMessage(0x861),
  };
  int index = 0;
  for (int y = 165; y < 305; y += 20, ++index) {
    this->m_labels[index] = new UIStaticText(*this->m_context, controls);
    if (this->m_labels[index]) {
      const char* label = labels[index];
      const TextLocale locale = {};
      this->m_labels[index]->SetSize(this->TextWidth(g_fontCache, locale, label, 0, 0, 12, 0, 0), 25);
      this->m_labels[index]->SetText(label, 0);
      this->m_labels[index]->Move(157, y);
      this->AddChild(this->m_labels[index]);
    }
  }

  for (int value = 0; value < 7; ++value) {
    this->m_values[value] = new UIStaticText(*this->m_context, controls);
    if (this->m_labels[value]) {
      this->m_values[value]->SetSize(157, 25);
      this->AddChild(this->m_values[value]);
    }
  }

  this->SelectQueue(first->second);

  const std::string path = "유저인터페이스\\battle_field\\";
  const char* names[9] = {
    "apply_individual", "apply_individual_a", "apply_individual_b", "apply_party", "apply_party_a",
    "apply_party_b",    "apply_guild",        "apply_guild_a",      "apply_guild_b",
  };
  int id = 0;
  const char** name = names;
  for (int x = 8; x < 287; x += 93, name += 3) {
    this->m_applyButtons[id] = new UIBitmapButton(*this->m_context, controls);
    this->m_applyButtons[id]->SetBitmap((path + name[0] + ".bmp").c_str(), 0, 0);
    this->m_applyButtons[id]->SetBitmap((path + name[1] + ".bmp").c_str(), 1, 0);
    this->m_applyButtons[id]->SetBitmap((path + name[2] + ".bmp").c_str(), 2, 0);
    this->m_applyButtons[id]->SetSize(this->m_applyButtons[id]->BitmapWidth(), this->m_applyButtons[id]->BitmapHeight());
    this->m_applyButtons[id]->Move(x, height - 24);
    this->m_applyButtons[id]->SetId(id);
    this->AddChild(this->m_applyButtons[id]);
    ++id;
  }

  this->m_helpButton = new UIBitmapButton(*this->m_context, controls);
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
  const TextLocale locale = {};
  this->DrawTitleBar(g_fontCache, locale, true, LookupMessage(0x603), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);

  EntryQueueInfo* info = g_entryQueueMgr->current;
  if (info) {
    const std::string path = "유저인터페이스\\battle_field\\";
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + info->unknown_0634).c_str())));
    if (bitmap) {
      this->DrawBitmap(157, 25, &bitmap->image, false);
    }
  }

  this->DrawShadowedText(10, 23, LookupMessage(0x865), static_cast<int>(std::strlen(LookupMessage(0x865))), 0, 0xC0C0C0, 0, 12, 0);
}

// OFFSET: 0x00BA7440
// FUNCTION: fn_19709
void __stdcall Nullsub429() {
}

// OFFSET: 0x00BA7450
// FUNCTION: fn_19710
int UIEntryQueueWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  EntryQueueInfo* info = g_entryQueueMgr->current;
  if (message == 6) {
    int error;
    switch (a) {
    case 0:
      if (g_session.GetMapProperty13() == 1) {
        error = 0x86B;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsActiveSkillEffectId(0x29C)) {
        error = 0x86C;
      } else {
        if (this->ConfirmApply(LookupMessage(0x862))) {
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
      } else if (ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount() == 0) {
        error = 0x86D;
      } else if (g_session.unknown_5464 == 0) {
        error = 0x86E;
      } else if (static_cast<unsigned>(g_entryQueueMgr->current->unknown_020C) <
                 static_cast<unsigned>(ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.OnlinePartyCount())) {
        error = 0x86F;
      } else {
        if (this->ConfirmApply(LookupMessage(0x863))) {
          g_entryQueueMgr->SendApply(2, info->name);
        }
        return 0;
      }
      break;
    case 2: {
      GuildInfo* guild = reinterpret_cast<GuildInfo*>(reinterpret_cast<char*>(&g_guildId) - offsetof(GuildInfo, m_guildId));
      if (g_session.GetMapProperty13() == 1) {
        error = 0x86B;
      } else if (static_cast<CGameMode*>(g_modeMgr.GetGameMode())->IsActiveSkillEffectId(0x29C)) {
        error = 0x86C;
      } else if (guild->m_guildId < 1) {
        error = 0x870;
      } else if (guild->unknown_00B4 == 0) {
        error = 0x871;
      } else if (static_cast<unsigned>(g_entryQueueMgr->current->unknown_020C) < static_cast<unsigned>(guild->unknown_0068)) {
        error = 0x872;
      } else {
        if (this->ConfirmApply(LookupMessage(0x864))) {
          g_entryQueueMgr->SendApply(4, info->name);
        }
        return 0;
      }
      break;
    }
    case 3:
      if (!this->m_frameContext->windows.CloseWindow(211)) {
        this->m_frameContext->windows.EnsureWindow(211);
      }
      return 0;
    case 4:
    case 201:
      this->m_entryQueue->recorder.Record(this->geometry.id, static_cast<int>(a), b, c, d);
      this->m_frameContext->windows.CloseWindow(157);
      return 0;
    default:
      return 0;
    }

    reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(error), 0, false, false, 0, "Message", 280, 120, 0);
    return 0;
  }

  if (message == 34) {
    this->saved = reinterpret_cast<EntryQueueWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  }

  if (message == 135 && this->m_list) {
    const int value = this->m_list->EntryValue(static_cast<int>(b));
    std::map<int, EntryQueueInfo*>::iterator it = g_entryQueueList.find(value);
    if (it != g_entryQueueList.end() && it->second) {
      this->SelectQueue(it->second);
    }
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
    case 1:
      strcpy_s(text, sizeof(text), info->unknown_0530);
      break;
    case 2:
      sprintf_s(text, sizeof(text), LookupMessage(0x866), info->unknown_020C, info->unknown_0210);
      break;
    case 3:
      switch (info->unknown_0218) {
      case 1:
        sprintf_s(text, sizeof(text), LookupMessage(0x867), info->unknown_021C);
        break;
      case 2:
        sprintf_s(text, sizeof(text), LookupMessage(0x868), info->unknown_021C);
        break;
      case 3:
        sprintf_s(text, sizeof(text), LookupMessage(0x869), info->unknown_021C, info->unknown_0220);
        break;
      default:
        sprintf_s(text, sizeof(text), LookupMessage(0x86A));
        break;
      }
      break;
    case 4:
      strcpy_s(text, sizeof(text), info->unknown_0224);
      break;
    case 5:
      strcpy_s(text, sizeof(text), info->unknown_0328);
      break;
    case 6:
      strcpy_s(text, sizeof(text), info->unknown_042C);
      break;
    }

    UIWindow* label = this->m_labels[index];
    const int x = label->geometry.x;
    const int width = label->geometry.width;
    const int y = label->geometry.y;
    const TextLocale locale = {};
    this->m_values[index]->Resize(this->TextWidth(g_fontCache, locale, text, 0, 0, 12, 0, 0), 25);
    this->m_values[index]->SetText(text, 0);
    this->m_values[index]->Move(x + width + 5, y);
    this->m_values[index]->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x00BA7AE0
// FUNCTION: fn_19712
bool UIEntryQueueWnd::ConfirmApply(const char* type) {
  EntryQueueInfo* info = g_entryQueueMgr->current;
  if (!info) {
    return false;
  }

  char text[0x104];
  std::memset(text, 0, sizeof(text));
  sprintf_s(text, sizeof(text), "%s %s", info->displayName, type);
  return reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 2, false, false, 0, "Message", 280, 120, 0) == 187;
}

// OFFSET: 0x00BA7B90
// FUNCTION: fn_19713
void UIEntryQueueWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00BA3A50
// FUNCTION: fn_19679
void UIEntryQueueHelpWnd::Slot15(int width, int height) {
  auto& text = this->m_entryQueue->text;
  auto& controls = text.buttons;

  this->Resize(280, 280);

  this->m_viewer = new UITextViewer(*this->m_context, text);
  this->m_viewer->Configure(10, 22, 270, 225, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->m_viewer->backgroundColor = 0xFF000000u | (blue << 16) | (green << 8) | red;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);

  char message[0x104];
  std::memset(message, 0, sizeof(message));
  strcpy_s(message, sizeof(message), LookupMessage(2116));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2117));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2118));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2119));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2120));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2121));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2122));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2123));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);
  strcpy_s(message, sizeof(message), LookupMessage(2124));
  this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(message), 38, 0, 0);

  const std::string path = "유저인터페이스\\";

  this->m_close = new UIBitmapButton(*this->m_context, controls);
  this->m_close->SetBitmap((path + "btn_close" + ".bmp").c_str(), 0, 0);
  this->m_close->SetBitmap((path + "btn_close_a" + ".bmp").c_str(), 1, 0);
  this->m_close->SetBitmap((path + "btn_close_b" + ".bmp").c_str(), 2, 0);
  this->m_close->SetSize(this->m_close->BitmapWidth(), this->m_close->BitmapHeight());
  this->m_close->Move(234, 256);
  this->m_close->SetId(0);
  this->AddChild(this->m_close);
}
