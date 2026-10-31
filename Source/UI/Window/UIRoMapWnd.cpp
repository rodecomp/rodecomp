#include "UI/Window/UIRoMapWnd.h"
#include "Script/LuaScript.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/NavigationManager.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/ScreenGeometry.h"
#include "Render/ScreenRectangle.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Messenger/ClientMessenger.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

// GLOBAL: gl_24799
CWorldViewMgr g_worldViewMgr;

// OFFSET: 0x00931080
// FUNCTION: fn_12697
void UIRoMapWnd::Slot25(int, int) {
}

// OFFSET: 0x0091BE80
// FUNCTION: fn_12611
void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker) {
  const int width = area.right - area.left;
  const int height = area.bottom - area.top;
  const float centerX = static_cast<float>(width) * g_depthAverageHalf + static_cast<float>(area.left);
  const float centerY = static_cast<float>(height) * g_depthAverageHalf + static_cast<float>(area.top);

  marker.name = name;
  marker.area = area;
  marker.centerX = centerX;
  marker.centerY = centerY;
  marker.width = static_cast<float>(width);
  marker.height = static_cast<float>(height);
}

// OFFSET: 0x0091CF40
// FUNCTION: fn_12622
void UIRoMapWnd::ClearRecords() {
  this->m_records.clear();
  this->unknown_00D0 = 0;
  this->unknown_00C4 = 0;
  this->unknown_00C8 = 0;
  this->unknown_00C0 = 0;
  this->unknown_00BC = 0;
  this->unknown_00CC = 0;
}

// OFFSET: 0x0091E0F0
// FUNCTION: fn_12625
void UIRoMapWnd::ResetKafraBounds() {
  *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x21C) = 0;
  *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x224) = 0;
  *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x218) = 0;
  *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x220) = 0;
}

// OFFSET: 0x0091EBA0
// FUNCTION: fn_12634
void UIRoMapWnd::UpdateNavigationButtons() {
  UIWindow** buttons = reinterpret_cast<UIWindow**>(reinterpret_cast<unsigned char*>(this) + 0x20C);
  if (g_navigationWindow != nullptr && !g_navigationMinimized) {
    buttons[0]->Move(-100, -100);
    buttons[1]->Move(-100, -100);
    buttons[2]->Move(-100, -100);
  } else {
    buttons[0]->Move(335, 0);
    buttons[1]->Move(375, 3);
    buttons[2]->Move(131, 0);
    g_navigationHidden = true;
  }

  this->Invalidate();
}

// OFFSET: 0x009335B0
// FUNCTION: fn_12725
void UIRoMapWnd::Slot33(int, int) {
  if (g_windowMgr.unknown_0214 != 0) {
    this->m_frameContext->windows.CloseWindow(26);
  }

  this->unknown_0208 = 0;
}

// OFFSET: 0x00919B50
// FUNCTION: fn_12598
void UIRoMapWnd::DrawHoverTooltip() {
  if (this->unknown_00C4 != nullptr) {
    delete this->unknown_00C4;
    this->unknown_00C4 = nullptr;
  }

  if (this->unknown_00BC == nullptr || this->unknown_00C8 == nullptr) {
    return;
  }

  const TextLocale locale = {};
  bool leftSide = false;
  int maxWidth = 0;
  const int lineHeight = this->TextHeight(g_fontCache, locale, " ", 0, 0, 12, 1, 0) + 3;
  const WorldViewRecord* record = this->unknown_00BC;
  const int* anchor = this->unknown_00C8;
  const int recordLeft = record->left;
  int x;
  if (anchor[2] < record->right) {
    leftSide = true;
    x = anchor[0] - 10;
  } else {
    x = anchor[2] + 10;
  }

  int y = anchor[1];
  int count = 0;
  for (std::list<std::string>::const_iterator it = record->unknown_004C.begin(); it != this->unknown_00BC->unknown_004C.end(); ++it) {
    const int width = this->TextWidth(g_fontCache, locale, it->c_str(), 0, 0, 12, 1, 0);
    if (maxWidth < width) {
      maxWidth = width;
    }
    ++count;
  }

  const int width = maxWidth + 10;
  const int height = count * lineHeight + 7;
  int boxX = leftSide ? x - width : x;
  if (boxX + width > g_renderer->viewport.width) {
    boxX = recordLeft - width - 10;
  } else if (!leftSide) {
    y += 17;
  }

  if (height + y > g_renderer->viewport.height) {
    y = g_renderer->viewport.height - height + 10;
  }

  int textY = y + 5;
  for (std::list<std::string>::const_iterator it = this->unknown_00BC->unknown_004C.begin(); it != this->unknown_00BC->unknown_004C.end(); ++it) {
    const std::uint32_t color = std::strcmp(it->c_str(), g_session.DecodePlayerName()) == 0 ? 0x8080F0 : 0xFFFFFF;
    DrawOutlinedText(*this, boxX + 5, textY, it->c_str(), 0, color, 0, 0, 12, 1, 0);
    textY += lineHeight;
  }

  this->unknown_00C4 = new RoMapTooltipBox{boxX, y, width, height, 1};
  DrawRoundedBorder(*this, boxX, y, width, height, 0xFFFFFF);
}

