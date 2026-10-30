#include "UI/Rodex/UIRodexComposeWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "File/ShortcutFilename.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/UIStaticText.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdlib>
#include <cstring>

// OFFSET: 0x007FF520
// FUNCTION: fn_10002
ITEM_INFO UIRodexComposeWnd::GetItemAtPoint(int x, int y) {
  if (x >= 30 && x <= 190 && y >= 309 && y <= 333) {
    this->m_itemIndex = 0;
    int left = 30;
    for (auto item = this->m_items.begin(); item != this->m_items.end(); ++item) {
      if (x > left && x < left + 24) {
        return *item;
      }
      left += 34;
      ++this->m_itemIndex;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00800480
// FUNCTION: fn_10006
void UIRodexEditCtrl3::Slot15(int width, int height) {
  auto* scrollBar = new UIRodexScrollBar(*this->m_context);
  this->unknown_0130 = scrollBar;
  scrollBar->Configure(width - 5, 0, 1, height);
  this->AddChild(this->unknown_0130);
}

// OFFSET: 0x00800DD0
// FUNCTION: fn_10008
void UIRodexScrollBar::Paint() {
  this->ClearSurface(0xFF00FF);

  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    this->FillRegion(0, 0, 5, this->geometry.height, 0xFFFFFF);
    const auto* upper = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_upper.bmp"))
    );
    this->DrawBitmap(0, 0, &upper->image, false);

    int offset = this->ThumbOffset() + this->arrowLength;
    if (offset < 5) {
      offset = 5;
    }

    int length = this->ThumbLength();
    if (this->geometry.height - 10 < length) {
      length = 1;
    }

    this->FillRegion(0, offset, 5, length, 0xE9E9E9);
    const auto* lower = static_cast<CBitmapRes*>(
      ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\rodexsystem\\renewal\\scrollbar_arrow_down.bmp"))
    );
    this->DrawBitmap(0, this->geometry.height - this->arrowLength, &lower->image, false);
  }

  this->minimumThumb = 0;
}

// OFFSET: 0x00800ED0
// FUNCTION: fn_10009
void UIRodexComposeWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (item.unknown_0000[1] > 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 15, item, item.quantity, -1);
  }
}

// OFFSET: 0x008011B0
// FUNCTION: fn_10011
void UIRodexComposeWnd::Slot19() {
  if (g_windowMgr.GetHoverWindow() == this->m_zeny) {
    this->m_zeny->Slot19();
  }
}

// OFFSET: 0x008011D0
// FUNCTION: fn_10012
void UIRodexComposeWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAtPoint(x, y);
  if (std::atoi(item.itemId.c_str()) != 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x007FEDD0
// FUNCTION: fn_09992
UIRodexComposeWnd::UIRodexComposeWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), unknown_00B4(nullptr), unknown_00B8(nullptr), unknown_00BC(nullptr), unknown_00C0(nullptr), unknown_00C4(nullptr),
      unknown_00C8(nullptr), unknown_00CC(0), unknown_00D0(nullptr), m_contents(nullptr), unknown_00D8(nullptr), m_zeny(nullptr), unknown_00E0(0),
      unknown_00E4(0), m_itemIndex(0), unknown_00F8(0), unknown_0100(nullptr) {
  this->m_items.clear();
}

// INLINED
UIRodexComposeWnd::~UIRodexComposeWnd() {
  if (this->unknown_0100) {
    reinterpret_cast<ItemTagRetirement*>(this->unknown_0100)->Clear();
    delete this->unknown_0100;
  }
}

// OFFSET: 0x007FF260
// FUNCTION: fn_09998
int UIRodexComposeWnd::MeasureText(const std::string& text, const unsigned& bytes) {
  return this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), bytes, 0, 12, 0, 0);
}

// OFFSET: 0x007FF290

// OFFSET: 0x007FF400

// OFFSET: 0x007FF4A0
