#include "Contents/Store/MCStoreInfoBackup.h"
#include "UI/Shop/MerchantShopRefresh.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindow.h"
#include "Utility/Text/GroupedInteger.h"

#include <algorithm>
#include <cstdio>

// OFFSET: 0x00970E60
// FUNCTION: fn_13262
void MerchantShopRefresh::Refresh() {
  if (this->backup.title.empty()) {
    this->buttonDisabled = 0;
    this->button.Invalidate();
    return;
  }

  this->titleEdit.SetText(this->backup.title.c_str());
  this->titleEdit.Invalidate();

  for (int index = 0; index < this->maximum; ++index) {
    UIEditCtrl& edit = *this->priceEdits[index];
    edit.SetText("");

    if (this->mode == 0) {
      edit.disabled = 1;
      edit.Invalidate();
    }

    const int background = this->mode != 0 ? 232 : 182;
    edit.Background(background, background, background);
  }

  for (const ItemInfo& item : this->offered) {
    const auto found =
      std::find_if(this->inventory.begin(), this->inventory.end(), [&](const ItemInfo& value) { return value.unknown_0000[1] == item.unknown_0000[1]; });

    if (found == this->inventory.end()) {
      this->inventory.push_back(item);
    } else {
      found->quantity = (found->quantity + item.quantity);
    }
  }

  this->offered.clear();
  this->backup.Restore(this->mode != 0, this->maximum, this->inventory, this->offered);

  const std::vector<ItemInfo> displayed(this->backup.items);
  int index = 0;

  for (const ItemInfo& item : displayed) {
    if (index >= this->maximum) {
      break;
    }

    UIEditCtrl& edit = *this->priceEdits[index];
    const int price = static_cast<int>(item.unknown_0014[0]);
    char value[24] = {};
    std::snprintf(value, sizeof(value), "%d", price);
    edit.disabled = 0;
    edit.Invalidate();
    edit.Background(232, 232, 232);
    edit.SelectAll();
    edit.SetText(value);

    if (this->mode != 0 && index < 5) {
      std::snprintf(value, sizeof(value), "%d", item.quantity);
      UIEditCtrl& count = *this->countEdits[index];
      count.SetText(value);
      count.Invalidate();
    } else {
      const PriceColors colors = PriceColor(price);
      edit.TextColor(colors.primary, colors.secondary);
      FormatGroupedInteger(price, value, sizeof(value));
      edit.SetText(value);
      this->titleLength = edit.Length();
    }

    ++index;
  }

  UIWindow* store = this->mode != 0 ? this->otherStoreWindow : this->storeWindow;

  if (store) {
    store->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  UIWindow* inventory = this->mode == 1 ? this->otherInventoryWindow : this->mode == 2 ? this->extraInventoryWindow : this->inventoryWindow;

  if (inventory) {
    inventory->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  this->buttonDisabled = 0;
  this->button.Invalidate();
  this->backup.Clear();
}

void StoreBackupContext::RefreshShop() {
  MerchantShopRefresh* window = this->shop ? this->shop : this->otherShop;

  if (window) {
    window->Refresh();
  }
}
