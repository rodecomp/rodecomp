#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Script/LuaScript.h"

#include <cstdint>

namespace {
  // OFFSET: 0x00DFBC00
  // FUNCTION: fn_23585
  bool IsTreeGroupTwo(int job) {
    if (static_cast<std::uint32_t>(job) - 4054u <= 58u) {
      return true;
    }

    switch (job) {
    case 4120:
    case 4127:
    case 4129:
    case 4132:
    case 4133:
    case 4140:
    case 4143:
    case 4148:
    case 4151:
    case 4173:
    case 4174:
    case 4175:
    case 4176:
    case 4177:
    case 4178:
    case 4179:
    case 4180:
    case 4182:
    case 4197:
    case 4198:
    case 4201:
    case 4202:
    case 4205:
    case 4206:
    case 4208:
    case 4209:
      return true;
    default:
      return false;
    }
  }

  // OFFSET: 0x00DFBCA0
  // FUNCTION: fn_23586
  bool IsTreeGroupThree(int job) {
    const auto value = static_cast<std::uint32_t>(job);
    return value - 4252u <= 47u || value - 4302u <= 27u || job == 4355 || job == 4360;
  }
}

// OFFSET: 0x00DFBCE0
// FUNCTION: fn_23587
bool __stdcall IsPlayerJob(int job) {
  const auto value = static_cast<std::uint32_t>(job);
  return value - 4001u <= 0x7CEu || value <= 30u;
}

// OFFSET: 0x00E19110
// FUNCTION: fn_23835
int JobInheritance::Lookup(int job) {
  const int inherited = this->script.TableInteger("JobInheritTable", 0, job);
  const int replacement = this->script.TableInteger("JOB_INHERIT_LIST2", 0, inherited);
  return replacement == -1 ? inherited : replacement;
}

// OFFSET: 0x00E25B70
// FUNCTION: fn_23907
void JobInheritance::InitializeSkillTree(CPlayerSkillBundle& bundle, int job) {
  int count = 0;
  if (!this->script.Call("GetInheritJob", "d>d", job, &count) || count == -1) {
    return;
  }

  bundle.ClearGroups();
  bundle.ClearExtra();
  bundle.ClearLevelChanges();

  for (int index = count; index > 0; --index) {
    const int inherited = this->Lookup(index);
    int group = 1;
    if (inherited == 0 || this->Lookup(index + 1) == 0 || inherited == 4218 || inherited == 4220) {
      group = 0;
    } else if (IsTreeGroupTwo(inherited) || inherited == 4239 || inherited == 4240 || inherited == 4241 || inherited == 4242) {
      group = 2;
    } else if (IsTreeGroupThree(inherited)) {
      group = 3;
    }

    switch (inherited) {
    case 4302:
    case 4303:
      group = 3;
      break;
    case 4304:
    case 4305:
    case 4306:
    case 4307:
    case 4355:
      group = 2;
      break;
    case 4308:
      group = 1;
      break;
    default:
      break;
    }

    this->script.Call("InitSkillTreeView", "dd", inherited, group);
  }
}
