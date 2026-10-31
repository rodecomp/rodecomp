#include "Contents/WorldView/WorldViewData.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

#include <cstring>

// INLINED
static bool AreaContains(int left, int top, int right, int bottom, int x, int y) {
  return left < x && x < right && top < y && y < bottom;
}

// OFFSET: 0x00612C70
// FUNCTION: fn_04345
CWorldViewWorld::CWorldViewWorld(const char* name, const char* imageName) {
  this->name = name;
  this->imageName = imageName;
}

// OFFSET: 0x00613050
// FUNCTION: fn_04346
CWorldViewField::CWorldViewField(WorldViewArea area, const char* name, const char* unknown2, const char* unknown3) {
  this->left = area.left;
  this->top = area.top;
  this->right = area.right;
  this->bottom = area.bottom;
  this->unknown_0010 = area.unknown_0010;
  if (name) {
    this->name = name;
  }
  if (unknown2) {
    this->unknown_002C = unknown2;
  }
  if (unknown3) {
    this->unknown_0044 = unknown3;
  }
}

// OFFSET: 0x00613360
// FUNCTION: fn_04349
CWorldViewWorld::~CWorldViewWorld() {
  for (std::vector<CWorldViewMap*>::iterator it = this->maps.begin(); it != this->maps.end(); ++it) {
    CWorldViewMap* map = *it;
    if (map) {
      for (std::vector<CWorldViewField*>::iterator field = map->fields.begin(); field != map->fields.end(); ++field) {
        if (*field) {
          delete *field;
        }
      }
      map->fields.clear();
      delete map;
    }
  }
  this->maps.clear();
}

// OFFSET: 0x006135C0
// FUNCTION: fn_04350
void CWorldViewWorld::AddField(int mapId, const char* name, CWorldViewField* field) {
  CWorldViewMap* map = nullptr;
  for (std::vector<CWorldViewMap*>::iterator it = this->maps.begin(); it != this->maps.end(); ++it) {
    if ((*it)->id == mapId) {
      map = *it;
      break;
    }
  }

  if (!map) {
    map = new CWorldViewMap;
    if (name) {
      map->name = name;
      map->displayName = name;
    }
    map->id = mapId;
    map->left = field->left;
    map->top = field->top;
    map->right = field->right;
    map->bottom = field->bottom;
    map->unknown_0044 = field->unknown_0010;
    this->maps.push_back(map);
  }

  if (field->left < map->left) {
    map->left = field->left;
  }
  if (field->top < map->top) {
    map->top = field->top;
  }
  if (map->right < field->right) {
    map->right = field->right;
  }
  if (map->bottom < field->bottom) {
    map->bottom = field->bottom;
  }
  map->fields.push_back(field);
}

// OFFSET: 0x006137B0
// FUNCTION: fn_04351
bool CWorldViewWorld::LoadTexture() {
  std::string path = "유저인터페이스\\";
  path += this->imageName;
  this->texture = g_texMgr.Load(path.c_str(), 0, true, true, false);
  return this->texture != nullptr;
}

// OFFSET: 0x00613AB0
// FUNCTION: fn_04352
CWorldViewField* CWorldViewWorld::FieldAt(unsigned int& mapIndex, int x, int y) {
  const unsigned int count = this->maps.size();
  for (unsigned int index = 0; index < count; ++index) {
    const CWorldViewMap* map = this->maps[index];
    bool hit;
    if (map->id < 100 && !map->fields.empty()) {
      const CWorldViewField* first = map->fields.front();
      hit = AreaContains(first->left, first->top, first->right, first->bottom, x, y);
    } else {
      hit = AreaContains(map->left, map->top, map->right, map->bottom, x, y);
    }

    if (hit) {
      mapIndex = index;
      CWorldViewMap* found = this->maps[index];
      if (!found) {
        return nullptr;
      }
      for (std::vector<CWorldViewField*>::iterator it = found->fields.begin(); it != found->fields.end(); ++it) {
        CWorldViewField* field = *it;
        if (AreaContains(field->left, field->top, field->right, field->bottom, x, y)) {
          return field;
        }
      }
      return nullptr;
    }
  }

  return nullptr;
}

