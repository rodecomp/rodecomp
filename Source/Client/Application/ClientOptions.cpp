#include "Client/Session/Session.h"

// OFFSET: 0x0091BE20
// FUNCTION: fn_12610
int CSession::GetOption(int id) const {
  const auto entry = this->m_options.Values().find(static_cast<int>(id));
  return static_cast<int>(entry == this->m_options.Values().end() ? 0 : entry->second);
}

// OFFSET: 0x005E7970
// FUNCTION: fn_03937
void CSession::SetOption(int id, int value, int extra) {
  this->m_options.Values()[static_cast<int>(id)] = (static_cast<std::int64_t>(extra) << 32) | static_cast<std::uint32_t>(value);
}
