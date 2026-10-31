#include "Contents/TipBoxTitles.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"
#include "Script/NLuaState.h"
#include "Utility/Cipher/XorBytes.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <mbstring.h>
#include <new>
#include <windows.h>

// GLOBAL: gl_24607
CTipBoxMgr* g_itemTagOwner = nullptr;

namespace {
  template <std::size_t Length, std::uint64_t Key>
  class EncryptedName {
  public:
    explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = source[index];
      }
    }

    ~EncryptedName() {
      for (std::uint64_t index = 0; index < Length; ++index) {
        this->m_text[index] = 0;
      }
    }

    const char* Text() {
      if (this->m_encrypted) {
        utility::XorBytes(this->m_text, Length, Key);
        this->m_encrypted = false;
      }

      return reinterpret_cast<const char*>(this->m_text);
    }

  private:
    std::uint8_t m_text[Length];
    bool m_encrypted;
  };

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

  this->m_entries[id].paragraphs.push_back(std::move(text));
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

int sub_7AEC00(lua_State* state);
int sub_7AE900(lua_State* state);
int sub_7AE600(lua_State* state);
int sub_7AEAF0(lua_State* state);
int sub_7AEA20(lua_State* state);

// OFFSET: 0x007AD480
// FUNCTION: fn_09129
bool CTipBoxMgr::RegisterLuaFunctions() {
  NLuaState state;
  if (!state.IsValid()) {
    return false;
  }

  luaL_openlibs(state.GetState());
  lua_register(state.GetState(), "AddTip", sub_7AEC00);
  lua_register(state.GetState(), "AddImgcoord", sub_7AE900);
  lua_register(state.GetState(), "AddPageEx", sub_7AE600);
  lua_register(state.GetState(), "AddPage", sub_7AEAF0);
  lua_register(state.GetState(), "AddIsTag", sub_7AEA20);

  const std::uint8_t source[] = {
    0xF4, 0xCA, 0x76, 0xBD, 0xC0, 0x0E, 0xB7, 0x7F,
    0xCE, 0xC3, 0x67, 0xA6, 0xDD, 0x4D, 0x87, 0x7E,
    0xC6, 0xB3
  };
  thread_local EncryptedName<sizeof(source), 0x0BEB63A5C905B3A7ULL> name(source);

  if (luaL_loadfile(state.GetState(), name.Text()) != 0) {
    const std::uint8_t secondSource[] = {
      0x9A, 0xCE, 0x56, 0xE3, 0xDE, 0x5E, 0xDF, 0x73,
      0xA0, 0xC7, 0x47, 0xF8, 0xC3, 0x1D, 0xEF, 0x72,
      0xAB, 0xB7
    };
    thread_local EncryptedName<sizeof(secondSource), 0x078333BB9725B7C9ULL> secondName(secondSource);

    if (luaL_loadfile(state.GetState(), secondName.Text()) != 0) {
      MessageBoxA(nullptr, lua_tostring(state.GetState(), -1), "TipBox", MB_OK);
      return false;
    }
  }

  if (lua_pcall(state.GetState(), 0, 0, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(state.GetState(), -1), "TipBox", MB_OK);
    return false;
  }

  lua_getglobal(state.GetState(), "main");
  if (lua_pcall(state.GetState(), 0, 2, 0) != 0) {
    MessageBoxA(nullptr, lua_tostring(state.GetState(), -1), "TipBox", MB_OK);
    return false;
  }

  if (!lua_isstring(state.GetState(), -1)) {
    MessageBoxA(nullptr, "second return value is not string...", "TipBox", MB_OK);
    return false;
  }

  if (lua_toboolean(state.GetState(), -2)) {
    return true;
  }

  MessageBoxA(nullptr, (std::string("error found :") + lua_tostring(state.GetState(), -1)).c_str(), "TipBox", MB_OK);
  return false;
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

// OFFSET: 0x007AEAF0
// FUNCTION: fn_09145
int sub_7AEAF0(lua_State* state) {
  const char* message;
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    message = "2st argment is must number";
  } else if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    message = "1th argment is must string";
  } else {
    const int id = static_cast<int>(lua_tonumber(state, -2));
    g_itemTagOwner->AddParagraph(id, std::string(lua_tostring(state, -1)));
    lua_pushboolean(state, 1);
    message = "good";
  }

  lua_pushstring(state, message);
  return 2;
}

// OFFSET: 0x007AE900
int sub_7AE900(lua_State* state) {
  if (!lua_isnumber(state, -1)) { lua_pushboolean(state, 0); lua_pushstring(state, "3nd argment is must number"); return 2; }
  if (!lua_isnumber(state, -2)) { lua_pushboolean(state, 0); lua_pushstring(state, "2nd argment is must number"); return 2; }
  if (!lua_isnumber(state, -3)) { lua_pushboolean(state, 0); lua_pushstring(state, "1st argment is must number"); return 2; }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  TipBoxEntry& entry = g_itemTagOwner->m_entries[id];
  entry.unknown_0044 = static_cast<int>(lua_tonumber(state, -2));
  entry.unknown_0048 = static_cast<int>(lua_tonumber(state, -1));
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AEA20
// FUNCTION: fn_09144
int sub_7AEA20(lua_State* state) {
  if (!lua_isnumber(state, -2)) { lua_pushboolean(state, 0); lua_pushstring(state, "2 argment is must number"); return 2; }
  if (!lua_isnumber(state, -1)) { lua_pushboolean(state, 0); lua_pushstring(state, "1 argment is must number"); return 2; }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  g_itemTagOwner->m_entries[id].searchable = static_cast<int>(lua_tonumber(state, -1)) != 0;
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AEC00
int sub_7AEC00(lua_State* state) {
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "4th argment is must string");
    return 2;
  }
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "3nd argment is must number");
    return 2;
  }
  if (!lua_isstring(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "2st argment is must string");
    return 2;
  }
  if (lua_tonumber(state, -4) == 0.0) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "1st argment is must number");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -4));
  TipBoxEntry& entry = g_itemTagOwner->m_entries[id];
  if (id > g_itemTagOwner->m_maximumId) {
    g_itemTagOwner->m_maximumId = id;
  }
  entry.title = lua_tostring(state, -3);
  entry.searchable = static_cast<int>(lua_tonumber(state, -2)) == 1;
  entry.unknown_001C = lua_tostring(state, -1);
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}
