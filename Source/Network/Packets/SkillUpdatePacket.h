#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_SKILLUPDATEPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_SKILLUPDATEPACKET_H

#include "Contents/Skills/ClientSkills.h"

struct WindowOrdering;

struct SkillUpdatePacket {
  int& pending;
  ClientSkills& client;
  CompanionSkills& first;
  CompanionSkills& other;
  LuaScript& script;
  const SkillUseLevels& levels;
  SkillWindows& windows;

  void Apply(const void* packet);
};

struct LearnedSkillReceiver {
  ClientSkills& client;
  LuaScript& script;
  const SkillUseLevels& levels;
  SkillWindows& windows;
  int& skill336;

  void Apply(
    std::uint16_t id, std::int16_t level, int unknown000C, std::uint8_t unknown0018, std::int16_t unknown0014, std::int16_t unknown001C,
    std::int16_t unknown0030
  );
};

struct AdditionalSkillPacket {
  int& pending;
  ClientSkills& client;
  LuaScript& script;
  int& capacity;
  WindowOrdering& ordering;
  int& frameLatch;
  CompanionSkills& windows;

  void Apply(const void* packet);
};

#endif
