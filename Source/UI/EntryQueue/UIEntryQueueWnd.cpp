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
#include <windows.h>

// OFFSET: 0x00BA5380
// FUNCTION: fn_19692
void UIEntryQueueRequestWnd::Slot18() {
}

// OFFSET: 0x00BA5390

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
