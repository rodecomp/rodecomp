#include "Contents/Skills/SkillInfo.h"

#include <functional>

struct EfstActionRegistrar {
  EfstActionRegistrar(int efstId, std::function<CEfstAction*()> factory);
};

struct SkillActionRegistrar {
  SkillActionRegistrar(int skillId, std::function<CSkillAction*()> factory);
};

// OFFSET: 0x005AFB60
// FUNCTION: fn_03336
EfstActionRegistrar::EfstActionRegistrar(int efstId, std::function<CEfstAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterEfstAction(efstId, factory);
}

// OFFSET: 0x005AFC10
// FUNCTION: fn_03337
SkillActionRegistrar::SkillActionRegistrar(int skillId, std::function<CSkillAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillAction(skillId, factory);
}
