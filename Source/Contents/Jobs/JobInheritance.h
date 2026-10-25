#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBINHERITANCE_H

class LuaScript;
class CPlayerSkillBundle;

struct JobInheritance {
  LuaScript& script;

  int Lookup(int job);

  void InitializeSkillTree(CPlayerSkillBundle& bundle, int job);
};

#endif
