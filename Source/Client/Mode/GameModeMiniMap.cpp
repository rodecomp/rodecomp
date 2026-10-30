#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Render/ActionSpriteDrawing.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>

// GLOBAL: gl_24934
int g_searchStoreX;
// GLOBAL: gl_24935
int g_searchStoreY;

// OFFSET: 0x00CCFCE0
// FUNCTION: fn_21246
void MiniMapNoOp() {
}

// OFFSET: 0x00CCFFA0
// FUNCTION: fn_21250
void CGameMode::StepTowardWalkableCell(const int& targetX, const int& targetY, int& x, int& y) {
  const int startX = x;
  const int startY = y;
  if (targetX > startX) {
    x = startX + 1;
  } else if (targetX < startX) {
    x = startX - 1;
  }
  if (targetY > y) {
    ++y;
  } else if (targetY < y) {
    --y;
  }

  if (this->scene->terrain->Grid().Walkable(x, y)) {
    return;
  }

  const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  for (int i = 0; i < 8; ++i) {
    x = startX + offsets[i][0];
    y = startY + offsets[i][1];
    if (this->scene->terrain->Grid().Walkable(x, y)) {
      return;
    }
  }
}

// OFFSET: 0x00CD0150
// FUNCTION: fn_21251
void CGameMode::FindWalkableCellToward(float x, float z, float originX, float originZ, int& column, int& row, int distance) {
  const float deltaZ = z - originZ;
  const float deltaX = x - originX;
  const float inverse = 1.0f / SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  const float step = static_cast<float>(distance * 5);
  this->WorldCell(inverse * deltaX * step + originX, inverse * deltaZ * step + originZ, column, row);

  const int startY = row;
  const int startX = column;
  if (this->scene->terrain->Grid().Walkable(startX, startY)) {
    return;
  }

  const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  for (int i = 0; i < 8; ++i) {
    column = startX + offsets[i][0];
    row = startY + offsets[i][1];
    if (this->scene->terrain->Grid().Walkable(column, row)) {
      return;
    }
  }
}

// OFFSET: 0x00CD0300
// FUNCTION: fn_21252
void CGameMode::RemoveMiniMapPing(std::uint32_t id) {
  const auto found = this->unknown_01C4.find(id);
  if (found != this->unknown_01C4.end()) {
    this->unknown_01C4.erase(found);
  }
}

// OFFSET: 0x00CD0380
// FUNCTION: fn_21253
void CGameMode::DrawMiniMapMembers(const TextureRegion& view) {
  int index = 0;
  if (g_windowMgr.window_4F1CC) {
    g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E4, 0, this->unknown_01B4.size(), 0, 0);
    g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E4, 1, this->unknown_01BC.size(), 0, 0);
  }

  for (auto it = this->unknown_01B4.begin(); it != this->unknown_01B4.end(); ++it) {
    this->DrawMiniMapMarker(view, it->second.x, it->second.y, 2, std::string(), 2, it->second.color, false);
    if (g_windowMgr.window_4F1CC) {
      g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E3, 0, index, it->second.x, it->second.y);
    }
    ++index;
  }

  for (auto it = this->unknown_01BC.begin(); it != this->unknown_01BC.end(); ++it) {
    if (this->unknown_01B4.find(it->first) == this->unknown_01B4.end()) {
      this->DrawMiniMapMarker(view, it->second.x, it->second.y, 2, std::string(), 1, it->second.color, false);
      if (g_windowMgr.window_4F1CC) {
        g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E3, 1, index, it->second.x, it->second.y);
      }
    }
    ++index;
  }

  for (auto it = this->minimapMarkers.begin(); it != this->minimapMarkers.end(); ++it) {
    if (it->second.unknown_0006 != 0) {
      const std::string name = FormatOwnedString("유저인터페이스\\basic_interface\\quest_%d.bmp", it->second.unknown_0006);
      this->DrawMiniMapMarker(view, it->second.unknown_0000, it->second.unknown_0002, 4, name, 0, 0, true);
    }
  }

  if (g_searchStoreX > -1) {
    const int y = g_searchStoreY;
    if (y > -1) {
      this->DrawMiniMapMarker(view, g_searchStoreX, y, 4, "유저인터페이스\\basic_interface\\search_store.bmp", 0, 0, true);
    }
  }
}

