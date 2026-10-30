#include "UI/EntryQueue/UIEntryQueueWnd.h"

#include "Client/ReplayPacketRecorder.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowDialogs.h"
#include <cstdio>
#include <cstring>
#include <utility>
#include <windows.h>

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

// OFFSET: 0x00BA6110

// OFFSET: 0x00BA6410
// FUNCTION: fn_19706
int UIEntryQueueWnd::SelectedIndex() {
  return this->m_list->selected;
}

// OFFSET: 0x00BA7440
// FUNCTION: fn_19709
void __stdcall Nullsub429() {
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
