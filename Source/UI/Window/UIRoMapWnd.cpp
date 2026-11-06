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
#include "Client/Replay.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

// GLOBAL: gl_24799
CWorldViewMgr g_worldViewMgr;

// OFFSET: 0x00931080
// FUNCTION: fn_12697
void UIRoMapWnd::Slot25(int, int) {
}

__declspec(noinline) void SetRoMapMarker(std::string name, const WorldViewArea& area, RoMapMarker& marker);

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
__declspec(noinline) void UIRoMapWnd::ClearRecords() {
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
  if (g_windowMgr.window_4F174 != nullptr && !g_navigationMinimized) {
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
    g_windowMgr.CloseWindow(26);
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

  bool leftSide = false;
  int maxWidth = 0;
  const int lineHeight = this->TextHeight(" ", 0, 0, 12, 1, 0) + 3;
  const WorldViewRecord* record = this->unknown_00BC;
  const int* anchor = this->unknown_00C8;
  const RECT rect = reinterpret_cast<const RECT&>(record->left);
  int x;
  if (anchor[2] < rect.right) {
    leftSide = true;
    x = anchor[0] - 10;
  } else {
    x = anchor[2] + 10;
  }

  int y = anchor[1];
  int count = 0;
  for (std::list<std::string>::const_iterator it = record->unknown_004C.begin(); it != this->unknown_00BC->unknown_004C.end(); ++it) {
    const int width = this->TextWidth(it->c_str(), 0, 0, 12, 1, 0);
    if (maxWidth < width) {
      maxWidth = width;
    }
    ++count;
  }

  const int width = maxWidth + 10;
  const int height = count * lineHeight + 7;
  int boxX = leftSide ? x - width : x;
  if (boxX + width > g_renderer->viewport.width) {
    boxX = rect.left - width - 10;
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

  this->unknown_00C4 = new RoMapTooltipBox(boxX, y, width, height);
  DrawRoundedBorder(*this, boxX, y, width, height, 0xFFFFFF);
}

// OFFSET: 0x00919E00
// FUNCTION: fn_12599
void UIRoMapWnd::DrawMapNames() {
  const int fontHeight = g_worldViewMgr.dungeonFontSize[g_worldViewMgr.screenScale];
  const std::uint32_t color = g_worldViewMgr.dungeonFontColor;
  const std::uint32_t outline = g_worldViewMgr.dungeonFontOutline;
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

    const int left = area[0];
    const int right = area[2];
    const int textWidth = this->TextWidth(std::string(name).c_str(), 0, 0, fontHeight, 0, 0);
    const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - right - left) * g_depthAverageHalf));
    const int y = area[1] + 2;
    this->DrawTextAt(x + 1, y + 1, name.c_str(), 0, 0, fontHeight, outline, 0, 0);
    this->DrawTextAt(x, y, name.c_str(), 0, 0, fontHeight, color, 0, 0);
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

      const int textWidth = this->TextWidth(name.c_str(), 0, 0, townHeight, 0, 0);
      const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - area[2] - area[0]) * g_depthAverageHalf));
      const int y = townHeight + 2 + area[1];
      this->DrawTextAt(x + 1, y + 1, name.c_str(), 0, 0, townHeight, townOutline, 0, 0);
      this->DrawTextAt(x, y, name.c_str(), 0, 0, townHeight, townColor, 0, 0);
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

      const int textWidth = this->TextWidth(name.c_str(), 0, 0, fieldHeight, 0, 0);
      const int x = std::abs(static_cast<int>(static_cast<float>(textWidth - field->right - field->left) * g_depthAverageHalf));
      this->DrawTextAt(x + 1, y + 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(x - 1, y - 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(x, y, name.c_str(), 0, 0, fieldHeight, fieldColor, 0, 0);
      name = (*fieldIt)->unknown_0044;
      if (name.empty()) {
        continue;
      }

      const int levelWidth = this->TextWidth(name.c_str(), 0, 0, levelHeight, 1, 0);
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

    const int width = it->right - it->left;
    const int height = it->bottom - it->top;
    DrawRoundedBorder(*this, it->left, it->top, width, height, mapName == it->name ? 0x23AEE8 : 0x7EE2F7);
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

      DrawTexturedRectangle(map->left, map->top, mapWidth, mapHeight, isSelected ? 0xFFFFFFFF : rectColor, positionTexture);
      x = field->left;
      y = field->top;
    }

    DrawTexturedRectangle(x, y, width, height, isSelected ? 0xFFFFFFFF : rectColor, selectTexture);
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
    DrawAlphaRectangle(x, y, width, height, g_worldViewMgr.runColor);
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
        DrawTexturedRectangle(drawX, drawY, textureWidth, textureHeight, 0xCCFFFFFF, texture);
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

    const int width = it->right - it->left;
    const int height = it->bottom - it->top;
    const int x = static_cast<int>(static_cast<float>(it->left - faceSize) + static_cast<float>(width) * 0.5f);
    const int y = static_cast<int>(static_cast<float>(it->top - faceSize) + static_cast<float>(height) * 0.5f);
    if (mapName == it->name) {
      image = "유저인터페이스\\worldview_interface\\wv_face1.bmp";
    } else {
      image = "유저인터페이스\\worldview_interface\\wv_face.bmp";
    }

    DrawTexturedRectangle(x, y, faceSize * 2, faceSize * 2, 0xEEFFFFFF, g_texMgr.Load(image.c_str(), 0, false, true, false));
  }
}

