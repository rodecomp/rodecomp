#include "Contents/WorldView/WorldViewData.h"
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
