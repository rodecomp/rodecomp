#include "Contents/Party/PartyBookingMgr.h"

#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/Sound/SoundMgr.h"
#include "UI/Window/UISeekPartyListWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdio>
#include <cstring>
#include <ctime>

// OFFSET: 0x00B15C80
// FUNCTION: fn_18432
PartyBookingMgr::PartyBookingMgr() : unknown_0000(0) {
  this->m_lastPage = true;
  this->m_lastIndex = -1;
  this->m_regionNames.clear();
  this->m_maps.clear();
  this->m_entries.clear();
  this->m_pending.clear();
  this->m_filterLevel = -1;
  this->m_filterMap = -1;
  this->m_filterJob = -1;
  this->m_alarm = false;
  this->m_filterEnabled = false;
}

// OFFSET: 0x00B16810
// FUNCTION: fn_18442
bool PartyBookingMgr::FlushPending() {
  if (this->m_pending.size() == 0) {
    return false;
  }

  int count = 0;
  for (auto it = this->m_pending.begin(); it != this->m_pending.end(); ++it, ++count) {
    this->m_entries.insert(std::pair<const int, PartyBookingEntry>(it->first, it->second));

    if (this->m_pending.size() == 1) {
      this->m_pending.clear();
      return false;
    }

    this->m_pending.erase(it);

    if (count >= 10) {
      break;
    }
  }

  return true;
}

