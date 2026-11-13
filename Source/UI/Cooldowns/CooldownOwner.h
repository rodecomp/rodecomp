#ifndef RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNOWNER_H
#define RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNOWNER_H

#include "UI/Cooldowns/CooldownDrawing.h"

#include <map>

class CooldownOwner {
public:
  CooldownOwner();
  ~CooldownOwner();
  void Register(unsigned id, std::uint32_t duration, std::uint32_t start, std::uint32_t elapsed);
  void ClearReverseCommands();

  // INLINED
  CooldownState* Find(unsigned id) {
    const auto entry = this->m_values.find(id);
    return entry == this->m_values.end() ? nullptr : &entry->second;
  }
  void Erase(unsigned id);
  // INLINED
  void Remove(unsigned id) {
    const auto entry = this->m_values.find(id);
    if (entry != this->m_values.end()) {
      this->m_values.erase(entry);
    }
  }
  // INLINED
  CooldownDrawing& Drawing() {
    return this->m_drawing;
  }

  /* 0000 */ bool m_initialized;
  /* 0004 */ CooldownDrawing m_drawing;
  /* 001C */ std::map<unsigned, CooldownState> m_values;
};

extern CooldownOwner* g_cooldownOwner;

bool CreateCooldownOwner();
void DestroyCooldownOwner();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CooldownOwner) == 36);
#endif

#endif
