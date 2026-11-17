#ifndef RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNOWNER_H
#define RAGNAROK_SOURCE_UI_COOLDOWNS_COOLDOWNOWNER_H

#include "UI/Cooldowns/CooldownState.h"

#include <cstddef>
#include <map>
#include <vector>

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
  void Draw(
    int centerX, int centerY, int radiusX, int radiusY, CooldownState& value, int limit, int offsetX, int offsetY
  );
  void SubmitNormal();
  void SubmitReverse();

  /* 0000 */ bool m_initialized;
  /* 0004 */ std::vector<RenderCommand*> m_normal;
  /* 0010 */ std::vector<RenderCommand*> m_reverse;
  /* 001C */ std::map<unsigned, CooldownState> m_values;

private:
  __declspec(noinline) void DrawReverse(CooldownState& value);
};

extern CooldownOwner* g_cooldownOwner;

bool CreateCooldownOwner();
void DestroyCooldownOwner();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CooldownOwner) == 36);
static_assert(offsetof(CooldownOwner, m_normal) == 0x04);
static_assert(offsetof(CooldownOwner, m_reverse) == 0x10);
#endif

#endif
