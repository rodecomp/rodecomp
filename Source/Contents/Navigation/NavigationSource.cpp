#include "Contents/Navigation/NavigationManager.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Text/FormatString.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <shlwapi.h>

// GLOBAL: gl_24627
CNavigationManager g_navigationManager;

// OFFSET: 0x00B96980
// FUNCTION: fn_19588
NavigationTarget CNavigationManager::GetCurrentTarget() const {
  return this->currentTarget;
}

// OFFSET: 0x00B96A10
// FUNCTION: fn_19589
std::string CNavigationManager::GetCurrentTargetName() const {
  std::string name;
  void* object = this->currentTarget.object;
  if (object != nullptr) {
    const int type = this->currentTarget.type;
    if (type == 0 || type == 1 || ((type == 2 || type == 3 || type == 6) && (object = static_cast<CNavi_Object*>(object)->GetInfo()) != nullptr)) {
      name.assign(static_cast<CNaviInfo*>(object)->GetName());
    }
  }

  return name;
}

// OFFSET: 0x00B96AE0
// FUNCTION: fn_19590
NavigationTarget CNavigationManager::GetSavedTarget() const {
  return this->savedTarget;
}

// OFFSET: 0x00B96B70
// FUNCTION: fn_19591
CNaviInfo* CNavigationManager::GetDestinationInfo() {
  return this->data.GetInfo();
}

// OFFSET: 0x00B96B80
// FUNCTION: fn_19592
NaviCell* CNavigationManager::GetDestinationCell(NaviCell* out) {
  this->data.GetCell(out);
  return out;
}

// OFFSET: 0x00B96F10
// FUNCTION: fn_19594
void CNavigationManager::ClearData() {
  this->drawDestination = false;
  this->data.Clear();
}

// OFFSET: 0x00B97090
// FUNCTION: fn_19596
bool CNavigationManager::IsCurrentTarget(const char* name) const {
  if (name == nullptr) {
    return false;
  }

  void* object = this->currentTarget.object;
  if (object == nullptr) {
    return false;
  }

  const int type = this->currentTarget.type;
  if (type != 0 && type != 1) {
    if (type != 2 && type != 3 && type != 6) {
      return false;
    }

    object = static_cast<CNavi_Object*>(object)->GetInfo();
    if (object == nullptr) {
      return false;
    }
  }

  const CNaviInfo* info = static_cast<CNaviInfo*>(object);
  if (std::strcmp(info->GetName(), name) == 0) {
    return true;
  }

  const std::string resolved = ResourceMgr::Instance().ResolveMapName(name);
  if (std::strcmp(info->GetName(), resolved.c_str()) == 0) {
    return true;
  }

  return false;
}

// OFFSET: 0x00B971D0
// FUNCTION: fn_19597
int CNavigationManager::GetCurrentTargetType() const {
  return this->currentTarget.type;
}

// OFFSET: 0x00BA0AE0
// FUNCTION: fn_19635
int CNavigationManager::SaveCurrentTarget() {
  const int type = this->currentTarget.type;
  if (type == 6) {
    return -92;
  }
  if ((type != 1 && type != 2 && type != 3 && type != 0) || !this->currentTarget.object) {
    return -97;
  }
  this->savedTarget = this->currentTarget;
  return 0;
}

// OFFSET: 0x00B880B0
// FUNCTION: fn_19363
NaviDataEntry::NaviDataEntry() : previous(-1), link(nullptr), cost(0) {
}

// OFFSET: 0x00B880D0
// FUNCTION: fn_19364
CNaviData::CNaviData() {
  this->info = nullptr;
  this->cellX = 0;
  this->cellY = 0;
  this->range = 50000;
}

// OFFSET: 0x00B88510
// FUNCTION: fn_19370
void NaviDataEntry::Reset() {
  this->previous = -1;
  this->link = nullptr;
  this->cost = 0;
}

