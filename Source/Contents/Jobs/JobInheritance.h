#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H

#include <cstdint>
#include <string>

enum ShieldViewId : int {
  SHIELD_NONE = 0,
  SHIELD_GUARD = 1,
  SHIELD_BUCKLER = 2,
  SHIELD_SHIELD = 3,
  SHIELD_MIRRORSHIELD = 4,
};

bool IsNavigationActive();
bool __stdcall IsJobStyleAllowed(int job, int style);
bool __stdcall IsJobHeadAllowed(int job, int head, int sex);

extern int g_screenShakeDuration;
extern char g_screenShakeMode;
extern const char g_playerNameKeyData[];
extern const char* g_playerNameKey;

#endif
