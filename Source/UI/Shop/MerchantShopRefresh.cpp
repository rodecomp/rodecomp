#include "Client/Session.h"
#include "Contents/Store/MCStoreInfoBackup.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/GroupedInteger.h"

#include <cstdlib>
#include <cstring>


// OFFSET: 0x00970E60
// FUNCTION: fn_13262
void UIMerchantShopMakeWnd::Refresh() {
  CMCStoreInfoBackup& backup = g_session.m_storeInfoBackup;

  if (std::string(backup.title).empty()) {
    this->actionButton->SetDisabled(0);
    this->actionButton->Invalidate();
    return;
  }

  this->titleEdit->SetText(std::string(backup.title).c_str());
  this->titleEdit->Invalidate();

  for (int index = 0; index < this->slotCount; ++index) {
    UIEditCtrl* edit = this->priceEdits[index];
    edit->SetText("");

    int background;

    if (this->mode != 0) {
      background = 232;
    } else {
      edit->disabled = 1;
      edit->Invalidate();
      background = 182;
    }

    edit->Background(background, background, background);
  }

  const int count = g_session.GetList11Size();

  for (int index = 0; index < count; ++index) {
    const ITEM_INFO item = g_session.GetItem1768At(index);
    g_session.AddItem1750(item, this->mode, 0);
  }

  g_session.ClearItems1768();
  backup.Restore(this->mode != 0, this->slotCount);

  const std::vector<ITEM_INFO> displayed(backup.items);
  int index = 0;
  UIEditCtrl** edit = this->priceEdits;

  for (const ITEM_INFO& item : displayed) {
    if (index >= this->slotCount) {
      break;
    }

    const int price = static_cast<int>(item.prices[0]);
    char value[24] = {};
    _itoa(price, value, 10);
    (*edit)->disabled = 0;
    (*edit)->Invalidate();
    (*edit)->Background(232, 232, 232);
    (*edit)->SelectAll();
    (*edit)->SetText(value);

    if (this->mode != 0 && index < 5) {
      char quantity[24] = {};
      _itoa(item.quantity, quantity, 10);
      this->quantityEdits[index]->SetText(quantity);
      this->quantityEdits[index]->Invalidate();
    } else {
      const PriceColors colors = PriceColor(price);
      (*edit)->TextColor(colors.primary, colors.secondary);
      char grouped[24] = {};
      FormatGroupedInteger(price, grouped, sizeof(grouped));
      (*edit)->SetText(grouped);
      this->previousPriceLength = (*edit)->Length();
    }

    ++edit;
    ++index;
  }

  g_session.RefreshItem1768Window(this->mode != 0, 0);
  g_session.RefreshItem1750Window(this->mode);
  this->actionButton->SetDisabled(0);
  this->actionButton->Invalidate();
  backup.Clear();
}
