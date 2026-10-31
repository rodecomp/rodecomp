#include "UI/Window/UISelCharForUServerWnd.h"
#include "Client/Mode/CMode.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"

#include <timeapi.h>

extern const char* LookupMessage(int id);

#pragma pack(push, 1)
struct PACKET_CZ_SELECT_CHARACTERS {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int accountId;
  /* 0006 */ int count;
  /* 000A */ int characterIds[9];
};
#pragma pack(pop)

// OFFSET: 0x009120E0

// OFFSET: 0x0093A7F0
// FUNCTION: fn_12758
int UISelCharForUServerWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(&g_windowMgr);
  if (argument1 == 0xB8) {
    if (dialogs.Confirm(LookupMessage(0x4BB), 2, false, false, 0, "Message", 280, 120, 0) != 0xBB) {
      return 0;
    }

    int characterIds[9] = {};
    int count = 0;
    const int viewCount = *reinterpret_cast<int*>(0x017334CC);
    for (int index = 0; index < viewCount; ++index) {
      if (this->m_checkBoxes[index]->geometry.unknown_0030 != 0) {
        CMode* mode = *reinterpret_cast<CMode**>(0x013495BC);
        const int* character = reinterpret_cast<const int*>(mode->SendMsg(8, index, 0, 0, 0));
        if (character) {
          characterIds[count++] = *character;
        }
      }
    }

    if (count != 0) {
      PACKET_CZ_SELECT_CHARACTERS packet;
      packet.id = 0x28C;
      packet.accountId = *reinterpret_cast<int*>(0x01733C3C);
      packet.count = count;
      for (int index = 0; index < 9; ++index) {
        packet.characterIds[index] = characterIds[index];
      }
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
      g_windowMgr.Schedule(this);
      return 0;
    }

    dialogs.Confirm(LookupMessage(0x4C1), 0, true, true, 0, "Message", 280, 120, 0);
  } else if (argument1 == 0xD5) {
    this->m_checkedCount += sender->geometry.unknown_0030 != 0 ? 1 : -1;
    if (this->m_checkedCount > 9) {
      dialogs.Confirm(LookupMessage(0x4BC), 0, true, true, 0, "Message", 280, 120, 0);
      static_cast<UICheckBox*>(sender)->SetChecked(0);
    }
  }

  return 0;
}

// OFFSET: 0x00932B40
// FUNCTION: fn_12714
void UISelCharForUServerWnd::Slot27(int x, int y) {
  reinterpret_cast<void(__thiscall*)(UISelCharForUServerWnd*, int, int, int)>(0x0093F1F0)(this, x, y, -1);
  UIFrameWnd::Slot27(x, y);
}
