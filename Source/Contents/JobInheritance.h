#ifndef RAGNAROK_SOURCE_CONTENTS_JOBINHERITANCE_H
#define RAGNAROK_SOURCE_CONTENTS_JOBINHERITANCE_H

class LuaScript;
class CPlayerSkillBundle;

// Borrowed client script owner, not a separate job table.
struct JobInheritance {
  LuaScript& script;

  int Lookup(int job);

  // Query GetInheritJob(job) before changing the bundle. A failed query or -1
  // retains it; other counts clear groups, extra skills and level changes.
  // Visit positive indices in descending order. Resolve each job through the
  // live Lua tables, then call InitSkillTreeView(job, group); callback failure
  // does not stop later indices. Other bundle collections remain untouched.
  void InitializeSkillTree(CPlayerSkillBundle& bundle, int job);
};

#endif
