#include "Client/Replay/Replay.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"
#include "Contents/Guild/GuildMgr.h"
#include "Resource/ResourceMgr.h"

// OFFSET: 0x0065E450
// FUNCTION: fn_05174
std::string CGuildMgr::GetEmblemFilename(std::uint32_t guildId) {
  const auto found = this->m_guilds.find(static_cast<std::uint32_t>(guildId));
  if (this->m_guilds.end() == found || found->second.m_emblemVersion <= 0) {
    return "";
  }

  std::string filename = this->EmblemFilename(guildId, found->second.m_emblemVersion);
  if (!ResourceMgr::Instance().Load(filename) && g_replayAnimationScaling == 0) {
    this->m_reqBundle->Add(guildId, this->GetEmblemVersion(guildId));
  }

  return filename;
}

// OFFSET: 0x0065E640
// FUNCTION: fn_05175
int CGuildMgr::GetEmblemVersion(std::uint32_t guildId) const {
  const auto it = this->m_guilds.find(guildId);
  if (this->m_guilds.end() == it) {
    return 0;
  }
  return it->second.m_emblemVersion;
}

// OFFSET: 0x0065EEA0
// FUNCTION: fn_05181
void CGuildMgr::SetEmblemVersion(std::uint32_t guildId, int version) {
  this->m_guilds[guildId].m_emblemVersion = version;
}
