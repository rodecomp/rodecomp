#include "Client/Session.h"

// OFFSET: 0x0091BE20
// FUNCTION: fn_12610
int CSession::GetOption(int id) const {
  const auto entry = this->m_options.Values().find(id);
  return entry == this->m_options.Values().end() ? 0 : static_cast<int>(entry->second);
}

// OFFSET: 0x005E7970
// FUNCTION: fn_03937
void CSession::SetOption(int id, int value, int extra) {
  const std::uint64_t bits = static_cast<std::uint32_t>(value) | (static_cast<std::uint64_t>(extra) << 32);
  this->m_options.Values()[id] = static_cast<std::int64_t>(bits);
}
