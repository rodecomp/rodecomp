#ifndef RAGNAROK_SOURCE_UI_SHOP_ITEMDRAWING_H
#define RAGNAROK_SOURCE_UI_SHOP_ITEMDRAWING_H

struct ITEM_INFO;
class UIWindow;
class ResourceMgr;

void DrawItemIcon(UIWindow& window, ResourceMgr& resources, int x, int y, const ITEM_INFO& item, unsigned char identified);
void DrawItemIconById(UIWindow& window, int x, int y, const char* id, unsigned char identified);
void DrawItemSlotBackground(UIWindow& window, int x, int y);
void DrawItemWithQuantity(
  UIWindow& window, ResourceMgr& resources, int x, int y, const ITEM_INFO& item,
  unsigned char identified, unsigned char plain
);
void DrawItemWithQuantityAuxiliary(
  UIWindow& window, int mode, int x, int y, const ITEM_INFO& item,
  unsigned char identified, unsigned char plain
);

#endif
