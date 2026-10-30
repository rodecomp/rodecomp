#include "Client/Session.h"
#include "Resource/ResourceMgr.h"
#include "Script/LuaScript.h"

#include <cstdio>

// OFFSET: 0x00E0AC40
// FUNCTION: fn_23804
const char* CSession::GetPetAccActName(int id) {
  const char* name = nullptr;
  if (this->m_hotkeyScript->Call("GetPetAccActName", "d>s", id, &name) && ResourceMgr::Instance().Exists(name)) {
    return name;
  }

  return "몬스터\\chocho_방독면.act";
}

// OFFSET: 0x00E0AE00
// FUNCTION: fn_23807
int CSession::GetPetFood(int job) {
  int food = 0;
  if (!this->m_hotkeyScript->Call("GetPetFood", "d>d", job, &food)) {
    return -1;
  }

  return food;
}

// OFFSET: 0x00E0B170
// FUNCTION: fn_23812
void CSession::GetPetIllustName(int job, int, char* buffer) {
  const char* name = nullptr;
  if (this->m_hotkeyScript->Call("GetPetIllustName_V3", "db>s", job, 1, &name)) {
    std::sprintf(buffer, name);
  } else {
    std::sprintf(buffer, "UserInterface\\illust\\PET_NOIMAGE.bmp");
  }
}

// OFFSET: 0x00E0B250
// FUNCTION: fn_23813
int CSession::GetPetJobByEggItem(int eggItemId) {
  int job = 0;
  if (!this->m_hotkeyScript->Call("GetPetJTID_by_PetEggITID", "d>d", eggItemId, &job)) {
    return -1;
  }

  return job;
}

// OFFSET: 0x00E12780
// FUNCTION: fn_23816
const char* CSession::GetPetString(int id) {
  const char* text = nullptr;
  if (!this->m_hotkeyScript->Call("GetPetString", "d>s", id, &text)) {
    return nullptr;
  }

  return text;
}
