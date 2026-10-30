#include "UI/Window/UIOpenRoulletteWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>

namespace {
#pragma pack(push, 1)
struct RouletteOpenPacket {
  /* 0000 */ std::int16_t id;
};
#pragma pack(pop)
}

// OFFSET: 0x0094F1C0
// FUNCTION: fn_12933
void UIOpenRoulletteWnd::Slot19() {
}

// OFFSET: 0x0094C4E0
// FUNCTION: fn_12897
void UIOpenRoulletteWnd::Slot15(int width, int height) {
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap("유저인터페이스\\basic_interface\\roullette\\RoulletteIcon.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\roullette\\RoulletteIcon.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\basic_interface\\roullette\\RoulletteIcon.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->SetId(192);
  this->AddChild(button);
}

// OFFSET: 0x0094D900
// FUNCTION: fn_12910
void UIOpenRoulletteWnd::Paint() {
}

// OFFSET: 0x00950B60
// FUNCTION: fn_12946
int UIOpenRoulletteWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  if (a == 192 && g_windowMgr.unknown_0464 == nullptr) {
    RouletteOpenPacket packet;
    packet.id = ID_CZ_REQ_OPEN_ROULETTE;
    CRagConnection& connection = GetRagConnection();
    connection.Send(g_packetLengths.FixedLength(packet.id), &packet);
  }

  return 0;
}
