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

// OFFSET: 0x00BA54A0

// OFFSET: 0x00BA5700

// OFFSET: 0x00BA5E50
// FUNCTION: fn_19699
void UIEntryQueueStandByWnd::Slot18() {
}

// OFFSET: 0x00BA5E60

// OFFSET: 0x00BA5E90
// FUNCTION: fn_19701
void UIEntryQueueStandByWnd::Slot19() {
}

// OFFSET: 0x00BA5EA0

// OFFSET: 0x00BA5EF0

// OFFSET: 0x00BA6410

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
