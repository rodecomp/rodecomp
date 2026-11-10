#include "Resource/Bitmap/BitmapResource.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIMileageWnd.h"
#include "Client/Session.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/ColoredText.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/CMode.h"

#include "Resource/MSGSTRINGID.h"
#include <cstddef>
#include <cstdint>
#include <windows.h>
#include <timeapi.h>

namespace {
#pragma pack(push, 1)
struct DynamicNpcCreatePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[24];
};
#pragma pack(pop)
}

// GLOBAL: gl_24859
int g_mileageWndActive;

// GLOBAL: gl_24860
int g_mileageWndPoint;

// GLOBAL: gl_24890
int g_mileageWndElapsed;

// GLOBAL: gl_24861
int g_mileageWndBonus;

// OFFSET: 0x0094D4D0
// FUNCTION: fn_12905
void UIMileageWnd::Paint() {
  this->ClearSurface(0xFF00FF);

  std::string text;
  const std::string background = FormatOwnedString("유저인터페이스\\basic_interface\\mileage_bg%d.bmp", this->m_bonus);
  text = background;
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text.c_str()))), true);

  if (this->m_point >= 300) {
    text = std::string("-- : --");
  } else {
    const std::string remaining = FormatOwnedString("%02d : %02d", this->m_remaining / 60, this->m_remaining % 60);
    text = remaining;
  }

  int textWidth = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
  DrawOutlinedText(*this, (this->geometry.width - textWidth) >> 1, 15, text.c_str(), 0, 0xFFFFFF, 0, 0, 12, 0, 0);

  text = FormatOwnedString("%d", this->m_point).c_str();
  textWidth = this->TextWidth(text.c_str(), 0, 0, 12, 0, 0);
  DrawOutlinedText(*this, this->geometry.width - textWidth - 9, this->geometry.height - 16, text.c_str(), 0, 0xFFFFFF, 0, 0, 12, 0, 0);
}

// OFFSET: 0x0094EE70
// FUNCTION: fn_12918
void UIMileageWnd::OnLBtnDown(int x, int y) {
}

// OFFSET: 0x0094EF10
// FUNCTION: fn_12923
void UIMileageWnd::OnLBtnUp(int x, int y) {
  if (g_session.mapPropertyId == 20) {
    return;
  }

  DynamicNpcCreatePacket packet = {ID_CZ_DYNAMICNPC_CREATE_REQUEST};
  StringFormat(packet.name, sizeof(packet.name), static_cast<std::size_t>(-1), "GOLDPCCAFE");
  GetRagConnection().Send(sizeof(packet), &packet);
}

// OFFSET: 0x0094EFC0
// FUNCTION: fn_12926
void UIMileageWnd::OnMouseHover(int x, int y) {
  if (this->m_tooltip.size() == 0) {
    return;
  }

  int tipX = 10;
  int tipY = -20;
  this->ToScreen(tipX, tipY);
  g_modeMgr.gameMode->ShowTooltip(this->m_tooltip.c_str(), tipX, tipY, -1, false, true);
}

// OFFSET: 0x0094F040
// FUNCTION: fn_12929
void UIMileageWnd::OnMouseMove(int x, int y) {
}

// OFFSET: 0x0094F130
// FUNCTION: fn_12932
void UIMileageWnd::OnProcess() {
  if (!this->geometry.visible || this->m_point >= 300 || this->m_remaining == 0) {
    return;
  }

  const std::uint32_t now = timeGetTime();
  const std::uint32_t elapsed = now - this->m_startTime;
  if (elapsed < 1000) {
    return;
  }

  const std::uint32_t seconds = elapsed / 1000;
  this->m_remaining -= seconds;
  if (this->m_remaining < 0) {
    this->m_remaining = 0;
  }

  this->m_startTime = seconds * 1000 - elapsed + now;
  this->Invalidate();
  g_mileageWndElapsed = 3600 - this->m_remaining;
}

// OFFSET: 0x0094C230
// FUNCTION: fn_12894
void UIMileageWnd::OnCreate(int width, int height) {
  this->m_tooltip = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_RESTRAINT_OF_TRADE_3), 300));
}

// OFFSET: 0x00950690
// FUNCTION: fn_12942
int UIMileageWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 34) {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  g_mileageWndActive = static_cast<int>(a);
  this->Show(static_cast<int>(a));

  if (a != 0) {
    this->m_point = static_cast<int>(b);
    g_mileageWndPoint = static_cast<int>(b);
    this->m_bonus = static_cast<int>(d);
    g_mileageWndBonus = static_cast<int>(d);

    int remaining = 3600 - static_cast<int>(c);
    if (remaining < 0) {
      remaining = 0;
    }

    this->m_remaining = remaining;
    this->m_startTime = timeGetTime();
    this->Show(g_serviceOptions.Get(219));
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x00946B50
// FUNCTION: fn_12850
UIMileageWnd::UIMileageWnd()
    : UIFrameWnd(0), m_startTime(0), m_point(0), m_remaining(0), m_bonus(1), m_tooltip("") {
}
