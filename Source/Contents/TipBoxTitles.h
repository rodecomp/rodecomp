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

class CTipBoxMgr {
public:
  CTipBoxMgr();
  virtual ~CTipBoxMgr();

  void AddParagraph(int id, std::string text);
  TipBoxEntry& SelectRandomTip();
  const TipBoxEntry& GetTip(int id) const;

  static bool Create();
  static void Destroy();

  int m_width;
  int m_height;
  int m_maximumId;
  std::map<int, TipBoxEntry> m_entries;
  std::vector<int> m_randomList;
  TipBoxEntry m_currentEntry;
  std::uint32_t unknown_007C[8];
  std::string unknown_009C;
  int unknown_00B4;
  bool m_initialized;
  TipBoxEntry m_missing;
  int unknown_0114;
  int unknown_0118;
};

extern CTipBoxMgr* g_itemTagOwner;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TipBoxEntry) == 88);
static_assert(sizeof(CTipBoxMgr) == 284);
#endif

#endif
