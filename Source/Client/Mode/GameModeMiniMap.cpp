#include "Client/Mode/GameModeEnums.h"
#include <new>
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Input/Mouse.h"
#include "Client/Session/Session.h"
#include "Contents/Map/TownInfoMgr.h"
#include "Contents/Map/MapSkyMgr.h"
#include "File/File.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ClientAngles.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "UI/Items/UIIllustWnd.h"
#include "UI/Navigation/UIMiniMapWnd.h"
#include "UI/Navigation/UIMinimapZoomWnd.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <timeapi.h>
#include <windows.h>

namespace {

// INLINED
__forceinline const char* TimeFontActionName() {
  // GLOBAL: gl_25508
  OBFUSCATE_DECLARE_ARRAY("이팩트\\TimeFont.act", 0x7D5D0F110B6B8371ULL, name);
  return name.Text();
}

// INLINED
__forceinline const char* TimeFontSpriteName() {
  // GLOBAL: gl_25509
  OBFUSCATE_DECLARE_ARRAY("이팩트\\TimeFont.spr", 0x114901C7E37F6D49ULL, name);
  return name.Text();
}

}

// GLOBAL: gl_25107
CFile g_whisperFile;

// GLOBAL: gl_26022
float g_shadedColorScale = 128.0f;
// GLOBAL: gl_26029
float g_angleDivisor = 180.0f;
// GLOBAL: gl_26040
double g_miniMapTop = 16.0;
// GLOBAL: gl_26041
double g_miniMapCenterOffset = 64.0;
// GLOBAL: gl_26042
double g_miniMapBottom = 144.0;

namespace {
struct PrimitivePoolAccess : RenderPrimitivePool {
  // INLINED
  inline RenderPrimitive* AcquireLocal() {
    if (this->m_commands.end() == this->m_current) {
      this->m_commands.emplace_back();
      this->m_current = this->m_commands.end();
      return &this->m_commands.back();
    }

    RenderPrimitive* command = &*this->m_current;
    ++this->m_current;
    new (command) RenderPrimitive;

    return command;
  }
};
}

// OFFSET: 0x00CCFCE0
// FUNCTION: fn_21246
void MiniMapNoOp() {
}

// OFFSET: 0x00CCFF10
// FUNCTION: fn_21249
void CGameMode::CloseWhisperRecord() {
  if (g_session.m_whisperRecord != 0) {
    if (g_session.IsAdminSubMode(7) != 0) {
      char path[256];
      char backup[256];
      std::sprintf(path, "%s", g_whisperFile.Path());
      std::sprintf(backup, "%s.bak", path);
      g_whisperFile.Close();
    }
  }
}

// OFFSET: 0x00CD0100
// FUNCTION: fn_46330
__forceinline void CGameMode::StepTowardCell(const int& targetX, const int& targetY, int& x, int& y, int step) {
  if (targetX > x) {
    x += step;
  } else if (targetX < x) {
    x -= step;
  }
  if (targetY > y) {
    y += step;
  } else if (targetY < y) {
    y -= step;
  }
}

// OFFSET: 0x00CCFFA0
// FUNCTION: fn_21250
void CGameMode::StepTowardWalkableCell(const int& targetX, const int& targetY, int& x, int& y) {
  const int startX = x;
  const int startY = y;
  this->StepTowardCell(targetX, targetY, x, y, 1);

  const TerrainHeightCell* cell = this->scene->terrain->CellAt(x, y);
  if (cell != nullptr && cell->type != TERRAIN_CELL_BLOCKED && cell->type != TERRAIN_CELL_CLIFF) {
    return;
  }

  const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  for (int i = 0; i < 8; ++i) {
    x = startX + offsets[i][0];
    y = startY + offsets[i][1];
    cell = this->scene->terrain->CellAt(x, y);
    if (cell != nullptr && cell->type != TERRAIN_CELL_BLOCKED && cell->type != TERRAIN_CELL_CLIFF) {
      return;
    }
  }
}

