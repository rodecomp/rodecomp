#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLLEVELREGISTRY_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLLEVELREGISTRY_H

class LuaScript;

struct SkillLevelRegistry {
  LuaScript& script;
  const char* baseKey;
  const int& character;
  int* useLevels;
  int* homunculusLevels;
  int* otherLevels;

  void Load();
};

#endif
