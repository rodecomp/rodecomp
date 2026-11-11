#include "Client/Replay/Replay.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"
#include "Contents/Guild/GuildMgr.h"
#include "Resource/ResourceMgr.h"

extern template std::string::basic_string(const std::string&);

// OFFSET: 0x0065E450
// FUNCTION: fn_05174
std::string CGuildMgr::GetEmblemFilename(std::uint32_t guildId) {
  const auto found = this->m_guilds.find(guildId);
  if (found == this->m_guilds.end() || found->second.m_emblemVersion <= 0) {
    return std::string();
  }

  std::string filename = this->EmblemFilename(guildId, found->second.m_emblemVersion);
  if (!ResourceMgr::Instance().Load(filename) && g_replayAnimationScaling == 0) {
    this->m_reqBundle->Add(guildId, this->GetEmblemVersion(guildId));
  }

  return filename;
}
