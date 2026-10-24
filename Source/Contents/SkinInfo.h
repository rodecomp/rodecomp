#ifndef RAGNAROK_SOURCE_CONTENTS_SKININFO_H
#define RAGNAROK_SOURCE_CONTENTS_SKININFO_H

#include <string>
#include <vector>

// Borrowed fields of original skin owner off_1334630. Names/prefixes and
// archive/prepared flags are kept on their real configuration owner.
struct SkinInfo {
  int& selected;
  const std::vector<std::string>& names;
  const std::vector<std::string>& prefixes;
  const std::vector<int>& archives;
  std::vector<int>& prepared;
  const char* applicationDirectory;

  bool Selected() const;
  const char* Prefix() const;
  const char* Name() const;
  const char* Name(int index) const;
  void Prepare();
};

#endif
