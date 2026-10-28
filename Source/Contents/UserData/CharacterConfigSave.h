#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERCONFIGSAVE_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_CHARACTERCONFIGSAVE_H

#include "Contents/Skills/SkillInfo.h"

#include <list>

class CPlayerSkillBundle;
struct ServiceOptions;
struct NormalItemSlot;
struct NormalItemSlotWindow;

struct CharacterConfigSave {
  CPlayerSkillBundle& skills;
  const std::list<CSkillInfo>& homunculusSkills;
  const int* useLevels;
  const int* additionalLevels;
  const int* homunculusLevels;
  const ServiceOptions& serviceOptions;
  const bool& sortPrivate;
  const NormalItemSlot* items;
  const NormalItemSlotWindow& saved;
  const bool& itemSlotActive;

  int UseLevel(int id) const;
};

#endif
