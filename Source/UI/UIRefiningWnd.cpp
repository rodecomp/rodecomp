#include "UI/UIRefiningWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007F5270
// FUNCTION: fn_09902
void UIRefiningWnd::Slot10(int x, int y, std::intptr_t value) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46 && drag->source == 1) {
    this->SendMsg(this, 23, drag->id, 0, 0, 0);
  }
  g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x007F52D0

// OFFSET: 0x007F54B0
// FUNCTION: fn_09904
std::uint32_t UIRefiningWnd::SelectedMaterialValue() {
  if (this->item.unknown_0000[1] != 0 && !this->materials.empty() && this->selectedMaterial >= 0 &&
      static_cast<unsigned>(this->selectedMaterial) < this->materials.size()) {
    return this->materials[this->selectedMaterial].unknown_0005;
  }
  return 0;
}

// OFFSET: 0x007F5500
// FUNCTION: fn_09905
int UIRefiningWnd::SlotAt(int x, int y) {
  int offsetX = x - 11;
  if (static_cast<unsigned>(offsetX) > 242 || static_cast<unsigned>(y - 55) > 52) {
    return -1;
  }
  int index = offsetX / 49;
  if (x > index * 49 + 57 || index >= 5) {
    return -1;
  }
  return index;
}

// OFFSET: 0x007F7030
// FUNCTION: fn_09911
void UIRefining_BtnReturn::Paint() {
  UIBitmapButton::Paint();
  auto& controls = *this->m_buttonContext;
  int width = this->TextWidth(controls.fonts, controls.locale, LookupMessage(2973), 0, 0, 12, 1, 0);
  std::uint32_t color = this->highlighted == 1 ? 0xE1C1F : 0x3E1B12;
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f),
    (this->geometry.unknown_0030 == 2) + 14, LookupMessage(2973), 0, 0, 12, color, 1, 0
  );
}

// OFFSET: 0x007F72A0
// FUNCTION: fn_09913
void UIRefiningWnd::Slot26(int x, int y) {
  if (this->item.unknown_0000[1] != 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    this->Reset();
  }
}

// OFFSET: 0x007F72D0
// FUNCTION: fn_09914
void UIRefiningWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->state == 1 && this->item.unknown_0000[1] > 0 && static_cast<unsigned>(x - 109) <= 46 && static_cast<unsigned>(y - 201) <= 46) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 28, this->item, 1, -1);
  }
}

// OFFSET: 0x007F7D80
// FUNCTION: fn_09918
void UIRefining_Notice::Slot19() {
  if (this->expireTick != 0 && this->expireTick < timeGetTime()) {
    this->expireTick = 0;
    if (this->autoShown == 1) {
      this->autoShown = 0;
      this->geometry.visible = 0;
    }
    this->text = this->savedText;
    this->highlight = this->savedHighlight;
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }
  }
}

// OFFSET: 0x007F7E10
// FUNCTION: fn_09919
void UIRefiningWnd::Slot32(int x, int y) {
}

// OFFSET: 0x007F8C40
// FUNCTION: fn_09925
void UIRefining_BtnAgain::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007F8C80
// FUNCTION: fn_09926
void UIRefining_BtnRefining::Enable(unsigned char value) {
  if (value == 0) {
    g_windowMgr.ReleaseCapture();
  }

  this->m_disabled = value ^ 1;
  this->Invalidate();
  this->geometry.visible = value;
}

// OFFSET: 0x007F8EA0
// FUNCTION: fn_09928
void UIRefining_Notice::SetNotice(const char* value, int duration, unsigned char highlight) {
  this->highlight = highlight;
  this->text = value;
  if (duration == 0) {
    this->savedText = value;
    this->savedHighlight = highlight;
  } else {
    this->expireTick = timeGetTime() + duration;
    if (this->geometry.visible == 0) {
      this->autoShown = 1;
      this->geometry.visible = 1;
    }
  }
  this->Invalidate();
  if (this->parent) {
    this->parent->Invalidate();
  }
}
