#include "Contents/ItemDescription.h"
#include "Contents/ItemInfo.h"
#include "Resource/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/ItemDrawing.h"
#include "UI/UIWindow.h"

#include <bit>
#include <cstdio>

// OFFSET: 0x008A8EE0
// FUNCTION: fn_11717
void DrawItemIcon(
  UIWindow& window,
  ResourceMgr& resources,
  ItemDescriptions& descriptions,
  int x,
  int y,
  const ItemInfo& item,
  unsigned char identified
) {
  char path[128];
  descriptions.IconPath(path, item.itemId.c_str(), item.identified != 0 || identified != 0);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(resources.Load(path));
  if (bitmap) {
    window.DrawBitmap(x, y, &bitmap->image, true);
  }
}

// OFFSET: 0x008A9040
// FUNCTION: fn_11720
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
) {
  DrawItemIcon(window, resources, descriptions, x, y, item, identified);
  if (item.quantity == -1 || item.quantity == 0) {
    return;
  }
  char text[16];
  std::sprintf(text, "%d", item.quantity);
  const auto add = [](int value, int offset) {
    return std::bit_cast<int>(static_cast<unsigned>(value) + static_cast<unsigned>(offset));
  };
  if (plain != 0) {
    window.DrawTextAt(fonts, locale, add(x, 16), add(y, 20), text, 0, 0, 11, 0, 0, 0);
  } else {
    const int left = add(x, item.quantity < 1000 ? 14 : 0);
    window.DrawTextAt(fonts, locale, add(left, 1), add(y, 15), text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, add(left, -1), add(y, 15), text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, add(y, 16), text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, add(y, 14), text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, add(y, 15), text, 0, 0, 11, 0, 0, 0);
  }
}
