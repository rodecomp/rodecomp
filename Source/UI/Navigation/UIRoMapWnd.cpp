#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "UI/Window/UIEnums.h"
#include "Script/LuaScript.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenGeometry.h"
#include "Render/Projection/ScreenRectangle.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Text/ColoredText.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "Client/Replay/Replay.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "Resource/MSGSTRINGID.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

template __declspec(noinline) std::vector<std::string>& std::map<std::string, std::vector<std::string>>::operator[](const std::string&);
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;

// GLOBAL: gl_24799
CWorldViewMgr g_worldViewMgr;

// OFFSET: 0x00931080
// FUNCTION: fn_12697
void UIRoMapWnd::OnLBtnDown(int, int) {
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
  marker.center[0] = centerX;
  marker.center[1] = centerY;
  marker.width = static_cast<float>(width);
  marker.height = static_cast<float>(height);
}

// OFFSET: 0x0091CF40
// FUNCTION: fn_12622
__declspec(noinline) void UIRoMapWnd::ClearRecords() {
  this->m_records.clear();
  this->previewTexture = nullptr;
  this->hoverTooltip = 0;
  this->previewArea = nullptr;
  this->selectedRecord = 0;
  this->hoveredRecord = 0;
  this->padding_00CC = 0;
}

// OFFSET: 0x0091E0F0
// FUNCTION: fn_12625
void UIRoMapWnd::ResetKafraBounds() {
  this->m_kafraTop = 0;
  this->m_kafraBottom = 0;
  this->m_kafraLeft = 0;
  this->m_kafraRight = 0;
}

// OFFSET: 0x0091EBA0
// FUNCTION: fn_12634
void UIRoMapWnd::UpdateNavigationButtons() {
    if (g_windowMgr.navigationWnd != nullptr && !g_navigationManager.expandedMap) {
    this->m_searchButton->Move(-100, -100);
    this->m_searchListButton->Move(-100, -100);
    this->m_searchEdit->Move(-100, -100);
  } else {
    this->m_searchButton->Move(335, 0);
    this->m_searchListButton->Move(375, 3);
    this->m_searchEdit->Move(131, 0);
    g_navigationManager.hideWindow = true;
  }

  this->Invalidate();
}

// OFFSET: 0x009335B0
// FUNCTION: fn_12725
void UIRoMapWnd::OnRBtnUp(int, int) {
  if (g_windowMgr.comboBoxWnd != 0) {
    g_windowMgr.CloseWindow(WID_COMBOBOXWND);
  }

  this->selectedMapIndex = 0;
}

// OFFSET: 0x00919B50
// FUNCTION: fn_12598
void UIRoMapWnd::DrawHoverTooltip() {
  if (this->hoverTooltip != nullptr) {
    delete this->hoverTooltip;
    this->hoverTooltip = nullptr;
  }

  if (this->hoveredRecord == nullptr || this->previewArea == nullptr) {
    return;
  }

  bool leftSide = false;
  int maxWidth = 0;
  const int lineHeight = this->TextHeight(" ", 0, 0, 12, 1, 0) + 3;
  const WorldViewRecord* record = this->hoveredRecord;
  const WorldViewArea* anchor = this->previewArea;
  const RECT rect = reinterpret_cast<const RECT&>(record->left);
  int x;
  if (anchor->right < rect.right) {
    leftSide = true;
    x = anchor->left - 10;
  } else {
    x = anchor->right + 10;
  }

  int y = anchor->top;
  int count = 0;
  for (std::list<std::string>::const_iterator it = record->relatedNames.begin(); it != this->hoveredRecord->relatedNames.end(); ++it) {
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
  for (std::list<std::string>::const_iterator it = this->hoveredRecord->relatedNames.begin(); it != this->hoveredRecord->relatedNames.end(); ++it) {
    const std::uint32_t color = std::strcmp(it->c_str(), g_session.DecodePlayerName()) != 0 ? 0xFFFFFF : 0x8080F0;
    this->DrawOutlinedText(boxX + 5, textY, it->c_str(), 0, color, 0, 0, 12, 1, 0);
    textY += lineHeight;
  }

  this->hoverTooltip = new RoMapTooltipBox(boxX, y, width, height);
  DrawRoundedBorder(*this, boxX, y, width, height, 0xFFFFFF);
}

// INLINED
static int CenteredTextX(UIWindow& window, std::string text, int left, int right, int fontHeight, unsigned char bold) {
  return std::abs(static_cast<int>(static_cast<float>(window.TextWidth(text.c_str(), 0, 0, fontHeight, bold, 0) - right - left) * g_depthAverageHalf));
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

    const int x = CenteredTextX(*this, name, area[0], area[2], fontHeight, 0);
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

      const int x = CenteredTextX(*this, name, area[0], area[2], townHeight, 0);
      const int y = townHeight + 2 + area[1];
      this->DrawTextAt(x + 1, y + 1, name.c_str(), 0, 0, townHeight, townOutline, 0, 0);
      this->DrawTextAt(x, y, name.c_str(), 0, 0, townHeight, townColor, 0, 0);
      continue;
    }

    for (std::vector<CWorldViewField*>::const_iterator fieldIt = map->fields.begin(); fieldIt != map->fields.end(); ++fieldIt) {
      name = (*fieldIt)->fieldName;
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

      const int x = CenteredTextX(*this, name, field->left, field->right, fieldHeight, 0);
      this->DrawTextAt(x + 1, y + 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(x - 1, y - 1, name.c_str(), 0, 0, fieldHeight, fieldShadow, 0, 0);
      this->DrawTextAt(x, y, name.c_str(), 0, 0, fieldHeight, fieldColor, 0, 0);
      name = (*fieldIt)->description;
      if (name.empty()) {
        continue;
      }

      const int levelX = CenteredTextX(*this, name, field->left, field->right, levelHeight, 1);
      this->DrawOutlinedText(levelX, levelY, name.c_str(), 0, levelColor, levelOutline, 0, levelHeight, 1, 0);
    }
  }
}

