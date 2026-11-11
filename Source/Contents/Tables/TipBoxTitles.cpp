#include "Contents/Tables/TipBoxTitles.h"
#include "Client/Input/InputCharset.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/NLuaState.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"

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

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const char (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }

  // OFFSET: 0x009B45D0
  // FUNCTION: fn_13832
  __declspec(noinline) void SplitTitle(const char* text, std::vector<std::string>& lines, int width) {
    const char* current = text;
    const char* end = current + std::char_traits<char>::length(current);
    const char* limit = current + width;
    const char* begin = current;

    while (current < end) {
      if (current >= limit || *current == '\n') {
        {
          const std::string line(begin, current);
          lines.push_back(line);
        }
        begin = current;
        limit = current + width;
      }

      current = g_serverLanguage->NextChar(current, 0);
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
      paragraphIndex(0),
      m_initialized(true),
      m_missing{},
      currentTipId(0),
      maxTipId(0) {
}

// INLINED
CTipBoxMgr::~CTipBoxMgr() = default;

// OFFSET: 0x007AC710
// FUNCTION: fn_09122
void CTipBoxMgr::AddParagraph(int id, std::string text) {
  {
    const std::string newline("\n");
    const std::string escaped("\\n");

    for (;;) {
      const auto pos = text.find(escaped);
      if (pos == std::string::npos) {
        break;
      }
      text.replace(pos, escaped.size(), newline);
    }
  }

  this->m_entries[id].paragraphs.push_back(text);
}

// OFFSET: 0x007AC990
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

// OFFSET: 0x007ACB20
// FUNCTION: fn_09124
void CTipBoxMgr::Destroy() {
  if (g_itemTagOwner) {
    delete g_itemTagOwner;
    g_itemTagOwner = nullptr;
  }
}

// OFFSET: 0x007ACB40
// FUNCTION: fn_09125
TipBoxEntry& CTipBoxMgr::SelectRandomTip() {
  this->m_randomList.clear();

  for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
    if (it->first < 1000 && it->second.randomEligible == 1) {
      this->m_randomList.push_back(it->first);
    }
  }

  const auto count = this->m_randomList.size();
  this->SelectTip(this->m_randomList[static_cast<unsigned>(rand()) % count]);
  return this->m_currentEntry;
}

// OFFSET: 0x007ACC00
// FUNCTION: fn_09126
const TipBoxEntry& CTipBoxMgr::GetTip(int id) const {
  if (this->m_entries.find(id) != this->m_entries.end()) {
    return this->m_entries.find(id)->second;
  }

  return this->m_missing;
}

// OFFSET: 0x007ACC70
// FUNCTION: fn_09127
std::string CTipBoxMgr::Title(int id, std::string* abbreviated, int width) const {
  const std::string& title = this->GetTip(id).title;

  if (title.size() == 0) {
    return title;
  }

  std::size_t length = title.find("#");
  if (length == std::string::npos) {
    length = title.size();
  }
  std::string result = title.substr(0, length);

  if (abbreviated) {
    if (width > 10 && result.size() > static_cast<std::size_t>(width + 4)) {
      std::vector<std::string> lines;
      SplitTitle(result.c_str(), lines, width);
      *abbreviated = lines[0] + "...";
    } else {
      *abbreviated = result;
    }
  }

  return result;
}

// OFFSET: 0x007ACF50
// FUNCTION: fn_09128
void CTipBoxMgr::Sub7ACF50() {
  const TipBoxEntry& entry = this->m_currentEntry;

  if (entry.bitmapName.size() == 0) {
    this->m_image.kind = 2;
    return;
  }

  const int cell = entry.imageCell - 1;
  const float cellWidth = static_cast<float>(this->m_width) / 3.0f;
  const int row = cell / 3;
  const int column = cell % 3;
  const float cellHeight = static_cast<float>(this->m_height) / 3.0f;
  const std::size_t found = entry.bitmapName.find("유저인터페이스\\");

  this->m_image.x = static_cast<float>(column) * cellWidth + cellWidth * 0.5f;
  this->m_image.y = static_cast<float>(this->m_maximumId) + cellHeight * 0.5f + static_cast<float>(row) * cellHeight;

  if (found != std::string::npos) {
    this->m_imagePath = entry.bitmapName + ".bmp";

    const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_imagePath.c_str())));
    if (!bitmap) {
      return;
    }

    const float scale = static_cast<float>(entry.imageScalePercent) * 0.01f;
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
    this->m_imagePath = "sprite\\" + entry.bitmapName;

    const auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_imagePath + ".spr"));
    if (!sprite) {
      return;
    }

    const SpriteRleImage* image = sprite->images[0][0];
    if (!image) {
      return;
    }

    const float scale = static_cast<float>(entry.imageScalePercent) * 0.01f;
    const float ratio = static_cast<float>(entry.imageScalePercent) / 100.0f;
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

