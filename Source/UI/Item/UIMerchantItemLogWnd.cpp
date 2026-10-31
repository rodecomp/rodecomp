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
