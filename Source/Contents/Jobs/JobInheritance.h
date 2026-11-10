#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H

#include <cstdint>
#include <string>

int __stdcall IsPlayerJob(int job);
bool __stdcall IsVisibleJob(int job);
int __stdcall GetWeaponViewValue(int weapon);
int __stdcall GetDualWieldWeaponType(int first, int second);
bool __stdcall IsJobWeaponAllowed(int job, int slot, std::int64_t weapon);
int __stdcall GetRealWeaponId(int weapon);
std::string GetJobTitleString(int job);
bool ResetHotkeySlots();
bool IsScreenShakeActive();
bool __stdcall ValidateCharacterName(const char* name);
bool IsNavigationActive();
bool __stdcall IsJobStyleAllowed(int job, int style);
bool __stdcall IsJobHeadAllowed(int job, int head, int sex);

extern int g_screenShakeDuration;
extern char g_screenShakeMode;
extern const char g_playerNameKeyData[];
extern const char* g_playerNameKey;

#endif
