#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIWindow.h"

#include <cstdio>

// OFFSET: 0x008A8EE0
// FUNCTION: fn_11717
void DrawItemIcon(UIWindow& window, ResourceMgr& resources, ItemDescriptions& descriptions, int x, int y, const ItemInfo& item, unsigned char identified) {
  char path[128];
  descriptions.IconPath(path, item.itemId.c_str(), item.identified != 0 || identified != 0);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(resources.Load(path));

  if (bitmap) {
    window.DrawBitmap(x, y, &bitmap->image, true);
  }
}

// OFFSET: 0x008A8F80
// FUNCTION: fn_11718
void DrawItemIconById(UIWindow& window, TextureNames& names, ItemDescriptions& descriptions, int x, int y, const char* id, unsigned char identified) {
  char path[128];
  descriptions.IconPath(path, id, identified);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve(path)));

  if (bitmap) {
    window.DrawBitmap(x, y, &bitmap->image, true);
  }
}

// OFFSET: 0x008A9000
// FUNCTION: fn_11719
void DrawItemSlotBackground(UIWindow& window, TextureNames& names, int x, int y) {
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(names.Resolve("유저인터페이스\\basic_interface\\itemwin_mid.bmp")));
  window.DrawBitmap(x, y, bitmap ? &bitmap->image : nullptr, false);
}

// OFFSET: 0x008A9040
// FUNCTION: fn_11720
void DrawItemWithQuantity(
  UIWindow& window, ResourceMgr& resources, ItemDescriptions& descriptions, FontCache& fonts, const TextLocale& locale, int x, int y, const ItemInfo& item,
  unsigned char identified, unsigned char plain
) {
  DrawItemIcon(window, resources, descriptions, x, y, item, identified);
  if (item.quantity == -1 || item.quantity == 0) {
    return;
  }

  char text[16];
  std::sprintf(text, "%d", item.quantity);
  if (plain != 0) {
    window.DrawTextAt(fonts, locale, x + 16, y + 20, text, 0, 0, 11, 0, 0, 0);
  } else {
    const int left = x + (item.quantity < 1000 ? 14 : 0);
    window.DrawTextAt(fonts, locale, left + 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left - 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, y + 16, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, y + 14, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    window.DrawTextAt(fonts, locale, left, y + 15, text, 0, 0, 11, 0, 0, 0);
  }
}
