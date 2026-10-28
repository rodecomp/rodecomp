#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
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
  const std::uint32_t flags = this->m_jobFlags;

  if (job == 7) {
    if (flags >> 5 & 1) {
      return 13;
    }
  } else if (job == 14) {
    if (flags >> 5 & 1) {
      return 21;
    }
  } else if (job == 4008) {
    if (flags >> 5 & 1) {
      return 4014;
    }
  } else if (job == 4030) {
    if (flags >> 5 & 1) {
      return 4036;
    }
  } else if (job == 4015) {
    if (flags >> 5 & 1) {
      return 4022;
    }
  } else if (job == 4037) {
    if (flags >> 5 & 1) {
      return 4044;
    }
  } else if (job == 4066) {
    if (flags >> 5 & 1) {
      return 4082;
    }
  } else if (job == 4073) {
    if (flags >> 5 & 1) {
      return 4083;
    }
  } else if (job == 4102) {
    if (flags >> 5 & 1) {
      return 4110;
    }
  } else if (job == 4054) {
    if (flags >> 19 & 1) {
      return 4080;
    }
    if (flags >> 23 & 1) {
      return 4088;
    }
    if (flags & 0x1000000) {
      return 4090;
    }
    if (flags >> 25 & 1) {
      return 4092;
    }
    if (flags >> 26 & 1) {
      return 4094;
    }
  } else if (job == 4060) {
    if (flags >> 19 & 1) {
      return 4081;
    }
    if (flags >> 23 & 1) {
      return 4089;
    }
    if (flags & 0x1000000) {
      return 4091;
    }
    if (flags >> 25 & 1) {
      return 4093;
    }
    if (flags >> 26 & 1) {
      return 4095;
    }
  } else if (job == 4096) {
    if (flags >> 19 & 1) {
      return 4109;
    }
  } else if (job == 4047) {
    if (flags >> 15 & 1) {
      return 4048;
    }
  } else if (job == 4056) {
    if (flags >> 21 & 1) {
      return 4084;
    }
  } else if (job == 4062) {
    if (flags >> 21 & 1) {
      return 4085;
    }
  } else if (job == 4098) {
    if (flags >> 21 & 1) {
      return 4111;
    }
  } else if (job == 4058) {
    if (this->unknown_8074 != 0) {
      return 4086;
    }
  } else if (job == 4064) {
    if (this->unknown_8074 != 0) {
      return 4087;
    }
  } else if (job == 4100) {
    if (this->unknown_8074 != 0) {
      return 4112;
    }
  } else if (job == 4253) {
    if (this->unknown_8074 != 0) {
      return 4279;
    }
  } else if (job == 4226) {
    if (flags >> 15 & 1) {
      return 4238;
    }
  } else if (job == 4239) {
    if (flags >> 15 & 1) {
      return 4243;
    }
  } else if (job == 4241) {
    if (flags >> 15 & 1) {
      return 4244;
    }
  } else if (job == 4257) {
    if (flags >> 21 & 1) {
      return 4278;
    }
  } else if (job == 4252) {
    if (flags >> 19 & 1) {
      return 4280;
    }
  } else if (job == 4258) {
    if (flags >> 5 & 1) {
      return 4281;
    }
  } else if (job == 4302) {
    if (flags >> 15 & 1) {
      return 4316;
    }
  }

  return job;
}

// OFFSET: 0x00E0A430
// FUNCTION: fn_23796
std::string CSession::GetSexName(int sex) {
  return this->m_sexNames[sex];
}

// OFFSET: 0x00BAB110
// FUNCTION: fn_19751
std::string EquipmentResourceMgr::GetHeadName(int type, int job, int sex, int* head) {
  if (job < 0 || (job > 30 && job < 1000) || (job > 1000 && job < 3999) || (job >= 20021 && job < 32000)) {
    job = 0;
  }

  std::string result;
  const int male = sex != 0 ? 1 : 0;
  std::string sexName;
  std::string headName;

  if (g_session.IsDoramJob(job) == true) {
    if (*head < 1 || *head > 10) {
      *head = 10;
    }

    if (male == 1) {
      headName = g_session.m_doramMaleHeadNames[*head];
    } else if (male == 0) {
      headName = g_session.m_doramFemaleHeadNames[*head];
    }
  } else {
    if (*head < 0 || *head > 45) {
      *head = 13;
    }

    if (male == 1) {
      headName = g_session.m_humanMaleHeadNames[*head];
    } else if (male == 0) {
      headName = g_session.m_humanFemaleHeadNames[*head];
    }
  }

  sexName = g_session.GetSexName(male);
  result = this->GetBodyRacePrefix(job) +
           FormatOwnedString("\\머리통\\%s\\%s_%s.%s", sexName.c_str(), headName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[type]);
  return result;
}

// OFFSET: 0x00DD9CC0
// FUNCTION: fn_23336
void CSession::NormalizeHead(int* head, int job) {
  if (*head != 0) {
    return;
  }

  switch (job) {
  case 1:
    *head = 2;
    break;
  case 2:
    *head = 3;
    break;
  case 3:
    *head = 4;
    break;
  case 4:
    *head = 5;
    break;
  case 5:
    *head = 6;
    break;
  case 6:
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
  std::string result;
  std::string sexName;
  std::string headName;

  if (g_session.IsDoramJob(job) == true) {
    if (head < 1 || head > 10) {
      head = 10;
    }

    if (sex == 1) {
      headName = g_session.m_doramMaleHeadNames[head];
    } else if (sex == 0) {
      headName = g_session.m_doramFemaleHeadNames[head];
    }

    if (!ResourceMgr::Instance().Exists(result.c_str())) {
      sexName = g_session.GetSexName(sex);
      result = FormatOwnedString("%s\\머리\\머리%s_%s_%d.pal", this->GetBodyRacePrefix(job).c_str(), headName.c_str(), sexName.c_str(), palette);
    }
  } else {
    g_session.NormalizeHead(&head, job);

    if (sex == 1) {
      headName = g_session.m_humanMaleHeadNames[head];
    } else if (sex == 0) {
      headName = g_session.m_humanFemaleHeadNames[head];
    }

    sexName = g_session.GetSexName(sex);
    result = FormatOwnedString("머리\\머리%s_%s_%d.pal", headName.c_str(), sexName.c_str(), palette);
  }

  return result;
}

// OFFSET: 0x00DDCD50
// FUNCTION: fn_23373
std::string CSession::GetHeadActName(int job, int sex, int* head) {
  return g_equipmentResourceMgr->GetHeadName(0, job, sex, head);
}

// OFFSET: 0x00DDCDC0
// FUNCTION: fn_23374
std::string CSession::GetPlayerHeadActName() {
  return g_equipmentResourceMgr->GetHeadName(0, this->GetDisplayJob(), this->SyncUnknown0E98(), &this->m_head);
}

// OFFSET: 0x00DDCEF0
// FUNCTION: fn_23375
std::string CSession::GetHeadPalName(int head, int job, int sex, int palette) {
  return g_equipmentResourceMgr->GetHeadPalName(head, job, sex, palette);
}

// OFFSET: 0x00DDCF60
// FUNCTION: fn_23376
std::string CSession::GetPlayerHeadPalName() {
  return g_equipmentResourceMgr->GetHeadPalName(this->m_head, this->m_job, this->unknown_0E98, this->m_headPalette);
}

// OFFSET: 0x00DDCFE0
// FUNCTION: fn_23377
std::string CSession::GetHeadSprName(int job, int sex, int* head) {
  return g_equipmentResourceMgr->GetHeadName(1, job, sex, head);
}