// OFFSET: 0x00CD0150
// FUNCTION: fn_21251
void CGameMode::FindWalkableCellToward(float x, float z, float originX, float originZ, int& column, int& row, int distance) {
  const float deltaX = x - originX;
  const float deltaZ = z - originZ;
  const float inverse = 1.0f / SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  const float step = static_cast<float>(distance * 5);
  this->WorldCell(inverse * deltaX * step + originX, inverse * deltaZ * step + originZ, column, row);

  const int startY = row;
  const int startX = column;
  const TerrainHeightCell* cell = this->scene->terrain->CellAt(startX, startY);
  if (cell != nullptr && cell->type != TERRAIN_CELL_BLOCKED && cell->type != TERRAIN_CELL_CLIFF) {
    return;
  }

  const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  for (int i = 0; i < 8; ++i) {
    column = startX + offsets[i][0];
    row = startY + offsets[i][1];
    cell = this->scene->terrain->CellAt(column, row);
    if (cell != nullptr && cell->type != TERRAIN_CELL_BLOCKED && cell->type != TERRAIN_CELL_CLIFF) {
      return;
    }
  }
}

// OFFSET: 0x00CD0300
// FUNCTION: fn_21252
void CGameMode::RemoveMiniMapPing(std::uint32_t id) {
  const auto found = this->compassPosList.find(id);
  if (found != this->compassPosList.end()) {
    this->compassPosList.erase(found);
  }
}

// OFFSET: 0x00CD0380
// FUNCTION: fn_21253
void CGameMode::DrawMiniMapMembers(const TextureRegion& view) {
  int index = 0;
  if (g_windowMgr.miniMapWnd) {
    g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_MARK_COUNT, 0, this->partyPosList.size(), 0, 0);
    g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_MARK_COUNT, 1, this->guildPosList.size(), 0, 0);
  }

  for (auto it = this->partyPosList.begin(); it != this->partyPosList.end(); ++it) {
    this->DrawMiniMapMarker(view, it->second.x, it->second.y, 2, std::string(""), 2, it->second.color, false);
    if (g_windowMgr.miniMapWnd) {
      g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_MARK_POS, 0, index, it->second.x, it->second.y);
    }
    ++index;
  }

  for (auto it = this->guildPosList.begin(); it != this->guildPosList.end(); ++it) {
    if (this->partyPosList.find(it->first) == this->partyPosList.end()) {
      this->DrawMiniMapMarker(view, it->second.x, it->second.y, 2, std::string(""), 1, it->second.color, false);
      if (g_windowMgr.miniMapWnd) {
        g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_MARK_POS, 1, index, it->second.x, it->second.y);
      }
    }
    ++index;
  }

  for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
    if (it->second.questIconId != 0) {
      const std::string name = FormatOwnedString("유저인터페이스\\basic_interface\\quest_%d.bmp", it->second.questIconId);
      this->DrawMiniMapMarker(view, it->second.x, it->second.y, 4, name, 0, 0, true);
    }
  }

  if (g_session.storeSearchX > -1) {
    const int y = g_session.storeSearchY;
    if (y > -1) {
      this->DrawMiniMapMarker(view, g_session.storeSearchX, y, 4, "유저인터페이스\\basic_interface\\search_store.bmp", 0, 0, true);
    }
  }
}

// OFFSET: 0x00CD0840
// FUNCTION: fn_46331
__forceinline void CGameMode::DrawMiniMapPings(const TextureRegion& view) {
  if (this->compassPosList.size() != 0) {
    for (auto it = this->compassPosList.begin(); it != this->compassPosList.end();) {
      const std::uint32_t tick = it->second.tick;
      if (it->second.persistent == 0 && timeGetTime() > tick + 15000) {
        it = this->compassPosList.erase(it);
      } else {
        ++it;
      }
    }
    for (auto it = this->compassPosList.begin(); it != this->compassPosList.end(); ++it) {
      const std::uint32_t tick = it->second.tick;
      const int elapsed = timeGetTime() - tick;
      if (elapsed % 1000 < 500) {
        this->DrawMiniMapMarker(view, it->second.x, it->second.y, 1, std::string(""), 3, it->second.color, false);
      }
    }
  }
}

