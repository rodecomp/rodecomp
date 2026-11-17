#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H

#include <cstdint>
#include <string>

bool IsNavigationActive();
bool __stdcall IsJobStyleAllowed(int job, int style);
bool __stdcall IsJobHeadAllowed(int job, int head, int sex);

extern int g_screenShakeDuration;
extern char g_screenShakeMode;
extern const char g_playerNameKeyData[];
extern const char* g_playerNameKey;

#endif
