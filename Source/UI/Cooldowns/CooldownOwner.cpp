#include "UI/Cooldowns/CooldownOwner.h"

// OFFSET: 0x00CA64C0
// FUNCTION: fn_20960
CooldownOwner::CooldownOwner() : m_initialized(false) {
  this->m_drawing.normal.clear();
  this->m_drawing.reverse.clear();
  this->m_values.clear();
  this->m_initialized = true;
}

// OFFSET: 0x00CA60D0
// FUNCTION: fn_20956
CooldownOwner::~CooldownOwner() {
  this->m_drawing.normal.clear();
  this->m_drawing.reverse.clear();
  this->m_values.clear();
}

// OFFSET: 0x00CA61A0
// FUNCTION: fn_20957
void CooldownOwner::Register(unsigned id, std::uint32_t duration, std::uint32_t start, std::uint32_t elapsed) {
  this->m_values.erase(id);
  this->m_values.emplace(id, CooldownState(0, 0, 0, duration, start, elapsed, 1));
}

// OFFSET: 0x00CA64B0
// FUNCTION: fn_20959
void CooldownOwner::ClearReverseCommands() {
  this->m_drawing.reverse.clear();
}

CooldownState* CooldownOwner::Find(unsigned id) {
  const auto entry = this->m_values.find(id);
  return entry == this->m_values.end() ? nullptr : &entry->second;
}

void CooldownOwner::Erase(unsigned id) {
  this->m_values.erase(id);
}

// STATIC: 0x0171DD1C
// GLOBAL: gl_24487
CooldownOwner* g_cooldownOwner = nullptr;

// OFFSET: 0x00CA6610
// FUNCTION: fn_20961
void DestroyCooldownOwner() {
  if (g_cooldownOwner) {
    delete g_cooldownOwner;
    g_cooldownOwner = nullptr;
  }
}
