#include "Contents/Skills/AutomaticSkills.h"
#include "Client/Session.h"

#include <cstring>

namespace {
  // INLINED
  int ActiveOption(std::int16_t id) {
    switch (id) {
    case 2036:
      return 2;
    case 6552:
      return 23;
    case 6601:
      return 21;
    default:
      return -1;
    }
  }

  // INLINED
  void Cancel(CConnection& connection, std::int16_t id) {
    const std::uint16_t packet[2] = {2833, static_cast<std::uint16_t>(id)};
    connection.Send(sizeof(packet), packet);
  }
}

// OFFSET: 0x007AEEE0
// FUNCTION: fn_09148
AutomaticSkills::AutomaticSkills(AutomaticSkillContext& context) : m_ids{2036, 6601, 6552}, m_context(&context) {
}

// OFFSET: 0x007AF130
// FUNCTION: fn_09149
AutomaticSkills::~AutomaticSkills() {
  this->DisableAll();
}

// OFFSET: 0x007AF490
// FUNCTION: fn_09152
bool AutomaticSkills::Contains(int id) const {
  return this->m_ids.contains(static_cast<std::uint16_t>(id));
}

// OFFSET: 0x007AF400
// FUNCTION: fn_09151
int AutomaticSkills::CancelOption(std::int16_t id) {
  switch (id) {
  case 2036:
    return 4;
  case 6552:
    return 24;
  case 6601:
    return 22;
  default:
    return -1;
  }
}

// OFFSET: 0x007AF2A0
// FUNCTION: fn_09150
void AutomaticSkills::DisableAll() {
  this->m_context->activeKey = 0;

  for (int id : this->m_ids) {
    const auto skill = static_cast<std::int16_t>(id);
    const int option = ActiveOption(skill);
    if (g_session.GetOption(option) != 0) {
      Cancel(this->m_context->connection, skill);
      g_session.SetOption(option, 0, 0);
    }
  }
}

// OFFSET: 0x007AF590
// FUNCTION: fn_09153
void AutomaticSkills::Use(std::int16_t id, std::int16_t level, std::uint32_t target) {
  unsigned char packet[10];
  const std::uint16_t header = 2832;
  std::memcpy(packet, &header, 2);
  std::memcpy(packet + 2, &id, 2);
  std::memcpy(packet + 4, &level, 2);
  std::memcpy(packet + 6, &target, 4);
  this->m_context->connection.Send(sizeof(packet), packet);
  const int cancel = AutomaticSkills::CancelOption(id);

  if (g_session.GetOption(cancel) != 0) {
    Cancel(this->m_context->connection, id);
    g_session.SetOption(cancel, 0, 0);
  } else {
    this->m_context->activeKey = this->m_context->currentKey;
    g_session.SetOption(ActiveOption(id), 1, 0);
  }
}

// OFFSET: 0x00CEFD20
// FUNCTION: fn_21406
void AutomaticSkills::Complete(int id) {
  if (this->Contains(id)) {
    g_session.m_options.Set(AutomaticSkills::CancelOption(static_cast<std::int16_t>(id)), 0);
  }
}