// OFFSET: 0x00CD0DD0
// FUNCTION: fn_21255
void CGameMode::DrawMiniMap() {
  if (!g_windowMgr.minimapZoomWnd) {
    return;
  }

  TextureRegion view;
  int reduction;
  {
    const float centerX = static_cast<float>(g_renderer->viewport.width - 144) + 64.0f;
    const int width = this->scene->terrain->width;
    const int height = this->scene->terrain->height;
    const CPlayer* player = this->scene->player;
    const float worldX = player->render.position.x;
    const float worldZ = player->render.position.z;
    int row;
    int column;
    this->WorldCell(worldX, worldZ, column, row);

    const float half = 1.0f / (g_windowMgr.minimapZoom * 2.0f);
    const float u = static_cast<float>(column) / static_cast<float>(width);
    const float v = static_cast<float>(height - row) / static_cast<float>(height);
    view.left = u - half;
    view.top = v - half;
    view.right = u + half;
    view.bottom = v + half;
    if (view.left < 0.0f) {
      view.left = 0.0f;
      view.right = half + half;
      if (view.right > 1.0f) {
        view.right = 1.0f;
      }
    }
    if (view.top < 0.0f) {
      view.top = 0.0f;
      view.bottom = half + half;
      if (view.bottom > 1.0f) {
        view.bottom = 1.0f;
      }
    }
    if (view.right > 1.0f) {
      view.right = 1.0f;
      view.left = 1.0f - (half + half);
      if (view.left < 0.0f) {
        view.left = 0.0f;
      }
    }
    if (view.bottom > 1.0f) {
      view.bottom = 1.0f;
      view.top = 1.0f - (half + half);
      if (view.top < 0.0f) {
        view.top = 0.0f;
      }
    }

    reduction = g_textureReduction;
    g_textureReduction = 1;
    CTexture* texture = g_texMgr.Load(reinterpret_cast<const char*>(this->minimapBmpName), 0, false, true, false);
    RECT rect;
    rect.left = static_cast<int>(centerX - 64.0f);
    rect.top = 16;
    rect.right = static_cast<int>(centerX + 64.0f);
    rect.bottom = 144;
    this->DrawTexture(rect, 0, 0.0998f, g_windowMgr.minimapColor, texture, view);

    this->DrawMiniMapMarker(view, column, row, 5, "유저인터페이스\\map\\map_arrow.bmp", 4, 0xFFFFFFFF, false);
  }

  for (auto it = g_miniMapEffectMarkers.begin(); it != g_miniMapEffectMarkers.end();) {
    this->DrawMiniMapMarker(view, it->second.x, it->second.y, 2, std::string(""), 2, 0xFFFF0000, false);
    if (--it->second.count <= 0) {
      it = g_miniMapEffectMarkers.erase(it);
    } else {
      ++it;
    }
  }

  if (this->bossAlarmAndCopyNameFlags[0]) {
    this->DrawMiniMapMarker(view, this->bossMonsterX, this->bossMonsterY, 6, "유저인터페이스\\map\\bossmonster.bmp", 5, 0, true);
  }

  if (g_windowMgr.minimapMarkersVisible) {
    for (int index = 0;; ++index) {
      CTownInfoMgr* townInfoMgr = g_townInfoMgr;
      const TownInfo info = townInfoMgr->GetTownInfo(std::string(this->mapName), index);
      if (info.image.size() == 0) {
        break;
      }
      if (info.type == 8) {
        break;
      }
      if (info.type == 9) {
        break;
      }
      this->DrawMiniMapMarker(view, info.x, info.y, 6, std::string(""), 6, index, true);
    }
  }

  this->DrawMiniMapMembers(view);

  this->DrawMiniMapPings(view);

  g_textureReduction = reduction;
}

// OFFSET: 0x00CD1450
// FUNCTION: fn_21256
void CGameMode::DrawMiniMapTriangle(float x, float y, float size, std::uint32_t color) {
  RenderPrimitive* primitive = static_cast<PrimitivePoolAccess&>(g_renderer->primitives).AcquireLocal();
  primitive->texture = g_texMgr.Default();
  RenderVertex* vertices = primitive->vertices;
  const float bottomZ = RhwToDepth(0.0999f);
  const float topZ = RhwToDepth(0.0999f);

  vertices[0].x = x;
  vertices[0].y = y - (size + size);
  vertices[1].x = x - SquareRoot(3.0f) * size;
  y += size;
  vertices[1].y = y;
  vertices[2].x = SquareRoot(3.0f) * size + x;
  vertices[2].y = y;
  vertices[3].x = x - SquareRoot(3.0f) * size;
  vertices[3].y = y;

  vertices[0].z = topZ;
  vertices[0].rhw = 0.0999f;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[0].diffuse = color;
  vertices[1].z = topZ;
  vertices[1].rhw = 0.0999f;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[1].diffuse = color;
  vertices[2].z = bottomZ;
  vertices[2].rhw = 0.0999f;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[2].diffuse = color;
  vertices[3].z = bottomZ;
  vertices[3].rhw = 0.0999f;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;
  vertices[3].diffuse = color;

  g_renderer->Submit(primitive, RSF_ALPHA | RSF_NO_DEPTH);
}

