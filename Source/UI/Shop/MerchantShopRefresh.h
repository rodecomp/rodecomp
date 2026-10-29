#ifndef RAGNAROK_SOURCE_UI_SHOP_MERCHANTSHOPREFRESH_H
#define RAGNAROK_SOURCE_UI_SHOP_MERCHANTSHOPREFRESH_H

#include "Contents/Items/ItemInfo.h"

#include <span>
#include <vector>

class UIWindow;
class CMCStoreInfoBackup;
class UIEditCtrl;

struct MerchantShopRefresh {
  CMCStoreInfoBackup& backup;
  std::list<ITEM_INFO>& inventory;
  std::list<ITEM_INFO>& offered;
  UIEditCtrl& titleEdit;
  std::span<UIEditCtrl*> priceEdits;
  std::span<UIEditCtrl*> countEdits;
  const int& maximum;
  const int& mode;
  int& titleLength;
  UIWindow& button;
  unsigned char& buttonDisabled;
  UIWindow* const& storeWindow;
  UIWindow* const& otherStoreWindow;
  UIWindow* const& inventoryWindow;
  UIWindow* const& otherInventoryWindow;
  UIWindow* const& extraInventoryWindow;

  void Refresh();
};

#endif
