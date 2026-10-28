#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H

class LuaScript;
class CPlayerSkillBundle;

struct JobInheritance {
  LuaScript& script;

  int Lookup(int job);

  void InitializeSkillTree(CPlayerSkillBundle& bundle, int job);
};

#include <cstdint>
#include <string>

bool __stdcall IsVisibleJob(int job);
int __stdcall GetWeaponViewValue(int weapon);
int __stdcall GetDualWieldWeaponType(int first, int second);
bool __stdcall IsJobWeaponAllowed(int job, int slot, std::int64_t weapon);
int __stdcall GetRealWeaponId(int weapon);
std::string GetJobTitleString(int job);
bool ResetHotkeySlots();

#endif