// OFFSET: 0x00B16A80
// FUNCTION: fn_18443
void PartyBookingMgr::Clear() {
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B16AD0
// FUNCTION: fn_18444
void PartyBookingMgr::Remove(int index) {
  auto entry = this->m_entries.find(index);
  if (entry != this->m_entries.end()) {
    this->m_entries.erase(entry);
  }

  auto pending = this->m_pending.find(index);
  if (pending != this->m_pending.end()) {
    this->m_pending.erase(pending);
  }
}

// OFFSET: 0x00B16CD0
// FUNCTION: fn_18446
void PartyBookingMgr::EraseEntry(int index) {
  this->m_entries.erase(index);
}

// OFFSET: 0x00B16CF0
// FUNCTION: fn_18447
std::pair<bool, PartyBookingEntry> PartyBookingMgr::MakeEntry(PARTY_BOOKING_AD_INFO info) {
  char buffer[255];
  std::memset(buffer, 0, sizeof(buffer));

  int region = info.detail.mapId >> 8;
  int mapIndex = static_cast<unsigned char>(info.detail.mapId);

  PartyBookingEntry entry;
  std::string regionName;

  if (region != 0) {
    regionName = this->m_regionNames.at(region - 1);
  } else {
    regionName = this->m_regionNames.at(0);
  }

  ++region;
  auto found = this->m_maps.find(region);
  if (found == this->m_maps.end()) {
    return std::pair<bool, PartyBookingEntry>(false, entry);
  }

  std::vector<PartyBookingMapInfo> maps = found->second;
  if (maps.size() < static_cast<unsigned int>(mapIndex)) {
    return std::pair<bool, PartyBookingEntry>(false, entry);
  }

  PartyBookingMapInfo map = maps.at(mapIndex);

  tm* expire = _localtime32(&info.expireTime);
  sprintf(buffer, " %02d : %02d ", expire->tm_hour, expire->tm_min);
  entry.time = buffer;
  entry.leaderName = entry.charName = info.charName;

  int maxLevel = info.detail.level + 15;
  if (maxLevel > g_session.m_maxLevel) {
    maxLevel = g_session.m_maxLevel;
  }
  sprintf(buffer, "%02d~%02d", info.detail.level, maxLevel);
  entry.level = buffer;

  entry.location = regionName + " " + map.name;

  std::memset(buffer, 0, sizeof(buffer));
  for (int i = 0; i < 6; ++i) {
    short job = info.detail.job[i];
    if (static_cast<unsigned short>(job) < 5) {
      std::strcat(buffer, LookupMessage(job + 0x6AF));
      std::strcat(buffer, ",");
    } else if (job >= 0) {
      std::strcat(buffer, LookupMessage(job + 0x658));
      std::strcat(buffer, ",");
    }
  }

  if (std::strlen(buffer) > 2) {
    buffer[std::strlen(buffer) - 1] = '\0';
  }

  entry.jobs = buffer;
  entry.isEnd = false;
  return std::pair<bool, PartyBookingEntry>(true, entry);
}

// OFFSET: 0x00B175B0
// FUNCTION: fn_18448
void PartyBookingMgr::OnAdvertisement(PARTY_BOOKING_AD_INFO info) {
  UISeekPartyListWnd* window = static_cast<UISeekPartyListWnd*>(g_windowMgr.LookupWindow(0xA9));
  if (window) {
    if (window->MatchesFilter(info)) {
      if (!this->m_lastPage) {
        this->AddEntries(&info, 1, false);
        window->NotifyNewAd();
      } else {
        std::pair<bool, PartyBookingEntry> result = this->MakeEntry(info);
        if (result.first) {
          this->m_pending.insert(std::pair<const int, PartyBookingEntry>(info.index, result.second));
        }
        window->NotifyNewAd();
      }
    }
    return;
  }

  if (this->m_filterEnabled) {
    PARTY_BOOKING_AD_INFO ad = info;
    if (this->m_filterLevel > 0 && (ad.detail.level < this->m_filterLevel || this->m_filterLevel + 15 < ad.detail.level)) {
      return;
    }

    if (this->m_filterMap > 0 && this->m_filterMap != ad.detail.mapId) {
      return;
    }

    if (this->m_filterJob > 0) {
      int i = 0;
      while (this->m_filterJob != ad.detail.job[i]) {
        if (++i >= 6) {
          return;
        }
      }
    }
  }

  if (this->m_alarm) {
    g_soundMgr->PlayWave("party_alarm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6D9)), 0xFFFF00, 15, 0, 0);
  }
}

// OFFSET: 0x00B17820
// FUNCTION: fn_18449
void PartyBookingMgr::AddEntries(const PARTY_BOOKING_AD_INFO* infos, int count, bool more) {
  this->m_lastPage = more;

  for (int i = 0; i < count; ++i) {
    std::pair<bool, PartyBookingEntry> result = this->MakeEntry(infos[i]);
    if (result.first) {
      this->m_entries.insert(std::pair<const int, PartyBookingEntry>(infos[i].index, result.second));
      this->m_lastIndex = infos[i].index;
      if (!more) {
        more = this->FlushPending();
      }
    }
  }

  if (more == true) {
    PartyBookingEntry last;
    last.isEnd = true;
    this->m_entries.insert(std::pair<const int, PartyBookingEntry>(this->m_lastIndex + 1, last));
  }
}

// OFFSET: 0x00B17C40
// FUNCTION: fn_18450
void PartyBookingMgr::Reset() {
  this->m_lastPage = true;
  this->m_lastIndex = -1;
  this->unknown_0000 = 0;
  this->m_entries.clear();
  this->m_pending.clear();
}

// OFFSET: 0x00B18210
// FUNCTION: fn_18451
void PartyBookingMgr::SetFilter(int level, int mapId, int job) {
  this->m_filterLevel = level;
  this->m_filterMap = mapId;
  this->m_filterJob = job;
}

// OFFSET: 0x00B18230
// FUNCTION: fn_18452
void PartyBookingMgr::SetJobs(int index, const short* jobs) {
  auto found = this->m_entries.find(index);
  if (found == this->m_entries.end()) {
    found = this->m_pending.find(index);
    if (found == this->m_pending.end()) {
      return;
    }
  }

  char buffer[255];
  std::memset(buffer, 0, sizeof(buffer));
  for (int i = 0; i < 6; ++i) {
    short job = jobs[i];
    if (static_cast<unsigned short>(job) < 5) {
      std::strcat(buffer, LookupMessage(job + 0x6AF));
      std::strcat(buffer, ",");
    } else if (job >= 0) {
      std::strcat(buffer, LookupMessage(job + 0x658));
      std::strcat(buffer, ",");
    }
  }

  if (std::strlen(buffer) > 2) {
    buffer[std::strlen(buffer) - 1] = '\0';
  }

  found->second.jobs = buffer;
}
