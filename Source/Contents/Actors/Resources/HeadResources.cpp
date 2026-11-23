#include "Client/Session/Session.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"

#include <string>

// GLOBAL: gl_24703
EquipmentResourceMgr* g_equipmentResourceMgr;
// GLOBAL: gl_24539
const char* g_equipmentResourceExtensions[2] = {"act", "spr"};

// OFFSET: 0x00DDDFE0
// FUNCTION: fn_23393
int CSession::GetDisplayJob() {
  const int job = this->m_job;

  if (job == JT_KNIGHT) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 6;
    }
  } else if (job == JT_CRUSADER) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 7;
    }
  } else if (job == JT_KNIGHT_H) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 6;
    }
  } else if (job == JT_KNIGHT_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 6;
    }
  } else if (job == JT_CRUSADER_H) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 7;
    }
  } else if (job == JT_CRUSADER_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 7;
    }
  } else if (job == JT_ROYAL_GUARD) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 16;
    }
  } else if (job == JT_ROYAL_GUARD_H) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 10;
    }
  } else if (job == JT_ROYAL_GUARD_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 8;
    }
  } else if (job == JT_RUNE_KNIGHT) {
    if (static_cast<bool>(g_session.m_jobFlags >> 19 & 1)) {
      return JT_RUNE_CHICKEN;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 23 & 1)) {
      return JT_RUNE_CHICKEN2;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 24 & 1)) {
      return JT_RUNE_CHICKEN3;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 25 & 1)) {
      return JT_RUNE_CHICKEN4;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 26 & 1)) {
      return JT_RUNE_CHICKEN5;
    }
  } else if (job == JT_RUNE_KNIGHT_H) {
    if (static_cast<bool>(g_session.m_jobFlags >> 19 & 1)) {
      return JT_RUNE_CHICKEN_H;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 23 & 1)) {
      return JT_RUNE_CHICKEN2_H;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 24 & 1)) {
      return JT_RUNE_CHICKEN3_H;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 25 & 1)) {
      return JT_RUNE_CHICKEN4_H;
    }
    if (static_cast<bool>(g_session.m_jobFlags >> 26 & 1)) {
      return JT_RUNE_CHICKEN5_H;
    }
  } else if (job == JT_RUNE_KNIGHT_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 19 & 1)) {
      return job + 13;
    }
  } else if (job == JT_STAR) {
    if (static_cast<bool>(g_session.m_jobFlags >> 15 & 1)) {
      return job + 1;
    }
  } else if (job == JT_RANGER) {
    if (static_cast<bool>(g_session.m_jobFlags >> 21 & 1)) {
      return job + 28;
    }
  } else if (job == JT_RANGER_H) {
    if (static_cast<bool>(g_session.m_jobFlags >> 21 & 1)) {
      return job + 23;
    }
  } else if (job == JT_RANGER_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 21 & 1)) {
      return job + 13;
    }
  } else if (job == JT_MECHANIC) {
    if (g_session.madogearType != 0) {
      return job + 28;
    }
  } else if (job == JT_MECHANIC_H) {
    if (g_session.madogearType != 0) {
      return job + 23;
    }
  } else if (job == JT_MECHANIC_B) {
    if (g_session.madogearType != 0) {
      return job + 12;
    }
  } else if (job == JT_MEISTER) {
    if (g_session.madogearType != 0) {
      return job + 26;
    }
  } else if (job == JT_STAR_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 15 & 1)) {
      return job + 12;
    }
  } else if (job == JT_STAR_EMPEROR) {
    if (static_cast<bool>(g_session.m_jobFlags >> 15 & 1)) {
      return job + 4;
    }
  } else if (job == JT_STAR_EMPEROR_B) {
    if (static_cast<bool>(g_session.m_jobFlags >> 15 & 1)) {
      return job + 3;
    }
  } else if (job == JT_WINDHAWK) {
    if (static_cast<bool>(g_session.m_jobFlags >> 21 & 1)) {
      return job + 21;
    }
  } else if (job == JT_DRAGON_KNIGHT) {
    if (static_cast<bool>(g_session.m_jobFlags >> 19 & 1)) {
      return job + 28;
    }
  } else if (job == JT_IMPERIAL_GUARD) {
    if (static_cast<bool>(g_session.m_jobFlags >> 5 & 1)) {
      return job + 23;
    }
  } else if (job == JT_SKY_EMPEROR) {
    if (static_cast<bool>(g_session.m_jobFlags >> 15 & 1)) {
      return job + 14;
    }
  }

  return job;
}

// OFFSET: 0x00E0A430
// FUNCTION: fn_23796
__declspec(noinline) std::string CSession::GetSexName(int sex) {
  return this->m_sexNames[sex];
}

// OFFSET: 0x00DDD100
// FUNCTION: fn_23380
std::string CSession::GetImfName(int job) {
  return this->m_imfNames[job];
}

// INLINED
static const std::string SexNameOf(int sex) {
  return g_session.GetSexName(sex);
}

