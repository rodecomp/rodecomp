#include "UI/Guild/UIAgitWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/GuildInfo.h"
#include "Contents/GuildMgr.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstring>
#include <windows.h>

// OFFSET: 0x009537B0
// FUNCTION: fn_12992
UIAgitMessageBox::UIAgitMessageBox()
    : UIFrameWnd(0), m_agitId(0) {
}

// OFFSET: 0x0095D580
// FUNCTION: fn_13068
void UIAgitMessageBox::Paint() {
  this->DrawTitleBar(true, LookupMessage(3640), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x00961260
// FUNCTION: fn_13122
int UIAgitMessageBox::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 6) {
    const int response = static_cast<int>(a);
    this->RecordResponse(response, b, c, d);
    switch (response) {
    case 187:
      g_modeMgr.GetGameMode()->SendMsg(315, this->m_agitId, 0, 0, 0);
      [[fallthrough]];
    case 188:
      g_windowMgr.Schedule(this);
    }
  }

  return 188;
}

// OFFSET: 0x0095D570
// FUNCTION: fn_13067
void UIListBoxAgit::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x00960090
// FUNCTION: fn_13092
void UIListBoxAgit::Slot25(int x, int y) {
  if (this->parent) {
    UIListBox::Slot25(x, y);
    this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
  }
}

// OFFSET: 0x009604B0
// FUNCTION: fn_13106
void UIListBoxAgit::Slot32(int x, int y) {
  if (this->parent) {
    UIListBox::Slot25(x, y);
    if (this->selected < 0) {
      this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
      return;
    }
    this->parent->SendMsg(nullptr, 162, this->geometry.width + this->geometry.x - 3, this->geometry.y - y % this->lineHeight + y + 2, 0, 0);
  }
}

// OFFSET: 0x0095F9E0
// FUNCTION: fn_13081
void UIListBoxAgitMenu::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  auto* leftUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")));
  this->DrawBitmap(0, 0, leftUp, false);
  auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, leftDown, false);
  auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, rightUp, false);
  auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, rightDown, false);

  SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
  for (int index = 0; index < 2; ++index) {
    if (index == this->selected) {
      this->FillRegion(3, this->lineHeight * index + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }
    this->DrawTextAt(3, this->lineHeight * index + 1, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x009600D0
// FUNCTION: fn_13093
void UIListBoxAgitMenu::Slot25(int x, int y) {
  UIListBox::Slot25(x, y);
  this->SendMsg(nullptr, 48, this->selected, 0, 0, 0);
  if (this->parent) {
    this->parent->SendMsg(nullptr, 163, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00960160
// FUNCTION: fn_13096
void UIListBoxAgitMenu::Slot28(int x, int y) {
  const int previous = this->selected;
  int line;
  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    line = -1;
  } else {
    line = y / this->lineHeight;

    if (line < 0) {
      line = 0;
    }

    line = std::min(line, 1);
  }

  this->selected = line;
  if (previous != line) {
    this->Invalidate();
  }
}

// OFFSET: 0x00964F80
// FUNCTION: fn_13136
int UIListBoxAgitMenu::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 48) {
    return this->UIListBox::SendMsg(sender, message, a, b, c, d);
  }

  switch (a) {
  case 0: {
    if (g_session.GetSceneUnknown004C() == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(3633)), 0xFF, 0, 0, 0);
      return 0;
    }

    GuildInfo* guildInfo = &g_guildInfo;
    uint16_t agitId = static_cast<uint16_t>(guildInfo->GetUnknown0114(this->m_agitIndex));

    char* agitName = nullptr;
    g_session.m_hotkeyScript->Call("get_agit_name", "d>s", agitId, &agitName);
    if (strnlen(agitName, 256) != 0) {
      int costZeny = 0;
      g_session.m_hotkeyScript->Call("get_agit_costzeny", "db>d", agitId, 0, &costZeny);
      int costGuildPoint = 0;
      g_session.m_hotkeyScript->Call("get_agit_costzeny", "db>d", agitId, 1, &costGuildPoint);

      UIAgitMessageBox* box = new UIAgitMessageBox();
      const char* text = FormatString(LookupMessage(3631), agitName, costZeny, costGuildPoint);
      box->InitializeMessageBox(text, 310, 130);

      int x = g_renderer->viewport.centerX - box->geometry.width / 2;
      int y = g_renderer->viewport.centerY - box->geometry.height / 2;
      box->Move(x, y);
      box->m_agitId = agitId;
      g_windowMgr.AddRendered(box);
    }
    break;
  }
  case 1: {
    CMode* mode = g_modeMgr.GetGameMode();
    GuildInfo* guildInfo = &g_guildInfo;
    uint16_t agitId = static_cast<uint16_t>(guildInfo->GetUnknown0114(this->m_agitIndex));
    mode->SendMsg(316, agitId, 0, 0, 0);
    break;
  }
  }

  return 0;
}
