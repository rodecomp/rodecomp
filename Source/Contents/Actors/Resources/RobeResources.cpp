#include "Client/Session/Session.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Resource/ResourceMgr.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

#include <string>

// INLINED
static std::string BodyJobNameOf(int job) {
  return g_session.GetBodyJobName(job);
}

// INLINED
static std::string SexNameOf(int sex) {
  return g_session.GetSexName(sex);
}

// INLINED
static std::string RobeNameOf(unsigned int robe) {
  return g_session.GetRobeName(robe);
}

// OFFSET: 0x00BAC050
// FUNCTION: fn_19755
std::string EquipmentResourceMgr::GetRobeResourceName(int type, int job, int sex, int robe, unsigned char riding, int, int costumeJob) {
  if (robe <= 0) {
    return "";
  }

  const int spriteJob = g_session.GetSpriteJob(riding == true ? g_session.GetRidingJob(job) : job, costumeJob, true);
  std::string path = "";
  std::string jobName = "";
  std::string sexName = "";
  std::string robeName = "";

  {
    const std::string& name = BodyJobNameOf(spriteJob);
    jobName = name;
  }
  {
    const std::string& name = SexNameOf(sex);
    sexName = name;
  }
  robeName = static_cast<const std::string&>(this->GetRobeSprName(robe, 0));
  if (robeName.size() == 0) {
    return "";
  }

  const char* extension = g_equipmentResourceExtensions[type];
  if (type == 1) {
    path = static_cast<const std::string&>(FormatOwnedString("sprite\\로브\\%s\\%s.%s", robeName.c_str(), robeName.c_str(), extension));
    if (g_session.IsDoramJob(job)) {
      path = static_cast<const std::string&>(FormatOwnedString(
        "sprite\\로브\\%s\\%s_Doram.%s", robeName.c_str(), robeName.c_str(), g_equipmentResourceExtensions[1]
      ));
    }

    if (!ResourceMgr::Instance().Exists(path.c_str())) {
      path = static_cast<const std::string&>(FormatOwnedString(
        "sprite\\로브\\%s\\%s\\%s_%s.%s", robeName.c_str(), sexName.c_str(), jobName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[1]
      ));
    }
  } else {
    path = static_cast<const std::string&>(FormatOwnedString(
      "sprite\\로브\\%s\\%s\\%s_%s.%s", robeName.c_str(), sexName.c_str(), jobName.c_str(), sexName.c_str(), extension
    ));
  }

  return path;
}

// OFFSET: 0x00BAC770
// FUNCTION: fn_19756
std::string EquipmentResourceMgr::GetRobeSprName(int robe, int) {
  const char* name = nullptr;
  if (g_session.m_hotkeyScript->Call("ReqRobSprName", "d>s", robe, &name)) {
    return name;
  }
  return name;
}

// OFFSET: 0x00BAC8E0
// FUNCTION: fn_19757
std::string EquipmentResourceMgr::GetShieldResourceName(int type, int job, int sex, int shield, int costumeJob) {
  const int spriteJob = g_session.GetSpriteJob(job, costumeJob, true);
  std::string path = "";
  std::string sexName = "";
  std::string shieldName = "";

  const int shieldView = g_session.GetItemDescription40(shield);
  {
    const std::string& name = SexNameOf(sex);
    sexName = name;
  }
  bool exists = false;
  if (shield >= 5 && g_session.HasJobShieldSprite(job, shield) == true) {
    path = static_cast<const std::string&>(FormatOwnedString("방패\\%s_%s_%d_방패.%s", g_session.m_shieldJobNames[spriteJob], sexName.c_str(), shield, g_equipmentResourceExtensions[type]));
    exists = ResourceMgr::Instance().Exists(path.c_str());
  }

  if (!exists) {
    if (!g_session.HasJobShieldSprite(job, shieldView)) {
      return "";
    }

    {
      const std::string& name = RobeNameOf(shieldView);
      shieldName = name;
    }
    path = static_cast<const std::string&>(FormatOwnedString("방패\\%s_%s%s.%s", g_session.m_shieldJobNames[spriteJob], sexName.c_str(), shieldName.c_str(), g_equipmentResourceExtensions[type]));
    exists = ResourceMgr::Instance().Exists(path.c_str());
  }

  return exists ? path : "";
}
