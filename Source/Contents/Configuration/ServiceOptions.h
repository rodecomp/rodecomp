#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H

#include "Contents/Skills/PlayerSkillBundle.h"
#include "Utility/Singleton.h"

#include <unordered_map>
#include <utility>

enum ServiceOption : int { ConfineCursor = 271 };

class CGameSettingsMgr;

bool IsServiceOption(int id);
std::pair<int, unsigned char> FindServiceOption(const char* name);
int SetOption(const char* name, unsigned char value);

struct ServiceOptions {
  /* 0000 */ std::unordered_map<std::uint32_t, unsigned char, SkillIdHash> values;

  unsigned char Get(int id) const;
  bool Set(int id, unsigned char value);
};

extern ServiceOptions g_serviceOptions;
extern float g_serviceOptionJitterScale;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ServiceOptions) == 0x20);
#endif

#endif
