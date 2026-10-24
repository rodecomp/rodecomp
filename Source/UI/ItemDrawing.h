#ifndef RAGNAROK_SOURCE_UI_ITEMDRAWING_H
#define RAGNAROK_SOURCE_UI_ITEMDRAWING_H

struct ItemInfo;
struct ItemDescriptions;
class UIWindow;
class ResourceMgr;
class FontCache;
struct TextLocale;

void DrawItemIcon(
  UIWindow& window,
  ResourceMgr& resources,
  ItemDescriptions& descriptions,
  int x,
  int y,
  const ItemInfo& item,
  unsigned char identified
);
void DrawItemWithQuantity(
  UIWindow& window,
  ResourceMgr& resources,
  ItemDescriptions& descriptions,
  FontCache& fonts,
  const TextLocale& locale,
  int x,
  int y,
  const ItemInfo& item,
  unsigned char identified,
  unsigned char plain
);

#endif
