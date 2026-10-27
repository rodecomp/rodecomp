#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H

#include "Contents/Skills/PlayerSkillBundle.h"

#include <unordered_map>

enum ServiceOption : int { ConfineCursor = 271 };

bool IsServiceOption(int id);

struct ServiceOptions {
  /* 0000 */ std::unordered_map<std::uint32_t, unsigned char, SkillIdHash> values;

  unsigned char Get(int id) const;
  bool Set(int id, unsigned char value);
};

extern ServiceOptions g_serviceOptions;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ServiceOptions) == 0x20);
#endif

#endif
