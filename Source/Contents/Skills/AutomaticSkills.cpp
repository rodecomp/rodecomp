#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Skills/SkillId.h"
#include "Client/Session/Session.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"

#include <cstring>

namespace {
  // INLINED
  int ActiveOption(std::uint16_t id) {
    switch (id) {
    case GC_ROLLINGCUTTER:
      return 2;
    case KR_CHOP_CHOP:
      return 23;
    case AT_FURIOS_STORM:
      return 21;
    default:
      return -1;
    }
  }

  // INLINED
  void Cancel(std::int16_t id, int option) {
    const std::uint16_t packet[2] = {2833, static_cast<std::uint16_t>(id)};
    GetRagConnection().Send(sizeof(packet), packet);
    g_session.SetOption(option, 0);
  }
}

// OFFSET: 0x007AEEE0
// FUNCTION: fn_09148
AutomaticSkills::AutomaticSkills() {
  this->m_ids.insert(2036);
  this->m_ids.insert(6601);
  this->m_ids.insert(6552);
}

// OFFSET: 0x007AF130
// FUNCTION: fn_09149
AutomaticSkills::~AutomaticSkills() {
  this->DisableAll();
}

// OFFSET: 0x007AF490
// FUNCTION: fn_09152
bool AutomaticSkills::Contains(int id) const {
  return this->m_ids.find(static_cast<std::uint16_t>(id)) != this->m_ids.end();
}

// OFFSET: 0x007AF400
// FUNCTION: fn_09151
int AutomaticSkills::CancelOption(int id) const {
  switch (static_cast<std::uint16_t>(id)) {
  case GC_ROLLINGCUTTER:
    return 4;
  case KR_CHOP_CHOP:
    return 24;
  case AT_FURIOS_STORM:
    return 22;
  default:
    return -1;
  }
}

// OFFSET: 0x007AF2A0
// FUNCTION: fn_09150
void AutomaticSkills::DisableAll() {
  g_session.automaticSkillHotkey = 0;

  for (int id : this->m_ids) {
    const auto skill = static_cast<std::int16_t>(id);
    const int option = ActiveOption(skill);
    if (static_cast<int>(g_session.m_options.Get(option)) != 0) {
      Cancel(skill, option);
    }
  }
}

// OFFSET: 0x007AF590
// FUNCTION: fn_09153
void AutomaticSkills::Use(std::int16_t id, std::int16_t level, std::uint32_t target) {
  unsigned char packet[10];
  const std::uint16_t header = ID_CZ_USE_SKILL_START;
  std::memcpy(packet, &header, 2);
  std::memcpy(packet + 2, &id, 2);
  std::memcpy(packet + 4, &level, 2);
  std::memcpy(packet + 6, &target, 4);
  GetRagConnection().Send(sizeof(packet), packet);
  const int cancel = this->CancelOption(id);

  if (static_cast<int>(g_session.m_options.Get(cancel)) != 0) {
    Cancel(id, cancel);
  } else {
    g_session.automaticSkillHotkey = g_session.m_currentHotkey;
    g_session.SetOption(ActiveOption(id), 1);
  }
}

