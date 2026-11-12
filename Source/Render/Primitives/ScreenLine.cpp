#include "Render/DeviceContext/DCSurface.h"
#include "UI/Navigation/UIRoMapWnd.h"

#include "Client/Session/Session.h"
#include "Render/Textures/TextureCache.h"
#include "Script/LuaScript.h"
#include "Script/MiniMapPosition.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/WindowBorder.h"

#include "Contents/Map/MapSkyMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenGeometry.h"
#include "Render/Primitives/TextureQuad.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Lists/UITabControl.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

namespace {
  __declspec(noinline) float ScreenLineSine(float angle) {
    return static_cast<float>(std::sin(static_cast<double>(angle)));
  }

  __declspec(noinline) float ScreenLineCosine(float angle) {
    return static_cast<float>(std::cos(static_cast<double>(angle)));
  }
}

// OFFSET: 0x00918380
// FUNCTION: fn_12594
void UIRoMapWnd::DrawDashedArrow(
  float startX, float startY, float endX, float endY, int stepWidth, int thickness, CTexture* body, int headSize, CTexture* head, float progress,
  std::uint32_t color, int offset
) {
  const float visibleDistance = SquareRoot((endY - startY) * (endY - startY) + (endX - startX) * (endX - startX)) * progress;
  const float deltaY = endY - startY;
  const float deltaX = endX - startX;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX);
  const float directionY = inverseLength * deltaY;
  const float directionX = inverseLength * deltaX;
  float angle = static_cast<float>(std::acos(static_cast<double>(directionX)));
  if (directionY < 0.0f) {
    angle = 6.283184f - angle;
  }
  angle += 1.570796f;

  const float sine = ScreenLineSine(angle);
  const float cosine = ScreenLineCosine(angle);
  const float inverseNormal = 1.0f / SquareRoot(cosine * cosine + sine * sine);
  const float normalX = cosine * inverseNormal;
  const float normalY = sine * inverseNormal;
  const float sideX = static_cast<float>(thickness) * normalX;
  const float sideY = static_cast<float>(thickness) * normalY;
  const float stepX = static_cast<float>(stepWidth) * directionX;
  const float stepY = static_cast<float>(stepWidth) * directionY;
  float currentX = startX - static_cast<float>(offset) * normalX;
  float currentY = startY - static_cast<float>(offset) * normalY;
  float nextX = stepX + currentX;
  float nextY = stepY + currentY;
  std::uint32_t segmentColor = 0x33FFFFFF;
  ScreenPoint quad[4];

  if (SquareRoot((nextY - startY) * (nextY - startY) + (nextX - startX) * (nextX - startX)) < visibleDistance) {
    const float halfStepX = stepX * 0.5f;
    const float halfStepY = stepY * 0.5f;
    float distanceToNext;
    do {
      quad[0] = {currentX, currentY};
      quad[1] = {currentX + sideX, currentY + sideY};
      quad[2] = {nextX, nextY};
      quad[3] = {nextX + sideX, nextY + sideY};
      const float remainingX = (directionX * visibleDistance + startX) - currentX;
      const float remainingY = (directionY * visibleDistance + startY) - currentY;
      if (SquareRoot(remainingY * remainingY + remainingX * remainingX) < 100.0f) {
        DrawScreenQuad(quad[0], quad[1], quad[2], quad[3], body, segmentColor);
        segmentColor += 0x33000000;
      }

      currentX = halfStepX + nextX;
      currentY = halfStepY + nextY;
      nextX = currentX + stepX;
      nextY = currentY + stepY;
      distanceToNext = SquareRoot((nextY - startY) * (nextY - startY) + (nextX - startX) * (nextX - startX));
    } while (distanceToNext < visibleDistance);
  }

  if (progress < 1.0f) {
    const float tipBaseX = (sideX * 0.5f + nextX) - static_cast<float>(thickness) * directionX;
    const float tipBaseY = (sideY * 0.5f + nextY) - static_cast<float>(thickness) * directionY;
    const float headX = static_cast<float>(headSize);
    const float tipX = tipBaseX + headX * normalX;
    const float tipY = tipBaseY + headX * normalY;
    const float otherBaseX = tipBaseX - headX * normalX;
    const float otherBaseY = tipBaseY - headX * normalY;
    const float doubledStepX = headX * directionX + headX * directionX;
    const float doubledStepY = headX * directionY + headX * directionY;
    quad[0] = {otherBaseX, otherBaseY};
    quad[1] = {tipX, tipY};
    quad[2] = {doubledStepX + otherBaseX, doubledStepY + otherBaseY};
    quad[3] = {tipX + doubledStepX, tipY + doubledStepY};
    DrawScreenQuad(quad[3], quad[2], quad[1], quad[0], head, color);
  }
}
// GLOBAL: gl_25021
std::uint32_t g_roMapRecordColors[12] = {
  0x66DC143C, 0x66FFA500, 0x66FFFF00, 0x66ADFF2F, 0x6600BFFF, 0x66483D8B, 0x669400D3, 0x66FF1493, 0x66FFF5EE, 0x66F5FFFA, 0x66F0F8FF, 0x66778899,
};

