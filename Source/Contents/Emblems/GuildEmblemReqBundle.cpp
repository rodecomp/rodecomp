#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"

#include <algorithm>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x0065C5F0
// FUNCTION: fn_05150
CGuildEmblemReqBundle::CGuildEmblemReqBundle() = default;

// INLINED
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
    const auto request = this->m_pending.front();
    const bool recent = std::any_of(this->m_recent.begin(), this->m_recent.end(), [&](const Recent& value) {
      return value.guildId == request.first && value.version == request.second;
    });
    this->m_pending.pop_front();

    if (recent) {
      continue;
    }

    if (request.first != 0) {
      g_emblemDataMgr->Download(request.first, request.second);
      this->m_recent.push_back({request.first, request.second, timeGetTime()});
    }

    return;
  }
}