// OFFSET: 0x0091BF80
// FUNCTION: fn_12612
void UIRoMapWnd::RefreshPartyRecords() {
  const int count = g_session.PartyCount();
  if (count > 0) {
    for (int index = 0; index < count; ++index) {
      MessengerMember member = g_session.PartyAt(index);
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
      reinterpret_cast<WorldViewArea&>(record.left) = reinterpret_cast<const WorldViewArea&>(*field);
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
  for (auto it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld == it->worldIndex && it->left < x && x < it->right && it->top < y && y < it->bottom) {
      this->unknown_00BC = &*it;
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
  reinterpret_cast<WorldViewArea&>(this->unknown_00C0->left) = reinterpret_cast<const WorldViewArea&>(*field);

  CWorldViewMap* map = world->maps[mapIndex];
  reinterpret_cast<WorldViewArea&>(this->unknown_00C0->unknown_000C) = reinterpret_cast<const WorldViewArea&>(map->left);
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
      WorldViewArea area = reinterpret_cast<const WorldViewArea&>(*field);
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

// OFFSET: 0x00940510
// FUNCTION: fn_12778
void UIRoMapWnd::DrawKafraMarkers(float elapsed) {
  if (!this) {
    return;
  }

  this->m_kafraMarkers.Update(this, elapsed);
  this->m_kafraTop = 0;
  this->m_kafraBottom = 0;
  this->m_kafraLeft = 0;
  this->m_kafraRight = 0;
  if (this->m_kafraMarkers.markers.empty()) {
    return;
  }

  std::map<std::string, RoMapMarker> targets;
  const int count = static_cast<int>(this->m_kafraMarkers.markers.size());
  for (int i = 0; i < count; ++i) {
    RoMapMarker from = this->m_kafraMarkers.current;
    RoMapMarker to = this->m_kafraMarkers.markers[i];
    targets.insert(std::pair<std::string, RoMapMarker>(to.name, to));
    if (to.name.size() == 10 && std::memcmp(to.name.c_str(), "izlude.rsw", 10) == 0) {
      from.centerX = static_cast<float>(from.area.right);
      to.centerX = to.width * 0.8f + static_cast<float>(to.area.left);
      to.centerY = static_cast<float>(to.area.top);
    }

    CTexture* head = g_texMgr.Load("mapArrow_head.bmp", 0, true, true, false);
    CTexture* body = g_texMgr.Load("mapArrow_body.bmp", 1, true, true, false);
    this->DrawArrow(
      from.centerX, from.centerY, to.centerX, to.centerY, static_cast<float>(from.area.left), static_cast<float>(from.area.top),
      static_cast<float>(from.area.right), static_cast<float>(from.area.bottom), static_cast<float>(to.area.left), static_cast<float>(to.area.top),
      static_cast<float>(to.area.right), static_cast<float>(to.area.bottom), 10, body, 20, head, this->m_kafraMarkers.blend, 0xAAFFFFFF
    );
  }

  this->DrawKafraIcon(static_cast<float>(this->m_kafraMarkers.current.area.right), static_cast<float>(this->m_kafraMarkers.current.area.top));
  for (std::map<std::string, RoMapMarker>::iterator it = targets.begin(); it != targets.end(); ++it) {
    this->DrawMapMarker(it->second, 1);
  }
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
  DrawAlphaRectangle(marker.area.left, marker.area.top, static_cast<int>(marker.width), static_cast<int>(marker.height), 0x66000000);
  std::string name = g_session.GetMapDisplayName(marker.name.c_str());
  DrawOutlinedText(*this, marker.area.left + 2, marker.area.top + 5, name.c_str(), 0, 0xD2FF, 0, 0, 12, 1, 0);
}

// OFFSET: 0x009119B0
// FUNCTION: fn_12522
UIRoMapWnd::UIRoMapWnd() : UIFrameWnd(0), unknown_00D0(0) {
  g_session.m_hotkeyScript->Call("ReadKafraMoveMapServiceInfo", "");
  g_session.m_hotkeyScript->Call("ReadKafraMiniMapPosInfo", "");
  this->unknown_01FC.clear();
  this->ClearRecords();
  this->m_showLabels = 0;
  this->m_lastTime = 0;
  this->m_resetTime = 1;
  this->m_flyingShipRoute.unknown_0038.assign("유저인터페이스\\worldview_interface\\wv_Airf3.bmp", 0x2F);
  this->m_flyingShipRoute.unknown_0030 = 0xAAFFFFFF;
  this->m_flyingShipRoute.unknown_002C = 1;
  this->m_flyingShipRoute2.unknown_0038.assign("유저인터페이스\\worldview_interface\\wv_Airf3.bmp", 0x2F);
  this->m_flyingShipRoute2.unknown_0030 = 0xAAFFFFFF;
  this->m_flyingShipRoute2.unknown_002C = 0;
  this->m_comboHeight = 0x12;
  this->m_mouseX = 0;
  this->m_mouseY = 0;
  this->m_kafraTop = 0;
  this->m_kafraBottom = 0;
  this->m_kafraLeft = 0;
  this->m_kafraRight = 0;
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
  switch (this->unknown_002C) {
  case 0:
    if (mapName == "izlude.rsw" || mapName == "ra_fild12.rsw" || mapName == "yuno.rsw") {
      return true;
    }
    break;
  case 1:
    if (mapName == "einbroch.rsw" || mapName == "lighthalzen.rsw" || mapName == "hugel.rsw" || mapName == "yuno.rsw") {
      return true;
    }
    break;
  }

  return false;
}

// OFFSET: 0x0091E120
// FUNCTION: fn_12626
void CFlyingShipRouteInfo::BuildRoute(const std::vector<std::string>& mapNames) {
  this->entries.clear();
  for (std::vector<std::string>::const_iterator it = mapNames.begin(); it != mapNames.end(); ++it) {
    CWorldViewField* field = g_worldViewMgr.worlds[0]->FindField(it->c_str());
    if (field) {
      std::string name = field->name;
      WorldViewArea area = {field->left, field->top, field->right, field->bottom, field->unknown_0010};
      RoMapMarker marker;
      SetRoMapMarker(name, area, marker);
      this->entries.push_back(reinterpret_cast<const FlyingShipRouteEntry&>(marker));
    }
  }
  reinterpret_cast<unsigned char*>(&this->unknown_0034)[1] = 1;
}

// OFFSET: 0x00938FF0
// FUNCTION: fn_12754
int UIRoMapWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 0:
    if (sender == this->m_searchEdit) {
      if (this->m_searchEdit->Length() > 0) {
        this->SendMsg(this->m_searchEdit, 6, 321, 0, 0, 0);
        return 0;
      }
    }
    break;
  case 6:
    switch (argument1) {
    case 192:
      this->m_showLabels = !this->m_showLabels;
      this->m_labelCheck->geometry.unknown_0030 = this->m_showLabels;
      this->m_labelCheck->Invalidate();
      this->Invalidate();
      return 0;
    case 200:
      g_windowMgr.EnsureWindow(203);
      if (g_windowMgr.window_4F174) {
        g_windowMgr.window_4F174->SendMsg(nullptr, 6, 200, 0, 0, 0);
      }
      this->UpdateNavigationButtons();
      return 0;
    case 201:
      g_windowMgr.CloseWindow(140);
      return 0;
    case 321:
      if (this->m_searchEdit->Length()) {
        std::string name(this->m_searchEdit->Text());
        if (g_navigationManager.SearchNavigation(name, 0, true) == true) {
          reinterpret_cast<UINavigationWnd*>(this)->RefreshWorldMapLevels();
        }
      }
      return 0;
    }
    return 0;
  case 39:
    if (this->m_worldCombo == sender) {
      g_worldViewMgr.currentWorld = argument1;
      this->Invalidate();
      return 0;
    }
    if (argument1 >= 0 && static_cast<unsigned int>(argument1) < this->unknown_01FC.size()) {
      {
      std::string name(g_worldViewMgr.CurrentWorld()->maps[this->unknown_0208]->fields[argument1]->name);
      if (name.size() > 4) {
        name.resize(name.size() - 4);
      }
      g_navigationMinimized = false;
      g_navigationManager.SearchNavigation(name, 0, false);
      if (g_windowMgr.LookupWindow(203)) {
        static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->SelectRoute(0);
        static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->SelectRouteNode(0);
      }
      this->UpdateNavigationButtons();
      }
      this->Invalidate();
      return 0;
    }
    return 0;
  case 60:
    this->ClearRecords();
    this->RefreshPartyRecords();
    this->Invalidate();
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0091CFB0
// FUNCTION: fn_12623
bool CFlyingShipRouteInfo::UpdateRoute(WorldViewRecord* record) {
  if (record != nullptr && this->IsRouteMap(record->name)) {
    if (this->m_routeBuilt == 1) {
      return true;
    }

    std::string name = record->name;

    switch (this->unknown_002C) {
    case 0:
      if (name == "izlude.rsw") {
        std::vector<std::string> route;
        route.push_back("izlude.rsw");
        route.push_back("yuno.rsw");
        route.push_back("ra_fild12.rsw");
        route.push_back("izlude.rsw");
        this->BuildRoute(route);
      } else if (name == "ra_fild12.rsw") {
        std::vector<std::string> route;
        route.push_back("ra_fild12.rsw");
        route.push_back("izlude.rsw");
        route.push_back("yuno.rsw");
        route.push_back("ra_fild12.rsw");
        this->BuildRoute(route);
      } else if (name == "yuno.rsw") {
        std::vector<std::string> route;
        route.push_back("yuno.rsw");
        route.push_back("ra_fild12.rsw");
        route.push_back("izlude.rsw");
        route.push_back("yuno.rsw");
        this->BuildRoute(route);
      } else {
        return false;
      }
      return true;
    case 1:
      if (name == "einbroch.rsw") {
        std::vector<std::string> route;
        route.push_back("einbroch.rsw");
        route.push_back("lighthalzen.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("yuno.rsw");
        route.push_back("hugel.rsw");
        route.push_back("yuno.rsw");
        route.push_back("einbroch.rsw");
        this->BuildRoute(route);
      } else if (name == "lighthalzen.rsw") {
        std::vector<std::string> route;
        route.push_back("lighthalzen.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("yuno.rsw");
        route.push_back("hugel.rsw");
        route.push_back("yuno.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("lighthalzen.rsw");
        this->BuildRoute(route);
      } else if (name == "hugel.rsw") {
        std::vector<std::string> route;
        route.push_back("hugel.rsw");
        route.push_back("yuno.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("lighthalzen.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("yuno.rsw");
        route.push_back("hugel.rsw");
        this->BuildRoute(route);
      } else if (name == "yuno.rsw") {
        std::vector<std::string> route;
        route.push_back("yuno.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("lighthalzen.rsw");
        route.push_back("einbroch.rsw");
        route.push_back("yuno.rsw");
        route.push_back("hugel.rsw");
        route.push_back("yuno.rsw");
        this->BuildRoute(route);
      } else {
        return false;
      }
      return true;
    }

    return false;
  }

  if (this->m_moving) {
    this->unknown_0028 = 1;
    this->unknown_0018 = 0;
    this->unknown_0024 = 0.0f;
    this->unknown_0034 = 0;
  }
  return false;
}

// INLINED
__forceinline void CFlyingShipRouteInfo::Update(UIRoMapWnd* wnd, float elapsed) {
  if (!this->UpdateRoute(wnd->unknown_00C0)) {
    return;
  }

  const int last = static_cast<int>(this->entries.size()) - 1;
  if (last > 0) {
    int index = this->unknown_0018;
    this->m_moving = 1;
    if (this->unknown_0024 > this->unknown_0020) {
      if (index >= last - 1) {
        wnd->Invalidate();
        index = -1;
      }
      ++index;
      this->unknown_0024 = 0.0f;
      this->unknown_0018 = index;
    }

    const float dx = this->entries[index].unknown_002C[0] - this->entries[index + 1].unknown_002C[0];
    const float dy = this->entries[index].unknown_002C[1] - this->entries[index + 1].unknown_002C[1];
    const float distance = SquareRoot(dx * dx + dy * dy);
    if (this->unknown_0024 == 0.0f) {
      this->unknown_0020 = distance;
    }
    this->unknown_0024 = elapsed * 200.0f + this->unknown_0024;
    this->unknown_001C = this->unknown_0024 / this->unknown_0020;
  }

  this->Draw(wnd);
}

// OFFSET: 0x009196E0
// FUNCTION: fn_12597
void UIRoMapWnd::DrawWorld() {
  g_worldViewMgr.Draw(this->m_showLabels, 0, 17, this->geometry.width, this->geometry.height - 17);
  const int count = g_worldViewMgr.LevelCount();
  for (int index = 0; index < count; ++index) {
    int kind = 0;
    const int* area = g_worldViewMgr.LevelAreaAt(kind, index, true);
    if (area == nullptr) {
      continue;
    }

    const int x = area[0] + 1;
    const int y = area[1] + 1;
    const int width = area[2] - area[0] - 2;
    const int height = area[3] - area[1] - 2;
    DrawAlphaRectangle(x, y, width, height, g_worldViewMgr.searchColors[kind]);
    DrawTexturedRectangle(x, y, width, height, 0xFFFFFFFF, g_texMgr.Load(g_worldViewMgr.searchImages[kind].c_str(), 0, false, true, false));
  }

  this->DrawMarks();
  if (!g_worldViewMgr.selection.name.empty() && g_worldViewMgr.currentWorld == g_worldViewMgr.selection.worldIndex) {
    const WorldViewRecord& selection = g_worldViewMgr.selection;
    DrawTexturedRectangle(selection.left, selection.top, selection.right - selection.left, selection.bottom - selection.top, 0xEEFFFFFF, g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_Select.bmp", 0, false, true, false));
  }

  this->DrawRecordFaces(1);
  this->DrawDungeonLinks();
  const WorldViewRecord* selected = this->unknown_00C0;
  if (selected != nullptr && this->unknown_00C8 != nullptr) {
    DrawAlphaRectangle(selected->left, selected->top, selected->right - selected->left, selected->bottom - selected->top, g_worldViewMgr.selectBoxColor);
    if (this->unknown_00D0 != 0) {
      DrawAlphaRectangle(this->unknown_00C8[0], this->unknown_00C8[1], 130, 130, 0x88000000);
      reinterpret_cast<CTexture*>(this->unknown_00D0)->Stretch(this->unknown_00C8[0], this->unknown_00C8[1], 130, 130);
    }
  }

  const RoMapTooltipBox* tooltip = this->unknown_00C4;
  if (tooltip != nullptr) {
    DrawAlphaRectangle(tooltip->x, tooltip->y, tooltip->width, tooltip->height, 0x66000000);
  }

  if (this->m_resetTime == 1) {
    this->m_lastTime = timeGetTime();
    this->m_resetTime = 0;
  }

  const std::uint32_t now = timeGetTime();
  const std::uint32_t delta = now - this->m_lastTime;
  this->m_lastTime = now;
  const float elapsed = static_cast<float>(delta) / 1000.0f;
  this->DrawKafraMarkers(elapsed);
  this->m_flyingShipRoute.Update(this, elapsed);
  this->m_flyingShipRoute2.Update(this, elapsed);
}

// OFFSET: 0x00925530
// FUNCTION: fn_12648
void UIRoMapWnd::Slot15(int, int) {
  this->m_worldCombo = new UIComboBoxDefWnd();
  this->m_worldCombo->dropdownHeight = 67;
  this->m_worldCombo->SetSize(110, this->m_comboHeight);
  this->m_worldCombo->Move(17, 0);
  this->m_worldCombo->borderRed = 100;
  this->m_worldCombo->borderGreen = 100;
  this->m_worldCombo->borderBlue = 100;
  this->m_worldCombo->red = 232;
  this->m_worldCombo->green = 232;
  this->m_worldCombo->blue = 232;
  this->AddChild(this->m_worldCombo);
  const int worldCount = static_cast<int>(g_worldViewMgr.worlds.size());
  for (int index = 0; index < worldCount; ++index) {
    this->m_worldCombo->AddOption(g_worldViewMgr.worlds[index]->name, 0);
  }

  g_worldViewMgr.SelectCurrentWorld();
  this->m_worldCombo->Select(g_worldViewMgr.currentWorld);
  g_worldViewMgr.LoadTextures();
  this->Invalidate();
  {
    std::string name = g_navigationManager.GetCurrentTargetName();
    if (!name.empty()) {
      name.append(".rsw", 4);
      g_worldViewMgr.SelectField(name.c_str());
    } else {
      g_worldViewMgr.ResetSelection();
    }
  }

  reinterpret_cast<UINavigationWnd*>(this)->RefreshWorldMapLevels();
  reinterpret_cast<UINavigationWnd*>(this)->RefreshWorldMapMarks();
  const int closeX = this->geometry.width - 18;
  UIBitmapButton* closeButton = new UIBitmapButton();
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_off.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_on.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_on.bmp", 2, 0);
  closeButton->SetSize(closeButton->BitmapWidth(), closeButton->BitmapHeight());
  closeButton->Move(closeX, 0);
  closeButton->SetId(201);
  closeButton->SetTooltip("ctrl+`");
  this->AddChild(closeButton);

  {
    std::string checked = "유저인터페이스\\checkbox_1.bmp";
    std::string unchecked = "유저인터페이스\\checkbox_0.bmp";
    this->m_labelCheck = new (std::nothrow) UICheckBox();
    this->m_labelCheck->SetSize(80, 12);
    this->m_labelCheck->SetImages(checked.c_str(), unchecked.c_str(), LookupMessage(0x988));
    this->m_labelCheck->SetId(192);
    this->m_labelCheck->Move(closeX - 82, 3);
    this->m_labelCheck->SetChecked(this->m_showLabels);
    this->m_labelCheck->SetTooltip("Tab");
    this->AddChild(this->m_labelCheck);

    this->m_searchButton = new UIBitmapButton();
    this->m_searchButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviSearch_A.bmp", 0, 0);
    this->m_searchButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviSearch_B.bmp", 1, 0);
    this->m_searchButton->SetBitmap("유저인터페이스\\navigation_interface\\NaviSearch_C.bmp", 2, 0);
    this->m_searchButton->SetSize(this->m_searchButton->BitmapWidth(), this->m_searchButton->BitmapHeight());
    this->m_searchButton->Move(-100, -100);
    this->m_searchButton->SetId(321);
    this->m_searchButton->SetTooltip(LookupMessage(0x894));
    this->AddChild(this->m_searchButton);

    this->m_searchListButton = new UIBitmapButton();
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 0, 0);
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 1, 0);
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 2, 0);
    this->m_searchListButton->SetSize(this->m_searchListButton->BitmapWidth(), this->m_searchListButton->BitmapHeight());
    this->m_searchListButton->Move(-100, -100);
    this->m_searchListButton->SetId(200);
    this->m_searchListButton->SetTooltip(LookupMessage(0xC8D));
    this->AddChild(this->m_searchListButton);

    this->m_searchEdit = new UIEditCtrl();
    this->m_searchEdit->SetSize(200, 18);
    this->m_searchEdit->Move(-100, -100);
    this->m_searchEdit->maximum = 60;
    this->m_searchEdit->Background(232, 232, 232);
    this->m_searchEdit->Border(200, 100, 100);
    this->m_searchEdit->flat = 0;
    UIEditCtrl* edit = this->m_searchEdit;
    if (const char* tooltip = LookupMessage(0x8A2)) {
      edit->tooltip = tooltip;
    }
    this->AddChild(this->m_searchEdit);
    if (g_replayAnimationScaling != 0) {
      this->m_searchEdit->disabled = 1;
      this->m_searchEdit->Invalidate();
    }

    this->UpdateNavigationButtons();
  }

  this->RefreshPartyRecords();
}
