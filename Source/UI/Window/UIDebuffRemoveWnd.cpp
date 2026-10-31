#include "UI/Window/UIDebuffRemoveWnd.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"

#include <cstdint>

namespace {
#pragma pack(push, 1)
struct DebuffRemovePacket {
  /* 0000 */ std::int16_t id;
};
#pragma pack(pop)
}

// OFFSET: 0x0094D260
// FUNCTION: fn_12903
void UIDebuffRemoveWnd::Paint() {
  const TextLocale locale = {};
  this->DrawTitleBar(g_fontCache, locale, false, "", 0);
  this->DrawPanelBackground(17, 0, "");
}

// OFFSET: 0x0094EE60
// FUNCTION: fn_12917
void UIDebuffRemoveWnd::Slot25(int x, int y) {
}

// OFFSET: 0x0094EF00
// FUNCTION: fn_12922
void UIDebuffRemoveWnd::Slot31(int x, int y) {
}

// OFFSET: 0x0094F030
// FUNCTION: fn_12928
void UIDebuffRemoveWnd::Slot27(int x, int y) {
}

// OFFSET: 0x00950400
// FUNCTION: fn_12940
int UIDebuffRemoveWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (a == 184) {
    DebuffRemovePacket packet;
    packet.id = 0xC2A;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }

  return 0;
}

// OFFSET: 0x00946970
// FUNCTION: fn_12848
UIDebuffRemoveWnd::UIDebuffRemoveWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_button(nullptr), m_controls(&controls) {
}
