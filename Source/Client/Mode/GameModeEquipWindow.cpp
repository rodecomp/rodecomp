#include "Client/Mode/CGameMode.h"
#include "UI/Window/WindowId.h"
#include "Client/Session/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <cstring>

namespace {
#pragma pack(push, 1)
  struct EquipWindowRecord {
    /* 0000 */ std::int16_t index;
    /* 0002 */ int itemId;
    /* 0006 */ unsigned char type;
    /* 0007 */ std::uint16_t location;
    /* 0009 */ std::uint16_t wearState;
    /* 000B */ unsigned char refine;
    /* 000C */ std::uint32_t cards[4];
    /* 001C */ std::uint32_t expireTime;
    /* 0020 */ std::uint16_t bindOnEquip;
    /* 0022 */ std::uint16_t sprite;
    /* 0024 */ unsigned char identified : 1;
    /* 0024 */ unsigned char damaged : 1;
    /* 0024 */ unsigned char etcTab : 1;
  };
#pragma pack(pop)

  static_assert(sizeof(EquipWindowRecord) == 37);
}

// OFFSET: 0x00D325F0
// FUNCTION: fn_21743
void CGameMode::ReceiveEquipWindowMicroscope(const unsigned char* packet) {
  g_session.ResetEquipped();
  g_session.ResetAlternateCostumeItems();

  const int count = (*reinterpret_cast<const std::int16_t*>(packet + 2) - 45) / sizeof(EquipWindowRecord);
  const EquipWindowRecord* records = reinterpret_cast<const EquipWindowRecord*>(packet + 45);
  for (int index = 0; index < count; ++index) {
    const EquipWindowRecord* record = &records[index];
    ITEM_INFO item;
    item.identity[0] = record->type;
    item.identity[2] = record->location;
    item.identity[1] = record->index;
    item.identity[3] = record->wearState;
    item.quantity = 1;
    std::memcpy(item.cards, record->cards, sizeof(item.cards));
    item.SetItemId(record->itemId);
    item.identified = record->identified;
    item.damaged = record->damaged;
    item.refiningLevel = record->refine;
    item.expireTime = record->expireTime;
    item.bindOnEquip = record->bindOnEquip;
    item.sprite = record->sprite;
    item.placeEtcTab = record->etcTab;

    if (item.identity[3] != 0) {
      if (g_session.IsCostumeLocation(item.identity[3])) {
        g_session.EquipAlternateCostumeItem(item);
      } else {
        g_session.Equip(item);
      }
    }
  }

  g_session.m_equipViewLook[0] = *reinterpret_cast<const std::int16_t*>(packet + 0x1C);
  g_session.m_equipViewLook[1] = packet[0x2C];
  g_session.m_equipViewLook[2] = *reinterpret_cast<const std::int16_t*>(packet + 0x1E);
  g_session.m_equipViewLook[3] = *reinterpret_cast<const std::int16_t*>(packet + 0x20);
  g_session.m_equipViewLook[4] = *reinterpret_cast<const std::int16_t*>(packet + 0x22);
  g_session.m_equipViewLook[5] = *reinterpret_cast<const std::int16_t*>(packet + 0x24);
  g_session.m_equipViewLook[6] = *reinterpret_cast<const std::int16_t*>(packet + 0x2A);
  g_session.m_equipViewLook[7] = *reinterpret_cast<const std::int16_t*>(packet + 0x28);
  std::memcpy(g_session.m_equipViewName, packet + 4, sizeof(g_session.m_equipViewName));
  g_session.m_equipViewLook[8] = *reinterpret_cast<const std::int16_t*>(packet + 0x26);

  if (!g_windowMgr.CloseWindow(WID_EQUIPWND_VIEWOTHER_1)) {
    g_windowMgr.EnsureWindow(WID_EQUIPWND_VIEWOTHER_1);
  }
}
