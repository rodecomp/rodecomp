#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <string>

// OFFSET: 0x0084EF70
// FUNCTION: fn_10655
UIChatRoomTitle::UIChatRoomTitle(UIWindowContext& context) : UIWindow(context, 0) {
}

// OFFSET: 0x0085AB80
// FUNCTION: fn_10785
void UIChatRoomTitle::Slot15(int, int) {
  this->label = new UIStaticText(*this->m_context, *this->m_controls);
  this->label->SetSize(this->geometry.width - 34, 15);
  this->label->Move(31, 8);
  this->label->SetTextFitted("");
  this->label->fontFamily = 0;
  this->label->fontHeight = 12;
  this->label->bold = 0;
  this->label->italic = 0;
  this->AddChild(this->label);
}

// OFFSET: 0x0085E8C0
// FUNCTION: fn_10831
void UIChatRoomTitle::Slot26(int, int) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (this->roomType == 0) {
    g_windowMgr.EnsureWindow(29)->SendMsg(nullptr, 47, this->roomId, 0, 0, 0);
  } else if (this->roomType != 3) {
    mode->SendMsg(45, this->roomId, 0, 0, 0);
  }
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

// OFFSET: 0x008637A0
// FUNCTION: fn_10910
int UIChatRoomTitle::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    return 0;
  }

  if (message != 34) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  const ChatRoomTitleInfo* info = reinterpret_cast<const ChatRoomTitleInfo*>(a);
  this->title = info->title;
  this->unknown_0098 = info->unknown_0018;
  this->roomType = info->type;
  this->memberCount = info->memberCount;
  this->memberLimit = info->memberLimit;
  this->roomId = info->id;

  const std::size_t lineEnd = this->title.find('\r');
  if (lineEnd != std::string::npos) {
    this->title = this->title.substr(0, lineEnd);
  }

  std::string status;
  char text[384];
  if (this->roomType == 0) {
    status = LookupMessage(89);
    std::sprintf(text, "%s (%d/%d)", this->title.c_str(), this->memberCount, this->memberLimit);
  } else if (this->roomType == 3) {
    status.clear();
    std::sprintf(text, "%s", this->title.c_str());
  } else {
    status = LookupMessage(90);
    std::sprintf(text, "%s (%d/%d)", this->title.c_str(), this->memberCount, this->memberLimit);
  }

  this->unknown_00C0 = static_cast<short>(b);
  switch (static_cast<short>(b)) {
  case 1:
    this->label->fontFamily = 21;
    break;
  case 2:
    this->label->fontFamily = 20;
    break;
  case 3:
    this->label->fontFamily = 22;
    break;
  case 4:
    this->label->fontFamily = 23;
    break;
  case 5:
    this->label->fontFamily = 24;
    break;
  case 6:
    this->label->fontFamily = 25;
    break;
  case 7:
    this->label->fontFamily = 26;
    break;
  case 8:
    this->label->fontFamily = 27;
    break;
  case 9:
    this->label->fontFamily = 28;
    break;
  default:
    this->label->fontFamily = 0;
    break;
  }
  this->label->bold = 0;
  this->label->italic = 0;
  this->label->fontHeight = 12;

  if (this->m_forbiddenWords->Contains(text)) {
    std::string masked;
    this->m_forbiddenWords->Mask(text, &masked);
  }

  this->label->SetTextFitted(text);
  this->Invalidate();
  return 0;
}
