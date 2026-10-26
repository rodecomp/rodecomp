#include "Contents/GuildMgr.h"

// OFFSET: 0x0065C440
// FUNCTION: fn_05147
CGuild::CGuild() : m_emblemVersion(0) {
}

// OFFSET: 0x0065C450
// FUNCTION: fn_05148
CGuild::~CGuild() = default;

// OFFSET: 0x0065E640
// FUNCTION: fn_05175
int CGuildMgr::GetEmblemVersion(std::uint32_t guildId) const {
  const auto it = this->m_guilds.find(guildId);
  if (it != this->m_guilds.end()) {
    return it->second.m_emblemVersion;
  }
  return 0;
}

// OFFSET: 0x0065EEA0
// FUNCTION: fn_05181
void CGuildMgr::SetEmblemVersion(std::uint32_t guildId, int version) {
  this->m_guilds[guildId].m_emblemVersion = version;
}