// OFFSET: 0x00919E00
// FUNCTION: fn_12599
void UIRoMapWnd::DrawMapNames() {
  const int fontHeight = g_worldViewMgr.dungeonFontSize[g_worldViewMgr.screenScale];
  const std::uint32_t color = g_worldViewMgr.dungeonFontColor;
  const std::uint32_t outline = g_worldViewMgr.dungeonFontOutline;
  const TextLocale locale = {};
  std::string name;
  std::vector<CWorldViewMap*>::iterator it = g_worldViewMgr.CurrentWorld()->maps.begin();
  const std::vector<CWorldViewMap*>::iterator end = g_worldViewMgr.CurrentWorld()->maps.end();
  for (; it != end; ++it) {
    const CWorldViewMap* map = *it;
    if (map->id >= 100) {
      continue;
    }

    const int* area = map->fields.empty() ? &map->left : &map->fields.front()->left;
    name = map->name;
    if (name.empty()) {
      continue;
    }

    const int textWidth = this->TextWidth(g_fontCache, locale, std::string(name).c_str(), 0, 0, fontHeight, 0, 0);
    const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - area[2] - area[0]) * g_depthAverageHalf));
    const int y = area[1] + 2;
    this->DrawTextAt(g_fontCache, locale, x + 1, y + 1, name.c_str(), 0, 0, fontHeight, outline, 0, 0);
    this->DrawTextAt(g_fontCache, locale, x, y, name.c_str(), 0, 0, fontHeight, color, 0, 0);
  }
}

