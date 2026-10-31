#include "UI/Clan/UIClanWnd.h"
#include "Contents/ClanInfo.h"
#include "Contents/MapInfoMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <cstring>
#include <windows.h>

// OFFSET: 0x0084CBD0
// FUNCTION: fn_10625
UIClanInfoManageWnd::UIClanInfoManageWnd(UIWindowContext& windows, UIFrameContext& frames) : UIClanWnd(windows, frames), m_emblem(nullptr) {}

// OFFSET: 0x0084CEF0
// FUNCTION: fn_10628
int UIClanInfoManageWnd::Slot53() {
  return 0;
}

// OFFSET: 0x0084CFD0
// FUNCTION: fn_10629
void UIClanInfoManageWnd::Slot15(int width, int height) {
  if (!this->m_title) {
    const char* title = LookupMessage(2355);
    this->m_title = new UIStaticText(*this->m_context, *this->m_controls);
    this->m_title->SetSize(62, 15);
    this->m_title->SetTextFitted(title);
    this->m_title->Move(3, 22);
    this->AddChild(this->m_title);
    this->AddCloseButton(*this->m_controls, "Ctrl+G", 0);
  }

  this->unknown_00BC = -1;
  this->unknown_00C0 = -1;
  if (g_clanInfo->m_hasClan) {
    g_clanInfo->unknown_0034 = 1;
  }

  this->LoadEmblem();
}

// OFFSET: 0x0084D0D0
// FUNCTION: fn_10630
void UIClanInfoManageWnd::Paint() {
  UIClanWnd::Paint();
  auto& controls = *this->m_controls;
  char text[1024];
  std::memset(text, 0, sizeof(text));
  std::sprintf(text, "%s : %s", LookupMessage(2357), g_clanInfo->m_clanName.c_str());
  this->DrawTextAt(controls.fonts, controls.locale, 8, 49, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %d", LookupMessage(2356), g_clanInfo->unknown_0034);
  this->DrawTextAt(controls.fonts, controls.locale, 8, 65, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : %s", LookupMessage(2362), g_clanInfo->m_masterName.c_str());
  this->DrawTextAt(controls.fonts, controls.locale, 8, 81, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s : ", LookupMessage(2363));
  this->DrawTextAt(controls.fonts, controls.locale, 8, 97, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s", "유저인터페이스\\basic_interface\\grp_online.bmp");
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
  this->DrawBitmap(128, 95, bitmap ? &bitmap->image : nullptr, true);
  std::sprintf(text, "   %d", g_clanInfo->m_connectedMembers);
  this->DrawTextAt(controls.fonts, controls.locale, 138, 97, text, 0, 0, 12, 0, 0, 0);

  {
    std::string map = g_clanInfo->m_mapName;
    if (map.size() > 4) {
      map.replace(map.find(".gat"), 4, ".rsw");
    }
    map = g_mapInfoMgr->GetDisplayName(std::string(map.c_str()));
    std::sprintf(text, "%s : %s", LookupMessage(2364), map.c_str());
    this->DrawTextAt(controls.fonts, controls.locale, 8, 113, text, 0, 0, 12, 0, 0, 0);
  }

  if (g_clanInfo->m_clanId != 0) {
    char path[256];
    std::memset(path, 0, sizeof(path));
    std::sprintf(path, "유저인터페이스\\clan_system\\clan_illust%02d.bmp", g_clanInfo->m_clanId);
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (!bitmap) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\illust\\펫_포포링.bmp")));
    }
    this->DrawBitmap(8, 145, bitmap ? &bitmap->image : nullptr, true);
  } else {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\illust\\펫_포포링.bmp")));
    this->DrawBitmap(50, 145, bitmap ? &bitmap->image : nullptr, true);
  }

  std::sprintf(text, "%s", LookupMessage(2358));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 65, text, 0, 0, 12, 0, 0, 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
  this->FillRegion(279, 64, 26, 26, RGB(red, green, blue));
  if (g_clanInfo->m_clanId != 0) {
    this->DrawBitmap(280, 65, this->m_emblem ? &this->m_emblem->image : nullptr, true);
  }
  std::sprintf(text, "%s", LookupMessage(2359));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 139, text, 0, 0, 12, 0, 0, 0);
  std::sprintf(text, "%s", LookupMessage(2360));
  this->DrawTextAt(controls.fonts, controls.locale, 200, 213, text, 0, 0, 12, 0, 0, 0);

  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(200, 155, 168, 48, RGB(red, green, blue));
  int row = 0;
  int y = 157;
  for (const auto& relation : g_clanInfo->m_relations) {
    if (y >= 205) {
      break;
    }
    if (relation.first == 0) {
      if (row == this->unknown_00BC) {
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
        this->FillRegion(204, y, 80, 16, RGB(red, green, blue));
      }
      this->DrawTextAt(controls.fonts, controls.locale, 204, y, relation.second.c_str(), 0, 0, 12, 0, 0, 0);
      ++row;
      y += 16;
    }
  }

  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(200, 229, 168, 48, RGB(red, green, blue));
  row = 0;
  y = 231;
  for (const auto& relation : g_clanInfo->m_relations) {
    if (y >= 279) {
      break;
    }
    if (relation.first == 1) {
      if (row == this->unknown_00C0) {
        SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
        this->FillRegion(204, y, 80, 16, RGB(red, green, blue));
      }
      this->DrawTextAt(controls.fonts, controls.locale, 204, y, relation.second.c_str(), 0, 0, 12, 0, 0, 0);
      ++row;
      y += 16;
    }
  }
}

// OFFSET: 0x0084D8F0
// FUNCTION: fn_10631
void UIClanWnd::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 10, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, 24, RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 18, 64, 23, RGB(red, green, blue));
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  this->FillRegion(0, 41, this->geometry.width, this->geometry.height - 41, RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 41, this->geometry.width - 2, this->geometry.height - 41, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x0084DA30
// FUNCTION: fn_10632
void UIClanInfoManageWnd::Slot25(int x, int y) {
  UIFrameWnd::Slot25(x, y);
}

// OFFSET: 0x0084DA40
// FUNCTION: fn_10633
int UIClanInfoManageWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 6) {
    if (a1 == 201) {
      g_windowMgr.CloseWindow(237);
    }
    return 0;
  }
  if (message == 60) {
    this->LoadEmblem();
    this->InvalidateChildren();
    return 0;
  }
  if (message == 34) {
    this->m_saved = reinterpret_cast<ClanWndInfo*>(a1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  }
  return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x0084DAE0
// FUNCTION: fn_10634
int UIClanWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a1, std::intptr_t a2, std::intptr_t a3, std::intptr_t a4) {
  if (message == 34) {
    this->m_saved = reinterpret_cast<ClanWndInfo*>(a1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;
  }

  if (message == 60) {
    this->InvalidateChildren();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, a1, a2, a3, a4);
}

// OFFSET: 0x0084DB50
// FUNCTION: fn_10635
void UIClanInfoManageWnd::LoadEmblem() {
  if (g_clanInfo->m_hasClan == true) {
    char path[256];
    std::memset(path, 0, sizeof(path));
    std::sprintf(path, "유저인터페이스\\clan_system\\clan_emblem%02d.bmp", g_clanInfo->m_clanId);
    this->m_emblem = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (!this->m_emblem) {
      this->m_emblem = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\clan_system\\clan_emblem01.bmp")));
    }
  }
}

// OFFSET: 0x0084DC00
// FUNCTION: fn_10636
void UIClanWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}
