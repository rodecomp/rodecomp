#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

#pragma pack(push, 1)
struct PACKET_CZ_REQNAME_BYGID2 {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ std::uint32_t gid;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PACKET_CZ_REQNAME_BYGID2) == 6);
#endif

// INLINED
CPlayerNameDictionary::CPlayerNameDictionary() {
  this->m_enabled = true;
}

// OFFSET: 0x0060A9D0
// FUNCTION: fn_04256
CPlayerNameDictionary::~CPlayerNameDictionary() {
}

// OFFSET: 0x0060B010
// FUNCTION: fn_04262
std::string CPlayerNameDictionary::Lookup(std::uint32_t id) {
  const auto entry = this->m_names.find(id);
  if (entry != this->m_names.end()) {
    return entry->second.name;
  }

  if (std::find(this->m_pending.begin(), this->m_pending.end(), id) == this->m_pending.end()) {
    this->m_pending.push_back(id);
  }

  return LookupMessage(MSI_NAMELESS_USER);
}

// OFFSET: 0x0060ABB0
// FUNCTION: fn_04258
void CPlayerNameDictionary::Set(std::uint32_t id, std::string name) {
  this->m_names[id] = Entry{name, timeGetTime()};
  const auto request = std::find_if(this->m_requests.begin(), this->m_requests.end(), [&](const Request& value) { return value.id == id; });
  if (request != this->m_requests.end()) {
    this->m_requests.erase(request);
  }
}

// OFFSET: 0x0060B2E0
// FUNCTION: fn_04263
void CPlayerNameDictionary::Clear() {
  this->m_names.clear();
  this->m_pending.clear();
  this->m_requests.clear();
}

// OFFSET: 0x0060B420
// FUNCTION: fn_04264
bool CPlayerNameDictionary::Contains(std::uint32_t id) const {
  return this->m_names.find(id) != this->m_names.end();
}

// OFFSET: 0x0060B470
// FUNCTION: fn_04265
void CPlayerNameDictionary::Update() {
  while (!this->m_requests.empty()) {
    const Request request = this->m_requests.front();
    if (timeGetTime() < request.tick + 10000u) {
      break;
    }

    this->m_requests.pop_front();
  }

  while (!this->m_pending.empty()) {
    const std::uint32_t id = this->m_pending.front();
    const auto cached = this->m_names.find(id);
    if (cached == this->m_names.end() || cached->second.IsExpired()) {
      const auto outstanding = std::find_if(this->m_requests.begin(), this->m_requests.end(), [&](const Request& request) { return request.id == id; });
      if (outstanding == this->m_requests.end()) {
        this->m_pending.pop_front();

        if (id != 0) {
          PACKET_CZ_REQNAME_BYGID2 packet;
          packet.packetType = ID_CZ_REQNAME_BYGID2;
          packet.gid = id;
          GetRagConnection().Send(sizeof(packet), &packet);
          this->m_requests.push_back({id, timeGetTime()});
        }

        return;
      }
    }

    this->m_pending.pop_front();
  }
}

// GLOBAL: gl_24677
CPlayerNameDictionary* g_playerNameDictionary = nullptr;

// OFFSET: 0x0060AE30
// FUNCTION: fn_04259
bool CreatePlayerNameDictionary() {
  if (g_playerNameDictionary) {
    return false;
  }

  g_playerNameDictionary = new (std::nothrow) CPlayerNameDictionary();
  if (!g_playerNameDictionary) {
    return false;
  }

  if (!g_playerNameDictionary->IsEnabled()) {
    delete g_playerNameDictionary;
    g_playerNameDictionary = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x0060AF90
// FUNCTION: fn_04260
void DestroyPlayerNameDictionary() {
  if (g_playerNameDictionary) {
    delete g_playerNameDictionary;
    g_playerNameDictionary = nullptr;
  }
}

// OFFSET: 0x0060AFB0
// FUNCTION: fn_04261
std::string LookupPlayerName(std::uint32_t id) {
  return g_playerNameDictionary->Lookup(id);
}

// OFFSET: 0x0060B650
// FUNCTION: fn_04266
__declspec(noinline) bool CPlayerNameDictionary::Entry::IsExpired() const {
  return this->tick + 600000u < timeGetTime();
}
