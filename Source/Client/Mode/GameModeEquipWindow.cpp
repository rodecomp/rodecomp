#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "Client/Mode/EquipWindowLook.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstring>


// GLOBAL: gl_25085
EquipWindowLook g_equipWindowLook;

// OFFSET: 0x00D325F0
// FUNCTION: fn_21743
void CGameMode::ReceiveEquipWindowMicroscope(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 37u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 37) {
    ITEM_INFO item;
    item.identity[0] = entry[6];
    item.identity[2] = *reinterpret_cast<const std::uint16_t*>(entry + 7);
    item.identity[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.identity[3] = *reinterpret_cast<const std::uint16_t*>(entry + 9);
    item.quantity = 1;
    std::memcpy(item.cards, entry + 12, sizeof(item.cards));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));

    const unsigned char flags = entry[36];
    item.identified = flags & 1;
    item.damaged = (flags >> 1) & 1;
    item.refiningLevel = entry[11];
    item.expireTime = *reinterpret_cast<const std::uint32_t*>(entry + 28);
    item.bindOnEquip = *reinterpret_cast<const std::uint16_t*>(entry + 32);
    item.sprite = *reinterpret_cast<const std::uint16_t*>(entry + 34);
    item.flags[0] = (flags >> 2) & 1;

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipItem3500(item);
      } else {
        g_session.Equip(item);
      }
    }
  }

  g_equipWindowLook.job = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_equipWindowLook.sex = packet[0x2C];
  g_equipWindowLook.head = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_equipWindowLook.accessory = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_equipWindowLook.accessory2 = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_equipWindowLook.accessory3 = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_equipWindowLook.bodyPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_equipWindowLook.headPalette = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  std::memcpy(g_equipWindowLook.name, packet + 4, sizeof(g_equipWindowLook.name));
  g_equipWindowLook.robe = *reinterpret_cast<const std::int16_t*>(packet + 0x26);

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}