// OFFSET: 0x0091A7D0
// FUNCTION: fn_12601
void UIRoMapWnd::DrawRecordBorders() {
  const std::string mapName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(MM_QUERYRSWNAME, 0, 0, 0, 0));
  for (std::list<WorldViewRecord>::const_iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld != it->worldIndex) {
      continue;
    }

    const int width = it->right - it->left;
    const int height = it->bottom - it->top;
    if (mapName == it->name) {
      this->DrawRoundedOutline(it->left, it->top, width, height, 0x23AEE8);
    } else {
      this->DrawRoundedOutline(it->left, it->top, width, height, 0x7EE2F7);
    }
  }
}

// OFFSET: 0x0091A960
// FUNCTION: fn_12602
void UIRoMapWnd::DrawDungeonLinks() {
  CTexture* const positionTexture = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_dunPos.bmp", 0, true, true, false);
  CTexture* const selectTexture = g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_dunSelect.bmp", 0, true, true, false);
  const std::uint32_t rectColor = g_worldViewMgr.dungeonOverlayColor;
  const std::uint32_t lineColor = g_worldViewMgr.dungeonColor;
  const std::uint32_t selectColor = g_worldViewMgr.dungeonSelectColor;
  std::vector<CWorldViewMap*>::iterator it = g_worldViewMgr.CurrentWorld()->maps.begin();
  const std::vector<CWorldViewMap*>::iterator end = g_worldViewMgr.CurrentWorld()->maps.end();
  for (; it != end; ++it) {
    const CWorldViewMap* map = *it;
    if (map->id >= 100) {
      continue;
    }

    const WorldViewRecord* selected = this->selectedRecord;
    bool isSelected;
    int x;
    int y;
    int width;
    int height;
    if (!map->fields.empty()) {
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
        DrawScreenLine(ScreenPoint{startX, startY}, ScreenPoint{endX, endY}, 1, lineTexture, (selectColor & 0xFFFFFF) - 0x1000000);
      } else {
        DrawScreenLine(ScreenPoint{startX, startY}, ScreenPoint{endX, endY}, 1, lineTexture, lineColor);
      }

      DrawTexturedRectangle(map->left, map->top, mapWidth, mapHeight, isSelected ? 0xFFFFFFFF : rectColor, positionTexture);
      x = field->left;
      y = field->top;
    } else {
      isSelected = selected != nullptr && map->left == selected->left && map->right == selected->right && map->top == selected->top &&
                   map->bottom == selected->bottom;
      x = map->left;
      y = map->top;
      width = map->right - map->left;
      height = map->bottom - map->top;
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
        DrawScreenLine(ScreenPoint{startX, startY}, ScreenPoint{endX, endY}, 5, g_texMgr.Load("유저인터페이스\\worldview_interface\\wv_Airf2.bmp", 0, false, true, false), 0x99FFFFFF);
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
  const std::string mapName = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(MM_QUERYRSWNAME, 0, 0, 0, 0));
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

// INLINED
static bool AddRelatedName(std::string map, std::string name, std::list<WorldViewRecord>& records) {
  for (WorldViewRecord& record : records) {
    if (record.name == map) {
      record.relatedNames.push_back(name);
      return true;
    }
  }

  return false;
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

      if (!AddRelatedName(member.map, member.name, this->m_records)) {
        WorldViewRecord record;
        record.name = member.map;
        record.relatedNames.push_back(member.name);
        if (this->LocateRecord(record)) {
          this->m_records.push_back(record);
        }
      }
    }
  } else {
    std::string map = reinterpret_cast<const char*>(g_modeMgr.GetGameMode()->SendMsg(MM_QUERYRSWNAME, 0, 0, 0, 0));
    WorldViewRecord record;
    record.name = map;
    record.relatedNames.push_back(g_session.DecodePlayerName());
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
void UIRoMapWnd::OnMouseMove(int x, int y) {
  if (this == g_windowMgr.Capture()) {
    return;
  }

  if (this->m_mouseX == x && this->m_mouseY == y) {
    return;
  }

  this->m_mouseX = x;
  this->m_mouseY = y;
  const int currentWorld = g_worldViewMgr.currentWorld;
  this->hoveredRecord = nullptr;
  for (auto it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (currentWorld == it->worldIndex && it->left < x && it->right > x && it->top < y && it->bottom > y) {
      this->hoveredRecord = &*it;
      break;
    }
  }

  this->Invalidate();

  if (this->selectedRecord) {
    delete this->selectedRecord;
    this->selectedRecord = nullptr;
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
  this->selectedRecord = new WorldViewRecord();
  this->selectedRecord->worldIndex = g_worldViewMgr.currentWorld;
  this->selectedRecord->mapIndex = mapId;
  this->selectedRecord->name = field->name;
  reinterpret_cast<WorldViewArea&>(this->selectedRecord->left) = reinterpret_cast<const WorldViewArea&>(*field);

  CWorldViewMap* map = world->maps[mapIndex];
  reinterpret_cast<WorldViewArea&>(this->selectedRecord->screenLeft) = reinterpret_cast<const WorldViewArea&>(map->left);
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
  WorldViewRecord* record = wnd->selectedRecord;
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
      from.center[0] = static_cast<float>(from.area.right);
      to.center[0] = to.width * 0.8f + static_cast<float>(to.area.left);
      to.center[1] = static_cast<float>(to.area.top);
    }

    CTexture* head = g_texMgr.Load("mapArrow_head.bmp", 0, true, true, false);
    CTexture* body = g_texMgr.Load("mapArrow_body.bmp", 1, true, true, false);
    this->DrawArrow(
      from.center[0], from.center[1], to.center[0], to.center[1], static_cast<float>(from.area.left), static_cast<float>(from.area.top),
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
    const WorldViewRecord* selected = this->selectedRecord;
    if (marker.area.left == selected->left && marker.area.right == selected->right && marker.area.top == selected->top &&
        marker.area.bottom == selected->bottom) {
      return;
    }
  }

  this->DrawRoundedOutline(marker.area.left, marker.area.top, static_cast<int>(marker.width), static_cast<int>(marker.height), 0xFFFFFF);
  DrawAlphaRectangle(marker.area.left, marker.area.top, static_cast<int>(marker.width), static_cast<int>(marker.height), 0x66000000);
  std::string name = g_session.GetMapDisplayName(marker.name.c_str());
  this->DrawOutlinedText(marker.area.left + 2, marker.area.top + 5, name.c_str(), 0, 0xD2FF, 0, 0, 12, 1, 0);
}

// OFFSET: 0x009119B0
// FUNCTION: fn_12522
UIRoMapWnd::UIRoMapWnd() : UIFrameWnd(0), previewTexture(nullptr) {
  g_session.m_hotkeyScript->Call("ReadKafraMoveMapServiceInfo", "");
  g_session.m_hotkeyScript->Call("ReadKafraMiniMapPosInfo", "");
  this->fieldNames.clear();
  this->ClearRecords();
  this->m_showLabels = 0;
  this->m_lastTime = 0;
  this->m_resetTime = 1;
  this->m_flyingShipRoute.lineTextureName.assign("유저인터페이스\\worldview_interface\\wv_Airf3.bmp", 0x2F);
  this->m_flyingShipRoute.lineColor = 0xAAFFFFFF;
  this->m_flyingShipRoute.routeType = 1;
  this->m_flyingShipRoute2.lineTextureName.assign("유저인터페이스\\worldview_interface\\wv_Airf3.bmp", 0x2F);
  this->m_flyingShipRoute2.lineColor = 0xAAFFFFFF;
  this->m_flyingShipRoute2.routeType = 0;
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
__declspec(noinline) UIRoMapWnd::~UIRoMapWnd() {
  delete this->previewArea;
  delete this->selectedRecord;
  this->previewTexture = nullptr;
}

// OFFSET: 0x0091E4D0
// FUNCTION: fn_12627
bool CFlyingShipRouteInfo::IsRouteMap(std::string mapName) {
  switch (this->routeType) {
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
      WorldViewArea area = {field->left, field->top, field->right, field->bottom, field->areaFlags};
      RoMapMarker marker;
      SetRoMapMarker(name, area, marker);
      this->entries.push_back(marker);
    }
  }
  this->m_routeBuilt = 1;
}

// OFFSET: 0x00938FF0
// FUNCTION: fn_12754
int UIRoMapWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_ROMAPWND);
      return 0;
    case ID_STATUS: {
      UICheckBox* labelCheck = this->m_labelCheck;
      this->m_showLabels = !this->m_showLabels;
      labelCheck->geometry.state = this->m_showLabels;
      labelCheck->Invalidate();
      this->Invalidate();
      return 0;
    }
    case ID_BASE:
      g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
      if (g_windowMgr.navigationWnd) {
        g_windowMgr.navigationWnd->SendMsg(nullptr, UM_COMMAND, ID_BASE, 0, 0, 0);
      }
      this->UpdateNavigationButtons();
      return 0;
    case ID_SEARCH:
      if (this->m_searchEdit->Length()) {
        std::string name(this->m_searchEdit->Text());
        if (g_navigationManager.SearchNavigation(name, 0, true) == true) {
          this->RefreshWorldMapLevels();
        }
      }
      return 0;
    }
    return 0;

  case UM_COMBOBOX_SELECTED:
    if (this->m_worldCombo == sender) {
      g_worldViewMgr.currentWorld = argument1;
      this->Invalidate();
      return 0;
    }
    if (argument1 >= 0 && static_cast<unsigned int>(argument1) < this->fieldNames.size()) {
      {
      std::string name(g_worldViewMgr.CurrentWorld()->maps[this->selectedMapIndex]->fields[argument1]->name);
      if (name.size() > 4) {
        name.resize(name.size() - 4);
      }
      g_navigationManager.expandedMap = false;
      g_navigationManager.SearchNavigation(name, 0, false);
      if (g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND)) {
        static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->SelectRoute(0);
        static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->SelectRouteNode(0);
      }
      this->UpdateNavigationButtons();
      }
      this->Invalidate();
      return 0;
    }
    return 0;

  case UM_REFRESH:
    this->ClearRecords();
    this->RefreshPartyRecords();
    this->Invalidate();
    return 0;

  case UM_DEFPUSH:
    if (this->m_searchEdit == sender) {
      if (this->m_searchEdit->Length() > 0) {
        this->SendMsg(this->m_searchEdit, UM_COMMAND, ID_SEARCH, 0, 0, 0);
        return 0;
      }
    }
    break;
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

    switch (this->routeType) {
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
    this->routeReset = 1;
    this->segmentIndex = 0;
    this->travelledDistance = 0.0f;
    this->routeState = 0;
  }
  return false;
}

