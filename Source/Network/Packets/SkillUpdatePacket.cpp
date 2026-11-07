#include "Client/Mode/CGameMode.h"
#include "Network/Packets/PacketId.h"
#include "Client/Session.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>

namespace {
  // INLINED
  template <class T> T Read(const unsigned char* bytes) {
    T value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
  }
}

// OFFSET: 0x00D42AA0
// FUNCTION: fn_21829
void CGameMode::ApplySkillUpdatePacket(const void* packet) {
  *reinterpret_cast<int*>(&this->unknown_0100[0x0C]) = 0;

  const auto* bytes = static_cast<const unsigned char*>(packet);
  const auto opcode = Read<std::uint16_t>(bytes);
  const auto id = Read<std::uint16_t>(bytes + 2);
  CSkillInfo value;

  switch (opcode) {
  case ID_ZC_SKILLINFO_UPDATE:
    value = g_session.m_skillBundle.Lookup(id);
    break;
  case ID_ZC_HOSKILLINFO_UPDATE:
    value = g_session.LookupHomunSkill(id);
    break;
  case 670:
    value = g_session.LookupMercSkill(id);
    break;
  }

  value.id = id;
  value.level = Read<std::int16_t>(bytes + 4);
  value.unknown_0014 = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 6));
  value.unknown_0018 = bytes[10];
  value.unknown_001C = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 8));
  value.unknown_0034 = value.UseApAmount(*g_session.m_hotkeyScript, value.level);
  g_session.m_skillBundle.ApplyLevelChange(value);

  switch (opcode) {
  case ID_ZC_SKILLINFO_UPDATE:
    g_session.UpdateSkill(value, 1, 0);
    break;
  case ID_ZC_HOSKILLINFO_UPDATE:
    g_session.UpdateHomunSkill(value, 1);
    break;
  case 670:
    g_session.UpdateMercSkill(value, 1);
    break;
  }
}

// OFFSET: 0x00D06AF0
// FUNCTION: fn_21577
void CGameMode::ReceiveLearnedSkill(
  std::uint16_t id, std::int16_t level, int unknown000C, std::uint8_t unknown0018, std::int16_t unknown0014, std::int16_t unknown001C, std::int16_t unknown0030
) {
  CSkillInfo value;
  value.unknown_000C = unknown000C;
  value.id = id;
  value.level = level;
  value.unknown_0018 = unknown0018;
  value.name = value.IdName(*g_session.m_hotkeyScript);
  value.unknown_0014 = unknown0014;
  value.unknown_001C = unknown001C;
  value.unknown_0030 = unknown0030;
  value.unknown_0034 = value.UseApAmount(*g_session.m_hotkeyScript, level);

  g_session.m_skillBundle.RecordLevelChange(value);
  if (value.id == 336) {
    g_session.unknown_5468 = 1;
  }

  g_session.UpdateSkill(value, 1, 0);
}

// OFFSET: 0x00D2B5C0
// FUNCTION: fn_45442
void CGameMode::ApplyAdditionalSkillPacket(const void* packet) {
  *reinterpret_cast<int*>(&this->unknown_0100[0x0C]) = 0;
  g_session.m_additionalSkills.clear();

  const auto* bytes = static_cast<const unsigned char*>(packet);

  const auto length = static_cast<std::uint32_t>(static_cast<int>(Read<std::int16_t>(bytes + 2)) - 6);
  const std::uint32_t count = length / 37;
  const unsigned char* record = bytes + 6;

  for (std::uint32_t index = 0; index < count; ++index, record += 37) {
    CSkillInfo value;
    value.level = Read<std::int16_t>(record + 6);
    value.id = Read<std::uint16_t>(record);
    value.unknown_0014 = static_cast<std::int32_t>(Read<std::int16_t>(record + 8));
    value.unknown_000C = Read<std::uint32_t>(record + 2);
    value.unknown_0018 = record[36];
    value.name = value.IdName(*g_session.m_hotkeyScript);
    value.unknown_0034 = value.UseApAmount(*g_session.m_hotkeyScript, value.level);

    if (value.id - 10000u <= 19u) {
      g_session.UpdateAdditionalSkill(value, 1);
    }
  }

  g_session.m_guildSkillPoint = Read<std::int16_t>(bytes + 4);
  g_session.InvalidateWindowFrames();
  if (g_windowMgr.window_032C) {
    g_windowMgr.window_032C->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }

  if (g_windowMgr.window_01E8) {
    g_windowMgr.window_01E8->SendMsg(nullptr, 23, 0, 0, 0, 0);
  }
}