// INLINED
inline void CTipBoxMgr::AddTip(int id, std::string title, double flag, std::string text) {
  if (this->maxTipId < id) {
    this->maxTipId = id;
  }
  this->m_entries[id].title = title;
  this->m_entries[id].randomEligible = static_cast<int>(flag) == 1;
  this->m_entries[id].bitmapName = text;
}

int AddTip(lua_State* state);
int AddImgcoord(lua_State* state);
int AddPageEx(lua_State* state);
int AddPage(lua_State* state);
int AddIsTag(lua_State* state);

// OFFSET: 0x007AD480
// FUNCTION: fn_09129
bool CTipBoxMgr::RegisterLuaFunctions() {
  NLuaState state;
  if (!state.IsValid()) {
    return false;
  }

  luaL_openlibs(state.GetState());
  lua_register(state.GetState(), "AddTip", ::AddTip);
  lua_register(state.GetState(), "AddImgcoord", AddImgcoord);
  lua_register(state.GetState(), "AddPageEx", AddPageEx);
  lua_register(state.GetState(), "AddPage", AddPage);
  lua_register(state.GetState(), "AddIsTag", AddIsTag);

  const char source[] = {'\xF4', '\xCA', '\x76', '\xBD', '\xC0', '\x0E', '\xB7', '\x7F', '\xCE', '\xC3', '\x67', '\xA6', '\xDD', '\x4D', '\x87', '\x7E', '\xC6', '\xB3'};

  if (luaL_loadfile(state.GetState(), DecryptName<0x0BEB63A5C905B3A7ULL>(source)) != 0) {
    const char secondSource[] = {'\x9A', '\xCE', '\x56', '\xE3', '\xDE', '\x5E', '\xDF', '\x73', '\xA0', '\xC7', '\x47', '\xF8', '\xC3', '\x1D', '\xEF', '\x72', '\xAB', '\xB7'};

    if (luaL_loadfile(state.GetState(), DecryptName<0x078333BB9725B7C9ULL>(secondSource)) != 0) {
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

  const std::string message = std::string("error found :") + lua_tostring(state.GetState(), -1);
  MessageBoxA(nullptr, message.c_str(), "TipBox", MB_OK);
  return false;
}

// OFFSET: 0x007AD890
// FUNCTION: fn_09130
bool CTipBoxMgr::IsLastTip() const {
  return this->paragraphIndex == static_cast<int>(this->m_currentEntry.paragraphs.size()) - 1;
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

  for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
    const TipBoxEntry entry = it->second;

    if (!entry.randomEligible) {
      continue;
    }

    std::string title = entry.title;
    std::transform(title.begin(), title.end(), title.begin(), [](unsigned char c) { return static_cast<char>(toupper(c)); });
    std::transform(keyword.begin(), keyword.end(), keyword.begin(), [](unsigned char c) { return static_cast<char>(toupper(c)); });

    if (_mbsstr(reinterpret_cast<const unsigned char*>(title.c_str()), reinterpret_cast<const unsigned char*>(keyword.c_str()))) {
      this->m_randomList.push_back(it->first);
    }
  }

  return this->m_randomList;
}

// OFFSET: 0x007AE050
// FUNCTION: fn_09134
void CTipBoxMgr::SelectTip(int id) {
  this->paragraphIndex = 0;

  const auto it = this->m_entries.find(id);
  if (it != this->m_entries.end()) {
    this->currentTipId = id;
    this->m_currentEntry = it->second;
  }

  this->Sub7ACF50();
}

// OFFSET: 0x007AEAF0
// FUNCTION: fn_09145
int AddPage(lua_State* state) {
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "2st argment is must number");
    return 2;
  }
  if (!lua_isstring(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "1th argment is must string");
    return 2;
  }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  const char* text = lua_tostring(state, -1);
  CTipBoxMgr* owner = g_itemTagOwner;
  owner->AddParagraph(id, text);
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AE900
// FUNCTION: fn_09143
int AddImgcoord(lua_State* state) {
  if (!lua_isnumber(state, -1)) { lua_pushboolean(state, 0); lua_pushstring(state, "3nd argment is must number"); return 2; }
  if (!lua_isnumber(state, -2)) { lua_pushboolean(state, 0); lua_pushstring(state, "2nd argment is must number"); return 2; }
  if (!lua_isnumber(state, -3)) { lua_pushboolean(state, 0); lua_pushstring(state, "1st argment is must number"); return 2; }

  const int id = static_cast<int>(lua_tonumber(state, -3));
  const int x = static_cast<int>(lua_tonumber(state, -2));
  const int y = static_cast<int>(lua_tonumber(state, -1));
  g_itemTagOwner->SetImageCoord(id, x, y);
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AEA20
// FUNCTION: fn_09144
int AddIsTag(lua_State* state) {
  if (!lua_isnumber(state, -2)) { lua_pushboolean(state, 0); lua_pushstring(state, "2 argment is must number"); return 2; }
  if (!lua_isnumber(state, -1)) { lua_pushboolean(state, 0); lua_pushstring(state, "1 argment is must number"); return 2; }

  const int id = static_cast<int>(lua_tonumber(state, -2));
  g_itemTagOwner->SetSearchable(id, static_cast<int>(lua_tonumber(state, -1)) != 0);
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AEC00
// FUNCTION: fn_09146
int AddTip(lua_State* state) {
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
  const char* title = lua_tostring(state, -3);
  const double flag = lua_tonumber(state, -2);
  const char* text = lua_tostring(state, -1);
  g_itemTagOwner->AddTip(id, title, flag, text);
  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}

// OFFSET: 0x007AE600
// FUNCTION: fn_09142
int AddPageEx(lua_State* state) {
  if (!lua_isnumber(state, -1)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "8nd argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -2)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "7nd argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -3)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "6st argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -4)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "5st argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -5)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "4st argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -6)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "3st argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -7)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "2st argment is must number");
    return 2;
  }
  if (!lua_isnumber(state, -8)) {
    lua_pushboolean(state, 0);
    lua_pushstring(state, "1st argment is must number");
    return 2;
  }

  int id = static_cast<int>(lua_tonumber(state, -8));
  const int kind = static_cast<int>(lua_tonumber(state, -7));
  const double x = lua_tonumber(state, -6);
  const double y = lua_tonumber(state, -5);
  const double width = lua_tonumber(state, -4);
  const double height = lua_tonumber(state, -3);
  const double clipX = lua_tonumber(state, -2);
  const float scale = static_cast<float>(lua_tonumber(state, -1));
  if (kind != -1 && id > 0) {
    TipBoxImage image;
    image.kind = kind;
    image.blink = static_cast<int>(x);
    image.fromX = static_cast<int>(y);
    image.fromY = static_cast<int>(width);
    image.toX = static_cast<int>(height);
    image.toY = static_cast<int>(clipX);
    image.duration = scale;
    g_itemTagOwner->AddImage(id, image);
  }

  lua_pushboolean(state, 1);
  lua_pushstring(state, "good");
  return 2;
}
