#include "UI/UIRoundBoxTooltipWnd.h"

#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowOrdering.h"

#include <algorithm>
#include <cstring>
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

// OFFSET: 0x0099A5E0
// FUNCTION: fn_13544
int UIRoundBoxTooltipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(276);
    }
  } else if (message == 147) {
    if (argument1 != 0) {
      std::string text;
      std::string longest;
      int lines = 0;

      this->viewer->Clear();
      const ItemMoveInfo info = FindItemMoveInfo(static_cast<int>(argument1));

      if (info.drop) {
        text += LookupMessage(0xAE4);
        longest = LookupMessage(0xAE4);
        ++lines;
      }

      if (info.storage) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAE5);
        if (std::strlen(LookupMessage(0xAE5)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAE5);
        }
        ++lines;
      }

      if (info.cart) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAE6);
        if (std::strlen(LookupMessage(0xAE6)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAE6);
        }
        ++lines;
      }

      if (info.mail) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAE7);
        if (std::strlen(LookupMessage(0xAE7)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAE7);
        }
        ++lines;
      }

      if (info.trade) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAE8);
        if (std::strlen(LookupMessage(0xAE8)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAE8);
        }
        ++lines;
      }

      if (info.guildStorage) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAEA);
        if (std::strlen(LookupMessage(0xAEA)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAEA);
        }
        ++lines;
      }

      if (info.sellToNpc) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(0xAEB);
        if (std::strlen(LookupMessage(0xAEB)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(0xAEB);
        }
        ++lines;
      }

      if (text.empty()) {
        reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(276, 0);
        return 0;
      }

      const int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, longest.c_str(), 0, 0, 12, 0, 0);
      this->viewer->lineLimit = lines;
      const int height = this->TextHeight(this->m_context->fonts, this->m_context->locale, text.c_str(), 0, 0, 12, 0, 0);
      this->viewer->Resize(width + 15, static_cast<int>(static_cast<float>(height * lines + 10) + static_cast<float>(lines - 1) * 4.8f + 1.0f));
      this->Resize(this->viewer->geometry.width + 2, this->viewer->geometry.height + 2);
      this->Move(g_screenWidth + 26, g_screenHeight);
      this->viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 0, 0, 0);
    }
  } else {
    return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  reinterpret_cast<WindowOrdering&>(g_windowMgr).Promote(this);
  return 0;
}