// OFFSET: 0x00918190
// FUNCTION: fn_12591
void UIRoMapWnd::DrawRecordAreas() {
  int color = 0;
  for (std::list<WorldViewRecord>::const_iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld != it->worldIndex) {
      continue;
    }

    if (color == 12) {
      color = 0;
    }
    DrawAlphaRectangle(it->left, it->top, it->right - it->left, it->bottom - it->top, g_roMapRecordColors[color]);
    ++color;
  }
}

// OFFSET: 0x009181F0
// FUNCTION: fn_12592
void UINewChatWnd::DrawTransBox() {
  int x = 3;
  int y = 0;
  this->ToScreen(x, y);
  if (this->geometry.state == 0) {
    return;
  }

  for (int i = 0; i < static_cast<int>(this->m_tabControl->labels.size()); ++i) {
    const int inactive = i != this->m_currentTab;
    DrawAlphaRectangle(x + 1 + this->m_tabWidth * i, y + 1 + inactive, this->m_tabWidth - 1, this->m_inputOffsetY - inactive, i == this->m_currentTab ? 0x66000000 : 0x99000000);
  }
  DrawAlphaRectangle(this->m_inputX + 1, this->m_inputY + 1, this->m_listWidth - 2, this->m_listHeight - 1, this->m_listColor);
}

// OFFSET: 0x00918300
// FUNCTION: fn_12593
void UISubChatHisWnd::DrawTransBox() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  DrawAlphaRectangle(x + 1, y + 1, this->m_inputWidth - 2, this->m_inputHeight, 0x66000000);
  DrawAlphaRectangle(this->m_state.x + 1, this->m_state.y + 1, this->m_state.width - 2, this->m_state.height - 2, this->m_state.backgroundColor);
}

