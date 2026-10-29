#include "Contents/Items/RuneSystem.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Session.h"
#include "Contents/Items/ItemInfo.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lua.h>
}

#include <format>
#include <new>

// GLOBAL: gl_24845
CRuneSystemMgr* g_runeSystemMgr;

namespace {
  struct RuneTagPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
  };

  struct RuneItemPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
    /* 0004 */ int runeId;
  };

  struct RuneSelectPacket {
    /* 0000 */ std::uint16_t id;
    /* 0002 */ std::uint16_t tagId;
    /* 0004 */ int runeId;
    /* 0008 */ std::uint8_t value;
  };

  struct RuneIdPacket {
    /* 0000 */ std::uint16_t id;
    /* 0004 */ int runeId;
  };

  struct RuneHeaderPacket {
    /* 0000 */ std::uint16_t id;
  };
}

// OFFSET: 0x00764C30

// OFFSET: 0x00764DE0

// OFFSET: 0x007657D0

// OFFSET: 0x007659C0

// OFFSET: 0x00765B10

// OFFSET: 0x007660C0

// OFFSET: 0x00766920

// OFFSET: 0x00766970

// OFFSET: 0x00766A10

// OFFSET: 0x00766A30

// OFFSET: 0x00766A70
// FUNCTION: fn_08117
int CRuneSystemMgr::GetTagCount() const {
  return static_cast<int>(this->m_tags.size());
}

// OFFSET: 0x00766A80
// FUNCTION: fn_08118
int CRuneSystemMgr::GetOwnedDecomItemCount() const {
  return static_cast<int>(this->m_ownedDecomItems.size());
}

// OFFSET: 0x00766B10
// FUNCTION: fn_08119
bool CRuneSystemMgr::IsInitialized() const {
  return this->m_initialized;
}

// OFFSET: 0x00766B20

// OFFSET: 0x00766B60

// OFFSET: 0x00766B90
// FUNCTION: fn_08122
int RuneSetData::CountSlots() const {
  int count = 0;
  for (RuneData* rune : this->m_slots) {
    if (rune) {
      ++count;
    }
  }

  return count;
}

// OFFSET: 0x00766C70

// OFFSET: 0x00766CA0
// FUNCTION: fn_08124
const char* RuneData::GetName() const {
  return this->m_name.c_str();
}

// OFFSET: 0x00766CB0

// OFFSET: 0x00766CE0

// OFFSET: 0x00766DB0
// FUNCTION: fn_08127
RuneSetData* CRuneSystemMgr::GetCurRuneSet() const {
  return this->m_curRuneSet;
}

// OFFSET: 0x00766DC0
// FUNCTION: fn_08128
int CRuneSystemMgr::GetCurSlotCount() const {
  return this->m_curSlotCount;
}

// OFFSET: 0x00766DD0

// OFFSET: 0x00766E40
// FUNCTION: fn_08130
std::int64_t RuneData::GetId() const {
  return this->m_id;
}

// OFFSET: 0x00766E50

// OFFSET: 0x00766E60

// OFFSET: 0x00766EB0

// OFFSET: 0x00766F00

// OFFSET: 0x00766F60

// OFFSET: 0x00766F80

// OFFSET: 0x00767070

// OFFSET: 0x007670C0

// OFFSET: 0x00767130
// FUNCTION: fn_08139
int CRuneSystemMgr::GetSelectTag() const {
  return this->m_selectTag;
}

// OFFSET: 0x00767140
// FUNCTION: fn_08140
int RuneData::GetTagId() const {
  return this->m_tagId;
}

// OFFSET: 0x00767150
// FUNCTION: fn_08141
int RuneData::GetActiveType() const {
  return this->m_activeType;
}

// OFFSET: 0x00767160
// FUNCTION: fn_08142
int RuneSetData::GetGradeValue() const {
  if (this->m_grade < 15) {
    return this->m_gradeList[this->m_grade];
  }

  return 0;
}

// OFFSET: 0x00767180

// OFFSET: 0x00767230
// FUNCTION: fn_08144
bool CRuneSystemMgr::IsRequestPending() const {
  return this->m_requestPending;
}

// OFFSET: 0x00767240
// FUNCTION: fn_08145
const char* RuneData::GetResName() const {
  return this->m_resName.c_str();
}

// OFFSET: 0x00767D20

// OFFSET: 0x00767E60
// FUNCTION: fn_08148
void CRuneSystemMgr::ClearCurRuneSet() {
  this->m_curRuneSet = nullptr;
}

// OFFSET: 0x00767E70

// OFFSET: 0x00767FA0

// OFFSET: 0x00767FF0

// OFFSET: 0x00768240

// OFFSET: 0x00768780

// OFFSET: 0x007688A0

// OFFSET: 0x00768920

// OFFSET: 0x00768960

// OFFSET: 0x00768A00

// OFFSET: 0x00769240

// OFFSET: 0x00769440

// OFFSET: 0x00769640

// OFFSET: 0x007698F0

// OFFSET: 0x00769B80

// OFFSET: 0x00769EE0

// OFFSET: 0x0076A240

// OFFSET: 0x0076A470

// OFFSET: 0x0076A690

// OFFSET: 0x0076A940

// OFFSET: 0x0076AA40

// OFFSET: 0x0076AC70

// OFFSET: 0x0076AFE0

// OFFSET: 0x0076B280

// OFFSET: 0x0076B4B0
