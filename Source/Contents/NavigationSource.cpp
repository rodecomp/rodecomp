#include "Contents/NavigationManager.h"
#include "Client/Session.h"
#include "Resource/ResourceMgr.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
  this->unknown_12B4 = false;
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

  const std::string resolved = ResourceMgr::Instance().aliases.ResolveMapName(name);
  return std::strcmp(info->GetName(), resolved.c_str()) == 0;
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

// OFFSET: 0x00B880D0
// FUNCTION: fn_19364
CNaviData::CNaviData() : info(nullptr), cellX(0), cellY(0), range(50000) {
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
  this->Clear();

  for (CNavi_Link*& object : this->objects) {
    if (object != nullptr) {
      delete object;
    }
    object = nullptr;
  }
  this->objects.clear();

  for (CNaviInfo*& info : this->destinations) {
    if (info != nullptr) {
      delete info;
    }
    info = nullptr;
  }
  this->destinations.clear();
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

  for (CNavi_Object*& npc : this->npcs) {
    if (npc != nullptr) {
      delete npc;
    }
    npc = nullptr;
  }
  this->npcs.clear();

  for (CNavi_Object*& mob : this->mobs) {
    if (mob != nullptr) {
      delete mob;
    }
    mob = nullptr;
  }
  this->mobs.clear();

  for (CNavi_Link*& link : this->links) {
    if (link != nullptr) {
      delete link;
    }
    link = nullptr;
  }
  this->links.clear();
}

// INLINED
NavigationTarget::~NavigationTarget() {
  this->type = -1;
  this->object = nullptr;
  this->x = -1;
  this->y = -1;
  this->name.clear();
}

// OFFSET: 0x00B92580
// FUNCTION: fn_19551
CNavigationManager::CNavigationManager() {
  this->viewIcon = 1;
  this->unknown_127C = false;
  this->unknown_127D = false;
  this->unknown_127E = false;
  this->unknown_1250 = -1;
  this->unknown_124C = -1;
  this->unknown_1248 = false;
  this->unknown_1254 = false;
  this->Reset(true);
  this->pathFinder.useNodePool = true;
  this->unknown_1244 = nullptr;

  char buffer[0x104];
  for (int i = 0; i < 8; ++i) {
    std::memset(buffer, 0, sizeof(buffer));
    std::sprintf(buffer, "유저인터페이스\\navigation_interface3\\navi_grid%d_%d.tga", this->viewIcon, i);
    this->labels[i].assign(buffer, std::strlen(buffer));
  }

  this->unknown_12B4 = false;
}

// OFFSET: 0x00B92950
// FUNCTION: fn_19552
NaviRouteLine::NaviRouteLine(int kind, int x, int y, const char* text, int color, int shadowColor) : x(0), y(0) {
  this->kind = kind;
  this->x = x;
  this->y = y;
  this->text = text;
  this->color = color;
  this->shadowColor = shadowColor;
}

// OFFSET: 0x00B92CA0
// FUNCTION: fn_45448
CNavigationManager::~CNavigationManager() {
  this->Reset(true);
  this->unknown_1244 = nullptr;
}

// OFFSET: 0x00B959D0
// FUNCTION: fn_19574
void CNavigationManager::ClearRouteNodes() {
  for (NaviRouteLine& line : this->routeLines) {
    line.text.clear();
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
  if (!this->unknown_1244->HasName(g_session.m_navigationPlayerMap)) {
    this->unknown_1244 = this->FindObject(g_session.m_navigationPlayerMap);
  }
  return this->unknown_1244;
}

// OFFSET: 0x00B96BA0
// FUNCTION: fn_19593
void CNavigationManager::Reset(bool clearAll) {
  if (clearAll) {
    this->ClearNodePaths();
    this->unknown_12C4 = -1;
    this->unknown_12C8 = -1;
    this->states.clear();
  }

  this->savedTarget = NavigationTarget();
  this->currentTarget = NavigationTarget();
  this->routeResult = 0;
  this->nodes.clear();
  this->unknown_115C = false;
  this->selectedNode = -1;
  this->movementRoute.segment = 0;
  this->unknown_117C = false;
  this->ground = nullptr;
  this->unknown_1258 = 0;
  this->unknown_1260 = 0;
  this->unknown_1264.clear();
  this->unknown_1280 = 0;
  this->unknown_1284 = 0;
  this->unknown_1288 = nullptr;
  this->unknown_128C = -1;
  this->unknown_1290 = -1;
  this->cells.clear();
  this->unknown_12A0 = nullptr;
  this->unknown_12A4 = nullptr;
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
  if ((firstValue != 0 && firstValue != 1) || (secondValue != 0 && secondValue != 1) || (thirdValue != 0 && thirdValue != 1)) {
    return false;
  }

  *first = firstValue == 1;
  *second = secondValue == 1;
  *third = thirdValue == 1;
  return true;
}

// OFFSET: 0x00B938F0
// FUNCTION: fn_19564
NaviCell CNavigationManager::ParseCell(const char* text) {
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
  } else {
    cell.x = -1;
    cell.y = -1;
  }
  return cell;
}