// OFFSET: 0x009189C0
// FUNCTION: fn_12595
void UIRoMapWnd::DrawKafraIcon(float x, float y) {
  const float scale = static_cast<float>(g_worldViewMgr.screenScale) * 0.2f + g_depthAverageHalf;
  CSprRes* sprite = dynamic_cast<CSprRes*>(ResourceMgr::Instance().Load("npc\\4_f_kafra6.spr"));
  CActRes* action = dynamic_cast<CActRes*>(ResourceMgr::Instance().Load("npc\\4_f_kafra6.act"));
  if (!sprite || !action) {
    return;
  }

  const std::vector<ActionFrame>& frames = action->Animation(0);
  const int count = static_cast<int>(frames.size());
  if (count == 0) {
    return;
  }

  const int tick = static_cast<int>(timeGetTime() % 4000);
  const int step = tick == 0 ? 0 : tick / (4000 / count);
  const int last = count > step ? step : count - 1;
  const int frame = last < 0 ? 0 : last;
  const std::uint32_t* palette = sprite->palette;

  const SpriteFrameCell* cell = &frames[0].Cell(0);
  if (!cell || cell->image == -1) {
    return;
  }

  const SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
  if (!image) {
    return;
  }

  const int top = static_cast<int>(y - (static_cast<float>(image->height) * scale + 23.0f));
  const int lift = top <= 0 ? top : 0;
  if (frame > 0) {
    cell = &frames[frame].Cell(0);
    if (!cell || cell->image == -1) {
      return;
    }

    image = sprite->images[cell->imageType][cell->image];
    if (!image) {
      return;
    }
  }

  SpriteAtlasRecord record{};
  CTexture* texture;
  if (cell->imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
  }

  const int height = static_cast<int>(static_cast<float>(image->height) * scale);
  const int halfWidth = static_cast<int>(static_cast<float>(image->width) * scale * g_depthAverageHalf);
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  const float left = x - static_cast<float>(halfWidth);
  const float right = static_cast<float>(halfWidth) + x;
  const float upper = y - static_cast<float>(height) - static_cast<float>(lift);
  const float lower = y - static_cast<float>(lift);
  vertices[0].x = left;
  vertices[0].y = upper;
  vertices[1].y = upper;
  vertices[2].x = left;
  vertices[1].x = right;
  vertices[3].x = right;
  vertices[3].z = 0.8f;
  vertices[2].z = 0.8f;
  vertices[1].z = 0.8f;
  vertices[0].z = 0.8f;
  vertices[2].y = lower;
  vertices[3].y = lower;
  vertices[3].rhw = 1.0f;
  vertices[2].rhw = 1.0f;
  vertices[1].rhw = 1.0f;
  vertices[0].rhw = 1.0f;
  vertices[3].diffuse = 0xFFFFFFFF;
  vertices[2].diffuse = 0xFFFFFFFF;
  vertices[1].diffuse = 0xFFFFFFFF;
  vertices[0].diffuse = 0xFFFFFFFF;
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;
  primitive->texture = texture;
  primitive->sourceBlend = 2;
  primitive->destinationBlend = 4;
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  this->m_kafraTop = static_cast<int>(vertices[0].y);
  this->m_kafraBottom = static_cast<int>(vertices[3].y);
  this->m_kafraLeft = static_cast<int>(vertices[0].x);
  this->m_kafraRight = static_cast<int>(vertices[3].x);
}

// OFFSET: 0x00917D70
// FUNCTION: fn_12590
void UIRoMapWnd::DrawArrow(
  float x0, float y0, float x1, float y1, float startLeft, float startTop, float startRight, float startBottom, float endLeft, float endTop, float endRight,
  float endBottom, int bodyWidth, CTexture* body, int headWidth, CTexture* head, float progress, std::uint32_t color
) {
  float dy = y1 - y0;
  float dx = x1 - x0;
  const float length = 1.0f / SquareRoot(dy * dy + dx * dx);
  const float dirY = length * dy;
  const float dirX = length * dx;
  float angle = static_cast<float>(std::acos(static_cast<double>(dirX)));
  if (dirY < 0.0f) {
    angle = 6.283184f - angle;
  }
  angle += 1.570796f;
  const float sine = ScreenLineSine(angle);
  const float cosine = ScreenLineCosine(angle);
  const float normal = 1.0f / SquareRoot(cosine * cosine + sine * sine);
  const float normalX = normal * cosine;
  const float normalY = normal * sine;
  const float offsetX = static_cast<float>(bodyWidth) * normalX;
  const float offsetY = static_cast<float>(bodyWidth) * normalY;
  float sideX0 = offsetX + x0;
  float sideX1 = offsetX + x1;
  float sideY1 = offsetY + y1;
  float sideY0 = offsetY + y0;

  while (x0 > startLeft && startRight > x0 && y0 > startTop && startBottom > y0) {
    x0 += dirX;
    y0 += dirY;
  }
  while (x1 > endLeft && endRight > x1 && y1 > endTop && endBottom > y1) {
    x1 -= dirX;
    y1 -= dirY;
  }
  while (sideX0 > startLeft && startRight > sideX0 && sideY0 > startTop && startBottom > sideY0) {
    sideX0 += dirX;
    sideY0 += dirY;
  }
  while (sideX1 > endLeft && endRight > sideX1 && sideY1 > endTop && endBottom > sideY1) {
    sideX1 -= dirX;
    sideY1 -= dirY;
  }

  const float midX = (x1 - x0) * progress + x0;
  const float midY = (y1 - y0) * progress + y0;
  float cornerX;
  float cornerY;
  if (progress == 1.0f) {
    cornerX = (sideX1 - sideX0) + sideX0;
    cornerY = (sideY1 - sideY0) + sideY0;
  } else {
    cornerX = midX + offsetX;
    cornerY = midY + offsetY;
  }
  DrawScreenQuad({x0, y0}, {sideX0, sideY0}, {midX, midY}, {cornerX, cornerY}, body, color);

  if (progress < 1.0f) {
    const float width = static_cast<float>(bodyWidth);
    const float size = static_cast<float>(headWidth);
    const float tipX = (offsetX * g_depthAverageHalf + midX) - width * dirX;
    const float tipY = (offsetY * g_depthAverageHalf + midY) - width * dirY;
    const float halfX = size * normalX * g_depthAverageHalf;
    const float halfY = size * normalY * g_depthAverageHalf;
    DrawScreenQuad({tipX, tipY}, {tipX - halfX, tipY - halfY}, {tipX + halfX, tipY + halfY}, {size * dirX + tipX, size * dirY + tipY}, head, color);
  }
}