// OFFSET: 0x00CD2BE0
// FUNCTION: fn_21262
void CGameMode::DrawMiniMapMarker(
  const TextureRegion& view, int x, int y, int size, std::string texture, int kind, std::uint32_t color, bool clamp
) {
  // GLOBAL: gl_24930
  static int s_lastMarkerX;
  // GLOBAL: gl_24931
  static int s_lastMarkerY;
  // GLOBAL: gl_24932
  static int s_lastPlayerX;
  // GLOBAL: gl_24933
  static int s_lastPlayerY;

  const float viewWidth = view.right - view.left;
  const float scale = g_shadedColorScale;
  const float xF = static_cast<float>(x);
  const int width = this->scene->terrain->width;
  const int height = this->scene->terrain->height;
  const double originX = static_cast<double>(g_renderer->viewport.width - 144) + g_miniMapCenterOffset;
  const int longest = width >= height ? width : height;
  const float longestF = static_cast<float>(longest);
  const float widthF = static_cast<float>(width);
  const double mapWidth = widthF * scale / longestF;
  const float heightF = static_cast<float>(height);
  const double mapHeight = heightF * scale / longestF;
  const float yF = static_cast<float>(y);
  if (xF < 0.0f || yF < 0.0f) {
    return;
  }

  const double marginX = (scale - static_cast<float>(mapWidth)) * g_depthAverageHalf;
  const double marginY = (scale - static_cast<float>(mapHeight)) * g_depthAverageHalf;
  const float viewLeft = view.left * scale / viewWidth;
  const double offsetX = (xF * static_cast<float>(mapWidth) / widthF + marginX) / viewWidth;
  const float viewHeight = view.bottom - view.top;
  const float viewTop = view.top * scale / viewHeight;
  const double offsetY = (mapHeight - static_cast<double>(yF * static_cast<float>(mapHeight) / heightF) + marginY) / viewHeight;
  const double left = originX - g_miniMapCenterOffset;
  double drawX = (left - viewLeft) + offsetX;
  double drawY = (g_miniMapTop - viewTop) + offsetY;

  if (clamp) {
    int direction = 0;
    if (drawX - size < left) {
      direction = 1;
      drawX = size + left;
    } else if (drawX + size > originX + g_miniMapCenterOffset) {
      direction = 2;
      drawX = originX + g_miniMapCenterOffset - size;
    }
    if (drawY - size < g_miniMapTop) {
      drawY = size + g_miniMapTop;
      if (direction == 0) {
        direction = 3;
      }
    } else if (drawY + size > g_miniMapBottom) {
      drawY = g_miniMapBottom - size;
      if (direction == 0) {
        direction = 4;
      }
    }
    if (kind != 5 && direction != 0) {
      const std::string arrow = FormatOwnedString("유저인터페이스\\basic_interface\\quest_arrow%d.bmp", direction);
      texture = arrow;
    }
  } else if (kind != 4) {
    if (drawX - size < left || drawX + size > originX + g_miniMapCenterOffset || drawY - size < g_miniMapTop || drawY + size > g_miniMapBottom) {
      return;
    }
  }

  RECT rect;
  rect.left = static_cast<int>(drawX - size);
  rect.top = static_cast<int>(drawY - size);
  rect.right = static_cast<int>(size + drawX);
  rect.bottom = static_cast<int>(size + drawY);

  if (kind == 0) {
    CTexture* marker = g_texMgr.Load(texture.c_str(), 0, false, true, false);
    this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, marker, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
  } else if (kind == 5) {
    CTexture* marker = g_texMgr.Load(texture.c_str(), 0, false, true, false);
    this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, marker, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    if (g_windowMgr.miniMapWnd && (s_lastMarkerX != x || s_lastMarkerY != y)) {
      s_lastMarkerX = x;
      s_lastMarkerY = y;
      g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_BOSS_POS, x, y, 0, 0);
    }
  } else if (kind == 1) {
    this->DrawMiniMapTriangle(static_cast<float>(drawX), static_cast<float>(drawY), static_cast<float>(size + 1), 0xAAFFFFFF);
    this->DrawMiniMapTriangle(static_cast<float>(drawX), static_cast<float>(drawY), static_cast<float>(size), color);
  } else if (kind == 2) {
    RECT border;
    border.left = rect.left - 1;
    border.top = rect.top - 1;
    border.right = rect.right + 1;
    border.bottom = rect.bottom + 1;
    CTexture* white = g_texMgr.Default();
    this->DrawTexture(border, 0, 0.0999f, 0x99FFFFFF, white, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    white = g_texMgr.Default();
    this->DrawTexture(rect, 0, 0.0999f, color, white, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    if (g_windowMgr.miniMapWnd) {
      g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_MARK_POS, x, y, 0, 0);
    }
  } else if (kind == 3) {
    const double arm = size * 4;
    RECT horizontal;
    horizontal.left = static_cast<int>(drawX - arm);
    horizontal.top = rect.top;
    horizontal.right = static_cast<int>(arm + drawX);
    horizontal.bottom = rect.bottom;
    RECT vertical;
    vertical.left = rect.left;
    vertical.top = static_cast<int>(drawY - arm);
    vertical.right = rect.right;
    vertical.bottom = static_cast<int>(arm + drawY);
    CTexture* white = g_texMgr.Default();
    this->DrawTexture(horizontal, 0, 0.0999f, color, white, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    white = g_texMgr.Default();
    this->DrawTexture(vertical, 0, 0.0999f, color, white, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
  } else if (kind == 4) {
    const int angle = static_cast<int>(g_angleDivisor - this->scene->player->render.angle);
    RECT shadow;
    shadow.left = rect.left - 2;
    shadow.top = rect.top - 2;
    shadow.right = rect.right + 2;
    shadow.bottom = rect.bottom + 4;
    CTexture* marker = g_texMgr.Load(texture.c_str(), 0, false, true, false);
    this->DrawTexture(shadow, angle, 0.0999f, 0x77333333, marker, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    marker = g_texMgr.Load(texture.c_str(), 0, false, true, false);
    this->DrawTexture(rect, angle, 0.0999f, color, marker, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    if (s_lastPlayerX != x || s_lastPlayerY != y) {
      s_lastPlayerX = x;
      s_lastPlayerY = y;
      if (g_windowMgr.miniMapWnd) {
        g_windowMgr.miniMapWnd->SendMsg(nullptr, UM_MINIMAP_SET_PLAYER_POS, x, y, 0, 0);
      }
      if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.minimapZoomWnd)) {
        window->Invalidate();
      }
    }
  } else if (kind == 6) {
    if (texture.size() == 0) {
      g_windowMgr.LookupWindow(WID_MINIMAPZOOMWND)->SendMsg(nullptr, UM_COMMAND, ID_SHOW_MINIMAP_ZOOM_BUTTON, color, rect.left, rect.top);
    } else {
      g_windowMgr.LookupWindow(WID_MINIMAPZOOMWND)->SendMsg(nullptr, UM_COMMAND, ID_HIDE_MINIMAP_ZOOM_BUTTON, color, rect.left, rect.top);
      CTexture* marker = g_texMgr.Load(texture.c_str(), 0, false, true, false);
      this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, marker, TextureRegion{0.0f, 0.0f, 1.0f, 1.0f});
    }
  }
}

// OFFSET: 0x00CD1C50
// FUNCTION: fn_21258
void CGameMode::DrawMiniMapCoordinates() {
  const int rank = this->scene->player->pkRank;
  const int total = this->scene->player->pkTotal;
  if (total <= 0 || rank <= 0) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.spr"));

  int number = total;
  if (number > 9999) {
    number = 9999;
  }
  if (number < 0) {
    number = 0;
  }

  int count = 1;
  if (number > 999) {
    count = 4;
  } else if (number > 99) {
    count = 3;
  } else if (number > 9) {
    count = 2;
  }

  int digits[4];
  digits[0] = number % 10;
  number /= 10;
  digits[1] = number % 10;
  number /= 10;
  digits[2] = number % 10;
  number /= 10;
  digits[3] = number;

  int x = g_renderer->viewport.width - 80;
  int y = g_renderer->viewport.height - 30;
  for (int i = 0; i < count; ++i) {
    this->DrawActionSprite(x + 25 * count - i * 25, y, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
  }

  x = g_renderer->viewport.width - 80;
  y = g_renderer->viewport.height - 40;
  this->DrawActionSprite(x, y, action, sprite, 10, 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);

  number = rank;
  if (number < 0) {
    number = 0;
  }

  count = 1;
  if (number > 999) {
    count = 4;
  } else if (number > 99) {
    count = 3;
  } else if (number > 9) {
    count = 2;
  }

  digits[0] = number % 10;
  number /= 10;
  digits[1] = number % 10;
  number /= 10;
  digits[2] = number % 10;
  number /= 10;
  digits[3] = number;

  x = g_renderer->viewport.width - 80;
  y = g_renderer->viewport.height - 50;
  for (int i = 0; i < count; ++i) {
    this->DrawActionSprite(x - i * 25 - 25, y, action, sprite, digits[i], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
  }
}

// GLOBAL: gl_25044
int g_minimapCoordinateValue;
// GLOBAL: gl_25045
int g_minimapCoordinateDigits;
// GLOBAL: gl_25046
int g_minimapCoordinateDigitValues[4];

// OFFSET: 0x00CD2A10
// FUNCTION: fn_21261
void CGameMode::DrawMiniMapNumber() {
  if (this->skillUseType == SUT_NOSKILL || this->skillUseLevel == 0) {
    return;
  }

  const int number = this->skillUseLevel;
  if (number != g_minimapCoordinateValue) {
    g_minimapCoordinateValue = number;
    int value = number;
    if (value > 9999) {
      value = 9999;
    }
    if (value < 0) {
      value = 0;
    }
    g_minimapCoordinateDigits = 1;
    if (value > 999) {
      g_minimapCoordinateDigits = 4;
    } else if (value > 99) {
      g_minimapCoordinateDigits = 3;
    } else if (value > 9) {
      g_minimapCoordinateDigits = 2;
    }

    g_minimapCoordinateDigitValues[0] = value % 10;
    value /= 10;
    g_minimapCoordinateDigitValues[1] = value % 10;
    value /= 10;
    g_minimapCoordinateDigitValues[2] = value % 10;
    g_minimapCoordinateDigitValues[3] = value / 10;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\숫자.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\숫자.spr"));
  for (int index = 0; index < g_minimapCoordinateDigits; ++index) {
    const int x = static_cast<int>(
      static_cast<float>(g_mouse.x) + this->cursorOffsetX + 24.0f - static_cast<float>(index * 16) +
      static_cast<float>(g_minimapCoordinateDigits * 16 - 16)
    );
    const int y = static_cast<int>(static_cast<float>(g_mouse.y) + this->cursorOffsetY);
    this->DrawActionSprite(
    x, y, action, sprite, 0, g_minimapCoordinateDigitValues[index], 2.0f, 0.0f, 0xFAFFFFFF, 0x201
    );
  }
}

// GLOBAL: gl_25050
int g_timeAttackFrame;

// OFFSET: 0x00CD2960
// FUNCTION: fn_46332
__forceinline void CGameMode::DrawTimeAttackTitle() {
  CActRes* titleAction = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\TimeAttack.act"));
  CSprRes* titleSprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\TimeAttack.spr"));
  const int frame = g_timeAttackFrame / 3;
  if (frame < static_cast<int>(titleAction->FrameCount(0))) {
    const int titleX = g_renderer->viewport.width / 2 - titleSprite->images[0][0]->height / 2;
    this->DrawActionSprite(titleX, 120, titleAction, titleSprite, 0, frame, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    ++g_timeAttackFrame;
  }
}

// OFFSET: 0x00CD1EE0
// FUNCTION: fn_21259
void CGameMode::DrawTimeAttackTimer() {
  const int rank = this->scene->player->pkRank;
  const int total = this->scene->player->pkTotal;
  if (total <= 0 || rank <= 0) {
    return;
  }
  if (total <= 1 || rank != 1) {
    return;
  }

  this->DrawTimeAttackCountdown(g_session.m_timeAttackStart, TIME_ATTACK_ELAPSED);

  if (g_session.m_timeAttackStart != 0 && timeGetTime() - g_session.m_timeAttackStart < 3000) {
    this->DrawTimeAttackTitle();
  }
}

// OFFSET: 0x00CD2430
// FUNCTION: fn_21260
void CGameMode::DrawTimeAttackCountdown(std::uint32_t value, int mode) {
  std::uint32_t current = 0;
  switch (mode) {
    case TIME_ATTACK_STATIC:
      current = value;
      if (this->countdownStartTick == 0) {
        this->countdownStartTick = timeGetTime();
      }
      if (timeGetTime() - this->countdownStartTick > 5000) {
        this->countdownStartTick = 0;
        g_session.m_timeAttackVisible = 0;
      }
      break;
    case TIME_ATTACK_ELAPSED:
      current = timeGetTime() - value;
      if (value == 0) {
        g_session.m_timeAttackVisible = 0;
      }
      break;
    case TIME_ATTACK_REMAINING:
      current = value - timeGetTime();
      if (current == 0) {
        g_session.m_timeAttackVisible = 0;
      }
      break;
    case TIME_ATTACK_FRAME_COUNTDOWN:
      current = g_session.m_timeAttackCountdown / 30;
      --g_session.m_timeAttackCountdown;
      if (current <= 0) {
        g_session.m_timeAttackVisible = 0;
      }
      break;
    default:
      break;
  }

  int seconds = current / 1000;
  int minutes = 0;
  int hours = 0;
  int days = 0;
  int digits[16] = {};
  if (seconds >= 60) {
    minutes = seconds / 60;
    seconds = seconds - minutes * 60;
    if (minutes >= 60) {
      hours = minutes / 60;
      minutes = minutes - hours * 60;
    }
    if (hours >= 24) {
      days = hours / 24;
      hours = hours - days * 24;
    }
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(TimeFontActionName()));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(TimeFontSpriteName()));

  int number;
  if (mode == 3) {
    number = current;
    minutes = 0;
    hours = 0;
    days = 0;
  } else {
    number = seconds;
  }

  if (number > 9999) {
    number = 9999;
  }
  if (number < 0) {
    number = 0;
  }

  int count = 2;
  digits[0] = number % 10;
  number /= 10;
  digits[1] = number % 10;
  if (minutes > 0) {
    number = minutes;
    digits[3] = number % 10;
    number /= 10;
    digits[4] = number % 10;
    count = 4;
  }
  if (hours > 0) {
    number = hours;
    digits[6] = number % 10;
    number /= 10;
    digits[7] = number % 10;
    count = 6;
  }
  if (days > 0) {
    number = days;
    digits[9] = number % 10;
    number /= 10;
    digits[10] = number % 10;
    count = 8;
  }

  const int x = g_renderer->viewport.width / 2;
  const int y = 40;
  const int separators = (count - 2) / 2;
  count = count + separators;
  int start = count * 40 / 2 * -1;
  int j = count - 1;
  for (int i = 0; i < count; ++i, --j) {
    if ((i + 1) % 3 != 0) {
      this->DrawActionSprite(x + start, y, action, sprite, digits[j], 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    } else {
      this->DrawActionSprite(x + start, y, action, sprite, 10, 0, 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
    }
    start += 40;
  }
}

// GLOBAL: gl_25101
int g_miniMapTooltipId = -1;
// STRING: st_16849
char g_miniMapTooltipText[256];

// OFFSET: 0x00CD1690
// FUNCTION: fn_21257
void CGameMode::RefreshMiniMapTooltips() {
  if (this->miniMapTooltipId == -1) {
    if (this->miniMapTooltipWindows.size() == 0) {
      return;
    }

    if (this->miniMapTooltipText.size() == 0) {
      for (auto it = this->miniMapTooltipWindows.begin(), end = this->miniMapTooltipWindows.end(); it != end; ++it) {
        g_windowMgr.CloseWindow(it->first);
      }
      this->miniMapTooltipWindows.clear();
    } else {
      for (auto it = this->miniMapTooltipWindows.begin(), end = this->miniMapTooltipWindows.end(); it != end; ++it) {
        if (it->second.text == this->miniMapTooltipText) {
          g_windowMgr.CloseWindow(it->first);
          this->miniMapTooltipWindows.erase(it);
          return;
        }
      }
    }
    return;
  }

  if (this->miniMapTooltipId == g_miniMapTooltipId && std::strcmp(g_miniMapTooltipText, this->miniMapTooltipText.c_str()) == 0) {
    return;
  }

  g_miniMapTooltipId = this->miniMapTooltipId;
  std::sprintf(g_miniMapTooltipText, this->miniMapTooltipText.c_str());
  if (this->miniMapTooltipText.size() == 0) {
    return;
  }

  UIWindow* window = nullptr;
  for (int windowId = 216; windowId < 225; ++windowId) {
    if (this->miniMapTooltipWindows.find(windowId) == this->miniMapTooltipWindows.end()) {
      window = g_windowMgr.EnsureWindow(windowId);
      MiniMapTooltip tooltip{};
      tooltip.text = this->miniMapTooltipText;
      tooltip.windowId = windowId;
      this->miniMapTooltipWindows.insert({windowId, tooltip});
      break;
    }
  }

  int x;
  int y = 0;
  switch (this->miniMapTooltipId) {
    case 10:
      g_windowMgr.CycleWindowVisibilityTwice();
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      window->SendMsg(nullptr, UM_SETINFO, 10, 0, 0, 0);
      x = g_renderer->viewport.centerX - window->geometry.width / 2;
      y = g_renderer->viewport.centerY - window->geometry.height / 2;
      break;
    case -1:
    case 11:
      g_windowMgr.RestoreHiddenWindows();
      this->miniMapTooltipWindows.clear();
      this->miniMapTooltipId = -1;
    default:
      x = 160;
      break;
    case 12:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = 0;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
    case 13:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = g_renderer->viewport.centerX - window->geometry.width / 2;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
    case 14:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = g_renderer->viewport.width - window->geometry.width;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
  }

  window->Move(x, y);
}

// OFFSET: 0x00CD0B30
// FUNCTION: fn_21254
void CGameMode::UpdateMiniMapTooltip() {
  int id = this->miniMapTooltipId;
  if (id > 9) {
    this->RefreshMiniMapTooltips();
    id = this->miniMapTooltipId;
    if (id == -1 && g_windowMgr.illustWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_ILLUSTWND);
    }
    return;
  }

  if (id == -1) {
    this->RefreshMiniMapTooltips();
    id = this->miniMapTooltipId;
    if (id == -1 && g_windowMgr.illustWnd != nullptr) {
      g_windowMgr.CloseWindow(WID_ILLUSTWND);
    }
    return;
  }

  if (this->miniMapTooltipText.size() != 0) {
    UIWindow* window = g_windowMgr.illustWnd;
    if (window == nullptr) {
      window = g_windowMgr.EnsureWindow(WID_ILLUSTWND);
      g_miniMapTooltipId = this->miniMapTooltipId;
      std::sprintf(g_miniMapTooltipText, this->miniMapTooltipText.c_str());
    } else {
      if (id == g_miniMapTooltipId && std::strcmp(g_miniMapTooltipText, this->miniMapTooltipText.c_str()) == 0) {
        return;
      }
      g_miniMapTooltipId = id;
      std::sprintf(g_miniMapTooltipText, this->miniMapTooltipText.c_str());
    }

    int x = 160;
    int y = 0;
    window->SendMsg(nullptr, UM_SETINFO, -1, 0, 0, 0);

    switch (this->miniMapTooltipId) {
    case ILLUSTRATION_BOTTOM_LEFT_INSET:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = 200;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
    case ILLUSTRATION_BOTTOM_CENTER:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = g_renderer->viewport.centerX;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
    case ILLUSTRATION_BOTTOM_RIGHT_INSET:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      x = g_renderer->viewport.width - 200;
      y = g_renderer->viewport.height - window->geometry.height;
      break;
    case ILLUSTRATION_CENTER_TITLED:
      window->SendMsg(nullptr, UM_SETINFO, -2, 0, 0, 0);
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      window->SendMsg(nullptr, UM_SETINFO, 3, 0, 0, 0);
      x = g_renderer->viewport.centerX;
      y = g_renderer->viewport.centerY - window->geometry.height / 2;
      break;
    case ILLUSTRATION_CENTER_CLOSABLE:
      window->SendMsg(nullptr, UM_SET_BITMAP, reinterpret_cast<std::intptr_t>(this->miniMapTooltipText.c_str()), 0, 0, 0);
      window->SendMsg(nullptr, UM_SETINFO, 4, 0, 0, 0);
      x = g_renderer->viewport.centerX;
      y = g_renderer->viewport.centerY - window->geometry.height / 2;
      break;
    default:
      break;
    }

    window->Move(x - window->geometry.width / 2, y);
  }
}