// OFFSET: 0x00CD1450
// FUNCTION: fn_21256
void CGameMode::DrawMiniMapTriangle(float x, float y, float size, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  primitive->texture = g_texMgr.Default();
  RenderVertex* vertices = primitive->vertices;
  const float bottomZ = RhwToDepth(0.0999f);
  const float topZ = RhwToDepth(0.0999f);

  vertices[0].x = x;
  vertices[0].y = y - (size + size);
  const float bottom = y + size;
  vertices[1].x = x - SquareRoot(3.0f) * size;
  vertices[1].y = bottom;
  vertices[2].x = SquareRoot(3.0f) * size + x;
  vertices[2].y = bottom;
  vertices[3].x = x - SquareRoot(3.0f) * size;
  vertices[3].y = bottom;

  vertices[0].z = topZ;
  vertices[0].rhw = 0.0999f;
  vertices[0].diffuse = color;
  vertices[0].u = 0.0f;
  vertices[0].v = 0.0f;
  vertices[1].z = topZ;
  vertices[1].rhw = 0.0999f;
  vertices[1].diffuse = color;
  vertices[1].u = 1.0f;
  vertices[1].v = 0.0f;
  vertices[2].z = bottomZ;
  vertices[2].rhw = 0.0999f;
  vertices[2].diffuse = color;
  vertices[2].u = 0.0f;
  vertices[2].v = 1.0f;
  vertices[3].z = bottomZ;
  vertices[3].rhw = 0.0999f;
  vertices[3].diffuse = color;
  vertices[3].u = 1.0f;
  vertices[3].v = 1.0f;

  g_renderer->queues.Submit(primitive, 0x201, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
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

  const int width = this->scene->terrain->width;
  const int height = this->scene->terrain->height;
  const int longest = width >= height ? width : height;
  const double originX = static_cast<double>(g_renderer->viewport.width - 144) + 64.0;
  const float mapWidth = static_cast<float>(width) * 128.0f / static_cast<float>(longest);
  const float mapHeight = static_cast<float>(height) * 128.0f / static_cast<float>(longest);
  if (static_cast<float>(x) < 0.0f || static_cast<float>(y) < 0.0f) {
    return;
  }

  const float viewWidth = view.right - view.left;
  const float viewHeight = view.bottom - view.top;
  const double left = originX - 64.0;
  const double offsetY = (static_cast<double>(mapHeight) - static_cast<double>(static_cast<float>(y) * mapHeight / static_cast<float>(height)) +
                          static_cast<double>((128.0f - mapHeight) * 0.5f)) /
                         static_cast<double>(viewHeight);
  double drawX = (left - static_cast<double>(view.left * 128.0f / viewWidth)) +
                 (static_cast<double>(static_cast<float>(x) * mapWidth / static_cast<float>(width)) +
                  static_cast<double>((128.0f - mapWidth) * 0.5f)) /
                   static_cast<double>(viewWidth);
  double drawY = (16.0 - static_cast<double>(view.top * 128.0f / viewHeight)) + offsetY;

  if (clamp) {
    int direction = 0;
    if (drawX - size < left) {
      direction = 1;
      drawX = size + left;
    } else if (drawX + size > originX + 64.0) {
      direction = 2;
      drawX = (originX + 64.0) - size;
    }
    if (drawY - size < 16.0) {
      drawY = size + 16.0;
      if (direction == 0) {
        direction = 3;
      }
    } else if (drawY + size > 144.0) {
      drawY = 144.0 - size;
      if (direction == 0) {
        direction = 4;
      }
    }
    if (kind != 5 && direction != 0) {
      const std::string arrow = FormatOwnedString("유저인터페이스\\basic_interface\\quest_arrow%d.bmp", direction);
      texture = arrow;
    }
  } else if (kind != 4) {
    if (drawX - size < left || drawX + size > originX + 64.0 || drawY - size < 16.0 || drawY + size > 144.0) {
      return;
    }
  }

  RECT rect;
  rect.left = static_cast<int>(drawX - size);
  rect.top = static_cast<int>(drawY - size);
  rect.right = static_cast<int>(size + drawX);
  rect.bottom = static_cast<int>(size + drawY);
  const TextureRegion region = {0.0f, 0.0f, 1.0f, 1.0f};

  if (kind == 0) {
    this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(texture.c_str(), 0, false, true, false), region);
  } else if (kind == 5) {
    this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(texture.c_str(), 0, false, true, false), region);
    if (g_windowMgr.window_4F1CC && (s_lastMarkerX != x || s_lastMarkerY != y)) {
      s_lastMarkerX = x;
      s_lastMarkerY = y;
      g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E2, x, y, 0, 0);
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
    this->DrawTexture(border, 0, 0.0999f, 0x99FFFFFF, g_texMgr.Default(), region);
    this->DrawTexture(rect, 0, 0.0999f, color, g_texMgr.Default(), region);
    if (g_windowMgr.window_4F1CC) {
      g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E3, x, y, 0, 0);
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
    this->DrawTexture(horizontal, 0, 0.0999f, color, g_texMgr.Default(), region);
    this->DrawTexture(vertical, 0, 0.0999f, color, g_texMgr.Default(), region);
  } else if (kind == 4) {
    RECT shadow;
    shadow.left = rect.left - 2;
    shadow.top = rect.top - 2;
    shadow.right = rect.right + 2;
    shadow.bottom = rect.bottom + 4;
    const int angle = static_cast<int>(180.0f - this->scene->player->render.angle);
    this->DrawTexture(shadow, angle, 0.0999f, 0x77333333, g_texMgr.Load(texture.c_str(), 0, false, true, false), region);
    this->DrawTexture(rect, angle, 0.0999f, color, g_texMgr.Load(texture.c_str(), 0, false, true, false), region);
    if (s_lastPlayerX != x || s_lastPlayerY != y) {
      s_lastPlayerX = x;
      s_lastPlayerY = y;
      if (g_windowMgr.window_4F1CC) {
        g_windowMgr.window_4F1CC->SendMsg(nullptr, 0x1E1, x, y, 0, 0);
      }
      if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_01C0)) {
        window->Invalidate();
      }
    }
  } else if (kind == 6) {
    if (texture.size() == 0) {
      g_windowMgr.LookupWindow(14)->SendMsg(nullptr, 6, 0x1C5, color, rect.left, rect.top);
    } else {
      g_windowMgr.LookupWindow(14)->SendMsg(nullptr, 6, 0x1C7, color, rect.left, rect.top);
      this->DrawTexture(rect, 0, 0.0999f, 0xFFFFFFFF, g_texMgr.Load(texture.c_str(), 0, false, true, false), region);
    }
  }
}

