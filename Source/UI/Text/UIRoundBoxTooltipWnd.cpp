#include "UI/Window/UIEnums.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Text/UIRoundBoxTooltipWnd.h"

#include "Client/Input/Mouse.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

// OFFSET: 0x00999FF0
// FUNCTION: fn_13536
UIColoringTextViewer::UIColoringTextViewer() : UIListBox(), color(0), lineLimit(0) {
}

// OFFSET: 0x0099A1F0
// FUNCTION: fn_13540
void UIRoundBoxTooltipWnd::OnCreate(int width, int height) {
  if (this->viewer) {
    return;
  }

  this->viewer = new UIColoringTextViewer();
  this->viewer->Configure(4, 4, width - 6, height - 6, 0);

  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(1, 1, red, green, blue);

  {
    auto* viewer = this->viewer;
    viewer->backgroundColor = 0xFFFF00FF;
    viewer->Invalidate();
  }
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
  int end = static_cast<int>(std::min<unsigned>(limit, this->lines.size()));
  if (this->lineLimit) {
    end = this->lineLimit;
  }

  for (int index = first; index < end; ++index) {
    const auto& line = this->lines[index];
    this->DrawColoredText(3, this->lineHeight * (index - first) + 4, line.c_str(), static_cast<unsigned>(line.size()), this->color, 0, 12, false);
  }
}

// OFFSET: 0x0099A3F0
// FUNCTION: fn_13542
void UIRoundBoxTooltipWnd::Paint() {
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, this->geometry.height, 0xEFB8B8);
  this->DrawFilledRoundedOutline(1, 1, this->geometry.width - 2, this->geometry.height - 2, 0xEFE7E7);
}

// OFFSET: 0x0099A430
// FUNCTION: fn_13543
int UIColoringTextViewer::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_SETTEXT) {
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  std::vector<std::string> wrapped;
  WrapRichText(reinterpret_cast<const char*>(argument1), wrapped, 35, argument3 != 0);
  for (unsigned index = 0; index < wrapped.size(); ++index) {
    this->AddLine(wrapped[index].c_str());
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x0099A5E0
// FUNCTION: fn_13544
int UIRoundBoxTooltipWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND) {
    if (argument1 == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_ROUNDBOXTOOLTIPWND);
    }
  } else if (message == UM_SET_ITEM_MOVE_TOOLTIP) {
    if (argument1 != 0) {
      std::string text;
      std::string longest;
      int lines = 0;

      this->viewer->Clear();
      const ItemMoveInfo info = FindItemMoveInfo(static_cast<int>(argument1));

      if (info.drop) {
        text += LookupMessage(MSI_ITEMMOVEINFO_MAIL);
        longest = LookupMessage(MSI_ITEMMOVEINFO_MAIL);
        ++lines;
      }

      if (info.storage) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_ITEMMOVEINFO_EXCHANGEWND);
        if (std::strlen(LookupMessage(MSI_ITEMMOVEINFO_EXCHANGEWND)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_ITEMMOVEINFO_EXCHANGEWND);
        }
        ++lines;
      }

      if (info.cart) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_ITEMMOVEINFO_AUCTION);
        if (std::strlen(LookupMessage(MSI_ITEMMOVEINFO_AUCTION)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_ITEMMOVEINFO_AUCTION);
        }
        ++lines;
      }

      if (info.mail) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_ITEMMOVEINFO_STOREOFGUILD);
        if (std::strlen(LookupMessage(MSI_ITEMMOVEINFO_STOREOFGUILD)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_ITEMMOVEINFO_STOREOFGUILD);
        }
        ++lines;
      }

      if (info.trade) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_ITEMMOVEINFO_TONPC);
        if (std::strlen(LookupMessage(MSI_ITEMMOVEINFO_TONPC)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_ITEMMOVEINFO_TONPC);
        }
        ++lines;
      }

      if (info.guildStorage) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_CREATE_CHARACTER);
        if (std::strlen(LookupMessage(MSI_CREATE_CHARACTER)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_CREATE_CHARACTER);
        }
        ++lines;
      }

      if (info.sellToNpc) {
        if (!text.empty()) {
          text += "\n";
        }
        text += LookupMessage(MSI_UNEQUIP_ARROW3);
        if (std::strlen(LookupMessage(MSI_UNEQUIP_ARROW3)) > std::strlen(longest.c_str())) {
          longest = LookupMessage(MSI_UNEQUIP_ARROW3);
        }
        ++lines;
      }

      if (text.empty()) {
        g_windowMgr.ShowWindowById(WID_ROUNDBOXTOOLTIPWND, 0);
        return 0;
      }

      const int width = this->TextWidth(longest.c_str(), 0, 0, 12, 0, 0);
      this->viewer->lineLimit = lines;
      const int height = this->TextHeight(text.c_str(), 0, 0, 12, 0, 0);
      this->viewer->Resize(width + 15, static_cast<int>(static_cast<float>(height * lines + 10) + static_cast<float>(lines - 1) * 4.8f + 1.0f));
      this->Resize(this->viewer->geometry.width + 2, this->viewer->geometry.height + 2);
      this->Move(g_mouse.x + 26, g_mouse.y);
      this->viewer->SendMsg(nullptr, UM_SETTEXT, reinterpret_cast<std::intptr_t>(text.c_str()), 0, 0, 0);
    }
  } else {
    return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  g_windowMgr.Promote(this);
  return 0;
}
