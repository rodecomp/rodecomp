#include "UI/Item/UIMerchantItemLogWnd.h"

#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Input/InputCharset.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"
#include "Utility/Timestamp.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x0099E0E0
// FUNCTION: fn_13571
UIMerchantItemLogWnd::UIMerchantItemLogWnd(UIWindowContext& windows, UIFrameContext& frames, int mode) : UIItemBaseWnd(windows, frames) {
  this->unknown_00FC = mode;
}

// OFFSET: 0x0099E510
// FUNCTION: fn_13575
void UIMerchantItemLogWnd::Slot15(int width, int height) {
  UIStaticText* title = new UIStaticText(*this->m_context, *this->m_controls);
  title->SetSize(160, 15);
  title->SetText(LookupMessage(2559 + (this->unknown_00FC != 0)), 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  UIResizer* resizer = new UIResizer(*this->m_context);
  this->resizer = resizer;
  resizer->SetSize(16, 16);
  resizer->SetDirection(3);
  resizer->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  resizer->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(resizer);

  UIScrollBar* scrollBar = new UIScrollBar(*this->m_context);
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(scrollBar);

  UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->buttons[0] = button;
  button->SetBitmap("유저인터페이스\\btn_close.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\btn_close_a.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\btn_close_b.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(this->geometry.width - 61, this->geometry.height - 24);
  button->SetId(201);
  this->AddChild(button);

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;
}

// OFFSET: 0x0099E7A0
// FUNCTION: fn_13576
void UIMerchantItemLogWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  unsigned red, green, blue;
  SkinPaletteChannels(controls.resources, 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  int slots = (this->geometry.height - 38) / 32;
  for (int y = 17; slots > 0; --slots, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  auto it = this->items.begin();
  auto log = this->logs.begin();

  for (int index = 0, visible = 0; visible < this->visibleRows; ++index, ++it, ++log) {
    if (index < this->scroll) {
      continue;
    }

    if (this->items.size() < static_cast<unsigned>(index + 1)) {
      break;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->unknown_005D) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int x = visible % this->columns * 32 + 44;
    DrawItemWithQuantity(*this, controls.resources, descriptions, controls.fonts, controls.locale, x, top + 21, *it, 0, 0);

    if (it->unknown_0088 > 0) {
      this->DrawBitmap(
        x, top + 33,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->unknown_0088).c_str()))
        )->image,
        true
      );
    }

    int textX = this->geometry.width - 370;
    const int textY = top + 29;

    char name[26] = {};
    StringFormat(name, sizeof(name), static_cast<std::size_t>(-1), "%s", log->name.c_str());
    this->DrawTextAt(controls.fonts, controls.locale, textX, textY, name, static_cast<int>(std::strlen(name)), 0, 12, 0, 0, 0);
    textX += 150;

    char price[64];
    char grouped[64];
    std::memset(price, 0, sizeof(price));
    std::memset(grouped, 0, sizeof(grouped));
    const int length = std::sprintf(price, " %10s Zeny", FormatGroupedInteger(static_cast<int>(it->unknown_0014[0]), grouped, sizeof(grouped)));
    this->DrawTextAt(controls.fonts, controls.locale, textX, textY, price, length, 0, 12, 0, 0, 0);
    textX += 110;

    char date[20] = {};
    Timestamp stamp(log->time);
    StringFormat(date, sizeof(date), static_cast<std::size_t>(-1), "%02d/%02d - %02d:%02d:%02d", stamp.Month(), stamp.Day(), stamp.Hour(), stamp.Minute(), stamp.Second());
    this->DrawTextAt(controls.fonts, controls.locale, textX, textY, date, static_cast<int>(std::strlen(date)), 0, 12, 0, 0, 0);
    ++visible;
  }

  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x0099ED00
// FUNCTION: fn_13577
void UIMerchantItemLogWnd::Slot28(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.unknown_0000[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    if (item.identified == 1 && item.unknown_0098 != 0) {
      reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)
          ->Show(FormatOwnedString(LookupMessage(2551), naming.DisplayName(item, 0).c_str(), item.unknown_0098, item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    } else {
      reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)
          ->Show(FormatOwnedString(LookupMessage(183), naming.DisplayName(item, 0).c_str(), item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x0099EF50
// FUNCTION: fn_13578
void UIMerchantItemLogWnd::Slot19() {
  bool changed = false;
  for (auto it = this->logs.begin(); it != this->logs.end(); ++it) {
    if (it->name.compare(LookupMessage(581)) == 0) {
      it->name = LookupPlayerName(it->characterId);
      changed = true;
    }
  }

  if (changed) {
    this->Invalidate();
  }
}

// OFFSET: 0x0099F090
// FUNCTION: fn_13579
void UIMerchantItemLogWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0099F160
// FUNCTION: fn_13580
int UIMerchantItemLogWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    if (a == 201) {
      this->m_frameContext->windows.CloseWindow(this->unknown_00FC != 0 ? 258 : 257);
    }
    break;
  case 14:
    if (a == 3) {
      int height = c <= 336 ? c : 336;
      if (height < 120) {
        height = 120;
      }

      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newHeight != this->geometry.height) {
        this->Resize(this->geometry.width, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      this->buttons[0]->Move(this->geometry.width - 61, this->geometry.height - 24);
      this->m_frameContext->snapping.Store(this);
    }
    break;
  case 23: {
    ITEM_INFO item(*reinterpret_cast<const ITEM_INFO*>(a));
    this->items.push_back(item);

    lcData data;
    data.name = LookupPlayerName(b);
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    data.unknown_001C = naming.DisplayName(item, 0);
    data.time = c;
    data.characterId = b;
    this->logs.push_back(data);
    this->Invalidate();
    this->UpdateScrollBar();
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}
