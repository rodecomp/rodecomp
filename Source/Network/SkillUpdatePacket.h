#ifndef RAGNAROK_SOURCE_NETWORK_SKILLUPDATEPACKET_H
#define RAGNAROK_SOURCE_NETWORK_SKILLUPDATEPACKET_H

#include "Contents/ClientSkills.h"

struct WindowOrdering;

// Borrowed original game-mode pending flag (+0x10C) and skill owners.
struct SkillUpdatePacket {
  int& pending;
  ClientSkills& client;
  CompanionSkills& first;
  CompanionSkills& other;
  LuaScript& script;
  const SkillUseLevels& levels;
  SkillWindows& windows;

  // Packet storage must provide the original 11-byte record. IDs/fields are
  // decoded at their observed widths; unknown opcodes still clear pending and
  // apply level-change bookkeeping, but do not publish a skill list update.
  void Apply(const void* packet);
};

struct LearnedSkillReceiver {
  ClientSkills& client;
  LuaScript& script;
  const SkillUseLevels& levels;
  SkillWindows& windows;
  int& skill336; // Set to 1 only for skill 336

  // Resolve ID name, then AP amount, record a grouped level increase, publish
  // the 336 latch, then update client skills. Does not reset the pending flag
  // used by SkillUpdatePacket; existing base unknown_000C remains unchanged.
  void Apply(
    std::uint16_t id,
    std::int16_t level,
    int unknown000C,
    std::uint8_t unknown0018,
    std::int16_t unknown0014,
    std::int16_t unknown001C,
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
  CompanionSkills& windows; // notification only: additional/shared windows

  // Six-byte header and 37-byte records; enclosing dispatcher owns extent
  // validation. Name/AP callbacks run even for IDs rejected by 10000..10019.
  void Apply(const void* packet);
};

#endif
