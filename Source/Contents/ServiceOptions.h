#ifndef RAGNAROK_SOURCE_CONTENTS_SERVICEOPTIONS_H
#define RAGNAROK_SOURCE_CONTENTS_SERVICEOPTIONS_H

#include "Contents/PlayerSkillBundle.h"

#include <unordered_map>


bool IsServiceOption(int id);
// Service-option hash table. Values retain byte width.
struct ServiceOptions {
  std::unordered_map<std::uint32_t, unsigned char, SkillIdHash>& values;

  unsigned char Get(int id) const;
  bool Set(int id, unsigned char value);
};

#endif
