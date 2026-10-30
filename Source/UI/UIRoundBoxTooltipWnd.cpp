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
// FUNCTION: fn_13541
void UIColoringTextViewer::Paint() {
  if (this->transparent) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(this->backgroundColor);
  }

  int spacing = this->lineHeight - this->bottomMargin;
  if (spacing == 0) {
    spacing = 1;
  }

  const int first = this->topLine;
  const int limit = this->geometry.height / spacing + first;
  int end = static_cast<int>(std::min<unsigned>(this->lines.size(), limit));
  if (this->lineLimit) {
    end = this->lineLimit;
  }

  auto& owner = *this->m_listContext;
  for (int index = first; index < end; ++index) {
    const auto& line = this->lines[index];
    DrawColoredText(*this, owner.codePage, 3, this->lineHeight * (index - first) + 4, line.c_str(), static_cast<unsigned>(line.size()), this->color, 0, 12, false);
  }
}

// OFFSET: 0x0099A3F0
// FUNCTION: fn_13542
void UIRoundBoxTooltipWnd::Paint() {
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xEFB8B8);
  DrawFilledRoundedBorder(*this, 1, 1, this->geometry.width - 2, this->geometry.height - 2, 0xEFE7E7);
}

// OFFSET: 0x0099A430
// FUNCTION: fn_13543
int UIColoringTextViewer::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 29) {
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  std::vector<std::string> wrapped;
  ItemDescriptions descriptions{g_itemDescriptions};
  WrapRichText(reinterpret_cast<const char*>(argument1), wrapped, 35, argument3 != 0, this->m_listContext->codePage, descriptions);
  for (unsigned index = 0; index < wrapped.size(); ++index) {
    this->AddLine(wrapped[index].c_str());
  }

  this->Invalidate();
  return 0;
}
