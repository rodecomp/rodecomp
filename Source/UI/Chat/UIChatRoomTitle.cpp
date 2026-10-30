#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Chat/UIChatRoomTitle.h"

// OFFSET: 0x0084EF70
// FUNCTION: fn_10655
UIChatRoomTitle::UIChatRoomTitle(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0085C690
// FUNCTION: fn_10802
void UIChatRoomTitle::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_lu.bmp")));
  this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_ru.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, 0, &bitmap->image, false);

  int segments = (this->geometry.width - 24) / 12 + ((this->geometry.width - 24) % 12 != 0);
  for (int index = 0; index < segments; ++index) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_mu.bmp")));
    this->DrawBitmap((index + 1) * 12, 0, bitmap ? &bitmap->image : nullptr, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - bitmap->image.height - 6, &bitmap->image, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_rd.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, this->geometry.height - bitmap->image.height - 6, &bitmap->image, false);

  segments = (this->geometry.width - 24) / 12 + ((this->geometry.width - 24) % 12 != 0);
  for (int index = 0; index < segments; ++index) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_md.bmp")));
    this->DrawBitmap((index + 1) * 12, this->geometry.height - bitmap->image.height - 6, &bitmap->image, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_arr_l.bmp")));
  this->DrawBitmap(this->geometry.width / 2 - bitmap->image.width / 2, this->geometry.height - bitmap->image.height, &bitmap->image, false);

  const char* icon = this->roomType != 0 ? "유저인터페이스\\chat_open.bmp" : "유저인터페이스\\chat_close.bmp";
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(icon)));
  this->DrawBitmap(3, 3, bitmap ? &bitmap->image : nullptr, true);
}

// OFFSET: 0x0085F980
// FUNCTION: fn_10876
void UIChatRoomTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}
