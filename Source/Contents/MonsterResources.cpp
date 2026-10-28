#include "Client/Session.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"

#include <string>

// OFFSET: 0x00E09AB0
// FUNCTION: fn_23788
std::string CSession::GetMonsterActName(int job) {
  std::string name = FormatOwnedString("몬스터\\%s.act", this->m_jobNamesBegin[job]);
  if (!ResourceMgr::Instance().Exists(name.c_str())) {
    name = "";
  }
  return name;
}

// OFFSET: 0x00E09DC0
// FUNCTION: fn_23790
std::string CSession::GetMonsterSprName(int job) {
  std::string name = FormatOwnedString("몬스터\\%s.spr", this->m_jobNamesBegin[job]);
  if (!ResourceMgr::Instance().Exists(name.c_str())) {
    name = "";
  }
  return name;
}
