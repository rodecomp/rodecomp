#include "Client/Mode/CGameMode.h"
#include "Client/Mode/EquipWindowLook.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstring>

struct ClientItemLists {
  bool Equip(const ITEM_INFO& item);
  void ResetEquipped();
};

// GLOBAL: gl_25085
EquipWindowLook g_equipWindowLook;

// OFFSET: 0x00D325F0
// FUNCTION: fn_21743
void CGameMode::ReceiveEquipWindowMicroscope(const unsigned char* packet) {
  reinterpret_cast<ClientItemLists*>(&g_session)->ResetEquipped();
  g_session.ResetItems3500();

  const int length = *reinterpret_cast<const std::int16_t*>(packet + 2);
  const int count = static_cast<unsigned>(length - 45) / 37u;
  const unsigned char* entry = packet + 45;
  for (int index = 0; index < count; ++index, entry += 37) {
    ITEM_INFO item;
    item.unknown_0000[0] = entry[6];
    item.unknown_0000[2] = *reinterpret_cast<const std::uint16_t*>(entry + 7);
    item.unknown_0000[1] = *reinterpret_cast<const std::int16_t*>(entry);
    item.unknown_0000[3] = *reinterpret_cast<const std::uint16_t*>(entry + 9);
    item.quantity = 1;
    std::memcpy(item.unknown_001C, entry + 12, sizeof(item.unknown_001C));
    item.SetItemId(*reinterpret_cast<const int*>(entry + 2));

    const unsigned char flags = entry[36];
    item.identified = flags & 1;
    item.unknown_005D = (flags >> 1) & 1;
    item.unknown_0060 = entry[11];
    item.unknown_0068 = *reinterpret_cast<const std::uint32_t*>(entry + 28);
    item.unknown_0064 = *reinterpret_cast<const std::uint16_t*>(entry + 32);
    item.unknown_0070 = *reinterpret_cast<const std::uint16_t*>(entry + 34);
    item.unknown_0074[0] = (flags >> 2) & 1;

    if (item.unknown_0000[3] != 0) {
      if (g_session.IsCostumeLocation(item.unknown_0000[3])) {
        g_session.EquipItem3500(item);
      } else {
        reinterpret_cast<ClientItemLists*>(&g_session)->Equip(item);
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

  if (!g_windowMgr.CloseWindow(0x8B)) {
    g_windowMgr.EnsureWindow(0x8B);
  }
}
