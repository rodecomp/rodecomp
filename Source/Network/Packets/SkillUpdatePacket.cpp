#include "Client/Mode/CGameMode.h"
#include "Network/Packets/PacketId.h"
#include "Client/Session/Session.h"

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
  this->isReqUpgradeSkillLevel = 0;

  const auto* bytes = static_cast<const unsigned char*>(packet);
  CSkillInfo value;

  if (Read<std::uint16_t>(bytes) == ID_ZC_SKILLINFO_UPDATE) {
    value = g_session.m_skillBundle.Lookup(Read<std::uint16_t>(bytes + 2));
  } else if (Read<std::uint16_t>(bytes) == ID_ZC_HOSKILLINFO_UPDATE) {
    value = g_session.LookupHomunSkill(Read<std::uint16_t>(bytes + 2));
  } else if (Read<std::uint16_t>(bytes) == ID_ZC_MER_SKILLINFO_UPDATE) {
    value = g_session.LookupMercSkill(Read<std::uint16_t>(bytes + 2));
  }

  value.id = Read<std::uint16_t>(bytes + 2);
  value.level = Read<std::int16_t>(bytes + 4);
  value.spCost = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 6));
  value.upgradable = bytes[10];
  value.range = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 8));
  value.apCost = value.UseApAmount(value.level);
  g_session.m_skillBundle.ApplyLevelChange(value);

  if (Read<std::uint16_t>(bytes) == ID_ZC_SKILLINFO_UPDATE) {
    g_session.UpdateSkill(value, 1, 0);
  } else if (Read<std::uint16_t>(bytes) == ID_ZC_HOSKILLINFO_UPDATE) {
    g_session.UpdateHomunSkill(value, 1);
  } else if (Read<std::uint16_t>(bytes) == ID_ZC_MER_SKILLINFO_UPDATE) {
    g_session.UpdateMercSkill(value, 1);
  }
}

// OFFSET: 0x00D06AF0
// FUNCTION: fn_21577
void CGameMode::ReceiveLearnedSkill(
  std::uint16_t id, std::int16_t level, int type, std::uint8_t upgradable, std::int16_t spCost, std::int16_t range, std::int16_t learnedLevel
) {
  CSkillInfo value;
  value.type = type;
  value.id = id;
  value.level = level;
  value.upgradable = upgradable;
  value.name = value.IdName();
  value.spCost = spCost;
  value.range = range;
  value.learnedLevel = learnedLevel;
  value.apCost = value.UseApAmount(level);

  g_session.m_skillBundle.RecordLevelChange(value);
  if (value.id == 336) {
    g_session.adoptionSkillAvailable = 1;
  }

  g_session.UpdateSkill(value, 1, 0);
}
