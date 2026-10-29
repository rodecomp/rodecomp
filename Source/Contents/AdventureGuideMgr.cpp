#include "Contents/AdventureGuideMgr.h"

#include "Client/Session.h"

#include <algorithm>
#include <iterator>
#include <optional>

// GLOBAL: gl_24502
CAdventureGuideMgr* g_adventureGuideMgr;

using AdventureGuideEntryTable = std::map<int, std::map<int, AdventureGuideEntry>>;
using AdventureGuideContentsTable = std::map<int, std::map<int, AdventureGuideContents>>;

// OFFSET: 0x0065F680
// FUNCTION: fn_05189
static std::optional<AdventureGuideEntryTable::mapped_type*> FindMapped(AdventureGuideEntryTable& map, const int& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return std::nullopt;
  }
  return &it->second;
}

// OFFSET: 0x0065F6D0
// FUNCTION: fn_05190
static std::optional<AdventureGuideContentsTable::mapped_type*> FindMapped(AdventureGuideContentsTable& map, const int& key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return std::nullopt;
  }
  return &it->second;
}

// OFFSET: 0x00660F70
// FUNCTION: fn_05222
CAdventureGuideMgr::CAdventureGuideMgr() : m_initialized(true) {
}

// OFFSET: 0x006629D0
// FUNCTION: fn_05227
CAdventureGuideMgr::~CAdventureGuideMgr() {
}

// OFFSET: 0x00663680
// FUNCTION: fn_05235
void CAdventureGuideMgr::Destroy() {
  if (g_adventureGuideMgr) {
    delete g_adventureGuideMgr;
    g_adventureGuideMgr = nullptr;
  }
}

// OFFSET: 0x00663890
// FUNCTION: fn_05237
void CAdventureGuideMgr::GetEpisodeList(std::list<AdventureGuideEpisode>& out) {
  out.clear();

  for (const AdventureGuideEpisode& episode : m_episodes) {
    if (episode.order <= g_session.m_level && g_session.m_level <= episode.unknown_001C) {
      out.push_back(episode);
    }
  }
}

// OFFSET: 0x006639D0
// FUNCTION: fn_05238
void CAdventureGuideMgr::GetContentsList(std::list<std::pair<int, AdventureGuideContents>>& out, int type, int id) {
  out.clear();

  std::map<int, std::map<int, AdventureGuideContents>>* table;
  switch (type) {
  case 1:
    table = &unknown_003C;
    break;
  case 3:
    table = &unknown_0044;
    break;
  case 4:
    table = &unknown_004C;
    break;
  default:
    return;
  }

  std::optional<std::map<int, AdventureGuideContents>*> contents = FindMapped(*table, id);
  if (!contents) {
    return;
  }

  std::copy_if((*contents)->begin(), (*contents)->end(), std::back_inserter(out),
               [](const std::pair<const int, AdventureGuideContents>& entry) {
                 return entry.second.unknown_0004 <= g_session.m_level && g_session.m_level <= entry.second.unknown_0008;
               });
  out.sort([](const std::pair<int, AdventureGuideContents>& lhs, const std::pair<int, AdventureGuideContents>& rhs) {
    return lhs.second.unknown_0004 < rhs.second.unknown_0004;
  });
}

// OFFSET: 0x006636A0
// FUNCTION: fn_05236
void CAdventureGuideMgr::GetEntryList(std::list<std::pair<int, AdventureGuideEntry>>& out, int type, int id) {
  out.clear();

  std::map<int, std::map<int, AdventureGuideEntry>>* table;
  switch (type) {
  case 5:
    table = &unknown_0054;
    break;
  case 6:
    table = &unknown_005C;
    break;
  default:
    return;
  }

  std::optional<std::map<int, AdventureGuideEntry>*> entries = FindMapped(*table, id);
  if (!entries) {
    return;
  }

  std::copy((*entries)->begin(), (*entries)->end(), std::back_inserter(out));
  out.sort([](const std::pair<int, AdventureGuideEntry>& lhs, const std::pair<int, AdventureGuideEntry>& rhs) {
    return lhs.second.unknown_0004 < rhs.second.unknown_0004;
  });
}
