#ifndef RAGNAROK_SOURCE_CONTENTS_REPUTERECORDS_H
#define RAGNAROK_SOURCE_CONTENTS_REPUTERECORDS_H

#include <string>
#include <vector>

struct ReputeGroupRecord {
  /* 0000 */ std::string id;
  /* 0018 */ std::string name;
  /* 0030 */ std::vector<int> reputes;
};

struct ReputeInfoRecord {
  /* 0000 */ std::string name;
  /* 0018 */ int maxPointPositive;
  /* 001C */ int maxPointNegative;
  /* 0020 */ std::string invisible;
};

#if defined(_WIN32) && !defined(_WIN64)
#include <cstddef>
static_assert(sizeof(ReputeGroupRecord) == 0x3C);
static_assert(sizeof(ReputeInfoRecord) == 0x38);
#endif

#endif
