#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <cstdio>
#include <string>

// OFFSET: 0x0084EF70
// FUNCTION: fn_10655
UIChatRoomTitle::UIChatRoomTitle() : UIWindow(0) {
}

// OFFSET: 0x00851440
// FUNCTION: fn_45637
UIChatRoomTitle::~UIChatRoomTitle() = default;

// OFFSET: 0x0085AB80
// FUNCTION: fn_10785
void UIChatRoomTitle::OnCreate(int, int) {
  this->label = new UIStaticText();
  this->label->SetSize(this->geometry.width - 34, 15);
  this->label->Move(31, 8);
  this->label->SetTextFitted("");
  auto* const label = this->label;
  label->fontFamily = 0;
  label->fontHeight = 12;
  label->bold = 0;
  label->italic = 0;
  this->AddChild(this->label);
}

// OFFSET: 0x0085E8C0
// FUNCTION: fn_10831
void UIChatRoomTitle::OnLBtnDblClk(int, int) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  if (this->info.type != 0) {
    if (this->info.type != 3) {
      mode->SendMsg(MM_REQ_ENTER_ROOM, this->info.id, 0, 0, 0);
    }
  } else {
    g_windowMgr.EnsureWindow(WID_PASSWORDWND)->SendMsg(nullptr, UM_SETROOMID, this->info.id, 0, 0, 0);
  }
}

// OFFSET: 0x0085C690
// FUNCTION: fn_10802
void UIChatRoomTitle::Paint() {
  this->ClearSurface(0xFFFF00FF);

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_lu.bmp")));
  this->DrawBitmap(0, 0, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_ru.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, 0, bitmap, false);

  int segments = (this->geometry.width - 24) / 12 + ((this->geometry.width - 24) % 12 != 0);
  for (int index = 0; index < segments; ++index) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_mu.bmp")));
    this->DrawBitmap((index + 1) * 12, 0, bitmap, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - bitmap->image.height - 6, bitmap, false);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_rd.bmp")));
  this->DrawBitmap(this->geometry.width - bitmap->image.width, this->geometry.height - bitmap->image.height - 6, bitmap, false);

  segments = (this->geometry.width - 24) / 12 + ((this->geometry.width - 24) % 12 != 0);
  for (int index = 0; index < segments; ++index) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_md.bmp")));
    this->DrawBitmap((index + 1) * 12, this->geometry.height - bitmap->image.height - 6, bitmap, false);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\sysbox_arr_l.bmp")));
  this->DrawBitmap(this->geometry.width / 2 - bitmap->image.width / 2, this->geometry.height - bitmap->image.height, bitmap, false);

  const char* icon = this->info.type == 0 ? "유저인터페이스\\chat_close.bmp" : "유저인터페이스\\chat_open.bmp";
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(icon)));
  this->DrawBitmap(3, 3, bitmap, true);
}

// OFFSET: 0x0085F980
// FUNCTION: fn_10876
void UIChatRoomTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    g_modeMgr.currentMode->SetCursorAction(2);
  } else {
    g_modeMgr.currentMode->SetCursorAction(0);
  }
}

// OFFSET: 0x008637A0
// FUNCTION: fn_10910
int UIChatRoomTitle::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == UM_COMMAND) {
    return 0;
  }

  if (message != UM_SETINFO) {
    return UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  this->info = *reinterpret_cast<const ChatRoomTitleInfo*>(a);

  const std::size_t lineEnd = this->info.title.find('\r');
  if (lineEnd != std::string::npos) {
    this->info.title = this->info.title.substr(0, lineEnd);
  }

  std::string status;
  char text[384];
  if (this->info.type == 0) {
    status = LookupMessage(MSI_ROOM_PRIVATE);
    std::sprintf(text, "%s (%d/%d)", this->info.title.c_str(), this->info.memberCount, this->info.memberLimit);
  } else if (this->info.type == 3) {
    status.clear();
    std::sprintf(text, "%s", this->info.title.c_str());
  } else {
    status = LookupMessage(MSI_ROOM_PUBLIC);
    std::sprintf(text, "%s (%d/%d)", this->info.title.c_str(), this->info.memberCount, this->info.memberLimit);
  }

  this->roomOption = static_cast<short>(b);
  UIStaticText* label;
  switch (static_cast<short>(b)) {
  case 1:
    label = this->label;
    label->fontFamily = 21;
    break;
  case 2:
    label = this->label;
    label->fontFamily = 20;
    break;
  case 3:
    label = this->label;
    label->fontFamily = 22;
    break;
  case 4:
    label = this->label;
    label->fontFamily = 23;
    break;
  case 5:
    label = this->label;
    label->fontFamily = 24;
    break;
  case 6:
    label = this->label;
    label->fontFamily = 25;
    break;
  case 7:
    label = this->label;
    label->fontFamily = 26;
    break;
  case 8:
    label = this->label;
    label->fontFamily = 27;
    break;
  case 9:
    label = this->label;
    label->fontFamily = 28;
    break;
  default:
    label = this->label;
    label->fontFamily = 0;
    break;
  }
  label->bold = 0;
  label->italic = 0;
  label->fontHeight = 12;

  if (g_forbiddenWords.Contains(text) == true) {
    std::string masked("");
    g_forbiddenWords.Mask(text, &masked);
  }

  this->label->SetTextFitted(text);
  this->Invalidate();
  return 0;
}