// OFFSET: 0x00B94890
// FUNCTION: fn_19568
std::vector<char> CNavigationManager::WriteTarget(NavigationTarget target) {
  std::vector<char> buffer;
  if (target.object == nullptr) {
    return buffer;
  }

  buffer.resize(sizeof(int), '\0');
  *reinterpret_cast<int*>(buffer.data()) = target.type;

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
std::vector<char> CNavigationManager::WriteObject(CNavi_Object* object) {
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

    NaviObjectRecord* record = reinterpret_cast<NaviObjectRecord*>(buffer.data());
    record->type = type;
    record->index = index;
    record->cellX = cell.x;
    record->cellY = cell.y;

    const std::size_t nameLength = static_cast<std::int16_t>(name.size());
    reinterpret_cast<NaviObjectRecord*>(buffer.data())->nameLength = static_cast<std::int16_t>(name.size());
    std::memcpy(buffer.data() + 0x12, name.c_str(), nameLength);

    const std::size_t titleLength = static_cast<std::int16_t>(title.size());
    *reinterpret_cast<std::int16_t*>(buffer.data() + nameLength + 0x12) = static_cast<std::int16_t>(title.size());
    std::memcpy(buffer.data() + nameLength + 0x14, title.c_str(), titleLength);

    const std::size_t offset = nameLength + 0x14 + titleLength;
    *reinterpret_cast<std::int16_t*>(buffer.data() + offset) = static_cast<std::int16_t>(map.size());
    std::memcpy(buffer.data() + offset + 2, map.c_str(), static_cast<std::int16_t>(map.size()));
  }

  return buffer;
}

// OFFSET: 0x00B92A70
// FUNCTION: fn_19554
NavigationTarget::NavigationTarget(int type, void* object) {
  this->type = type;
  this->object = object;
  this->x = -1;
  this->y = -1;
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
  int length = header->nameLength;
  std::string name(record + offset, record + offset + length);
  offset += length;

  length = *reinterpret_cast<const std::int16_t*>(record + offset);
  offset += 2;
  std::string title(record + offset, record + offset + length);
  offset += length;

  length = *reinterpret_cast<const std::int16_t*>(record + offset);
  offset += 2;
  std::string map(record + offset, record + offset + length);
  offset += length;

  const int unknown80 = *reinterpret_cast<const int*>(record + offset);
  const int unknown84 = *reinterpret_cast<const int*>(record + offset + 4);
  length = *reinterpret_cast<const std::int16_t*>(record + offset + 8);
  offset += 10;
  std::string linkedMap(record + offset, record + offset + length);

  CNaviInfo* info = this->FindObject(map.c_str());
  if (info == nullptr) {
    return nullptr;
  }
  CNaviInfo* linkedInfo = this->FindObject(linkedMap.c_str());
  if (linkedInfo == nullptr) {
    return nullptr;
  }
  return info->FindMatchingLink(
    CNavi_Link(type, index, name.c_str(), title.c_str(), cellX, cellY, info, linkedInfo, unknown80, unknown84)
  );
}

// OFFSET: 0x00B950B0
// FUNCTION: fn_19571
void CNavigationManager::ClearSavedTarget() {
  this->savedTarget = NavigationTarget();
  this->unknown_115C = false;
  this->routeResult = 0;
  this->nodes.clear();
  this->selectedNode = -1;
  this->movementRoute.segment = 0;
  this->unknown_117C = false;
  this->ground = nullptr;
  this->cells.clear();
  this->unknown_1258 = 1;
}

// OFFSET: 0x00B95260
// FUNCTION: fn_19572
void CNavigationManager::ClearCurrentObject() {
  this->currentTarget.type = 0;
  this->currentTarget.object = nullptr;
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

// OFFSET: 0x00B95DC0
// FUNCTION: fn_19579
int CNavigationManager::FindNodePath(unsigned int index) {
  if (this->states[index].type == 1 || this->states[index].type == 0) {
    return -1;
  }

  for (unsigned int i = 0; i < this->nodePaths.size(); ++i) {
    if (this->nodePaths[i].empty()) {
      return -1;
    }

    const unsigned int first = this->nodePaths[i][0];
    if (first >= this->states.size()) {
      return -1;
    }

    NavigationTarget& target = this->states[index];
    if (target.type == 3 && target.x == -1000 && target.y == -1000) {
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
  if (index >= 0 && this->states.size() > static_cast<unsigned int>(index)) {
    return this->states[index];
  }
  return NavigationTarget();
}

// OFFSET: 0x00B96340
// FUNCTION: fn_19584
std::string CNavigationManager::StateMapName(int index, int* distance) {
  std::string name;
  if (index < 0 || this->states.size() <= static_cast<unsigned int>(index)) {
    return name;
  }
  if (this->states[index].object != nullptr) {
    CNaviInfo* info = nullptr;
    int type = this->states[index].type;
    if (type == 0 || type == 1) {
      info = static_cast<CNaviInfo*>(this->states[index].object);
    } else if (type == 2 || type == 3 || type == 6) {
      info = static_cast<CNavi_Object*>(this->states[index].object)->GetInfo();
    }
    if (info != nullptr) {
      name.assign(info->GetName());
    }
  }
  if (this->states[index].type == 3) {
    *distance = static_cast<CNavi_Object*>(this->states[index].object)->GetFlag();
  } else {
    *distance = this->states[index].type == 2 ? 10000 : 0;
  }
  return name;
}

// OFFSET: 0x00B96520
// FUNCTION: fn_19585
bool CNavigationManager::GetRouteFlag() const {
  return this->unknown_115C;
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
