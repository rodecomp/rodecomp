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

  CooldownState* Find(unsigned id);
  void Erase(unsigned id);
  // INLINED
  CooldownDrawing& Drawing() {
    return this->m_drawing;
  }

private:
  /* 0000 */ bool m_initialized;
  /* 0004 */ CooldownDrawing m_drawing;
  /* 001C */ std::map<unsigned, CooldownState> m_values;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CooldownOwner) == 36);
#endif

#endif
