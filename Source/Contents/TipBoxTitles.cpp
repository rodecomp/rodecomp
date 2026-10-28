#include "Contents/TipBoxTitles.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <mbstring.h>
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
TipBoxEntry::TipBoxEntry() {
}

// INLINED
CTipBoxMgr::CTipBoxMgr()
    : m_width(240),
      m_height(320),
      m_maximumId(17),
      m_entries{},
      m_randomList{},
      m_currentEntry{},
      m_image{},
      m_imagePath{},
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

// OFFSET: 0x007ACF50
// FUNCTION: fn_09128
void CTipBoxMgr::Sub7ACF50() {
  const TipBoxEntry& entry = this->m_currentEntry;

  if (entry.unknown_001C.size() == 0) {
    this->m_image.kind = 2;
    return;
  }

  const int cell = entry.unknown_0044 - 1;
  const float cellWidth = static_cast<float>(this->m_width) / 3.0f;
  const float cellHeight = static_cast<float>(this->m_height) / 3.0f;
  const std::size_t found = entry.unknown_001C.find("유저인터페이스\\");

  this->m_image.x = static_cast<float>(cell % 3) * cellWidth + cellWidth * 0.5f;
  this->m_image.y = static_cast<float>(this->m_maximumId) + cellHeight * 0.5f + static_cast<float>(cell / 3) * cellHeight;

  if (found != std::string::npos) {
    this->m_imagePath = entry.unknown_001C + ".bmp";

    const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_imagePath.c_str())));
    if (!bitmap) {
      return;
    }

    const float scale = static_cast<float>(entry.unknown_0048) * 0.01f;
    this->m_image.x -= static_cast<float>(bitmap->image.width) * 0.5f * scale;
    this->m_image.y -= static_cast<float>(bitmap->image.height) * 0.5f * scale;

    if (1.0f > this->m_image.x) {
      const float clip = 1.0f - this->m_image.x;
      this->m_image.x = 1.0f;
      this->m_image.clipX = static_cast<int>(clip);
    } else {
      this->m_image.clipX = 0;
    }

    if (17.0f > this->m_image.y) {
      this->m_image.clipY = static_cast<int>(17.0f - this->m_image.y);
      this->m_image.y = static_cast<float>(this->m_maximumId);
    } else {
      this->m_image.clipY = 0;
    }

    this->m_image.width = static_cast<float>(bitmap->image.width) * scale;
    this->m_image.kind = 0;
    this->m_image.scale = scale;
    this->m_image.height = static_cast<float>(bitmap->image.height) * scale;
  } else {
    this->m_imagePath = "sprite\\" + entry.unknown_001C;

    const auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_imagePath + ".spr"));
    if (!sprite) {
      return;
    }

    const SpriteRleImage* image = sprite->images[0][0];
    if (!image) {
      return;
    }

    const float scale = static_cast<float>(entry.unknown_0048) * 0.01f;
    const float ratio = static_cast<float>(entry.unknown_0048) / 100.0f;
    this->m_image.x -= static_cast<float>(image->width) * 0.5f * scale;
    this->m_image.y -= static_cast<float>(image->height) * 0.5f * scale;

    const float right = static_cast<float>(image->width) * ratio + this->m_image.x;
    if (right > static_cast<float>(this->m_width)) {
      this->m_image.x -= right - static_cast<float>(this->m_width);
    }

    const float bottom = static_cast<float>(image->height) * ratio + this->m_image.y;
    if (bottom > static_cast<float>(this->m_height)) {
      this->m_image.y -= bottom - static_cast<float>(this->m_height);
    }

    this->m_image.width = static_cast<float>(image->width) * scale;
    this->m_image.scale = scale;
    this->m_image.kind = 1;
    this->m_image.height = static_cast<float>(image->height) * scale;
  }
}

// OFFSET: 0x007AD890
// FUNCTION: fn_09130
bool CTipBoxMgr::IsLastTip() const {
  return this->unknown_00B4 == static_cast<int>(this->m_randomList.size() / 6) - 1;
}

// OFFSET: 0x007AD8C0
// FUNCTION: fn_09131
bool CTipBoxMgr::HasImages() const {
  return !this->m_currentEntry.images.empty();
}

// OFFSET: 0x007AD8D0
// FUNCTION: fn_09132
bool CTipBoxMgr::HasSearchResults() const {
  return !this->m_randomList.empty();
}

// OFFSET: 0x007AD940
// FUNCTION: fn_09133
std::vector<int>& CTipBoxMgr::Search(std::string& keyword) {
  this->m_randomList.clear();

  for (const auto& item : this->m_entries) {
    const TipBoxEntry entry = item.second;

    if (entry.searchable) {
      std::string title = entry.title;
      std::transform(title.begin(), title.end(), title.begin(), toupper);
      std::transform(keyword.begin(), keyword.end(), keyword.begin(), toupper);

      if (_mbsstr(reinterpret_cast<const unsigned char*>(title.c_str()), reinterpret_cast<const unsigned char*>(keyword.c_str()))) {
        this->m_randomList.push_back(item.first);
      }
    }
  }

  return this->m_randomList;
}

// OFFSET: 0x007AE050
// FUNCTION: fn_09134
void CTipBoxMgr::SelectTip(int id) {
  this->unknown_00B4 = 0;

  const auto it = this->m_entries.find(id);
  if (it != this->m_entries.end()) {
    this->unknown_0114 = id;
    this->m_currentEntry = it->second;
  }

  this->Sub7ACF50();
}