// OFFSET: 0x00613B80
// FUNCTION: fn_04353
CWorldViewField* CWorldViewWorld::FindField(unsigned int& mapIndex, const char* name) {
  const unsigned int count = this->maps.size();
  for (unsigned int index = 0; index < count; ++index) {
    CWorldViewMap* map = this->maps[index];
    for (std::vector<CWorldViewField*>::iterator it = map->fields.begin(); it != map->fields.end(); ++it) {
      CWorldViewField* field = *it;
      if (std::strcmp(field->name.c_str(), name) == 0) {
        if (field) {
          mapIndex = index;
          return field;
        }
        break;
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00613C30
// FUNCTION: fn_04354
CWorldViewField* CWorldViewWorld::FindField(const char* name) {
  for (std::vector<CWorldViewMap*>::iterator it = this->maps.begin(); it != this->maps.end(); ++it) {
    if (CWorldViewField* field = (*it)->FindField(name)) {
      return field;
    }
  }

  return nullptr;
}

// OFFSET: 0x00613CB0
// FUNCTION: fn_04355
CWorldViewField* CWorldViewMap::FindField(const char* name) {
  for (std::vector<CWorldViewField*>::iterator it = this->fields.begin(); it != this->fields.end(); ++it) {
    CWorldViewField* field = *it;
    if (std::strcmp(field->name.c_str(), name) == 0) {
      return field;
    }
  }

  return nullptr;
}

// OFFSET: 0x00613DD0
// FUNCTION: fn_04356
CWorldViewMap* CWorldViewWorld::FindMap(int id) {
  for (std::vector<CWorldViewMap*>::iterator it = this->maps.begin(); it != this->maps.end(); ++it) {
    if (id == (*it)->id) {
      return *it;
    }
  }

  return nullptr;
}

// OFFSET: 0x00613E00
// FUNCTION: fn_04357
CWorldViewMap* CWorldViewWorld::MapAt(unsigned int& mapIndex, int x, int y) {
  const unsigned int count = this->maps.size();
  for (unsigned int index = 0; index < count; ++index) {
    const CWorldViewMap* map = this->maps[index];
    if (map->id < 100) {
      if (map->fields.empty()) {
        if (AreaContains(map->left, map->top, map->right, map->bottom, x, y)) {
          mapIndex = index;
          return this->maps[index];
        }
      } else {
        const CWorldViewField* first = map->fields.front();
        if (AreaContains(first->left, first->top, first->right, first->bottom, x, y)) {
          mapIndex = index;
          return this->maps[index];
        }
      }
    }
  }

  return nullptr;
}

// OFFSET: 0x00613F10
// FUNCTION: fn_04358
void CWorldViewWorld::Draw(int, int x, int y, int width, int height) {
  std::string path = "유저인터페이스\\";
  path += this->imageName;
  this->texture = g_texMgr.Load(path.c_str(), 0, true, true, false);
  this->texture->Stretch(x, y, width, height);
}

// OFFSET: 0x006145E0
// FUNCTION: fn_04367
CWorldViewMgr::CWorldViewMgr() {
  this->screenScale = 0;
  this->dungeonColor = 0;
  this->unknown_0084 = 0;
  this->dungeonFontColor = 0xFFFFFFFF;
  this->dungeonFontOutline = 0xFF000000;
  this->dungeonSelectColor = 0xFFFFFFFF;
  this->unknown_00A4 = 0xFFFFFFFF;
  this->mobLevelColor = 0xFFFFFFFF;
  this->mobLevelOutline = 0xFF000000;
  this->mobColor = 0xFFFFFFFF;
  this->mobOutline = 0xFF000000;
  this->boxLineColor = 0x282828;

  const std::string base = "유저인터페이스\\worldview_interface\\wv_Sreach";
  for (int index = 0; index < 16; ++index) {
    this->searchColors[index] = 0xEEFFFFFF;
    this->searchImages[index] = base + "NoImage.bmp";
  }

  this->searchImages[0] = base + "Map.bmp";
  this->searchImages[1] = base + "Mob1.bmp";
  this->searchImages[2] = base + "Mob2.bmp";
  this->searchImages[3] = base + "Mob3.bmp";
  this->searchImages[4] = base + "Mob4.bmp";
  this->searchImages[5] = base + "Mob5.bmp";
  this->searchImages[10] = base + "Npc.bmp";
  this->searchImages[11] = base + "Npc_Mob1.bmp";
  this->searchImages[12] = base + "Npc_Mob2.bmp";
  this->searchImages[13] = base + "Npc_Mob3.bmp";
  this->searchImages[14] = base + "Npc_Mob4.bmp";
  this->searchImages[15] = base + "Npc_Mob5.bmp";

  this->runColor = 0;
  this->runFontColor = 0xFF000000;
  for (int index = 0; index < 4; ++index) {
    this->dungeonFontSize[index] = 10;
    this->mobLevelFont[index] = 10;
    this->mobFont[index] = 10;
    this->runFont[index] = 10;
  }
}

// INLINED
static void ScaleArea(int* area, int textureWidth, int textureHeight) {
  if (!area) {
    return;
  }

  const float scaleX = static_cast<float>(g_renderer->viewport.width) / static_cast<float>(textureWidth);
  const float scaleY = static_cast<float>(g_renderer->viewport.height - 17) / static_cast<float>(textureHeight);
  area[0] = static_cast<int>(static_cast<float>(area[0]) * scaleX + 0.5);
  area[1] = static_cast<int>(static_cast<float>(area[1]) * scaleY + 0.5) + 17;
  area[2] = static_cast<int>(static_cast<float>(area[2]) * scaleX + 0.5);
  area[3] = static_cast<int>(static_cast<float>(area[3]) * scaleY + 0.5) + 17;
}

// OFFSET: 0x006166E0
// FUNCTION: fn_04374
bool CWorldViewMgr::ScaleToScreen() {
  for (std::vector<CWorldViewWorld*>::iterator it = this->worlds.begin(); it != this->worlds.end(); ++it) {
    CWorldViewWorld* world = *it;
    if (!world->texture) {
      continue;
    }

    const int textureWidth = world->texture->Width();
    const int textureHeight = world->texture->Height();
    for (std::vector<CWorldViewMap*>::iterator map = world->maps.begin(); map != world->maps.end(); ++map) {
      ScaleArea(&(*map)->left, textureWidth, textureHeight);
      for (std::vector<CWorldViewField*>::iterator field = (*map)->fields.begin(); field != (*map)->fields.end(); ++field) {
        if (*field) {
          ScaleArea(&(*field)->left, textureWidth, textureHeight);
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00616970
// FUNCTION: fn_04376
unsigned int CWorldViewMgr::FindWorld(int mapId) {
  std::string name;
  if (mapId == -1) {
    CMode* mode = g_modeMgr.GetGameMode();
    if (!mode) {
      return 0;
    }
    name = std::string(reinterpret_cast<const char*>(mode->SendMsg(9, 0, 0, 0, 0)));
  }

  for (unsigned int index = 0; index < this->worlds.size(); ++index) {
    if (this->worlds[index]->FindField(name.c_str())) {
      return index;
    }
  }

  return 0;
}

// OFFSET: 0x00616C20
// FUNCTION: fn_04377
void CWorldViewMgr::ResetSelection() {
  this->selection.worldIndex = 0;
  this->selection.mapIndex = 0;
  this->selection.name.clear();
  this->selection.left = 0;
  this->selection.top = 0;
  this->selection.right = 0;
  this->selection.bottom = 0;
  this->selection.unknown_004C.clear();
}

// OFFSET: 0x00616CC0
// FUNCTION: fn_04378
bool CWorldViewMgr::SelectField(const char* name) {
  this->ResetSelection();
  if (!name) {
    return false;
  }

  unsigned int mapIndex = static_cast<unsigned int>(-1);
  for (unsigned int index = 0; index < this->worlds.size(); ++index) {
    CWorldViewField* field = this->worlds[index]->FindField(mapIndex, name);
    if (field && this->worlds[index]->maps[mapIndex]->id > 99) {
      this->selection.mapIndex = mapIndex;
      this->selection.worldIndex = index;
      this->selection.name = field->name;
      this->selection.left = field->left;
      this->selection.top = field->top;
      this->selection.right = field->right;
      this->selection.bottom = field->bottom;
      this->selection.unknown_0048 = field->unknown_0010;
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00616D80
// FUNCTION: fn_04379
void CWorldViewMgr::Draw(int unused, int x, int y, int width, int height) {
  if (this->currentWorld < 0) {
    this->currentWorld = 0;
  }

  const unsigned int count = this->worlds.size();
  if (count <= static_cast<unsigned int>(this->currentWorld)) {
    this->currentWorld = count - 1;
  }

  this->worlds[this->currentWorld]->Draw(unused, x, y, width, height);
}

// OFFSET: 0x00616DD0
// FUNCTION: fn_04380
bool CWorldViewMgr::AddMark(const char* name) {
  if (!name) {
    return false;
  }

  {
    const std::string key(name);
    for (std::vector<WorldViewRecord>::iterator it = this->marks.begin(); it != this->marks.end(); ++it) {
      if (it->name == key) {
        return true;
      }
    }
  }

  unsigned int mapIndex = static_cast<unsigned int>(-1);
  for (unsigned int index = 0; index < this->worlds.size(); ++index) {
    CWorldViewField* field = this->worlds[index]->FindField(mapIndex, name);
    if (field && this->worlds[index]->maps[mapIndex]->id > 99) {
      WorldViewRecord mark;
      mark.mapIndex = mapIndex;
      mark.level = 0;
      mark.worldIndex = index;
      mark.name = field->name;
      mark.unknown_0048 = field->unknown_0010;
      mark.left = field->left;
      mark.top = field->top;
      mark.right = field->right;
      mark.bottom = field->bottom;
      this->marks.push_back(mark);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00617070
// FUNCTION: fn_04381
void CWorldViewMgr::ClearMarks() {
  this->marks.clear();
}

// OFFSET: 0x006171D0
// FUNCTION: fn_04382
WorldViewRecord* CWorldViewMgr::MarkAt(unsigned int index, bool currentWorldOnly) {
  if (static_cast<int>(index) >= 0 && index < this->marks.size()) {
    WorldViewRecord* mark = &this->marks[index];
    if (!currentWorldOnly || mark->worldIndex == this->currentWorld) {
      return mark;
    }
  }

  return nullptr;
}

// OFFSET: 0x00617220
// FUNCTION: fn_04383
int* CWorldViewMgr::MarkAreaAt(unsigned int index, bool currentWorldOnly) {
  if (static_cast<int>(index) >= 0 && index < this->marks.size()) {
    WorldViewRecord* mark = &this->marks[index];
    if (!currentWorldOnly || mark->worldIndex == this->currentWorld) {
      return &mark->left;
    }
  }

  return nullptr;
}

// OFFSET: 0x00617270
// FUNCTION: fn_04384
int CWorldViewMgr::MarkCount() {
  return static_cast<int>(this->marks.size());
}

// OFFSET: 0x00617290
// FUNCTION: fn_04385
void CWorldViewMgr::SelectCurrentWorld() {
  this->currentWorld = this->FindWorld(-1);
}

// OFFSET: 0x006172E0
// FUNCTION: fn_04386
bool CWorldViewMgr::AddLevel(const char* name, int level) {
  if (!name) {
    return false;
  }

  {
    const std::string key(name);
    for (std::vector<WorldViewRecord>::iterator it = this->levels.begin(); it != this->levels.end(); ++it) {
      if (it->name == key) {
        if (level != 0) {
          const int current = it->level;
          if (level == 10000) {
            if (current < 10000) {
              it->level = current + 10000;
            }
          } else if (current < 10000) {
            if (current < level) {
              it->level = level;
            }
          } else if (current - 10000 < level) {
            it->level = level + 10000;
          }
        }
        return true;
      }
    }
  }

  unsigned int mapIndex = static_cast<unsigned int>(-1);
  for (unsigned int index = 0; index < this->worlds.size(); ++index) {
    CWorldViewField* field = this->worlds[index]->FindField(mapIndex, name);
    if (field && this->worlds[index]->maps[mapIndex]->id > 99) {
      WorldViewRecord record;
      record.mapIndex = mapIndex;
      record.worldIndex = index;
      record.name = field->name;
      record.unknown_0048 = field->unknown_0010;
      record.left = field->left;
      record.top = field->top;
      record.right = field->right;
      record.bottom = field->bottom;
      record.level = level;
      this->levels.push_back(record);
      return true;
    }
  }

  return false;
}

// OFFSET: 0x006175D0
// FUNCTION: fn_04387
void CWorldViewMgr::ClearLevels() {
  this->levels.clear();
}

// OFFSET: 0x00617730
// FUNCTION: fn_04388
int* CWorldViewMgr::LevelAreaAt(int& kind, unsigned int index, bool currentWorldOnly) {
  if (static_cast<int>(index) >= 0 && index < this->levels.size() && (!currentWorldOnly || this->levels[index].worldIndex == this->currentWorld)) {
    WorldViewRecord& record = this->levels[index];
    int base = 10;
    int value = record.level;
    if (value < 10000) {
      base = kind;
    }
    value %= 10000;

    if (value >= 100) {
      kind = base + 5;
    } else if (value >= 50) {
      kind = base + 4;
    } else if (value >= 20) {
      kind = base + 3;
    } else if (value >= 5) {
      kind = base + 2;
    } else {
      kind = base + (value >= 1 ? 1 : 0);
    }
    return &record.left;
  }

  return nullptr;
}

// OFFSET: 0x00617860
// FUNCTION: fn_04389
int CWorldViewMgr::LevelCount() {
  return static_cast<int>(this->levels.size());
}

// OFFSET: 0x00617940
// FUNCTION: fn_04390
CWorldViewWorld* CWorldViewMgr::CurrentWorld() {
  return this->worlds[this->currentWorld];
}

// OFFSET: 0x00617950
// FUNCTION: fn_04391
bool IsMapDisplayNameTranslated(const char* mapName) {
  const std::string placeholder = LookupMessage(0xBB);
  const std::string displayName = g_session.GetMapDisplayName(mapName);
  return placeholder != displayName;
}