// OFFSET: 0x00BAB110
// FUNCTION: fn_19751
std::string EquipmentResourceMgr::GetHeadName(int type, int job, int sex, int* head) {
  if (job < JT_NOVICE || (job > JT_SUMMER2 && job < JT_MON_BEGIN) || (job > JT_MON_BEGIN && job < JT_MONSTER_LAST) || (job >= JT_EXTENDED_JOB_NAME_START && job < JT_EXTENDED_JOB_NAME_END)) {
    job = 0;
  }

  if (sex != 0) {
    sex = 1;
  }
  std::string result = "";
  std::string sexName = "";
  std::string headName = "";

  if (g_session.IsDoramJob(job) == true) {
    if (*head < 1 || *head >= 11) {
      *head = 10;
    }

    if (sex == 1) {
      headName = g_session.m_doramMaleHeadNames[*head];
    } else if (sex == 0) {
      headName = g_session.m_doramFemaleHeadNames[*head];
    }
  } else {
    if (*head < 0 || *head >= 46) {
      *head = 13;
    }

    if (sex == 1) {
      headName = g_session.m_humanMaleHeadNames[*head];
    } else if (sex == 0) {
      headName = g_session.m_humanFemaleHeadNames[*head];
    }
  }

  sexName = SexNameOf(sex);
  result = static_cast<const std::string&>(this->GetBodyRacePrefix(job)) +
           static_cast<const std::string&>(FormatOwnedString("\\머리통\\%s\\%s_%s.%s", sexName.c_str(), headName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[type]));
  return result;
}

// OFFSET: 0x00DD9CC0
// FUNCTION: fn_23336
void CSession::NormalizeHead(int* head, int job) {
  if (*head != 0) {
    return;
  }

  switch (job) {
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_NOVICE_H:
  case JT_NOVICE_B:
  case JT_SUPERNOVICE_B:
    *head = 1;
    break;
  case JT_SWORDMAN:
    *head = 2;
    break;
  case JT_ACOLYTE:
    *head = 5;
    break;
  case JT_MAGICIAN:
    *head = 3;
    break;
  case JT_ARCHER:
    *head = 4;
    break;
  case JT_MERCHANT:
    *head = 6;
    break;
  case JT_THIEF:
    *head = 7;
    break;
  default:
    *head = 1;
    break;
  }
}

// OFFSET: 0x00BAAB10
// FUNCTION: fn_19750
std::string EquipmentResourceMgr::GetHeadPalName(int head, int job, int sex, int palette) {
  std::string result = "";
  std::string sexName = "";
  std::string headName = "";

  if (g_session.IsDoramJob(job) == true) {
    if (head < 1 || head >= 11) {
      head = 10;
    }

    if (sex == 1) {
      headName = g_session.m_doramMaleHeadNames[head];
    } else if (sex == 0) {
      headName = g_session.m_doramFemaleHeadNames[head];
    }

    if (!ResourceMgr::Instance().Exists(result.c_str())) {
      sexName = SexNameOf(sex);
      result = static_cast<const std::string&>(FormatOwnedString("%s\\머리\\머리%s_%s_%d.pal", this->GetBodyRacePrefix(job).c_str(), headName.c_str(), sexName.c_str(), palette));
    }

    return result;
  } else {
    g_session.NormalizeHead(&head, job);

    if (sex == 1) {
      headName = g_session.m_humanMaleHeadNames[head];
    } else if (sex == 0) {
      headName = g_session.m_humanFemaleHeadNames[head];
    }

    sexName = SexNameOf(sex);
    result = static_cast<const std::string&>(FormatOwnedString("머리\\머리%s_%s_%d.pal", headName.c_str(), sexName.c_str(), palette));
    return result;
  }
}

// OFFSET: 0x00DDCD50
// FUNCTION: fn_23373
std::string CSession::GetHeadActName(int job, int sex, int* head) {
  return g_equipmentResourceMgr->GetHeadName(0, job, sex, head);
}

// OFFSET: 0x00DDCDC0
// FUNCTION: fn_23374
std::string CSession::GetPlayerHeadActName() {
  EquipmentResourceMgr* const mgr = g_equipmentResourceMgr;
  return mgr->GetHeadName(0, this->GetDisplayJob(), this->GetCurrentSex(), &this->m_head);
}

// OFFSET: 0x00DDCEF0
// FUNCTION: fn_23375
std::string CSession::GetHeadPalName(int head, int job, int sex, int palette) {
  return g_equipmentResourceMgr->GetHeadPalName(head, job, sex, palette);
}

// OFFSET: 0x00DDCF60
// FUNCTION: fn_23376
std::string CSession::GetPlayerHeadPalName() {
  return g_equipmentResourceMgr->GetHeadPalName(this->m_head, this->m_job, this->cachedSex, this->m_headPalette);
}

// OFFSET: 0x00DDCFE0
// FUNCTION: fn_23377
std::string CSession::GetHeadSprName(int job, int sex, int* head) {
  return g_equipmentResourceMgr->GetHeadName(1, job, sex, head);
}

// OFFSET: 0x00DDD050
// FUNCTION: fn_23378
std::string CSession::GetPlayerHeadSprName() {
  EquipmentResourceMgr* resources = g_equipmentResourceMgr;
  const int sex = this->GetCurrentSex();
  const int job = this->GetDisplayJob();
  return resources->GetHeadName(1, job, sex, &this->m_head);
}

// OFFSET: 0x00DDE6B0
// FUNCTION: fn_23397
std::string CSession::GetBodySprName(int job, int sex, int costumeJob, int costume) {
  return g_equipmentResourceMgr->GetBodyResourceName(1, job, sex, costumeJob, costume);
}
