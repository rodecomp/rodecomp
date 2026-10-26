#include "Contents/TipBoxTitles.h"

#include <cstdlib>
#include <new>
#include <windows.h>

// GLOBAL: gl_24607
CTipBoxMgr* g_itemTagOwner = nullptr;

namespace {
  // OFFSET: 0x009B45D0
  // FUNCTION: fn_13832
  void SplitTitle(const char* text, std::vector<std::string>& lines, int width, unsigned short codePage) {
    const char* begin = text;
    const char* current = text;
    const char* end = text + std::char_traits<char>::length(text);
    const char* limit = text + width;

    while (current < end) {
      if (current >= limit || *current == '\n') {
        lines.emplace_back(begin, current);
        begin = current;
        limit = current + width;
      }

      current = CharNextExA(codePage, current, 0);
    }
  }
}

// OFFSET: 0x007ABEC0
// FUNCTION: fn_09118
TipBoxEntry::TipBoxEntry() : unknown_0018(0), unknown_0044(0), unknown_0048(0) {
  this->unknown_004C[0] = 0;
  this->unknown_004C[1] = 0;
  this->unknown_004C[2] = 0;
}

// INLINED
CTipBoxMgr::CTipBoxMgr()
    : m_width(240),
      m_height(320),
      m_maximumId(17),
      m_entries{},
      m_randomList{},
      m_currentEntry{},
      unknown_007C{},
      unknown_009C{},
      unknown_00B4(0),
      m_initialized(true),
      m_missing{},
      unknown_0114(0),
      unknown_0118(0) {
}

// INLINED
CTipBoxMgr::~CTipBoxMgr() = default;

// FUNCTION: fn_09122
void CTipBoxMgr::AddParagraph(int id, std::string text) {
  for (;;) {
    const auto pos = text.find("\\n");
    if (pos == std::string::npos) {
      break;
    }
    text.replace(pos, 2, "\n");
  }

  this->m_entries[id].paragraphs.push_back(text);
}

// FUNCTION: fn_09123
bool CTipBoxMgr::Create() {
  if (g_itemTagOwner) {
    return false;
  }

  g_itemTagOwner = new (std::nothrow) CTipBoxMgr();
  if (!g_itemTagOwner) {
    return false;
  }

  if (!g_itemTagOwner->m_initialized) {
    delete g_itemTagOwner;
    g_itemTagOwner = nullptr;
    return false;
  }

  return true;
}

// FUNCTION: fn_09124
void CTipBoxMgr::Destroy() {
  if (g_itemTagOwner) {
    delete g_itemTagOwner;
    g_itemTagOwner = nullptr;
  }
}

// FUNCTION: fn_09125
TipBoxEntry& CTipBoxMgr::SelectRandomTip() {
  this->m_randomList.clear();

  for (const auto& entry : this->m_entries) {
    if (entry.first < 1000 && entry.second.unknown_0018 == 1) {
      this->m_randomList.push_back(entry.first);
    }
  }

  const auto count = this->m_randomList.size();
  const auto index = static_cast<unsigned>(rand()) % count;
  const auto it = this->m_entries.find(this->m_randomList[index]);
  if (it != this->m_entries.end()) {
    this->unknown_0114 = this->m_randomList[index];
    this->m_currentEntry = it->second;
  }
  return this->m_currentEntry;
}

// FUNCTION: fn_09126
const TipBoxEntry& CTipBoxMgr::GetTip(int id) const {
  const auto it = this->m_entries.find(id);
  if (it == this->m_entries.end()) {
    return this->m_missing;
  }

  return it->second;
}

// OFFSET: 0x007ACC70
// FUNCTION: fn_09127
std::string TipBoxTitles::Title(int id, std::string* abbreviated, int width) const {
  const auto entry = this->entries.find(id);
  const auto& title = entry == this->entries.end() ? this->missing.title : entry->second.title;

  if (title.empty()) {
    return title;
  }

  std::string result = title.substr(0, title.find('#'));

  if (abbreviated) {
    if (width <= 10 || result.size() <= static_cast<unsigned>(width) + 4u) {
      *abbreviated = result;
    } else {
      std::vector<std::string> lines;
      SplitTitle(result.c_str(), lines, width, this->codePage);
      abbreviated->clear();

      for (const auto& line : lines) {
        *abbreviated += line;
      }

      *abbreviated += "...";
    }
  }

  return result;
}
