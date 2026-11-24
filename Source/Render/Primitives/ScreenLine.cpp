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
#include "Render/Projection/RenderProjection.h"
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

// GLOBAL: gl_24530
float ScreenLineQuarterTurn = 1.570796f;

// GLOBAL: gl_24531
float ScreenLineFullTurn = 6.283184f;

// OFFSET: 0x00918380
// FUNCTION: fn_12594
void UIRoMapWnd::DrawDashedArrow(
  float startX, float startY, float endX, float endY, int stepWidth, int thickness, CTexture* body, int headSize, CTexture* head, float progress,
  std::uint32_t color, int offset
) {
  const float visibleDistance = SquareRoot((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY)) * progress;
  const float deltaY = endY - startY;
  const float deltaX = endX - startX;
  const float inverseLength = 1.0f / SquareRoot(deltaY * deltaY + deltaX * deltaX);
  const float directionY = inverseLength * deltaY;
  const float directionX = inverseLength * deltaX;
  float angle = static_cast<float>(std::acos(static_cast<double>(directionX)));
  if (directionY < 0.0f) {
    angle = ScreenLineFullTurn - angle;
  }
  angle += ScreenLineQuarterTurn;

  const float sine = ClientSin(angle);
  const float cosine = ClientCos(angle);
  const float inverseNormal = 1.0f / SquareRoot(cosine * cosine + sine * sine);
  const float normalX = cosine * inverseNormal;
  const float normalY = inverseNormal * sine;
  std::uint32_t segmentColor = 0x33FFFFFF;
  const float sideX = static_cast<float>(thickness) * normalX;
  const float sideY = static_cast<float>(thickness) * normalY;
  float currentX = startX - static_cast<float>(offset) * normalX;
  float currentY = startY - static_cast<float>(offset) * normalY;
  const float stepX = static_cast<float>(stepWidth) * directionX;
  const float stepY = static_cast<float>(stepWidth) * directionY;
  float nextX = stepX + currentX;
  float nextY = stepY + currentY;
  const float visibleEndX = directionX * visibleDistance + startX;
  const float visibleEndY = directionY * visibleDistance + startY;
  ScreenPoint quad[4];

  if (SquareRoot((nextX - startX) * (nextX - startX) + (nextY - startY) * (nextY - startY)) < visibleDistance) {
    const float halfStepX = stepX * g_depthAverageHalf;
    const float halfStepY = stepY * g_depthAverageHalf;
    float distanceToNext;
    do {
      quad[0] = {currentX, currentY};
      quad[1] = {currentX + sideX, currentY + sideY};
      quad[2] = {nextX, nextY};
      quad[3] = {nextX + sideX, nextY + sideY};
      const float remainingX = visibleEndX - currentX;
      const float remainingY = visibleEndY - currentY;
      if (SquareRoot(remainingX * remainingX + remainingY * remainingY) < 100.0f) {
        DrawScreenQuad(quad[0], quad[1], quad[2], quad[3], body, segmentColor);
        segmentColor += 0x33000000;
      }

      currentX = halfStepX + nextX;
      currentY = halfStepY + nextY;
      nextX = currentX + stepX;
      nextY = currentY + stepY;
      distanceToNext = SquareRoot((nextX - startX) * (nextX - startX) + (nextY - startY) * (nextY - startY));
    } while (distanceToNext < visibleDistance);
  }

  if (progress < g_projectionDepthNumerator) {
    const float tipBaseX = (sideX * g_depthAverageHalf + nextX) - static_cast<float>(thickness) * directionX;
    const float tipBaseY = (sideY * g_depthAverageHalf + nextY) - static_cast<float>(thickness) * directionY;
    const float headX = static_cast<float>(headSize);
    const float tipX = tipBaseX + headX * normalX;
    const float otherBaseX = tipBaseX - headX * normalX;
    const float tipY = tipBaseY + headX * normalY;
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
  const int last = count <= step ? count - 1 : step;
  const int frame = last >= 0 ? last : 0;
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
  record.u0 = 0.0f;
  record.v0 = 0.0f;
  CTexture* texture;
  if (cell->imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(image, palette, record);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
  }

  const int height = static_cast<int>(static_cast<float>(image->height) * scale);
  const int halfWidth = static_cast<int>(static_cast<float>(image->width) * scale * g_depthAverageHalf);
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  RenderVertex* vertices = primitive->vertices;
  vertices[0].x = x - static_cast<float>(halfWidth);
  vertices[0].y = y - static_cast<float>(height) - static_cast<float>(lift);
  vertices[1].x = static_cast<float>(halfWidth) + x;
  vertices[1].y = y - static_cast<float>(height) - static_cast<float>(lift);
  vertices[2].x = x - static_cast<float>(halfWidth);
  vertices[2].y = y - static_cast<float>(lift);
  vertices[3].x = static_cast<float>(halfWidth) + x;
  vertices[3].y = y - static_cast<float>(lift);
  for (int index = 0; index < 4; ++index) {
    vertices[index].z = 0.8f;
    vertices[index].rhw = 1.0f;
    vertices[index].diffuse = 0xFFFFFFFF;
  }
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
  g_renderer->Submit(primitive, 513);
  this->m_kafraBounds.top = static_cast<int>(vertices[0].y);
  this->m_kafraBounds.bottom = static_cast<int>(vertices[3].y);
  this->m_kafraBounds.left = static_cast<int>(vertices[0].x);
  this->m_kafraBounds.right = static_cast<int>(vertices[3].x);
}

// OFFSET: 0x00917D70
// FUNCTION: fn_12590
void UIRoMapWnd::DrawArrow(
  float x0, float y0, float x1, float y1, float startLeft, float startTop, float startRight, float startBottom, float endLeft, float endTop, float endRight,
  float endBottom, int bodyWidth, CTexture* body, int headWidth, CTexture* head, float progress, std::uint32_t color
) {
  float dx = x1 - x0;
  float dy = y1 - y0;
  const float length = 1.0f / SquareRoot(dy * dy + dx * dx);
  const float dirX = length * dx;
  const float dirY = length * dy;
  float angle = static_cast<float>(std::acos(static_cast<double>(dirX)));
  if (dirY < 0.0f) {
    angle = 6.283184f - angle;
  }
  angle += 1.570796f;
  const float sine = ClientSin(angle);
  const float cosine = ClientCos(angle);
  const float normal = 1.0f / SquareRoot(cosine * cosine + sine * sine);
  const float normalX = normal * cosine;
  const float normalY = normal * sine;
  const float offsetX = static_cast<float>(bodyWidth) * normalX;
  const float offsetY = static_cast<float>(bodyWidth) * normalY;
  ScreenPoint sideStart = {offsetX + x0, offsetY + y0};
  ScreenPoint sideEnd = {offsetX + x1, offsetY + y1};
  ScreenPoint start = {x0, y0};
  ScreenPoint end = {x1, y1};

  while (start.x > startLeft && startRight > start.x && start.y > startTop && startBottom > start.y) {
    start.x += dirX;
    start.y += dirY;
  }
  while (end.x > endLeft && endRight > end.x && end.y > endTop && endBottom > end.y) {
    end.x -= dirX;
    end.y -= dirY;
  }
  while (sideStart.x > startLeft && startRight > sideStart.x && sideStart.y > startTop && startBottom > sideStart.y) {
    sideStart.x += dirX;
    sideStart.y += dirY;
  }
  while (sideEnd.x > endLeft && endRight > sideEnd.x && sideEnd.y > endTop && endBottom > sideEnd.y) {
    sideEnd.x -= dirX;
    sideEnd.y -= dirY;
  }

  ScreenPoint mid;
  mid.x = (end.x - start.x) * progress + start.x;
  mid.y = (end.y - start.y) * progress + start.y;
  ScreenPoint corner;
  if (progress != g_projectionDepthNumerator) {
    corner.x = mid.x + offsetX;
    corner.y = mid.y + offsetY;
  } else {
    corner.x = (sideEnd.x - sideStart.x) + sideStart.x;
    corner.y = (sideEnd.y - sideStart.y) + sideStart.y;
  }
  DrawScreenQuad(start, sideStart, mid, corner, body, color);

  if (progress < g_projectionDepthNumerator) {
    const float width = static_cast<float>(bodyWidth);
    const float size = static_cast<float>(headWidth);
    ScreenPoint tip;
    tip.x = (offsetX * g_depthAverageHalf + mid.x) - width * dirX;
    tip.y = (offsetY * g_depthAverageHalf + mid.y) - width * dirY;
    const float halfX = size * normalX * g_depthAverageHalf;
    const float halfY = size * normalY * g_depthAverageHalf;
    ScreenPoint left = {tip.x - halfX, tip.y - halfY};
    ScreenPoint right = {tip.x + halfX, tip.y + halfY};
    ScreenPoint point = {size * dirX + tip.x, size * dirY + tip.y};
    DrawScreenQuad(tip, left, right, point, head, color);
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
    const int right = record->screenRight;
    const int left = record->screenLeft;
    const int top = record->screenTop;
    const int bottom = record->screenBottom;
    this->DrawRoundedOutline(left, top, right - left, bottom - top, g_worldMapNameColor);
  }

  {
    const WorldViewRecord* record = this->selectedRecord;
    const int right = record->right;
    const int left = record->left;
    const int top = record->top;
    const int bottom = record->bottom;
    this->DrawRoundedOutline(left, top, right - left, bottom - top, g_worldMapNameColor);
  }

  {
    WorldViewRecord* record = this->selectedRecord;
    const WorldViewArea savedArea = record->area;

    bool hasKafraService;
    {
      const std::string mapName = record->name;
      hasKafraService = g_kafraServiceMapList.find(mapName) != g_kafraServiceMapList.end();
    }
    if (hasKafraService) {
      WorldViewRecord* current = this->selectedRecord;
      if (g_exceptionMiniMapPosList.find(current->name) != g_exceptionMiniMapPosList.end()) {
        const MiniMapPosition offset = g_exceptionMiniMapPosList[current->name];
        current->left = static_cast<int>(static_cast<float>(current->left) + offset.x);
        current->top = static_cast<int>(static_cast<float>(current->top) + offset.y);
        current->right = static_cast<int>(static_cast<float>(current->right) + offset.x);
        current->bottom = static_cast<int>(static_cast<float>(current->bottom) + offset.y);
      }
    }

    this->selectedRecord->area = savedArea;
  }

  WorldViewRecord* record = this->selectedRecord;
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
  if (previous != nullptr) {
    delete previous;
  }

  {
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
}
