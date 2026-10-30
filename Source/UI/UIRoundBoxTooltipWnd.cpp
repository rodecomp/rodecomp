#include "UI/UIRoundBoxTooltipWnd.h"

#include "Contents/Items/ItemDescription.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>
#include <string>
#include <vector>

// INLINED
UIColoringTextViewer::UIColoringTextViewer(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list), color(0), lineLimit(0) {
}

// OFFSET: 0x0099A1F0
// FUNCTION: fn_13540
void UIRoundBoxTooltipWnd::Slot15(int width, int height) {
  if (this->viewer) {
    return;
  }

  this->viewer = new UIColoringTextViewer(*this->m_context, *this->m_listContext);
  this->viewer->Configure(4, 4, width - 6, height - 6, 0);

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 1, 1, red, green, blue);

  this->viewer->backgroundColor = 0xFFFF00FF;
  this->viewer->Invalidate();
  this->viewer->color = 0x803B1A;
  this->AddChild(this->viewer);
}

// OFFSET: 0x0099A300

// OFFSET: 0x0099A3F0
// FUNCTION: fn_13542
void UIRoundBoxTooltipWnd::Paint() {
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xEFB8B8);
  DrawFilledRoundedBorder(*this, 1, 1, this->geometry.width - 2, this->geometry.height - 2, 0xEFE7E7);
}

// OFFSET: 0x0099A430
