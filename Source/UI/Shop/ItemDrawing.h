#ifndef RAGNAROK_SOURCE_UI_SHOP_ITEMDRAWING_H
#define RAGNAROK_SOURCE_UI_SHOP_ITEMDRAWING_H

struct ItemInfo;
struct ItemDescriptions;
class UIWindow;
class ResourceMgr;
class FontCache;
struct TextLocale;
struct TextureNames;

void DrawItemIcon(UIWindow& window, ResourceMgr& resources, ItemDescriptions& descriptions, int x, int y, const ItemInfo& item, unsigned char identified);
void DrawItemIconById(UIWindow& window, TextureNames& names, ItemDescriptions& descriptions, int x, int y, const char* id, unsigned char identified);
void DrawItemSlotBackground(UIWindow& window, TextureNames& names, int x, int y);
void DrawItemWithQuantity(
  UIWindow& window, ResourceMgr& resources, ItemDescriptions& descriptions, FontCache& fonts, const TextLocale& locale, int x, int y, const ItemInfo& item,
  unsigned char identified, unsigned char plain
);

#endif
