#include "Client/Replay.h"
#include "Contents/Emblems/EmblemCache.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"
#include "Contents/GuildMgr.h"
#include "Resource/ResourceMgr.h"

// OFFSET: 0x0065E450
// FUNCTION: fn_05174
std::string CGuildMgr::GetEmblemFilename(std::uint32_t guildId) {
  const auto found = this->m_guilds.find(guildId);
  if (found == this->m_guilds.end() || found->second.m_emblemVersion <= 0) {
    return std::string();
  }

  std::string filename = reinterpret_cast<const EmblemCache*>(this)->Filename(guildId, found->second.m_emblemVersion);
  if (!ResourceMgr::Instance().Load(filename) && g_replayAnimationScaling == 0) {
    this->m_reqBundle->Add(guildId, this->GetEmblemVersion(guildId));
  }

  return filename;
}
