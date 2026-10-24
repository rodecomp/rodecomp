#ifndef RAGNAROK_SOURCE_CONTENTS_CHARACTERCONFIGSAVE_H
#define RAGNAROK_SOURCE_CONTENTS_CHARACTERCONFIGSAVE_H

#include "Contents/SkillInfo.h"

#include <list>

class CPlayerSkillBundle;
struct ClientOptions;
struct ServiceOptions;
struct NormalItemSlot;
struct NormalItemSlotWindow;

// Borrows original client collections and persistence state, not a second owner.
struct CharacterConfigSave {
  CPlayerSkillBundle& skills;
  const std::list<CSkillInfo>& homunculusSkills;
  const int* useLevels;
  const int* additionalLevels;
  const int* homunculusLevels;
  const ClientOptions& options;
  const ServiceOptions& serviceOptions;
  const bool& sortPrivate;
  const NormalItemSlot* items;
  const NormalItemSlotWindow& saved;
  const bool& itemSlotActive;

  int UseLevel(int id) const;
};

#endif
