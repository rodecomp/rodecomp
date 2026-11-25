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
  ScreenPoint start, ScreenPoint end, int stepWidth, int thickness, CTexture* body, int headSize, CTexture* head, float progress, std::uint32_t color,
  int offset
) {
  const float visibleDistance = PointDistance(start, end) * progress;
  ScreenPoint direction = {end.x - start.x, end.y - start.y};
  NormalizePoint(direction);
  float angle = static_cast<float>(std::acos(static_cast<double>(direction.x)));
  if (direction.y < 0.0f) {
    angle = ScreenLineFullTurn - angle;
  }
  angle += ScreenLineQuarterTurn;

  const float sine = ClientSin(angle);
  const float cosine = ClientCos(angle);
  ScreenPoint normal = {cosine, sine};
  NormalizePoint(normal);
  std::uint32_t segmentColor = 0x33FFFFFF;
  const float sideX = static_cast<float>(thickness) * normal.x;
  const float sideY = static_cast<float>(thickness) * normal.y;
  ScreenPoint current = {start.x - static_cast<float>(offset) * normal.x, start.y - static_cast<float>(offset) * normal.y};
  const float stepX = static_cast<float>(stepWidth) * direction.x;
  const float stepY = static_cast<float>(stepWidth) * direction.y;
  ScreenPoint next = {stepX + current.x, stepY + current.y};
  const ScreenPoint visibleEnd = {direction.x * visibleDistance + start.x, direction.y * visibleDistance + start.y};

  if (PointDistance(start, next) < visibleDistance) {
    const float halfStepX = stepX * 0.5f;
    const float halfStepY = stepY * 0.5f;
    do {
      const ScreenPoint p0 = {current.x, current.y};
      const ScreenPoint p1 = {current.x + sideX, current.y + sideY};
      const ScreenPoint p2 = {next.x, next.y};
      const ScreenPoint p3 = {next.x + sideX, next.y + sideY};
      if (PointDistance(current, visibleEnd) < 100.0f) {
        DrawScreenQuad(p0, p1, p2, p3, body, segmentColor);
        segmentColor += 0x33000000;
      }

      current.x = halfStepX + next.x;
      current.y = halfStepY + next.y;
      next.x = current.x + stepX;
      next.y = current.y + stepY;
    } while (PointDistance(start, next) < visibleDistance);
  }

  if (progress < g_projectionDepthNumerator) {
    const float tipBaseX = (sideX * 0.5f + next.x) - static_cast<float>(thickness) * direction.x;
    const float tipBaseY = (sideY * 0.5f + next.y) - static_cast<float>(thickness) * direction.y;
    const float headX = static_cast<float>(headSize);
    const float tipX = tipBaseX + headX * normal.x;
    const float otherBaseX = tipBaseX - headX * normal.x;
    const float tipY = tipBaseY + headX * normal.y;
    const float otherBaseY = tipBaseY - headX * normal.y;
    const float doubledStepX = headX * direction.x + headX * direction.x;
    const float doubledStepY = headX * direction.y + headX * direction.y;
    const ScreenPoint p0 = {otherBaseX, otherBaseY};
    const ScreenPoint p1 = {tipX, tipY};
    const ScreenPoint p2 = {doubledStepX + otherBaseX, doubledStepY + otherBaseY};
    const ScreenPoint p3 = {tipX + doubledStepX, tipY + doubledStepY};
    DrawScreenQuad(p3, p2, p1, p0, head, color);
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
void UIRoMapWnd::DrawKafraIcon(ScreenPoint point) {
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

  const int top = static_cast<int>(point.y - (static_cast<float>(image->height) * scale + 23.0f));
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
  vertices[2].x = vertices[0].x = point.x - static_cast<float>(halfWidth);
  vertices[1].y = vertices[0].y = point.y - static_cast<float>(height) - static_cast<float>(lift);
  vertices[3].x = vertices[1].x = static_cast<float>(halfWidth) + point.x;
  vertices[3].y = vertices[2].y = point.y - static_cast<float>(lift);
  vertices[0].z = vertices[1].z = vertices[2].z = vertices[3].z = 0.8f;
  vertices[0].rhw = vertices[1].rhw = vertices[2].rhw = vertices[3].rhw = 1.0f;
  vertices[0].diffuse = vertices[1].diffuse = vertices[2].diffuse = vertices[3].diffuse = 0xFFFFFFFF;
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
  ScreenPoint from, ScreenPoint to, ScreenPoint startMin, ScreenPoint startMax, ScreenPoint endMin, ScreenPoint endMax, int bodyWidth, CTexture* body,
  int headWidth, CTexture* head, float progress, std::uint32_t color
) {
  float dx = to.x - from.x;
  float dy = to.y - from.y;
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
  const float offsetX = static_cast<float>(bodyWidth) * normalX;
  const float normalY = normal * sine;
  const float offsetY = static_cast<float>(bodyWidth) * normalY;
  ScreenPoint sideStart = {offsetX + from.x, offsetY + from.y};
  ScreenPoint sideEnd = {offsetX + to.x, offsetY + to.y};
  ScreenPoint start = {from.x, from.y};
  ScreenPoint end = {to.x, to.y};

  while (start.x > startMin.x && startMax.x > start.x && start.y > startMin.y && startMax.y > start.y) {
    start.x += dirX;
    start.y += dirY;
  }
  while (end.x > endMin.x && endMax.x > end.x && end.y > endMin.y && endMax.y > end.y) {
    end.x -= dirX;
    end.y -= dirY;
  }
  while (sideStart.x > startMin.x && startMax.x > sideStart.x && sideStart.y > startMin.y && startMax.y > sideStart.y) {
    sideStart.x += dirX;
    sideStart.y += dirY;
  }
  while (sideEnd.x > endMin.x && endMax.x > sideEnd.x && sideEnd.y > endMin.y && endMax.y > sideEnd.y) {
    sideEnd.x -= dirX;
    sideEnd.y -= dirY;
  }

  ScreenPoint mid;
  mid.x = (end.x - start.x) * progress + start.x;
  mid.y = (end.y - start.y) * progress + start.y;
  ScreenPoint corner;
  if (progress != g_projectionDepthNumerator) {
    corner.y = mid.y + offsetY;
    corner.x = mid.x + offsetX;
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
  this->previewArea = new WorldViewArea(x, y, x + 130, y + 130);
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
