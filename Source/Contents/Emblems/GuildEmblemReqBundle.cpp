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
  const auto request = std::pair{guildId, version};

  if (std::find(this->m_pending.begin(), this->m_pending.end(), request) == this->m_pending.end()) {
    this->m_pending.push_back(request);
  }
}

// OFFSET: 0x0065CB90
// FUNCTION: fn_05155
void CGuildEmblemReqBundle::Update() {
  while (!this->m_recent.empty()) {
    const Recent oldest = this->m_recent.front();

    if (timeGetTime() < oldest.tick + 60000u) {
      break;
    }

    this->m_recent.pop_front();
  }

  while (!this->m_pending.empty()) {
    const auto [guildId, version] = this->m_pending.front();
    const auto end = this->m_recent.end();
    if (std::find_if(this->m_recent.begin(), end, [guildId, version](const Recent& value) {
          return guildId == value.guildId && version == value.version;
        }) != end) {
      this->m_pending.pop_front();
      continue;
    }

    this->m_pending.pop_front();
    if (guildId != 0) {
      g_emblemDataMgr->Download(guildId, version);
      this->m_recent.emplace_back(guildId, version, timeGetTime());
    }

    return;
  }
}
