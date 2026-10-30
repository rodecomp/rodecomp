#ifndef RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EQUIPMENTRESOURCEMGR_H

#include <string>
#include <vector>

// VTABLE: vt_01791
class EquipmentResourceMgr {
public:
  static bool Create();
  static void Destroy();

  /* 0004 */ bool m_initialized;
  /* 0008 */ std::vector<std::string> unknown_0008;
  /* 0014 */ std::vector<std::string> unknown_0014;
  /* 0020 */ std::vector<std::string> unknown_0020;

  // INLINED
  EquipmentResourceMgr() {
    this->m_initialized = this->Init();
  }

  // INLINED
  virtual ~EquipmentResourceMgr() {
    this->Clear();
  }

  bool Init();
  void Clear();
  std::string GetBodyRacePrefix(int job);
  std::string GetBodyResourceName(int type, int job, int sex, int costumeJob, int costume);
  std::string GetRaceBodyResourceName(int type, int raceJob, int job, int sex, int costumeJob);
  std::string GetBodyPalName(int job, int sex, int palette, int costumeJob, int costume, int);
  std::string GetRobeSprName(int robe, int);
  std::string GetRobeResourceName(int type, int job, int sex, int robe, bool riding, int unused, int costumeJob);
  std::string GetHeadName(int type, int job, int sex, int* head);
  std::string GetHeadPalName(int head, int job, int sex, int palette);
  std::string GetWeaponResourceName(int type, int job, int sex, int weapon, int weaponItem, int costume);
  std::string GetAccessoryResourceName(int type, int job, int sex, int head, int accessory);
  std::string GetNamedAccessoryResourceName(int type, int sex, std::string accessoryName);
  std::string GetWeaponEffectResourceName(int type, int job, int sex, int weapon, int weaponItem, int effect, int costume);
};

static_assert(sizeof(EquipmentResourceMgr) == 0x2C);

extern EquipmentResourceMgr* g_equipmentResourceMgr;
extern const char* g_equipmentResourceExtensions[2];

#endif
