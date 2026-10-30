#include "UI/EntryQueue/UIEntryQueueWnd.h"

#include "Client/ReplayPacketRecorder.h"
#include "Contents/EntryQueueMgr.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdio>
#include <cstring>
#include <utility>
#include <windows.h>

// OFFSET: 0x00BA4260
// FUNCTION: fn_19680
void UIEntryQueueHelpWnd::Slot18() {
}

// OFFSET: 0x00BA4270

// OFFSET: 0x00BA42A0
// FUNCTION: fn_19682
void UIEntryQueueHelpWnd::Slot19() {
}

// OFFSET: 0x00BA42B0

// OFFSET: 0x00BA42E0

// OFFSET: 0x00BA4490

// OFFSET: 0x00BA46A0

// OFFSET: 0x00BA4770

// OFFSET: 0x00BA48E0

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

// OFFSET: 0x00BA6410
// FUNCTION: fn_19706
int UIEntryQueueWnd::SelectedIndex() {
  return this->m_list->selected;
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
