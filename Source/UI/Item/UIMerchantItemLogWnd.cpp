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
#include "Utility/Text/FormatString.h"

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