// OFFSET: 0x0091A0B0
// FUNCTION: fn_12600
void UIRoMapWnd::DrawFieldLabels() {
  if (!this->m_showLabels) {
    return;
  }

  const int townHeight = g_worldViewMgr.dungeonFontSize[g_worldViewMgr.screenScale];
  const int fieldHeight = g_worldViewMgr.mobFont[g_worldViewMgr.screenScale];
  const int levelHeight = g_worldViewMgr.mobLevelFont[g_worldViewMgr.screenScale];
  const std::uint32_t townColor = g_worldViewMgr.dungeonFontColor;
  const std::uint32_t townOutline = g_worldViewMgr.dungeonFontOutline;
  const std::uint32_t fieldColor = g_worldViewMgr.mobColor;
  const std::uint32_t fieldShadow = g_worldViewMgr.mobOutline;
  const std::uint32_t levelColor = g_worldViewMgr.mobLevelColor;
  const std::uint32_t levelOutline = g_worldViewMgr.mobLevelOutline;
  const std::uint32_t borderColor = g_worldViewMgr.boxLineColor;
  const TextLocale locale = {};
  std::string name;
  bool atBottom = false;
  std::vector<CWorldViewMap*>::iterator it = g_worldViewMgr.CurrentWorld()->maps.begin();
  const std::vector<CWorldViewMap*>::iterator end = g_worldViewMgr.CurrentWorld()->maps.end();
  for (; it != end; ++it) {
    const CWorldViewMap* map = *it;
    if (map->id < 100) {
      const int* area = map->fields.empty() ? &map->left : &map->fields.front()->left;
      if (!this->m_showLabels) {
        continue;
      }

      name = map->displayName;
      if (name.empty()) {
        continue;
      }

      const int textWidth = this->TextWidth(g_fontCache, locale, std::string(name).c_str(), 0, 0, townHeight, 0, 0);
      const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - area[2] - area[0]) * g_depthAverageHalf));
      const int y = townHeight + 2 + area[1];
      this->DrawTextAt(g_fontCache, locale, x + 1, y + 1, name.c_str(), 0, 0, townHeight, townOutline, 0, 0);
      this->DrawTextAt(g_fontCache, locale, x, y, name.c_str(), 0, 0, townHeight, townColor, 0, 0);
      continue;
    }

    for (std::vector<CWorldViewField*>::const_iterator fieldIt = map->fields.begin(); fieldIt != map->fields.end(); ++fieldIt) {
      name = (*fieldIt)->unknown_002C;
      if (name.empty()) {
        continue;
      }

      const CWorldViewField* field = *fieldIt;
      DrawRoundedBorder(*this, field->left, field->top, field->right - field->left, field->bottom - field->top, borderColor);
      int y;
      int levelY;
      if (atBottom) {
        y = field->top + 3;
        levelY = y + (fieldHeight + 3);
        atBottom = false;
      } else {
        y = field->bottom - fieldHeight - 3;
        atBottom = true;
        levelY = y - levelHeight - 3;
      }

      const int textWidth = this->TextWidth(g_fontCache, locale, std::string(name).c_str(), 0, 0, fieldHeight, 0, 0);
      const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - field->right - field->left) * g_depthAverageHalf));
      this->DrawTextAt(g_fontCache, locale, x + 1, y + 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(g_fontCache, locale, x - 1, y - 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(g_fontCache, locale, x, y, name.c_str(), 0, 0, fieldHeight, fieldColor, 0, 0);
      name = (*fieldIt)->unknown_0044;
      if (name.empty()) {
        continue;
      }

      const int levelWidth = this->TextWidth(g_fontCache, locale, std::string(name).c_str(), 0, 0, levelHeight, 1, 0);
      const int levelX = std::abs(static_cast<int>(static_cast<float>(levelWidth - field->right - field->left) * g_depthAverageHalf));
      DrawOutlinedText(*this, levelX, levelY, name.c_str(), 0, levelColor, levelOutline, 0, levelHeight, 1, 0);
    }
  }
}

// OFFSET: 0x0091A7D0
// FUNCTION: fn_12601
void UIRoMapWnd::DrawRecordBorders() {
  const std::string mapName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(9, 0, 0, 0, 0));
  for (std::list<WorldViewRecord>::const_iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld != it->worldIndex) {
      continue;
    }

    DrawRoundedBorder(*this, it->left, it->top, it->right - it->left, it->bottom - it->top, mapName == it->name ? 0x23AEE8 : 0x7EE2F7);
  }
}

