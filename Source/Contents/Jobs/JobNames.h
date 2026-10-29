#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBNAMES_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBNAMES_H

#include <vector>

class LuaScript;

struct JobNameInitialization {
  LuaScript& playerScript;
  LuaScript& otherScript;
  std::vector<const char*>& sexLabels;
  std::vector<int>& levels20;
  std::vector<int>& useLevels;
  std::vector<int>& otherLevels;
  std::vector<int>& homunculusLevels;
};

struct JobNames {
  static unsigned BaseJob(unsigned job);
};

#endif
