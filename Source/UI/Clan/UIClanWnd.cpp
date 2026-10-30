#include "UI/Clan/UIClanWnd.h"
#include "Contents/ClanInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"

#include <cstdio>
#include <cstring>
#include <windows.h>

// OFFSET: 0x0084D8F0

// OFFSET: 0x0084DA30
// FUNCTION: fn_10632
void UIClanInfoManageWnd::Slot25(int x, int y) {
  UIFrameWnd::Slot25(x, y);
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