// GLOBAL: gl_24803
std::uint32_t g_worldMapNameColor;

// OFFSET: 0x00918D70
// FUNCTION: fn_12596
void UIRoMapWnd::DrawSelectedMapPreview() {
  if (this->selectedRecord == nullptr) {
    return;
  }

  if (this->selectedRecord->mapIndex < 100) {
    const WorldViewRecord* record = this->selectedRecord;
    DrawRoundedBorder(
      *this, record->screenLeft, record->screenTop, record->screenRight - record->screenLeft, record->screenBottom - record->screenTop,
      g_worldMapNameColor
    );
  }

  {
    const WorldViewRecord* record = this->selectedRecord;
    DrawRoundedBorder(*this, record->left, record->top, record->right - record->left, record->bottom - record->top, g_worldMapNameColor);
  }

  WorldViewRecord* record = this->selectedRecord;
  const int savedLeft = record->left;
  const int savedTop = record->top;
  const int savedRight = record->right;
  const int savedBottom = record->bottom;
  const int savedAreaFlags = record->areaFlags;

  if (g_kafraServiceMapList.find(std::string(record->name)) != g_kafraServiceMapList.end()) {
    WorldViewRecord* current = this->selectedRecord;
    if (g_exceptionMiniMapPosList.find(current->name) != g_exceptionMiniMapPosList.end()) {
      const MiniMapPosition offset = g_exceptionMiniMapPosList[current->name];
      current->left = static_cast<int>(static_cast<float>(current->left) + offset.x);
      current->top = static_cast<int>(static_cast<float>(current->top) + offset.y);
      current->right = static_cast<int>(static_cast<float>(current->right) + offset.x);
      current->bottom = static_cast<int>(static_cast<float>(current->bottom) + offset.y);
    }
  }

  record = this->selectedRecord;
  record->left = savedLeft;
  record->top = savedTop;
  record->right = savedRight;
  record->bottom = savedBottom;
  record->areaFlags = savedAreaFlags;

  record = this->selectedRecord;
  int x = record->right + 10;
  int y = record->top + 10;
  if (x + 130 > g_renderer->viewport.width) {
    x = record->left - 140;
  }
  if (y + 130 > g_renderer->viewport.height) {
    y = g_renderer->viewport.height - 130;
  }

  WorldViewArea* previous = this->previewArea;
  this->previewArea = new WorldViewArea{x, y, x + 130, y + 130, 1};
  delete previous;

  tagRECT area = {x, y, x + 130, y + 130};
  this->FillSurface(0xFFFF00FF, &area);

  std::string name = this->selectedRecord->name;
  if (name.size() > 4) {
    name.resize(name.size() - 4);
  }

  this->DrawOutlinedText(x + 3, y + 115, name.c_str(), 0, 0xFFFFFF, 0x5C5C5C, 0, 12, 1, 0);

  {
    std::string path = "유저인터페이스\\map\\";
    path += name;
    path += ".bmp";
    this->previewTexture = g_texMgr.Load(path.c_str(), 0, true, true, false);
  }

  name = g_session.GetMapDisplayName(this->selectedRecord->name.c_str());
  this->DrawOutlinedText(x + 3, y + 3, name.c_str(), 0, 0xD2FF, 0, 0, 12, 1, 0);
}
