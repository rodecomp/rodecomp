#ifndef RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H
#define RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct TipBoxEntry {
  TipBoxEntry();
  std::string title;
  std::int32_t unknown_0018;
  std::string unknown_001C;
  unsigned char unknown_0034;
  std::vector<std::string> paragraphs;
  int unknown_0044;
  int unknown_0048;
  std::int32_t unknown_004C[3];
};

struct TipBoxTitles {
  const std::map<int, TipBoxEntry>& entries;
  const TipBoxEntry& missing;
  const int& maximumId;
  const unsigned short& codePage;

  std::string Title(int id, std::string* abbreviated, int width) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TipBoxEntry) == 88);
#endif

#endif
