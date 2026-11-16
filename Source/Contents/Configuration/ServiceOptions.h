#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEOPTIONS_H

#include "Contents/Skills/PlayerSkillBundle.h"
#include "Utility/Singleton.h"

#include <unordered_map>
#include <utility>
#include <windows.h>

enum ServiceOption : int { ConfineCursor = 271 };

class CGameSettingsMgr;

bool IsServiceOption(int id);
std::pair<int, bool> FindServiceOption(const char* name);
int SetOption(const char* name, bool value);

struct ServiceOptions {
  /* 0000 */ std::unordered_map<int, bool> values;

  static bool Get(int id);
  static bool Set(int id, bool value);
};

extern ServiceOptions g_serviceOptions;
extern float g_serviceOptionJitterScale;
extern const GUID g_defaultDrawGuidDX7;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ServiceOptions) == 0x20);
#endif

#endif
