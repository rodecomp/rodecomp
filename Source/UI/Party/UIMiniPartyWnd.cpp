#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007426F0
// FUNCTION: fn_07737
UIMiniPartyWnd::UIMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext)
    : UIFrameWnd(windowContext, frameContext, 1), m_cell(-1), m_infoWnd(nullptr), m_background(nullptr), m_sizeMode(0) {
  this->m_imageName = "유저인터페이스\\renewalparty\\img_partymini_bg.tga";
  this->geometry.id = 301;
}

// OFFSET: 0x00742AA0
// FUNCTION: fn_07739
void UIMiniPartyWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y;
}

// OFFSET: 0x00742AC0
// FUNCTION: fn_07740
void UIMiniPartyWnd::Slot15(int, int) {
  UIMiniPartyInfoWnd* info = new UIMiniPartyInfoWnd(*this->m_context, *this->m_frameContext);
  this->m_infoWnd = info;
  info->m_owner = this;
  info->SetSize(174, 52);
  info->Move(0, 0);
  this->AddChild(info);
  this->m_widths.resize(3);
  this->m_widths[0] = 174;
  this->m_widths[1] = 101;
  this->m_widths[2] = 64;
}

// OFFSET: 0x00742BE0
// FUNCTION: fn_07741
void UIMiniPartyWnd::Paint() {
  this->m_primitives.clear();
  const int count = this->m_member.offline != 1 ? 3 : 1;
  for (int index = 0; index < count; ++index) {
    this->DrawNamed(this->m_context->primitives, 0, 0, this->m_imageName, 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x00742D80
// FUNCTION: fn_07742
void UIMiniPartyWnd::Slot25(int x, int y) {
  if (g_dragWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  this->ToScreen(x, y);
  this->m_dragX = x;
  this->m_dragY = y;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->geometry.alphaStart = timeGetTime();
  this->m_frameContext->snapping.graph.CaptureOrigins(this);
}

// OFFSET: 0x00740D10
// FUNCTION: fn_07732
void UIMiniPartyInfoWnd::Slot31(int x, int y) {
  if (g_messengerGroupCollapsed != true && this == g_windowMgr.Capture()) {
    this->m_owner->Slot31(x, y);
  }
}

// OFFSET: 0x00741280
// FUNCTION: fn_07734
void UIMiniPartyInfoWnd::Slot33(int, int) {
  if (!g_messengerGroupCollapsed) {
    this->SendMsg(nullptr, 0x31, 0, 0, 0, 0);
  }
}
