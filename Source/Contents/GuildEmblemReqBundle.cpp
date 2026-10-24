#include "Contents/EmblemDataMgr.h"
#include "Contents/GuildEmblemReqBundle.h"

#include <algorithm>
#include <windows.h>
// clang-format off
#include <mmsystem.h>
// clang-format on

// OFFSET: 0x0065C5F0
CGuildEmblemReqBundle::CGuildEmblemReqBundle() = default;

// OFFSET: 0x0065C8C0
// FUNCTION: fn_05153
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
void CGuildEmblemReqBundle::Update(
  CEmblemDataMgr& emblems,
  std::uint32_t accountId,
  const std::string& accountName,
  const char* token16,
  CConnection& connection,
  CAsyncWorkMgr& manager
) {
  while (!this->m_recent.empty()) {
    if (timeGetTime() < this->m_recent.front().tick + 60000u) {
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
      emblems.Download(request.first, request.second, accountId, accountName, token16, connection, manager);
      this->m_recent.push_back({request.first, request.second, timeGetTime()});
    }

    // At most one non-recent pending entry is consumed per update, including 0.
    return;
  }
}
