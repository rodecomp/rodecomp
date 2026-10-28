#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Network/Connection/Connection.h"

#include <algorithm>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// INLINED
CPlayerNameDictionary::CPlayerNameDictionary(PlayerNameContext& context) : m_enabled(true), m_context(&context) {
}

// INLINED
CPlayerNameDictionary::CPlayerNameDictionary() : m_enabled(true), m_context(nullptr) {
}

// OFFSET: 0x0060A9D0
// OFFSET: 0x0060AAE0
// FUNCTION: fn_04256
CPlayerNameDictionary::~CPlayerNameDictionary() = default;

// OFFSET: 0x0060AFB0
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

  return LookupMessage(581);
}

// OFFSET: 0x0060ABB0
// FUNCTION: fn_04258
void CPlayerNameDictionary::Set(std::uint32_t id, std::string name) {
  this->m_names.insert_or_assign(id, Entry{std::move(name), timeGetTime()});
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
  return this->m_names.contains(id);
}

// OFFSET: 0x0060B470
// FUNCTION: fn_04265
void CPlayerNameDictionary::Update() {
  while (!this->m_requests.empty()) {
    if (timeGetTime() < this->m_requests.front().tick + 10000u) {
      break;
    }

    this->m_requests.pop_front();
  }

  while (!this->m_pending.empty()) {
    const std::uint32_t id = this->m_pending.front();
    const auto cached = this->m_names.find(id);
    const bool fresh = cached != this->m_names.end() && !(cached->second.tick + 600000u < timeGetTime());
    const auto outstanding = std::find_if(this->m_requests.begin(), this->m_requests.end(), [&](const Request& request) { return request.id == id; });
    this->m_pending.pop_front();

    if (fresh || outstanding != this->m_requests.end()) {
      continue;
    }

    if (id != 0) {
      unsigned char packet[6];
      const std::uint16_t header = 873;
      std::memcpy(packet, &header, sizeof(header));
      std::memcpy(packet + 2, &id, sizeof(id));
      this->m_context->connection.Send(sizeof(packet), packet);
      this->m_requests.push_back({id, timeGetTime()});
    }

    return;
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
