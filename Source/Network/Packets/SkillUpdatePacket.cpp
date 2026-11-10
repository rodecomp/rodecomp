#include "Client/Mode/CGameMode.h"
#include "UI/Window/UIEnums.h"
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
  *reinterpret_cast<int*>(&this->equipmentRequestState[0x0C]) = 0;

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
  value.apCost = value.UseApAmount(*g_session.m_hotkeyScript, value.level);
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
  std::uint16_t id, std::int16_t level, int type, std::uint8_t upgradable, std::int16_t spCost, std::int16_t range, std::int16_t unknown0030
) {
  CSkillInfo value;
  value.type = type;
  value.id = id;
  value.level = level;
  value.upgradable = upgradable;
  value.name = value.IdName(*g_session.m_hotkeyScript);
  value.spCost = spCost;
  value.range = range;
  value.learnedLevel = unknown0030;
  value.apCost = value.UseApAmount(*g_session.m_hotkeyScript, level);

  g_session.m_skillBundle.RecordLevelChange(value);
  if (value.id == 336) {
    g_session.adoptionSkillAvailable = 1;
  }

  g_session.UpdateSkill(value, 1, 0);
}

// OFFSET: 0x00D2B5C0
// FUNCTION: fn_45442
void CGameMode::ApplyAdditionalSkillPacket(const void* packet) {
  *reinterpret_cast<int*>(&this->equipmentRequestState[0x0C]) = 0;
  g_session.m_additionalSkills.clear();

  const auto* bytes = static_cast<const unsigned char*>(packet);

  const auto length = static_cast<std::uint32_t>(static_cast<int>(Read<std::int16_t>(bytes + 2)) - 6);
  const int count = static_cast<int>(length / 37);
  const unsigned char* record = bytes + 6;

  for (int index = 0; index < count; ++index, record += 37) {
    CSkillInfo value;
    value.level = Read<std::int16_t>(record + 6);
    value.id = Read<std::uint16_t>(record);
    value.spCost = static_cast<std::int32_t>(Read<std::int16_t>(record + 8));
    value.type = Read<std::uint32_t>(record + 2);
    value.upgradable = record[36];
    value.name = value.IdName(*g_session.m_hotkeyScript);
    value.apCost = value.UseApAmount(*g_session.m_hotkeyScript, Read<std::int16_t>(record + 6));

    if (value.id - 10000u <= 19u) {
      g_session.UpdateAdditionalSkill(value, 1);
    }
  }

  g_session.m_guildSkillPoint = Read<std::int16_t>(bytes + 4);
  g_session.InvalidateWindowFrames();
  if (g_windowMgr.guildSkillWnd) {
    g_windowMgr.guildSkillWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }

  if (g_windowMgr.shortCutWnd) {
    g_windowMgr.shortCutWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}
