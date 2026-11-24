#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"

#include <algorithm>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x0065C5F0
// FUNCTION: fn_05150
CGuildEmblemReqBundle::CGuildEmblemReqBundle() = default;

// OFFSET: 0x0065C860
// FUNCTION: fn_46444
CGuildEmblemReqBundle::~CGuildEmblemReqBundle() = default;

// OFFSET: 0x0065CA90
// FUNCTION: fn_05154
void CGuildEmblemReqBundle::Add(std::uint32_t guildId, std::uint32_t version) {
  if (this->m_pending.end() == std::find(this->m_pending.begin(), this->m_pending.end(), std::make_pair(guildId, version))) {
    this->m_pending.push_back(std::make_pair(guildId, version));
  }
}

// OFFSET: 0x0065C940
// FUNCTION: fn_46687
void CGuildEmblemReqBundle::ExpireRecentRequests() {
  while (!this->m_recent.empty()) {
    const Recent oldest = this->m_recent.front();

    if (timeGetTime() < oldest.tick + 60000u) {
      break;
    }

    this->m_recent.pop_front();
  }
}

// OFFSET: 0x0065C9A0
// FUNCTION: fn_46688
std::pair<std::uint32_t, std::uint32_t> CGuildEmblemReqBundle::TakeNextRequest() {
  while (!this->m_pending.empty()) {
    const auto [guildId, version] = this->m_pending.front();
    const auto end = this->m_recent.end();
    if (end != std::find_if(this->m_recent.begin(), end, [guildId, version](const Recent& value) {
          return guildId == value.guildId && version == value.version;
        })) {
      this->m_pending.pop_front();
      continue;
    }

    this->m_pending.pop_front();
    return {guildId, version};
  }
  return {};
}

// OFFSET: 0x0065CB90
// FUNCTION: fn_05155
void CGuildEmblemReqBundle::Update() {
  ExpireRecentRequests();
  const auto [guildId, version] = TakeNextRequest();
  if (guildId != 0) {
    g_emblemDataMgr->Download(guildId, version);
    this->m_recent.push_back(Recent(guildId, version, timeGetTime()));
  }
}
