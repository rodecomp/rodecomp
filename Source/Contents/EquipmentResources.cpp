#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"

#include <string>

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

// OFFSET: 0x00E0A2C0
// FUNCTION: fn_23794
std::string CSession::GetAccessoryName(int id) {
  if (id < 0 || static_cast<unsigned int>(id) >= this->m_accessoryNames.size()) {
    return "_병아리모자";
  }
  return this->m_accessoryNames[id];
}
