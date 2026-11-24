#include "Client/Session/Session.h"
#include "Contents/Store/MCStoreInfoBackup.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdlib>
#include <cstring>

// INLINED
static __forceinline std::string StoredTitle(const CMCStoreInfoBackup& backup) {
  return backup.title;
}

// OFFSET: 0x00970E60
// FUNCTION: fn_13262
void UIMerchantShopMakeWnd::Refresh() {
  CMCStoreInfoBackup& backup = g_session.m_storeInfoBackup;

  if (StoredTitle(backup).empty()) {
    UIBasicButton* button = this->actionButton;
    button->SetDisabled(0);
    button->Invalidate();
    return;
  }

  this->titleEdit->SetText(StoredTitle(backup).c_str());
  this->titleEdit->Invalidate();

  for (int index = 0; index < this->slotCount; ++index) {
    this->priceEdits[index]->SetText("0");

    if (this->mode != 0) {
      this->priceEdits[index]->Background(232, 232, 232);
    } else {
      UIEditCtrl* edit = this->priceEdits[index];
      edit->disabled = 1;
      edit->Invalidate();
      this->priceEdits[index]->Background(182, 182, 182);
    }
  }

  const int count = g_session.GetNumMerchantPurchaseSavedItems();

  for (int index = 0; index < count; ++index) {
    const ITEM_INFO item = g_session.GetMerchantPurchaseSavedItemInfoBySequence(index);
    g_session.AddMerchantMyShopItem(item, this->mode, 0);
  }

  g_session.ClearMerchantPurchaseSavedItems();
  backup.Restore(this->mode != 0, this->slotCount);

  int index = 0;
  const std::vector<ITEM_INFO> displayed(backup.items);

  for (auto itemIt = displayed.begin(); itemIt != displayed.end(); ++itemIt, ++index) {
    const ITEM_INFO& item = *itemIt;
    if (this->slotCount <= index) {
      break;
    }

    const int price = static_cast<int>(item.prices[0]);
    char value[24] = {};
    _itoa(price, value, 10);
    UIEditCtrl* edit = this->priceEdits[index];
    edit->disabled = 0;
    edit->Invalidate();
    this->priceEdits[index]->Background(232, 232, 232);
    this->priceEdits[index]->SelectAll();
    this->priceEdits[index]->SetText(value);

    if (this->mode != 0 && index < 5) {
      char quantity[24] = {};
      _itoa(item.quantity, quantity, 10);
      this->quantityEdits[index]->SetText(quantity);
      this->quantityEdits[index]->Invalidate();
    } else {
      const PriceColors colors = PriceColor(price);
      this->priceEdits[index]->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
      char grouped[24] = {};
      FormatGroupedInteger(static_cast<std::int64_t>(price), grouped, sizeof(grouped));
      this->priceEdits[index]->SetText(grouped);
      this->previousPriceLength = this->priceEdits[index]->Length();
    }
  }

  g_session.RefreshItem1768Window(this->mode != 0, 0);
  g_session.RefreshItem1750Window(this->mode);
  UIBasicButton* button = this->actionButton;
  button->SetDisabled(0);
  button->Invalidate();
  backup.Clear();
}