// OFFSET: 0x0091A960
// FUNCTION: fn_12602
void UIRoMapWnd::DrawDungeonLinks() {
  CTexture* const positionTexture = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_dunPos.bmp", 0, true, true, false);
  CTexture* const selectTexture = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_dunSelect.bmp", 0, true, true, false);
  const std::uint32_t rectColor = g_worldViewMgr.unknown_0084;
  const std::uint32_t lineColor = g_worldViewMgr.dungeonColor;
  const std::uint32_t selectColor = g_worldViewMgr.dungeonSelectColor;
  std::vector<CWorldViewMap*>::iterator it = g_worldViewMgr.CurrentWorld()->maps.begin();
  const std::vector<CWorldViewMap*>::iterator end = g_worldViewMgr.CurrentWorld()->maps.end();
  for (; it != end; ++it) {
    const CWorldViewMap* map = *it;
    if (map->id >= 100) {
      continue;
    }

    const WorldViewRecord* selected = this->unknown_00C0;
    bool isSelected;
    int x;
    int y;
    int width;
    int height;
    if (map->fields.empty()) {
      isSelected = selected != nullptr && map->left == selected->left && map->right == selected->right && map->top == selected->top &&
                   map->bottom == selected->bottom;
      x = map->left;
      y = map->top;
      width = map->right - map->left;
      height = map->bottom - map->top;
    } else {
      const CWorldViewField* field = map->fields.front();
      isSelected = selected != nullptr && field->left == selected->left && field->right == selected->right && field->top == selected->top &&
                   field->bottom == selected->bottom;
      width = field->right - field->left;
      height = field->bottom - field->top;
      const int mapWidth = map->right - map->left;
      const int mapHeight = map->bottom - map->top;
      const float startX = static_cast<float>(width) * g_depthAverageHalf + static_cast<float>(field->left);
      const float startY = static_cast<float>(height) * g_depthAverageHalf + static_cast<float>(field->top);
      const float endX = static_cast<float>(mapWidth) * g_depthAverageHalf + static_cast<float>(map->left);
      const float endY = static_cast<float>(mapHeight) * g_depthAverageHalf + static_cast<float>(map->top) + 1.0f;
      CTexture* const lineTexture = g_texMgr.Default();
      if (isSelected) {
        DrawScreenLine(
          g_renderer->primitives, g_renderer->queues, startX, startY, endX, endY, 1, lineTexture, (selectColor & 0xFFFFFF) - 0x1000000,
          g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
      } else {
        DrawScreenLine(
          g_renderer->primitives, g_renderer->queues, startX, startY, endX, endY, 1, lineTexture, lineColor, g_renderer->fog.supported != 0,
          g_depthStart, g_depthEnd
        );
      }

      DrawTexturedRectangle(
        g_renderer->primitives, g_renderer->queues, map->left, map->top, mapWidth, mapHeight, isSelected ? 0xFFFFFFFF : rectColor, positionTexture,
        g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      x = field->left;
      y = field->top;
    }

    DrawTexturedRectangle(
      g_renderer->primitives, g_renderer->queues, x, y, width, height, isSelected ? 0xFFFFFFFF : rectColor, selectTexture,
      g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
  }
}

// OFFSET: 0x0091AC50
// FUNCTION: fn_12603
void UIRoMapWnd::DrawMarks() {
  const int count = g_worldViewMgr.MarkCount();
  for (int index = 0; index < count; ++index) {
    const int* area = g_worldViewMgr.MarkAreaAt(index, true);
    if (area == nullptr) {
      continue;
    }

    const int x = area[0] + 1;
    const int y = area[1] + 1;
    const int width = area[2] - area[0] - 2;
    const int height = area[3] - area[1] - 2;
    DrawAlphaRectangle(
      g_renderer->primitives, g_renderer->queues, x, y, width, height, g_worldViewMgr.runColor, g_renderer->fog.supported != 0, g_depthStart,
      g_depthEnd
    );
    if (index != 0) {
      const int* previous = g_worldViewMgr.MarkAreaAt(index - 1, true);
      if (previous != nullptr) {
        const float startX = static_cast<float>(previous[2] - previous[0]) * g_depthAverageHalf + static_cast<float>(previous[0]);
        const float startY = static_cast<float>(previous[3] - previous[1]) * g_depthAverageHalf + static_cast<float>(previous[1]);
        const float endX = static_cast<float>(area[2] - area[0]) * g_depthAverageHalf + static_cast<float>(area[0]);
        const float endY = static_cast<float>(area[3] - area[1]) * g_depthAverageHalf + static_cast<float>(area[1]);
        DrawScreenLine(
          g_renderer->primitives, g_renderer->queues, startX, startY, endX, endY, 5,
          g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_Airf2.bmp", 0, false, true, false), 0x99FFFFFF,
          g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
      }
    }

    if (count - 1 != index) {
      continue;
    }

    const WorldViewRecord* mark = g_worldViewMgr.MarkAt(count - 1, true);
    if (mark == nullptr) {
      continue;
    }

    std::string destination = g_navigationManager.GetDestinationInfo()->GetName();
    destination += ".rsw";
    if (destination == mark->name) {
      CTexture* texture = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_End.bmp", 0, false, true, false);
      if (texture != nullptr) {
        const std::uint32_t textureHeight = texture->Height();
        const std::uint32_t textureWidth = texture->Width();
        const int drawY = static_cast<int>(
          static_cast<float>(height) * g_depthAverageHalf + static_cast<float>(y) - static_cast<float>(texture->Height())
        );
        const int drawX = static_cast<int>(
          static_cast<float>(width) * g_depthAverageHalf + static_cast<float>(x) - static_cast<float>(texture->Width())
        );
        DrawTexturedRectangle(
          g_renderer->primitives, g_renderer->queues, drawX, drawY, textureWidth, textureHeight, 0xCCFFFFFF, texture,
          g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
      }
    }
  }
}

// OFFSET: 0x0091B170
// FUNCTION: fn_12604
void UIRoMapWnd::DrawRecordFaces(int) {
  const std::string mapName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(9, 0, 0, 0, 0));
  const int faceSize = g_worldViewMgr.screenScale * 2 + 8;
  std::string image = "유저인터페이스\\worldview_interface\\wv_face1.bmp";
  for (std::list<WorldViewRecord>::const_iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld != it->worldIndex) {
      continue;
    }

    const int x = static_cast<int>(static_cast<float>(it->left - faceSize) + static_cast<float>(it->right - it->left) * g_depthAverageHalf);
    const int y = static_cast<int>(static_cast<float>(it->top - faceSize) + static_cast<float>(it->bottom - it->top) * g_depthAverageHalf);
    if (mapName == it->name) {
      image = "유저인터페이스\\worldview_interface\\wv_face1.bmp";
    } else {
      image = "유저인터페이스\\worldview_interface\\wv_face.bmp";
    }

    DrawTexturedRectangle(
      g_renderer->primitives, g_renderer->queues, x, y, faceSize * 2, faceSize * 2, 0xEEFFFFFF,
      g_texMgr.Load(image.c_str(), 0, false, true, false), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
  }
}

// OFFSET: 0x0091BF80
// FUNCTION: fn_12612
void UIRoMapWnd::RefreshPartyRecords() {
  const int count = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyCount();
  if (count > 0) {
    for (int index = 0; index < count; ++index) {
      MessengerMember member = ClientMessenger{g_session.m_partyMembers, g_session.m_friends}.PartyAt(index);
      if (member.valid == 0 || member.offline != 0) {
        continue;
      }

      bool found;
      {
        std::string name = member.name;
        std::string map = member.map;
        found = false;
        for (std::list<WorldViewRecord>::iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
          if (it->name == map) {
            it->unknown_004C.push_back(name);
            found = true;
            break;
          }
        }
      }

      if (!found) {
        WorldViewRecord record;
        record.name = member.map;
        record.unknown_004C.push_back(member.name);
        if (this->LocateRecord(record)) {
          this->m_records.push_back(record);
        }
      }
    }
  } else {
    std::string map = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(9, 0, 0, 0, 0));
    WorldViewRecord record;
    record.name = map;
    std::string name = g_session.DecodePlayerName();
    record.unknown_004C.push_back(std::move(name));
    if (this->LocateRecord(record)) {
      this->m_records.push_back(record);
    }
  }
}

// OFFSET: 0x0091C400
// FUNCTION: fn_12613
bool UIRoMapWnd::LocateRecord(WorldViewRecord& record) {
  std::string name = record.name;
  for (unsigned int index = 0; index < g_worldViewMgr.worlds.size(); ++index) {
    if (CWorldViewField* field = g_worldViewMgr.worlds[index]->FindField(name.c_str())) {
      record.worldIndex = index;
      record.left = field->left;
      record.top = field->top;
      record.right = field->right;
      record.bottom = field->bottom;
      record.unknown_0048 = field->unknown_0010;
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00932920
// FUNCTION: fn_12713
void UIRoMapWnd::Slot27(int x, int y) {
  if (this == g_windowMgr.Capture()) {
    return;
  }

  if (this->m_mouseX == x && this->m_mouseY == y) {
    return;
  }

  this->m_mouseX = x;
  this->m_mouseY = y;
  this->unknown_00BC = nullptr;
  for (WorldViewRecord& record : this->m_records) {
    if (g_worldViewMgr.currentWorld == record.worldIndex && record.left < x && x < record.right && record.top < y && y < record.bottom) {
      this->unknown_00BC = &record;
      break;
    }
  }

  this->Invalidate();

  if (this->unknown_00C0) {
    delete this->unknown_00C0;
    this->unknown_00C0 = nullptr;
  }

  unsigned int mapIndex = 0;
  CWorldViewWorld* world = g_worldViewMgr.CurrentWorld();
  if (!world) {
    return;
  }

  CWorldViewField* field = world->FieldAt(mapIndex, x, y);
  if (!field) {
    return;
  }

  const int mapId = world->maps[mapIndex]->id;
  this->unknown_00C0 = new WorldViewRecord();
  this->unknown_00C0->worldIndex = g_worldViewMgr.currentWorld;
  this->unknown_00C0->mapIndex = mapId;
  this->unknown_00C0->name = field->name;
  this->unknown_00C0->left = field->left;
  this->unknown_00C0->top = field->top;
  this->unknown_00C0->right = field->right;
  this->unknown_00C0->bottom = field->bottom;
  this->unknown_00C0->unknown_0048 = field->unknown_0010;

  CWorldViewMap* map = world->maps[mapIndex];
  this->unknown_00C0->unknown_000C = map->left;
  this->unknown_00C0->unknown_0010 = map->top;
  this->unknown_00C0->unknown_0014 = map->right;
  this->unknown_00C0->unknown_0018 = map->bottom;
  this->unknown_00C0->unknown_001C = map->unknown_0044;
  this->Invalidate();
}

// OFFSET: 0x0091DBB0
// FUNCTION: fn_12624
void CKafraMoveMapServiceInfo::Build(std::string mapName) {
  std::vector<std::string> names = g_kafraServiceMapList[mapName];
  this->markers.clear();
  for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it) {
    CWorldViewField* field = g_worldViewMgr.worlds[0]->FindField(it->c_str());
    if (field) {
      std::string name = field->name;
      WorldViewArea area = {field->left, field->top, field->right, field->bottom, field->unknown_0010};
      RoMapMarker marker;
      SetRoMapMarker(name, area, marker);
      this->markers.push_back(marker);
    }
  }
}

// OFFSET: 0x00932C80
// FUNCTION: fn_12716
void CKafraMoveMapServiceInfo::Update(UIRoMapWnd* wnd, float elapsed) {
  WorldViewRecord* record = wnd->unknown_00C0;
  if (record && g_kafraServiceMapList.find(record->name) != g_kafraServiceMapList.end()) {
    if (this->mapName != record->name) {
      this->mapName = record->name;
      this->Build(record->name);
      SetRoMapMarker(record->name, reinterpret_cast<const WorldViewArea&>(record->left), this->current);
      this->blend = 0.0f;
    }

    this->blend += elapsed;
    if (this->blend > 1.0f) {
      this->blend = 1.0f;
    }
    return;
  }

  this->mapName = "";
  this->markers.clear();
}

// OFFSET: 0x009409E0
// FUNCTION: fn_12779
void UIRoMapWnd::DrawMapMarker(const RoMapMarker& marker, unsigned char skipSelected) {
  if (skipSelected == 1) {
    const WorldViewRecord* selected = this->unknown_00C0;
    if (marker.area.left == selected->left && marker.area.right == selected->right && marker.area.top == selected->top &&
        marker.area.bottom == selected->bottom) {
      return;
    }
  }

  DrawRoundedBorder(*this, marker.area.left, marker.area.top, static_cast<int>(marker.width), static_cast<int>(marker.height), 0xFFFFFF);
  DrawAlphaRectangle(
    g_renderer->primitives, g_renderer->queues, marker.area.left, marker.area.top, static_cast<int>(marker.width), static_cast<int>(marker.height), 0x66000000,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
  std::string name = g_session.GetMapDisplayName(marker.name.c_str());
  DrawOutlinedText(*this, marker.area.left + 2, marker.area.top + 5, name.c_str(), 0, 0xD2FF, 0, 0, 12, 1, 0);
}

// OFFSET: 0x009144B0
// FUNCTION: fn_12541
UIRoMapWnd::~UIRoMapWnd() {
  delete reinterpret_cast<WorldViewArea*>(this->unknown_00C8);
  delete this->unknown_00C0;
  this->unknown_00D0 = 0;
}

// OFFSET: 0x0091E4D0
// FUNCTION: fn_12627
bool CFlyingShipRouteInfo::IsRouteMap(std::string mapName) {
  if (this->unknown_002C == 0) {
    if ((mapName.size() == 10 && std::memcmp(mapName.c_str(), "izlude.rsw", 10) == 0) ||
        (mapName.size() == 13 && std::memcmp(mapName.c_str(), "ra_fild12.rsw", 13) == 0) ||
        (mapName.size() == 8 && std::memcmp(mapName.c_str(), "yuno.rsw", 8) == 0)) {
      return true;
    }
  } else if (this->unknown_002C == 1) {
    if ((mapName.size() == 12 && std::memcmp(mapName.c_str(), "einbroch.rsw", 12) == 0) ||
        (mapName.size() == 15 && std::memcmp(mapName.c_str(), "lighthalzen.rsw", 15) == 0) ||
        (mapName.size() == 9 && std::memcmp(mapName.c_str(), "hugel.rsw", 9) == 0)) {
      return true;
    }
  }
  return false;
}
