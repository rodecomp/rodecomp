#ifndef RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H
#define RAGNAROK_SOURCE_CONTENTS_TIPBOXTITLES_H

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct TipBoxImage {
  /* 0000 */ int kind;
  /* 0004 */ int blink;
  /* 0008 */ int fromX;
  /* 000C */ int fromY;
  /* 0010 */ int toX;
  /* 0014 */ int toY;
  /* 0018 */ float duration;
};

struct TipBoxEntry {
  TipBoxEntry();
  /* 0000 */ std::string title;
  /* 0018 */ unsigned char randomEligible;
  /* 001C */ std::string bitmapName;
  /* 0034 */ unsigned char searchable;
  /* 0038 */ std::vector<std::string> paragraphs;
  /* 0044 */ int imageCell;
  /* 0048 */ int imageScalePercent;
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

class CTipBoxMgr {
public:
  CTipBoxMgr();
  virtual ~CTipBoxMgr();

  __declspec(noinline) void AddParagraph(int id, std::string text);
  TipBoxEntry& SelectRandomTip();
  const TipBoxEntry& GetTip(int id) const;
  void Sub7ACF50();
  bool RegisterLuaFunctions();
  bool IsLastTip() const;
  std::string Title(int id, std::string* abbreviated, int width) const;
  bool HasImages() const;
  bool HasSearchResults() const;
  std::vector<int>& Search(std::string& keyword);
  __declspec(noinline) void SelectTip(int id);
  static bool Create();
  static void Destroy();
  void AddTip(int id, std::string title, double flag, std::string text);
  // INLINED
  void SetImageCoord(int id, int x, int y) {
    this->m_entries[id].imageCell = x;
    this->m_entries[id].imageScalePercent = y;
  }
  // INLINED
  void SetSearchable(int id, bool searchable) {
    this->m_entries[id].searchable = searchable;
  }
  // INLINED
  void AddImage(int id, const TipBoxImage& image) {
    this->m_entries[id].images.push_back(image);
  }

  int m_width;
  int m_height;
  int m_maximumId;
  std::map<int, TipBoxEntry> m_entries;
  std::vector<int> m_randomList;
  TipBoxEntry m_currentEntry;
  TipBoxLayout m_image;
  std::string m_imagePath;
  int paragraphIndex;
  bool m_initialized;
  TipBoxEntry m_missing;
  int currentTipId;
  int maxTipId;
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
static_assert(offsetof(CTipBoxMgr, paragraphIndex) == 0xB4);
static_assert(offsetof(CTipBoxMgr, m_image) == 0x7C);
static_assert(offsetof(CTipBoxMgr, m_imagePath) == 0x9C);
static_assert(offsetof(CTipBoxMgr, currentTipId) == 0x114);
#endif

#endif