// INLINED
__forceinline void CFlyingShipRouteInfo::Update(UIRoMapWnd* wnd, float elapsed) {
  if (!this->UpdateRoute(wnd->selectedRecord)) {
    return;
  }

  const int last = static_cast<int>(this->entries.size()) - 1;
  if (last > 0) {
    int index = this->segmentIndex;
    this->m_moving = 1;
    if (this->travelledDistance > this->segmentLength) {
      if (index >= last - 1) {
        wnd->Invalidate();
        index = -1;
      }
      ++index;
      this->travelledDistance = 0.0f;
      this->segmentIndex = index;
    }

    const float dx = this->entries[index].center[0] - this->entries[index + 1].center[0];
    const float dy = this->entries[index].center[1] - this->entries[index + 1].center[1];
    const float distance = SquareRoot(dx * dx + dy * dy);
    if (this->travelledDistance == 0.0f) {
      this->segmentLength = distance;
    }
    this->travelledDistance = elapsed * 200.0f + this->travelledDistance;
    this->segmentProgress = this->travelledDistance / this->segmentLength;
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
  const WorldViewRecord* selected = this->selectedRecord;
  if (selected != nullptr && this->previewArea != nullptr) {
    DrawAlphaRectangle(selected->left, selected->top, selected->right - selected->left, selected->bottom - selected->top, g_worldViewMgr.selectBoxColor);
    if (this->previewTexture != nullptr) {
      DrawAlphaRectangle(this->previewArea->left, this->previewArea->top, 130, 130, 0x88000000);
      this->previewTexture->Stretch(this->previewArea->left, this->previewArea->top, 130, 130);
    }
  }

  const RoMapTooltipBox* tooltip = this->hoverTooltip;
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
void UIRoMapWnd::OnCreate(int, int) {
  this->m_worldCombo = new UIComboBoxDefWnd();
  this->m_worldCombo->dropdownHeight = 67;
  this->m_worldCombo->SetSize(110, this->m_comboHeight);
  this->m_worldCombo->Move(17, 0);
  this->m_worldCombo->SetBorderColor(100, 100, 100);
  this->m_worldCombo->SetColor(232, 232, 232);
  this->AddChild(this->m_worldCombo);
  const int worldCount = static_cast<int>(g_worldViewMgr.worlds.size());
  for (int index = 0; index < worldCount; ++index) {
    this->m_worldCombo->AddOption(g_worldViewMgr.worlds[index]->name, 0);
  }

  this->m_worldCombo->Select(g_worldViewMgr.SelectCurrentWorld());
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

  this->RefreshWorldMapLevels();
  this->RefreshWorldMapMarks();
  const int closeX = this->geometry.width - 18;
  UIBitmapButton* closeButton = new UIBitmapButton();
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_off.bmp", 0, 0);
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_on.bmp", 1, 0);
  closeButton->SetBitmap("유저인터페이스\\basic_interface\\world_close_on.bmp", 2, 0);
  closeButton->SetSize(closeButton->BitmapWidth(), closeButton->BitmapHeight());
  closeButton->Move(closeX, 0);
  closeButton->SetId(ID_CLOSE);
  closeButton->SetTooltip("ctrl+`");
  this->AddChild(closeButton);

  {
    std::string checked = "유저인터페이스\\checkbox_1.bmp";
    std::string unchecked = "유저인터페이스\\checkbox_0.bmp";
    this->m_labelCheck = new (std::nothrow) UICheckBox();
    this->m_labelCheck->SetSize(80, 12);
    this->m_labelCheck->SetImages(checked.c_str(), unchecked.c_str(), LookupMessage(MSI_WORLDMAP_MSG4));
    this->m_labelCheck->SetId(ID_STATUS);
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
    this->m_searchButton->SetId(ID_SEARCH);
    this->m_searchButton->SetTooltip(LookupMessage(MSI_NAVIGATION_SEARCHINFO));
    this->AddChild(this->m_searchButton);

    this->m_searchListButton = new UIBitmapButton();
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 0, 0);
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 1, 0);
    this->m_searchListButton->SetBitmap("유저인터페이스\\scroll0down.bmp", 2, 0);
    this->m_searchListButton->SetSize(this->m_searchListButton->BitmapWidth(), this->m_searchListButton->BitmapHeight());
    this->m_searchListButton->Move(-100, -100);
    this->m_searchListButton->SetId(ID_BASE);
    this->m_searchListButton->SetTooltip(LookupMessage(MSI_NAVIGATION_V4_HELP01));
    this->AddChild(this->m_searchListButton);

    this->m_searchEdit = new UIEditCtrl();
    this->m_searchEdit->SetSize(200, 18);
    this->m_searchEdit->Move(-100, -100);
    this->m_searchEdit->maximum = 60;
    this->m_searchEdit->Background(232, 232, 232);
    this->m_searchEdit->Border(200, 100, 100);
    this->m_searchEdit->flat = 0;
    UIEditCtrl* edit = this->m_searchEdit;
    if (const char* tooltip = LookupMessage(MSI_NAVIGATION_SERVICE)) {
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
