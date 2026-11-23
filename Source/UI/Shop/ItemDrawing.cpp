#include "Client/Session/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIWindow.h"

#include <cstdio>

// OFFSET: 0x008A8D30
// FUNCTION: fn_11716
void UIWindow::DrawItemAndQuantityAuxiliary(
  int mode, int x, int y, const ITEM_INFO& item,
  unsigned char identified, unsigned char plain
) {
  char text[128] = {};
  g_session.IconPath(item.itemId.c_str(), text, identified);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
  this->DrawAuxiliary(mode, x, y, bitmap, true, false);

  if (item.quantity == 0) {
    return;
  }

  std::sprintf(text, "%d", item.quantity);
  if (plain != 0) {
    this->DrawTextAt(x + 16, y + 20, text, 0, 0, 11, 0, 0, 0);
  } else {
    const int offset = item.quantity < 1000 ? 14 : 0;
    this->DrawTextAt(x + offset + 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset - 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 16, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 14, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 15, text, 0, 0, 11, 0, 0, 0);
  }
}

// OFFSET: 0x008A8EE0
// FUNCTION: fn_11717
void UIWindow::DrawItemIcon(int x, int y, const ITEM_INFO& item, unsigned char identified) {
  char path[128];
  g_session.IconPath(item.itemId.c_str(), path, item.identified != 0 || identified != 0);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));

  if (bitmap) {
    this->DrawBitmap(x, y, bitmap, true);
  }
}

// OFFSET: 0x008A8F80
// FUNCTION: fn_11718
void UIWindow::DrawItemIconFromId(int x, int y, const char* id, unsigned char identified) {
  char path[128];
  g_session.IconPath(id, path, identified);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));

  if (bitmap) {
    this->DrawBitmap(x, y, bitmap, true);
  }
}

// OFFSET: 0x008A9000
// FUNCTION: fn_11719
void UIWindow::DrawItemSlotFrame(int x, int y) {
  this->DrawBitmap(x, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_mid.bmp"))), false);
}

// OFFSET: 0x008A9040
// FUNCTION: fn_11720
void UIWindow::DrawItemAndQuantity(
  int x, int y, const ITEM_INFO& item,
  unsigned char identified, unsigned char plain
) {
  this->DrawItemIcon(x, y, item, identified);
  if (item.quantity == -1 || item.quantity == 0) {
    return;
  }

  char text[16];
  std::sprintf(text, "%d", item.quantity);
  if (plain != 0) {
    this->DrawTextAt(x + 16, y + 20, text, 0, 0, 11, 0, 0, 0);
  } else {
    const int offset = item.quantity < 1000 ? 14 : 0;
    this->DrawTextAt(x + offset + 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset - 1, y + 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 16, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 14, text, 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + offset, y + 15, text, 0, 0, 11, 0, 0, 0);
  }
}
