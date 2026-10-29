#ifndef RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H
#define RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct TipBoxImage {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ float unknown_0018;
};

struct TipBoxEntry {
  TipBoxEntry();
  /* 0000 */ std::string title;
  /* 0018 */ unsigned char unknown_0018;
  /* 001C */ std::string unknown_001C;
  /* 0034 */ unsigned char searchable;
  /* 0038 */ std::vector<std::string> paragraphs;
  /* 0044 */ int unknown_0044;
  /* 0048 */ int unknown_0048;
  /* 004C */ std::vector<TipBoxImage> images;
};

struct TipBoxLayout {
  /* 0000 */ int kind;
  /* 0004 */ float x;
  /* 0008 */ float y;
  /* 000C */ float width;
  /* 0010 */ float height;
  /* 0014 */ int clipX;
  /* 0018 */ int clipY;
  /* 001C */ float scale;
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
  void Sub7ACF50();
  bool RegisterLuaFunctions();
  bool IsLastTip() const;
  bool HasImages() const;
  bool HasSearchResults() const;
  std::vector<int>& Search(std::string& keyword);
  void SelectTip(int id);
  static bool Create();
  static void Destroy();

  int m_width;
  int m_height;
  int m_maximumId;
  std::map<int, TipBoxEntry> m_entries;
  std::vector<int> m_randomList;
  TipBoxEntry m_currentEntry;
  TipBoxLayout m_image;
  std::string m_imagePath;
  int unknown_00B4;
  bool m_initialized;
  TipBoxEntry m_missing;
  int unknown_0114;
  int unknown_0118;
};

extern CTipBoxMgr* g_itemTagOwner;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TipBoxImage) == 28);
static_assert(sizeof(TipBoxEntry) == 88);
static_assert(offsetof(TipBoxEntry, searchable) == 0x34);
static_assert(offsetof(TipBoxEntry, images) == 0x4C);
static_assert(sizeof(CTipBoxMgr) == 284);
static_assert(offsetof(CTipBoxMgr, m_randomList) == 0x18);
static_assert(offsetof(CTipBoxMgr, m_currentEntry) == 0x24);
static_assert(offsetof(CTipBoxMgr, unknown_00B4) == 0xB4);
static_assert(offsetof(CTipBoxMgr, m_image) == 0x7C);
static_assert(offsetof(CTipBoxMgr, m_imagePath) == 0x9C);
static_assert(offsetof(CTipBoxMgr, unknown_0114) == 0x114);
#endif

#endif