// OFFSET: 0x00B88530
// FUNCTION: fn_19371
CNaviData::~CNaviData() {
  this->info = nullptr;
  this->cellX = 0;
  this->cellY = 0;
  this->range = 50000;
  this->startLinks.clear();
  this->endLinks.clear();
  this->entries.clear();

  for (std::vector<CNavi_Link*>::iterator it = this->objects.begin(); it != this->objects.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->objects.clear();
  auto& destinations = this->destinations;

  for (std::vector<CNaviInfo*>::iterator it = destinations.begin(); it != destinations.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  destinations.clear();
}

// OFFSET: 0x00B8CC10
// FUNCTION: fn_19429
CNaviInfo::~CNaviInfo() {
  this->index = 0;
  this->type = 5001;
  this->name.clear();
  this->title.clear();
  this->cellX = 0;
  this->cellY = 0;

  for (std::vector<CNavi_Object*>::iterator it = this->npcs.begin(); it != this->npcs.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->npcs.clear();

  for (std::vector<CNavi_Object*>::iterator it = this->mobs.begin(); it != this->mobs.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->mobs.clear();

  for (std::vector<CNavi_Link*>::iterator it = this->links.begin(); it != this->links.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->links.clear();
}

// OFFSET: 0x00B92580
// FUNCTION: fn_19551
CNavigationManager::CNavigationManager() {
  this->viewIcon = 1;
  this->allowGates = false;
  this->includeObjects = false;
  this->allowWarps = false;
  this->savedCellY = -1;
  this->savedCellX = -1;
  this->hideWindow = false;
  this->expandedMap = false;
  this->Reset(true);
  this->pathFinder.useNodePool = true;
  this->playerMapInfo = nullptr;

  char buffer[0x104];
  for (int i = 0; i < 8; ++i) {
    std::memset(buffer, 0, sizeof(buffer));
    std::sprintf(buffer, "유저인터페이스\\navigation_interface3\\navi_grid%d_%d.tga", this->viewIcon, i);
    this->labels[i] = buffer;
  }

  this->drawDestination = false;
}

// OFFSET: 0x00B92CA0
// FUNCTION: fn_45448
CNavigationManager::~CNavigationManager() {
  this->Reset(true);
  this->playerMapInfo = nullptr;
}

// OFFSET: 0x00B959D0
// FUNCTION: fn_19574
void CNavigationManager::ClearRouteNodes() {
  for (std::vector<NaviRouteLine>::iterator it = this->routeLines.begin(); it != this->routeLines.end(); ++it) {
    it->text.clear();
  }
  this->routeLines.clear();
}

// OFFSET: 0x00B95A20
// FUNCTION: fn_19575
void CNavigationManager::ClearNodePaths() {
  for (std::size_t i = 0; i < this->nodePaths.size(); ++i) {
    this->nodePaths[i].clear();
  }
  this->nodePaths.clear();
}

// OFFSET: 0x00B961A0
// FUNCTION: fn_45452
CNaviInfo* CNavigationManager::GetLinkedPlayerInfo() {
  if (!this->playerMapInfo->HasName(g_session.m_navigationPlayerMap)) {
    this->playerMapInfo = this->FindObject(g_session.m_navigationPlayerMap);
  }
  return this->playerMapInfo;
}

// OFFSET: 0x00B96BA0
// FUNCTION: fn_19593
void CNavigationManager::Reset(bool clearAll) {
  if (clearAll) {
    this->ClearNodePaths();
    this->selectedPath = -1;
    this->selectedPathNode = -1;
    this->states.clear();
  }

  this->savedTarget = NavigationTarget();
  this->currentTarget = NavigationTarget();
  this->routeResult = 0;
  this->nodes.clear();
  this->routeFlag = false;
  this->selectedNode = -1;
  this->movementRoute.ResetSegment();
  this->drawRoute = false;
  this->ground = nullptr;
  this->viewMode = 0;
  this->searchType = 0;
  this->searchText.clear();
  this->routeSide = 0;
  this->previewType = 0;
  this->previewTexture = nullptr;
  this->previewX = -1;
  this->previewY = -1;
  this->cells.clear();
  this->previewAction = nullptr;
  this->previewSprite = nullptr;
  this->ClearRouteNodes();
}

// OFFSET: 0x00B93110
// FUNCTION: fn_19562
bool CNavigationManager::AddState(const std::vector<std::string>& tokens) {
  if (tokens.size() < 2) {
    return false;
  }

  CNaviInfo* info = this->FindObject(tokens[0].c_str());
  if (info == nullptr) {
    return false;
  }

  const NaviCell cell = this->ParseCell(tokens[1].c_str());
  if (cell.x == -1 || cell.y == -1) {
    return false;
  }

  std::string name;
  for (std::size_t i = 2; i < tokens.size(); ++i) {
    name.append(" ", 1);
    name.append(tokens[i]);
  }

  NavigationTarget target(0, info, cell.x, cell.y, name);
  this->states.push_back(target);
  return true;
}

// OFFSET: 0x00B93670
// FUNCTION: fn_19563
bool CNavigationManager::ParseFlags(const char* text, bool* first, bool* second, bool* third) {
  const char* firstSlash = std::strstr(text, "/");
  if (firstSlash == nullptr) {
    return false;
  }

  const char* rest = firstSlash + 1;
  const char* secondSlash = std::strstr(rest, "/");
  if (secondSlash == nullptr) {
    return false;
  }

  std::string firstText(text, firstSlash);
  if (firstText.empty()) {
    return false;
  }

  std::string secondText(rest, secondSlash);
  if (secondText.empty()) {
    return false;
  }

  std::string thirdText(secondSlash + 1, text + std::strlen(text));
  if (thirdText.empty()) {
    return false;
  }

  const int firstValue = std::atoi(firstText.c_str());
  const int secondValue = std::atoi(secondText.c_str());
  const int thirdValue = std::atoi(thirdText.c_str());
  if ((firstValue == 0 || firstValue == 1) && (secondValue == 0 || secondValue == 1) && (thirdValue == 0 || thirdValue == 1)) {
    *first = firstValue == 1;
    *second = secondValue == 1;
    *third = thirdValue == 1;
    return true;
  }
  return false;
}

// OFFSET: 0x00B938F0
// FUNCTION: fn_19564
__forceinline NaviCell CNavigationManager::ParseCell(const char* text) {
  NaviCell cell;
  const char* slash;
  if (text == nullptr || (slash = std::strstr(text, "/")) == nullptr) {
    cell.x = -1;
    cell.y = -1;
    return cell;
  }

  std::string xText(text, slash);
  if (xText.empty()) {
    cell.x = -1;
    cell.y = -1;
    return cell;
  }

  std::string yText(slash + 1, text + std::strlen(text));
  if (yText.empty()) {
    cell.x = -1;
    cell.y = -1;
    return cell;
  }

  const int x = std::atoi(xText.c_str());
  const int y = std::atoi(yText.c_str());
  if (x >= 0 && x <= 500 && y >= 0 && y <= 500) {
    cell.x = x;
    cell.y = y;
    return cell;
  } else {
    cell.x = -1;
    cell.y = -1;
    return cell;
  }
}

// OFFSET: 0x00B93BA0
// FUNCTION: fn_19565
NavigationTarget CNavigationManager::ReadTarget(const char* data, int size) {
  NavigationTarget target;
  std::memcpy(&target.type, data, sizeof(target.type));
  std::vector<char> buffer(size - 3, '\0');
  if (target.type == 1 || target.type == 0) {
    std::memcpy(buffer.data(), data + 4, size - 4);
    void* object = this->FindObject(buffer.data());
    if (object == nullptr) {
      return NavigationTarget(0, nullptr);
    }
    target.object = object;
  } else {
    target.object = this->ReadTargetObject(data + 4);
  }

  return target;
}

// OFFSET: 0x00B94170
// FUNCTION: fn_19567
CNavi_Object* CNavigationManager::ReadTargetObject(const char* record) {
  const NaviObjectRecord* header = reinterpret_cast<const NaviObjectRecord*>(record);
  const int type = header->type;
  if (!(type > 99 && type < 199) && !(type >= 300 && type <= 398)) {
    return nullptr;
  }

  const int cellX = header->cellX;
  const int cellY = header->cellY;

  std::size_t offset = 0x12;
  std::int16_t length = header->nameLength;
  std::string name(record + offset, record + offset + length);
  offset += length;

  length = reinterpret_cast<const NaviStringRecord*>(record + offset)->length;
  offset += 2;
  std::string title(record + offset, record + offset + length);
  offset += length;

  length = reinterpret_cast<const NaviStringRecord*>(record + offset)->length;
  offset += 2;
  std::string map(record + offset, record + offset + length);

  CNaviInfo* info = this->FindObject(map.c_str());
  if (info == nullptr) {
    return nullptr;
  }

  if (type >= 100 && type <= 104) {
    CNavi_Object* npc = info->FindNpcAt(cellX, cellY);
    if (npc != nullptr && std::strcmp(npc->GetName(), name.c_str()) == 0) {
      return npc;
    }
  } else {
    CNavi_Object* mob = info->FindMobByName(name.c_str());
    if (mob != nullptr) {
      return mob;
    }
  }
  return nullptr;
}

// OFFSET: 0x00B94890
// FUNCTION: fn_19568
std::vector<char> CNavigationManager::WriteTarget(NavigationTarget target) {
  std::vector<char> buffer;
  if (target.object == nullptr) {
    return buffer;
  }

  buffer.resize(sizeof(int), '\0');
  reinterpret_cast<NaviTargetRecord*>(buffer.data())->type = target.type;

  if (target.type == 1 || target.type == 0) {
    std::string name = static_cast<CNaviInfo*>(target.object)->GetName();
    buffer.insert(buffer.end(), name.begin(), name.end());
  } else if (target.type == 3) {
    std::vector<char> object = this->WriteObject(static_cast<CNavi_Object*>(target.object));
    buffer.insert(buffer.end(), object.begin(), object.end());
  } else if (target.type == 2) {
    std::vector<char> object = this->WriteObject(static_cast<CNavi_Object*>(target.object));
    buffer.insert(buffer.end(), object.begin(), object.end());
  }

  return buffer;
}

// OFFSET: 0x00B94B50
// FUNCTION: fn_19569
__declspec(noinline) std::vector<char> CNavigationManager::WriteObject(CNavi_Object* object) {
  std::vector<char> buffer;
  if (object != nullptr) {
    const int type = object->GetType();
    const int index = object->GetIndex();
    NaviCell cell;
    object->GetCell(&cell);
    std::string name = object->GetName();
    std::string title = object->GetTitle();
    std::string map = object->GetInfo()->GetName();

    buffer.resize(map.size() + 0x16 + title.size() + name.size(), '\0');

    reinterpret_cast<NaviObjectRecord*>(buffer.data())->type = type;
    reinterpret_cast<NaviObjectRecord*>(buffer.data())->index = index;
    std::memcpy(&reinterpret_cast<NaviObjectRecord*>(buffer.data())->cellX, &cell, sizeof(cell));

    std::int16_t length = static_cast<std::int16_t>(name.size());
    reinterpret_cast<NaviObjectRecord*>(buffer.data())->nameLength = length;
    std::memcpy(buffer.data() + 0x12, name.c_str(), length);
    std::size_t offset = length + 0x12;
    length = static_cast<std::int16_t>(title.size());
    reinterpret_cast<NaviStringRecord*>(buffer.data() + offset)->length = length;
    offset += 2;
    std::memcpy(buffer.data() + offset, title.c_str(), length);
    offset += length;
    length = static_cast<std::int16_t>(map.size());
    reinterpret_cast<NaviStringRecord*>(buffer.data() + offset)->length = length;
    std::memcpy(buffer.data() + offset + 2, map.c_str(), length);
  }

  return buffer;
}

// OFFSET: 0x00B8EC70
// FUNCTION: fn_19501
void CNaviInfo::CollectMobsInRange(int minimum, int maximum, std::vector<NavigationTarget>& out) {
  NavigationTarget target(3, nullptr);
  const std::vector<CNavi_Object*>::iterator end = this->mobs.end();
  for (std::vector<CNavi_Object*>::iterator it = this->mobs.begin(); it != end; ++it) {
    NaviCell lower;
    NaviCell upper;
    if (minimum <= (*it)->GetCell(&lower)->x && maximum >= (*it)->GetCell(&upper)->x) {
      NaviCell cell;
      const NaviCell* position = (*it)->GetCell(&cell);
      target.cell.first = position->x;
      target.cell.second = position->y;
      target.name = (*it)->GetName();
      target.object = *it;
      out.push_back(target);
    }
  }
}

// OFFSET: 0x00B8FC40
// FUNCTION: fn_19505
void CNaviInfo::CollectMobsByTitle(std::string title, std::vector<NavigationTarget>& out) {
  NavigationTarget target(3, nullptr);
  for (std::vector<CNavi_Object*>::iterator it = this->mobs.begin(); it != this->mobs.end(); ++it) {
    if (std::strcmp((*it)->GetTitle(), title.c_str()) == 0) {
      NaviCell cell;
      const NaviCell* position = (*it)->GetCell(&cell);
      target.cell.first = position->x;
      target.cell.second = position->y;
      target.name = (*it)->GetName();
      target.object = *it;
      out.push_back(target);
    }
  }
}

// OFFSET: 0x00B8E970
// FUNCTION: fn_19500
void CNaviInfo::CollectTargets(int type, std::vector<NavigationTarget>& out) {
  NavigationTarget target(type, nullptr);

  if (std::strstr(this->name.c_str(), "_in") == nullptr && (type == 1 || type == 0)) {
    target.name = this->name.c_str();
    target.object = this;
    target.type = 1;
    out.push_back(target);
  }

  if (type == 2 || type == 0) {
    target.type = 2;
    const std::vector<CNavi_Object*>::iterator end = this->npcs.end();
    for (std::vector<CNavi_Object*>::iterator it = this->npcs.begin(); it != end; ++it) {
      NaviCell cell;
      const NaviCell* position = (*it)->GetCell(&cell);
      target.cell.first = position->x;
      target.cell.second = position->y;
      target.name = (*it)->GetName();
      target.object = *it;
      out.push_back(target);
    }
  }

  if (type == 3 || type == 0) {
    target.type = 3;
    const std::vector<CNavi_Object*>::iterator end = this->mobs.end();
    for (std::vector<CNavi_Object*>::iterator it = this->mobs.begin(); it != end; ++it) {
      NaviCell cell;
      const NaviCell* position = (*it)->GetCell(&cell);
      target.cell.first = position->x;
      target.cell.second = position->y;
      target.name = (*it)->GetName();
      target.object = *it;
      out.push_back(target);
    }
  }
}

// OFFSET: 0x00B8EE50
// FUNCTION: fn_19502
void CNaviInfo::SearchMap(const std::vector<std::string>& keywords, std::vector<NavigationTarget>& out) {
  NavigationTarget target(1, nullptr);
  wchar_t buffer[100];
  std::memset(buffer, 0, sizeof(buffer));

  std::vector<std::wstring> wideKeywords;
  for (auto it = keywords.begin(); it != keywords.end(); ++it) {
    MultiByteToWideChar(CP_ACP, 0, it->c_str(), -1, buffer, 100);
    wideKeywords.push_back(std::wstring(buffer));
  }

  std::string text = this->name;
  text.push_back(' ');
  text += this->title;
  text.push_back(' ');
  MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, buffer, 100);
  const std::wstring wideText(buffer);

  const char* mapName = this->name.c_str();
  if (std::strcmp(mapName, "gl_in01") != 0 && std::strstr(mapName, "_in") != nullptr) {
    return;
  }

  bool matched = true;
  for (std::vector<std::wstring>::iterator it = wideKeywords.begin(); it != wideKeywords.end(); ++it) {
    matched &= StrStrIW(wideText.c_str(), it->c_str()) != nullptr;
  }
  if (!matched) {
    return;
  }

  target.name = this->name;
  target.object = this;
  out.push_back(target);

  target.type = 3;
  for (std::vector<CNavi_Object*>::iterator it = this->mobs.begin(); it != this->mobs.end(); ++it) {
    target.cell.first = -1000;
    target.cell.second = -1000;
    target.name = (*it)->GetName();
    target.object = *it;
    out.push_back(target);
  }
}

// OFFSET: 0x00B93DE0
// FUNCTION: fn_19566
CNavi_Link* CNavigationManager::ReadObject(const char* record) {
  const NaviObjectRecord* header = reinterpret_cast<const NaviObjectRecord*>(record);
  const int type = header->type;
  if (static_cast<unsigned int>(type - 199) > 100) {
    return nullptr;
  }

  const int index = header->index;
  const int cellX = header->cellX;
  const int cellY = header->cellY;

  std::size_t offset = 0x12;
  std::int16_t length = header->nameLength;
  std::string name(record + offset, record + offset + length);
  offset += length;

  length = reinterpret_cast<const NaviStringRecord*>(record + offset)->length;
  offset += 2;
  std::string title(record + offset, record + offset + length);
  offset += length;

  length = reinterpret_cast<const NaviStringRecord*>(record + offset)->length;
  offset += 2;
  std::string map(record + offset, record + offset + length);
  offset += length;

  const int linkedCellX = reinterpret_cast<const NaviLinkedMapRecord*>(record + offset)->cellX;
  const int linkedCellY = reinterpret_cast<const NaviLinkedMapRecord*>(record + offset)->cellY;
  length = reinterpret_cast<const NaviLinkedMapRecord*>(record + offset)->nameLength;
  offset += 10;
  std::string linkedMap(record + offset, record + offset + length);

  CNaviInfo* info = this->FindObject(map.c_str());
  if (info != nullptr) {
    CNaviInfo* linkedInfo = this->FindObject(linkedMap.c_str());
    if (linkedInfo != nullptr) {
      CNavi_Link* link = info->FindMatchingLink(
        CNavi_Link(type, index, name.c_str(), title.c_str(), cellX, cellY, info, linkedInfo, linkedCellX, linkedCellY)
      );
      if (link != nullptr) {
        return link;
      }
    }
  }
  return nullptr;
}

// OFFSET: 0x00B950B0
// FUNCTION: fn_19571
void CNavigationManager::ClearSavedTarget() {
  this->savedTarget = NavigationTarget();
  this->routeFlag = false;
  this->routeResult = 0;
  this->nodes.clear();
  this->selectedNode = -1;
  this->movementRoute.ResetSegment();
  this->drawRoute = false;
  this->ground = nullptr;
  this->cells.clear();
  this->viewMode = 1;
}

// OFFSET: 0x00B95260
// FUNCTION: fn_19572
void CNavigationManager::ClearCurrentObject() {
  this->currentTarget.type = 0;
  this->currentTarget.object = nullptr;
}

// OFFSET: 0x00B95280
// FUNCTION: fn_19573
int CNavigationManager::RouteFromPlayer() {
  NaviCell player;
  NaviCell start;
  int result;
  NaviCell destination;
  NaviCell cell;
  CNaviInfo* info;
  NaviCell cell2;
  NaviCell linked;
  NaviCell cellX;
  NaviCell cellY;
  NaviCell cellX2;
  NaviCell cellY2;
  NaviCell cellX3;
  NaviCell cellY3;

  info = this->FindObject(g_session.m_navigationPlayerMap);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().ResolveMapName(g_session.m_navigationPlayerMap);
    info = this->FindObject(resolved.c_str());
    if (info == nullptr) {
      this->drawRoute = false;
      this->routeFlag = false;
      this->drawDestination = false;
      return -93;
    }
  }

  result = -96;
  this->drawRoute = false;
  this->routeFlag = false;
  this->drawDestination = false;
  player.x = 0;
  player.y = 0;
  if (CGameMode* mode = g_modeMgr.GetGameMode()) {
    mode->PlayerCell(&player.x, &player.y);
  }

  if (this->data.GetInfo() == info) {
    const int type = this->savedTarget.type;
    if (type != 2 && type != 0) {
      int code = type != 1 ? -96 : 6;
      if (type == 3) {
        code = 8;
      }
      this->CancelRoute();
      return code;
    }

    if (player.x == this->data.GetCell(&cell)->x && player.y == this->data.GetCell(&cell)->y) {
      this->movementRoute.points.clear();
      PathCell point = {player.x, player.y, 0, 0};
      this->movementRoute.points.push_back(point);
      this->CancelRoute();
      return 11;
    }

    if (this->BuildRoute(player.x, player.y, this->data.GetCell(&destination)->x, this->data.GetCell(&cell)->y, true)) {
      this->nodes.clear();
      this->routeResult = std::abs(player.x - this->data.GetCell(&destination)->x) + std::abs(player.y - this->data.GetCell(&cell)->y);
      this->selectedNode = -1;
      this->currentTarget = this->savedTarget;
      this->LoadTargetPreview();
      const int targetType = this->savedTarget.type;
      result = targetType != 2 ? -96 : 7;
      this->drawRoute = true;
      this->routeFlag = true;
      this->drawDestination = true;
      if (targetType == 0) {
        result = 9;
        this->viewMode = 2;
        return result;
      }
      if (targetType == 2) {
        this->viewMode = 2;
        return result;
      }
    }
  }

  start = player;
  if (this->nodes.empty() || info == nullptr) {
    return result;
  }

  CGameMode* mode = g_modeMgr.GetGameMode();
  if (mode && mode->scene) {
    C3dAttr* terrain = mode->scene->terrain;
    if (terrain) {
      this->ground = mode->scene->ground;
      this->pathFinder.SetAttr(terrain);
    }
  }

  {
    std::vector<int> candidates;
    for (int index = static_cast<int>(this->nodes.size()) - 1; index > 0; --index) {
      if (this->nodes[index]->GetInfo() != info) {
        continue;
      }

      linked = this->nodes[index - 1]->GetLinkedCell();
      if (linked.x == start.x && linked.y == start.y) {
        if (this->BuildRoute(start.x, start.y, this->nodes[index]->GetCell(&cellX)->x, this->nodes[index]->GetCell(&cellY)->y, true)) {
          this->selectedNode = index;
          this->SelectTarget(index);
          this->drawRoute = true;
          goto selected;
        }
      } else {
        candidates.push_back(index);
      }
    }

    if (this->nodes[0]->GetInfo() == info) {
      this->movementRoute.points.clear();
      if (start.x == this->nodes[0]->GetCell(&cell)->x && start.y == this->nodes[0]->GetCell(&cell2)->y) {
        this->movementRoute.points.push_back({start.x, start.y, 0, 0});
      } else if (!this->BuildRoute(start.x, start.y, this->nodes[0]->GetCell(&cellX2)->x, this->nodes[0]->GetCell(&cellY2)->y, true)) {
        goto search;
      }
      this->selectedNode = 0;
      this->SelectTarget(0);
      this->drawRoute = true;
      goto selected;
    }

  search:
    for (int index = 0; index < static_cast<int>(candidates.size()); ++index) {
      if (this->BuildRoute(start.x, start.y, this->nodes[candidates[index]]->GetCell(&cellX3)->x, this->nodes[candidates[index]]->GetCell(&cellY3)->y, true)) {
        this->selectedNode = candidates[index];
        this->SelectTarget(candidates[index]);
        this->drawRoute = true;
        goto selected;
      }
    }

    this->drawRoute = false;
    if (this->nodes[0]->GetType() != 400) {
      return result;
    }
    this->selectedNode = 0;
    this->SelectTarget(0);
  }

selected:
  if (this->selectedNode == 0 && this->nodes[0]->GetType() == 400) {
    result = 4;
  } else {
    result = 5;
  }
  this->viewMode = 2;
  return result;
}

// OFFSET: 0x00B95AD0
// FUNCTION: fn_19576
void CNavigationManager::ClearStates() {
  this->states.clear();
}

// OFFSET: 0x00B95AF0
// FUNCTION: fn_19577
void CNavigationManager::ClearNodes() {
  this->nodes.clear();
}

// OFFSET: 0x00B95B00
// FUNCTION: fn_19578
bool CNavigationManager::GetMobInfo(
  std::string title, std::string* name, std::string* sprite, int* cellX, int* cellYByte1, int* element, int* elementLevel, int* cellYByte0, int* type
) {
  if (title.empty()) {
    return false;
  }

  CNavi_Object* mob = this->data.FindMobByTitle(title);
  if (mob == nullptr) {
    return false;
  }

  name->assign(mob->GetName());

  std::string path = FormatOwnedString("몬스터\\%s", g_session.GetJobName(mob->GetIndex(), -1));
  const bool exists = ResourceMgr::Instance().Exists((path + ".spr").c_str()) && ResourceMgr::Instance().Exists((path + ".act").c_str());
  if (exists) {
    *sprite = path;
  } else {
    *sprite = static_cast<const std::string&>(FormatOwnedString("npc\\%s", g_session.GetJobName(mob->GetIndex(), -1)));
  }

  int packed;
  {
    NaviCell cell;
    *cellX = mob->GetCell(&cell)->x;
    packed = mob->GetCell(&cell)->y;
  }
  const unsigned int elementValue = static_cast<unsigned char>(packed >> 16);
  *elementLevel = elementValue / 20;
  *element = elementValue % 10;
  *cellYByte1 = static_cast<unsigned char>(packed >> 8);
  *cellYByte0 = static_cast<unsigned char>(packed);
  *type = mob->GetType();
  return true;
}

// OFFSET: 0x00B95DC0
// FUNCTION: fn_19579
int CNavigationManager::FindNodePath(unsigned int index) {
  if (this->states[index].type == 1 || this->states[index].type == 0) {
    return -1;
  }

  for (unsigned int i = 0; i < this->nodePaths.size(); ++i) {
    if (this->nodePaths[i].size() == 0) {
      return -1;
    }

    const unsigned int first = this->nodePaths[i][0];
    if (first >= this->states.size()) {
      return -1;
    }

    NavigationTarget& target = this->states[index];
    if (target.type == 3 && target.cell.first == -1000 && target.cell.second == -1000) {
      if (this->states[first].type != 1) {
        continue;
      }

      CNaviInfo* info = static_cast<CNaviInfo*>(this->states[first].object);
      if (info == nullptr) {
        return -1;
      }

      CNavi_Object* object = static_cast<CNavi_Object*>(target.object);
      if (object == nullptr || object->GetInfo() == nullptr) {
        return -1;
      }

      if (std::strcmp(info->GetName(), object->GetInfo()->GetName()) == 0) {
        return i;
      }
    }

    NavigationTarget& other = this->states[first];
    if (other.type == this->states[index].type) {
      CNavi_Object* otherObject = static_cast<CNavi_Object*>(other.object);
      if (otherObject == nullptr) {
        return -1;
      }

      CNavi_Object* object = static_cast<CNavi_Object*>(this->states[index].object);
      if (object == nullptr) {
        return -1;
      }

      if (std::strcmp(otherObject->GetName(), object->GetName()) == 0) {
        return i;
      }
    }
  }

  return -1;
}

// OFFSET: 0x00B95FB0
// FUNCTION: fn_19580
void CNavigationManager::SelectNodePath(std::string name) {
  if (name.empty() || this->nodePaths.size() < 2) {
    return;
  }

  std::vector<int> first = this->nodePaths[0];
  for (unsigned int i = 0; i < this->nodePaths.size(); ++i) {
    if (this->states[this->nodePaths[i][0]].name == name) {
      this->nodePaths[0] = this->nodePaths[i];
      this->nodePaths[i] = first;
      break;
    }
  }
}

// OFFSET: 0x00B96210
// FUNCTION: fn_19582
int CNavigationManager::GetRouteResult() const {
  return this->routeResult;
}

// OFFSET: 0x00B96220
// FUNCTION: fn_19583
NavigationTarget CNavigationManager::GetState(int index) const {
  if (index < 0 || this->states.size() <= static_cast<unsigned int>(index)) {
    return NavigationTarget();
  }
  return this->states[index];
}

// OFFSET: 0x00B96340
// FUNCTION: fn_19584
std::string CNavigationManager::StateMapName(int index, int* distance) {
  std::string name;
  if (index < 0 || this->states.size() <= static_cast<unsigned int>(index)) {
    return name;
  }
  const NavigationTarget& state = this->states[index];
  if (state.object != nullptr) {
    CNaviInfo* info = nullptr;
    int type = state.type;
    if (type == 0 || type == 1) {
      info = static_cast<CNaviInfo*>(state.object);
    } else if (type == 2 || type == 3 || type == 6) {
      info = static_cast<CNavi_Object*>(state.object)->GetInfo();
    }
    if (info != nullptr) {
      name.assign(info->GetName());
    }
  }
  const NavigationTarget& current = this->states[index];
  int stateType = current.type;
  *distance = stateType == 3 ? static_cast<CNavi_Object*>(current.object)->GetFlag() : (stateType == 2 ? 10000 : 0);
  return name;
}

// OFFSET: 0x00B96520
// FUNCTION: fn_19585
bool CNavigationManager::GetRouteFlag() const {
  return this->routeFlag;
}

// OFFSET: 0x00B96910
// FUNCTION: fn_19586
int CNavigationManager::GetSelectedNode() const {
  return this->selectedNode;
}

// OFFSET: 0x00B96920
// FUNCTION: fn_19587
CNavi_Link* CNavigationManager::GetNode(int index) const {
  if (index >= 0 && this->nodes.size() > static_cast<unsigned int>(index)) {
    return this->nodes[index];
  }
  return nullptr;
}

// OFFSET: 0x00B96F20
// FUNCTION: fn_19595
void CNavigationManager::ResetCurrentTarget() {
  this->currentTarget = NavigationTarget();
  this->LoadTargetPreview();
  this->ClearRouteNodes();
}

// OFFSET: 0x00B94E90
// FUNCTION: fn_19570
void CNavigationManager::CheckArrival(int cellX, int cellY) {
  if (this->viewMode == 2) {
    if (g_windowMgr.navigationWnd != nullptr) {
      g_windowMgr.navigationWnd->Invalidate();
    }
    if (this->routeFlag) {
      NaviCell cell;
      this->data.GetCell(&cell);
      if (std::abs(cell.x - cellX) <= 5 && std::abs(cell.y - cellY) <= 5) {
        this->CancelRoute();
      }
    }
  }
}

// OFFSET: 0x00B9DE60
// FUNCTION: fn_19626
void CNavigationManager::BuildNodePaths() {
  this->ClearNodePaths();

  if (this->searchType == 0 || this->searchType == 1) {
    for (int index = 0; index < this->states.size(); ++index) {
      if (this->states[index].type == 1) {
        std::vector<int> path;
        path.push_back(index);
        this->nodePaths.push_back(path);
      }
    }
  }

  for (int index = 0; index < this->states.size(); ++index) {
    if (this->searchType != 0 && this->states[index].type != this->searchType) {
      continue;
    }
    if (this->states[index].type == 1) {
      continue;
    }

    const int path = this->FindNodePath(index);
    if (path == -1) {
      std::vector<int> single;
      single.push_back(index);
      this->nodePaths.push_back(single);
    } else {
      this->nodePaths[path].push_back(index);
    }
  }
}

// OFFSET: 0x00B9E140
// FUNCTION: fn_19627
void CNavigationManager::BuildSingleNodePaths() {
  this->ClearNodePaths();

  for (int index = 0; index < this->states.size(); ++index) {
    std::vector<int> path;
    path.push_back(index);
    this->nodePaths.push_back(path);
  }
}
