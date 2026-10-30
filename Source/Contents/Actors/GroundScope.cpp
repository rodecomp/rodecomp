#include "Contents/Actors/GroundScope.h"

#include "Contents/Skills/SkillInfo.h"

#include <windows.h>
#include <mmsystem.h>

// OFFSET: 0x00E4C2F0
// FUNCTION: fn_24284
bool CGroundScope::OnProcess() {
  if (this->m_lastingTime < timeGetTime()) {
    return false;
  }

  return true;
}

// OFFSET: 0x00E4AE10
// FUNCTION: fn_24267
int CGroundScope::SkillScopeType(int skillId) {
  switch (skillId) {
    case 0x0B:
    case 0x19:
    case 0x2F:
    case 0x45:
    case 0x50:
    case 0x56:
    case 0x58:
    case 0x6E:
    case 0x81:
    case 0x82:
    case 0x89:
    case 0x8C:
    case 0x17D:
    case 0x1E3:
    case 0x8D5:
      return 3;
    case 0x11:
    case 0x15:
    case 0x46:
    case 0x5B:
    case 0x5C:
    case 0x1E4:
    case 0x302:
    case 0x7D5:
    case 0x7F0:
    case 0x7FC:
      return 5;
    case 0x4F:
    case 0x1DE:
    case 0x2DD:
    case 0x2E3:
    case 0x8BD:
    case 0x992:
    case 0x994:
    case 0x995:
    case 0x996:
    case 0x138E:
    case 0x1395:
    case 0x13A4:
    case 0x13B2:
    case 0x146B:
    case 0x146D:
    case 0x146E:
    case 0x149F:
      return 7;
    case 0x53:
    case 0x59:
    case 0x2D3:
    case 0x2DB:
    case 0x301:
    case 0x7D8:
    case 0x8D4:
    case 0x98E:
    case 0x138C:
    case 0x1462:
    case 0x1466:
    case 0x14C7:
    case 0x14F9:
    case 0x14FA:
    case 0x14FB:
    case 0x14FC:
    case 0x14FD:
      return 9;
    case 0x55:
    case 0x1461:
      return 11;
    case 0x2C4:
    case 0x8A5:
      return 18;
    case 0x149E:
      return 19;
    case 0x2E0:
    case 0x98F:
    case 0x991:
      return -2;
    default:
      return CSkillActionMgr::GetInstance()->ActionSlot14(skillId);
  }
}
