#ifndef RAGNAROK_SOURCE_UI_COOLDOWNOWNER_H
#define RAGNAROK_SOURCE_UI_COOLDOWNOWNER_H

#include "UI/CooldownDrawing.h"

#include <map>

// Singleton value, 36 bytes on x86. Global application
// creation/destruction remain the startup owner's responsibility.
class CooldownOwner {
public:
  CooldownOwner();
  ~CooldownOwner();
  void Register(unsigned id, std::uint32_t duration, std::uint32_t start, std::uint32_t elapsed);
  void ClearReverseCommands();

  // Source accessors for original direct map/list access by render callers.
  CooldownState* Find(unsigned id);
  void Erase(unsigned id);
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
