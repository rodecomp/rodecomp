#ifndef RAGNAROK_SOURCE_CONTENTS_JOBS_JOBNAMES_H
#define RAGNAROK_SOURCE_CONTENTS_JOBS_JOBNAMES_H

#include <cstdint>
#include <map>
#include <string>
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
  std::vector<const char*>& names;
  std::map<unsigned, const char*>& female;
  std::map<unsigned, const char*>& male;
  const int& currentSex;
  int& cachedSex;

  static unsigned BaseJob(unsigned job);
  const char* Lookup(unsigned job, int sex);
  void Initialize(JobNameInitialization& context);
};

#endif