// OFFSET: 0x00CD1C50
// FUNCTION: fn_21258
void CGameMode::DrawMiniMapCoordinates() {
  const char* player = reinterpret_cast<const char*>(this->scene->player);
  const int coordinateX = *reinterpret_cast<const int*>(player + 0x494);
  const int coordinateY = *reinterpret_cast<const int*>(player + 0x498);
  if (coordinateY <= 0 || coordinateX <= 0) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\RankFont.spr"));

  int digits[4];
  int count;
  int value = coordinateY;
  if (value > 9999) {
    value = 9999;
    count = 4;
  } else {
    count = 1;
    if (value > 999) {
      count = 4;
    } else if (value > 99) {
      count = 3;
    } else if (value > 9) {
      count = 2;
    }
  }

  digits[0] = value % 10;
  digits[1] = value / 10 % 10;
  digits[2] = value / 100 % 10;
  digits[3] = value / 1000 % 10;

  int x = g_renderer->viewport.width - 80 + count * 25;
  const int y = g_renderer->viewport.height - 30;
  for (int index = 0; index < count; ++index, x -= 25) {
    DrawActionSprite(reinterpret_cast<CRenderSystemBase&>(*this), x, y, action, sprite, 0, digits[index], 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
  }

  DrawActionSprite(
    reinterpret_cast<CRenderSystemBase&>(*this), g_renderer->viewport.width - 80, g_renderer->viewport.height - 40, action, sprite, 0, 10, 1.0f, 0.0f, 0xFFFFFFFF, 0x201
  );

  value = coordinateX;
  count = 1;
  if (value > 999) {
    count = 4;
  } else if (value > 99) {
    count = 3;
  } else if (value > 9) {
    count = 2;
  }

  digits[0] = value % 10;
  digits[1] = value / 10 % 10;
  digits[2] = value / 100 % 10;
  digits[3] = value / 1000 % 10;

  x = g_renderer->viewport.width - 105;
  const int secondY = g_renderer->viewport.height - 50;
  for (int index = 0; index < count; ++index, x -= 25) {
    DrawActionSprite(reinterpret_cast<CRenderSystemBase&>(*this), x, secondY, action, sprite, 0, digits[index], 1.0f, 0.0f, 0xFFFFFFFF, 0x201);
  }
}

// GLOBAL: gl_25044
int g_minimapCoordinateValue;
// GLOBAL: gl_25045
int g_minimapCoordinateDigits;
// GLOBAL: gl_25046
int g_minimapCoordinateDigitValues[4];
// GLOBAL: gl_24672
int g_screenWidth;


// FUNCTION: fn_21261
void CGameMode::DrawMiniMapNumber() {
  if (this->unknown_0408 == nullptr || this->unknown_0414 == 0) {
    return;
  }

  if (this->unknown_0414 != g_minimapCoordinateValue) {
    g_minimapCoordinateValue = this->unknown_0414;
    int value;
    int count;
    if (this->unknown_0414 > 9999) {
      value = 9999;
      count = 4;
    } else {
      value = this->unknown_0414;
      if (value < 0) {
        value = 0;
      }
      count = 1;
      if (value > 999) {
        count = 4;
      } else if (value > 99) {
        count = 3;
      } else if (value > 9) {
        count = 2;
      }
    }

    g_minimapCoordinateDigits = count;
    g_minimapCoordinateDigitValues[0] = value % 10;
    g_minimapCoordinateDigitValues[1] = value / 10 % 10;
    g_minimapCoordinateDigitValues[2] = value / 100 % 10;
    g_minimapCoordinateDigitValues[3] = value / 1000 % 10;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("이팩트\\숫자.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("이팩트\\숫자.spr"));
  for (int index = 0; index < g_minimapCoordinateDigits; ++index) {
    const int x = static_cast<int>(
      static_cast<float>(g_screenWidth) + this->cursorOffsetX + 24.0f - static_cast<float>(index * 16) +
      static_cast<float>(g_minimapCoordinateDigits * 16 - 16)
    );
    const int y = static_cast<int>(static_cast<float>(g_screenHeight) + this->cursorOffsetY);
    DrawActionSprite(
      reinterpret_cast<CRenderSystemBase&>(*this), x, y, action, sprite, 0, g_minimapCoordinateDigitValues[index], 2.0f, 0.0f, 0xFAFFFFFF, 0x201
    );
  }
}
