#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <new>
#include <string>

// OFFSET: 0x00BA9950
// FUNCTION: fn_19742
bool EquipmentResourceMgr::Init() {
  return true;
}

// OFFSET: 0x00BA9960
// FUNCTION: fn_19743
bool EquipmentResourceMgr::Create() {
  if (g_equipmentResourceMgr) {
    return false;
  }

  g_equipmentResourceMgr = new (std::nothrow) EquipmentResourceMgr();
  if (!g_equipmentResourceMgr) {
    return false;
  }

  if (g_equipmentResourceMgr->m_initialized) {
    return true;
  }

  delete g_equipmentResourceMgr;
  g_equipmentResourceMgr = nullptr;
  return false;
}

// OFFSET: 0x00BA9A60
// FUNCTION: fn_19744
void EquipmentResourceMgr::Clear() {
  this->unknown_0008.clear();
  this->unknown_0014.clear();
  this->unknown_0020.clear();
}

// OFFSET: 0x00BA9AD0
// FUNCTION: fn_19745
void EquipmentResourceMgr::Destroy() {
  if (g_equipmentResourceMgr) {
    delete g_equipmentResourceMgr;
    g_equipmentResourceMgr = nullptr;
  }
}

// OFFSET: 0x00BA9AF0
// FUNCTION: fn_19746
std::string EquipmentResourceMgr::GetAccessoryResourceName(int type, int job, int sex, int head, int accessory) {
  if (accessory == 0) {
    return std::string();
  }

  std::string path;
  std::string sexName;
  std::string headName;
  std::string accessoryName;

  if (sex == 1) {
    headName = g_session.m_humanMaleHeadNames[head];
  } else if (sex == 0) {
    headName = g_session.m_humanFemaleHeadNames[head];
  }

  sexName = g_session.GetSexName(sex);
  accessoryName = g_session.GetAccessoryName(accessory);

  if (accessoryName.size() == 0) {
    return "";
  }

  if (g_session.IsDoramJob(job) == true) {
    path = FormatOwnedString("악세사리\\%s_doram\\%s%s.%s", sexName.c_str(), sexName.c_str(), accessoryName.c_str(), g_equipmentResourceExtensions[type]);

    if (ResourceMgr::Instance().Exists(path.c_str()) == true) {
      return path;
    }
  }

  path = FormatOwnedString("악세사리\\%s\\%s%s.%s", sexName.c_str(), sexName.c_str(), accessoryName.c_str(), g_equipmentResourceExtensions[type]);
  return path;
}

// OFFSET: 0x00BAA110
// FUNCTION: fn_19747
std::string EquipmentResourceMgr::GetNamedAccessoryResourceName(int type, int sex, std::string accessoryName) {
  std::string path;
  std::string sexName;

  sexName = g_session.GetSexName(sex);
  path = FormatOwnedString("sprite\\악세사리\\%s\\%s%s.%s", sexName.c_str(), sexName.c_str(), accessoryName.c_str(), g_equipmentResourceExtensions[type]);
  return path;
}

// OFFSET: 0x00BAA2E0
// FUNCTION: fn_19748
std::string EquipmentResourceMgr::GetBodyPalName(int job, int sex, int palette, int costumeJob, int costume, int) {
  const int style = g_session.GetBodyStyle(job, palette, costumeJob);
  const bool secondJob = g_session.IsSecondJob(costumeJob);
  int spriteJob = g_session.GetSpriteJob(job, costumeJob, false);
  if (spriteJob > 3950) {
    spriteJob -= 3950;
  }

  std::string result;
  std::string sexName;

  sexName = g_session.GetSexName(sex);

  if (g_session.IsDoramJob(job) == true) {
    if (secondJob == true) {
      result = FormatOwnedString("%s\\body\\costume_%d\\%s_%s_%d_%d.pal", this->GetBodyRacePrefix(job).c_str(), 1, g_session.m_palJobNames[spriteJob],
                                 sexName.c_str(), style, 1);
    } else {
      result = FormatOwnedString("%s\\body\\%s_%s_%d.pal", this->GetBodyRacePrefix(job).c_str(), g_session.m_palJobNames[spriteJob], sexName.c_str(), style);
    }
  } else {
    if (secondJob == true) {
      result = FormatOwnedString("몸\\costume_%d\\%s_%s_%d_%d.pal", 1, g_session.m_palJobNames[spriteJob], sexName.c_str(), style, 1);
    } else {
      result = FormatOwnedString("몸\\%s_%s_%d.pal", g_session.m_palJobNames[spriteJob], sexName.c_str(), style);
    }

    if (costume == 2) {
      std::size_t found = result.rfind("마도기어_");
      if (found != std::string::npos) {
        result.replace(found, 9, "마도아머_", 9);
      }

      found = result.rfind("MEISTER_MADOGEAR1");
      if (found != std::string::npos) {
        result.replace(found, 17, "MEISTER_MADOGEAR2", 17);
      }
    }
  }

  return result;
}

// OFFSET: 0x00BAA670
// FUNCTION: fn_19749
std::string EquipmentResourceMgr::GetRaceBodyResourceName(int type, int raceJob, int job, int sex, int costumeJob) {
  std::string result;
  std::string sexName;
  std::string jobName;

  sexName = g_session.GetSexName(sex);
  const bool secondJob = g_session.IsSecondJob(costumeJob);
  jobName = g_session.GetBodyJobName(g_session.GetSpriteJob(job, costumeJob, true));

  if (secondJob == true) {
    result = this->GetBodyRacePrefix(raceJob) +
             FormatOwnedString("\\몸통\\%s\\costume_%d\\%s_%s_%d.%s", sexName.c_str(), 1, jobName.c_str(), sexName.c_str(), 1,
                               g_equipmentResourceExtensions[type]);
  } else {
    result = this->GetBodyRacePrefix(raceJob) +
             FormatOwnedString("\\몸통\\%s\\%s_%s.%s", sexName.c_str(), jobName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[type]);
  }

  return result;
}

// OFFSET: 0x00DDBA70
// FUNCTION: fn_23357
std::string CSession::GetAccessoryActName(int job, int sex, int head, int accessory) {
  return g_equipmentResourceMgr->GetAccessoryResourceName(0, job, sex, head, accessory);
}

// OFFSET: 0x00DDBAE0
// FUNCTION: fn_23358
std::string CSession::GetAccessorySprName(int job, int sex, int head, int accessory) {
  return g_equipmentResourceMgr->GetAccessoryResourceName(1, job, sex, head, accessory);
}

// OFFSET: 0x00DE0C50
// FUNCTION: fn_23468
std::string CSession::GetShieldActName(int job, int sex, int shield, int costumeJob) {
  return g_equipmentResourceMgr->GetShieldResourceName(0, job, sex, shield, costumeJob);
}

// OFFSET: 0x00DE0CC0
// FUNCTION: fn_23469
std::string CSession::GetShieldSprName(int job, int sex, int shield, int costumeJob) {
  return g_equipmentResourceMgr->GetShieldResourceName(1, job, sex, shield, costumeJob);
}

// OFFSET: 0x00E0A2C0
// FUNCTION: fn_23794
std::string CSession::GetAccessoryName(int id) {
  if (id < 0 || static_cast<unsigned int>(id) >= this->m_accessoryNames.size()) {
    return "_병아리모자";
  }
  return this->m_accessoryNames[id];
}
