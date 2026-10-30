#include "Contents/NavigationManager.h"

#include "Contents/MessageTable.h"
#include "Utility/Text/FormatString.h"
#include "Client/Registry.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/RegistryKey.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

// OFFSET: 0x00B88800
// FUNCTION: fn_19376
CNaviInfo* CNavigationManager::FindObject(const char* name) const {
  for (CNaviInfo* info : this->data.destinations) {
    if (info->HasName(name)) {
      return info;
    }
  }
  return nullptr;
}

// OFFSET: 0x00B88840
// FUNCTION: fn_19377
CNavi_Link* CNaviData::FindLink(unsigned int key, bool byId) {
  if (byId) {
    for (CNavi_Link* object : this->objects) {
      if (object->HasId(key)) {
        return object;
      }
    }
    return nullptr;
  }
  if (this->objects.empty() || key >= this->objects.size()) {
    return nullptr;
  }
  return this->objects[key];
}

// OFFSET: 0x00B88910
// FUNCTION: fn_19379
int NaviCell::GetY() const {
  return this->y;
}

// OFFSET: 0x00B88930
// FUNCTION: fn_19381
void CNaviData::ClearAll() {
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

// OFFSET: 0x00B8CF70
// FUNCTION: fn_19432
bool CNavi_Object::HasId(const unsigned int& id) const {
  return static_cast<unsigned int>(this->id) == id;
}

// OFFSET: 0x00B8D0D0
// FUNCTION: fn_19433
bool CNavi_Object::IsNamed(const char* name) const {
  if (name == nullptr) {
    return false;
  }
  return this->name == std::string(name);
}

// OFFSET: 0x00B8E8E0
// FUNCTION: fn_19496
int CNaviInfo::GetCellX() const {
  return this->cellX;
}

// OFFSET: 0x00B97750
// FUNCTION: fn_19600
bool CNavigationManager::BuildRoute(int startX, int startY, int destX, int destY, bool collectCells) {
  this->movementRoute.points.clear();
  const bool found = this->pathFinder.FindPath(0, startX, startY, 8, 8, destX, destY, 1, this->movementRoute, 15);
  this->pathFinder.Reset();
  if (!collectCells) {
    return found;
  }

  this->cells.clear();
  if (!found) {
    return found;
  }

  CNaviInfo* info = this->FindObject(g_session.m_navigationPlayerMap);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().aliases.ResolveMapName(g_session.m_navigationPlayerMap);
    info = this->FindObject(resolved.c_str());
    if (info == nullptr) {
      return found;
    }
  }

  const int height = info->GetCellY();
  const int width = info->GetCellX();
  int offsetX;
  int offsetY;
  const float scale = this->FitToSquare(&offsetX, &offsetY, width, height, 126.0f);
  for (const PathCell& point : this->movementRoute.points) {
    NaviCell cell;
    cell.x = static_cast<int>(point.x * scale + offsetX);
    cell.y = static_cast<int>(126.0f - (point.y * scale + offsetY));

    bool exists = false;
    for (const NaviCell& other : this->cells) {
      if (other.x == cell.x && other.y == cell.y) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      this->cells.push_back(cell);
    }
  }

  if (g_windowMgr.window_4F174 != nullptr) {
    g_windowMgr.window_4F174->Invalidate();
  }
  return found;
}

// OFFSET: 0x00B97E90
// FUNCTION: fn_19603
NaviCell* CNavigationManager::MapCellToView(NaviCell* out, const char* mapName, int cellX, int cellY, float size) {
  CNaviInfo* info = this->FindObject(mapName);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().aliases.ResolveMapName(mapName);
    info = this->FindObject(resolved.c_str());
  }

  out->x = -1;
  out->y = -1;
  if (info != nullptr) {
    const int height = info->GetCellY();
    const int width = info->GetCellX();
    int offsetX;
    int offsetY;
    const float scale = this->FitToSquare(&offsetX, &offsetY, width, height, size);
    out->x = static_cast<int>(cellX * scale + offsetX);
    out->y = static_cast<int>(static_cast<int>(size) - (cellY * scale + offsetY));
  }
  return out;
}

// OFFSET: 0x00B98630
// FUNCTION: fn_19608
std::string CNavigationManager::RouteText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  CNaviInfo* info = this->data.GetInfo();
  if (info != nullptr) {
    NaviCell cell;
    if (this->data.GetCell(&cell)->x > 0) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "[%s] -> %s %d %d", LookupMessage(3585), info->GetName(), x, y);
    } else {
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "[%s] %s", LookupMessage(3585), info->GetName());
    }
  }

  this->routeText.assign(buffer, std::strlen(buffer));
  return this->routeText;
}

// OFFSET: 0x00B98780
// FUNCTION: fn_19609
void CNavigationManager::ShowRouteResult(int result, int mode, WindowDialogs& dialogs) {
  std::string message;
  switch (result) {
  case -100:
    message = LookupMessage(2294);
    break;
  case -99:
    message = LookupMessage(2295);
    break;
  case -98:
    message = LookupMessage(2296);
    break;
  case -97:
    message = LookupMessage(2297);
    break;
  case -96:
    message = LookupMessage(2298);
    break;
  case -95:
    message = LookupMessage(2299);
    break;
  case -94:
    message = LookupMessage(2300);
    break;
  case -93:
    message = LookupMessage(2301);
    break;
  case -92:
    message = LookupMessage(2302);
    break;
  case -90:
    message = LookupMessage(4692);
    break;
  case 0:
    message = LookupMessage(2303);
    break;
  case 1:
    message = LookupMessage(2304);
    break;
  case 2:
    message = LookupMessage(2305);
    break;
  case 3:
    message = LookupMessage(2306);
    break;
  case 4:
    message = LookupMessage(2307);
    break;
  case 5:
    message = LookupMessage(2308);
    break;
  case 6:
    message = LookupMessage(2309);
    break;
  case 7:
    message = LookupMessage(2310);
    break;
  case 8:
    message = LookupMessage(2311);
    break;
  case 9:
    message = LookupMessage(2313);
    break;
  case 10:
    message = LookupMessage(2312);
    break;
  case 11:
    message = LookupMessage(2314);
    break;
  default:
    message = "NoMsg.";
    break;
  }

  if (mode == 0 || mode == 1) {
    char buffer[250];
    std::memset(buffer, 0, sizeof(buffer));
    std::sprintf(buffer, LookupMessage(2315), message.c_str());
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);

    if (result == 5 && this->selectedNode != -1 && static_cast<unsigned>(this->selectedNode) < this->nodes.size()) {
      const int type = this->nodes[this->selectedNode]->GetType();
      if (type == 201 || type == 202 || type == 203 || type == 204) {
        std::sprintf(buffer, LookupMessage(2316), this->nodes[this->selectedNode]->GetName());
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);

        CNaviInfo* info = static_cast<CNavi_Link*>(this->nodes[this->selectedNode])->GetLinkedInfo();
        if (info != nullptr) {
          char text[0x104];
          std::memset(text, 0, sizeof(text));
          std::sprintf(text, " %s[ %s ] ", info->GetTitle(), info->GetName());
          std::sprintf(buffer, LookupMessage(2317), text);
        }
      } else if (type == 205) {
        std::sprintf(buffer, LookupMessage(2316), this->nodes[this->selectedNode]->GetName());
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);
        std::sprintf(buffer, LookupMessage(2319));
      } else {
        std::sprintf(buffer, LookupMessage(2320));
      }
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);
    }
  }

  if (mode == 0 || mode == 2) {
    dialogs.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    if (mode == 4) {
      CNavi_Object* node = g_navigationManager.nodes.size() == 0 ? nullptr : g_navigationManager.nodes[0];
      std::string text(LookupMessage(2321));
      text += node->GetName();
      text += " ";
      text += static_cast<CNavi_Link*>(node)->GetLinkedName();
      dialogs.Confirm(text.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    }
  }
}

// OFFSET: 0x00B8D270
// FUNCTION: fn_19434
bool CNaviInfo::HasName(const char* name) const {
  return std::strcmp(this->name.c_str(), name) == 0;
}

// OFFSET: 0x00B97FC0
// FUNCTION: fn_19604
std::string CNavigationManager::SelectedNodeText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->selectedNodeText.assign(buffer, std::strlen(buffer));
      return this->selectedNodeText;
    }

    CNavi_Object* node = this->nodes[this->selectedNode];
    NaviCell cellY;
    NaviCell cellX;
    const int y = node->GetCell(&cellY)->y;
    const int x = node->GetCell(&cellX)->x;
    std::sprintf(buffer, "%s %d %d...", node->GetInfo()->GetName(), x, y);
  } else if (this->unknown_1258 == 2) {
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, "%s %d %d...", info->GetName(), x, y);
    }
  }

  this->selectedNodeText.assign(buffer, std::strlen(buffer));
  return this->selectedNodeText;
}

// OFFSET: 0x00B981A0
// FUNCTION: fn_19605
std::string CNavigationManager::SubDestinationText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  CNaviInfo* info = this->data.GetInfo();
  if (info != nullptr) {
    NaviCell cell;
    if (this->data.GetCell(&cell)->x > 0) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, ">.%s %d %d", info->GetName(), x, y);
    } else {
      std::sprintf(buffer, ">.%s", info->GetName());
    }
  }

  this->subDestinationText.assign(buffer, std::strlen(buffer));
  return this->subDestinationText;
}

// OFFSET: 0x00B982D0
// FUNCTION: fn_19606
std::string CNavigationManager::DestinationText() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->destinationText.assign(buffer, std::strlen(buffer));
      return this->destinationText;
    }

    CNavi_Object* node = this->nodes[this->selectedNode];
    NaviCell cellY;
    NaviCell cellX;
    const int y = node->GetCell(&cellY)->y;
    const int x = node->GetCell(&cellX)->x;
    std::sprintf(buffer, "%s %d %d", node->GetInfo()->GetName(), x, y);
  } else if (this->unknown_1258 == 2) {
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      NaviCell cellY;
      NaviCell cellX;
      const int y = this->data.GetCell(&cellY)->y;
      const int x = this->data.GetCell(&cellX)->x;
      std::sprintf(buffer, "%s %d %d", info->GetName(), x, y);
    }
  }

  this->destinationText.assign(buffer, std::strlen(buffer));
  return this->destinationText;
}

// OFFSET: 0x00B984C0
// FUNCTION: fn_19607
std::string CNavigationManager::DestinationTitle() {
  char buffer[0x32];
  std::memset(buffer, 0, sizeof(buffer));

  if (!this->nodes.empty() && this->selectedNode >= 0) {
    if (this->nodes.size() <= static_cast<unsigned>(this->selectedNode)) {
      this->destinationTitle.assign(buffer, std::strlen(buffer));
      return this->destinationTitle;
    }

    this->destinationTitle.assign(this->nodes[this->selectedNode]->GetInfo()->GetTitle());
    return this->destinationTitle;
  }

  if (this->unknown_1258 == 2) {
    NaviCell cell;
    this->data.GetCell(&cell);
    CNaviInfo* info = this->data.GetInfo();
    if (info != nullptr) {
      this->destinationTitle.assign(info->GetTitle());
      return this->destinationTitle;
    }
  }

  this->destinationTitle.assign(buffer, std::strlen(buffer));
  return this->destinationTitle;
}

// OFFSET: 0x00B996D0
// FUNCTION: fn_19613
void CNavigationManager::LoadSettings() {
  RegistryKey key;
  std::string subKey(g_subKey);
  subKey += "\Navigation";
  if (key.Open(HKEY_LOCAL_MACHINE, subKey.c_str())) {
    DWORD viewIcon = 0;
    if (key.Query("ViewIcon", REG_DWORD, reinterpret_cast<BYTE*>(&viewIcon), sizeof(viewIcon))) {
      this->viewIcon = viewIcon;
    }
  }
}

// OFFSET: 0x00BA1330
// FUNCTION: fn_19642
void CNavigationManager::SaveSettings() {
  RegistryKey key;
  std::string subKey(g_subKey);
  subKey += "\\Navigation";
  if (key.Create(HKEY_LOCAL_MACHINE, subKey.c_str())) {
    DWORD viewIcon = this->viewIcon;
    key.Set("ViewIcon", REG_DWORD, reinterpret_cast<const BYTE*>(&viewIcon), sizeof(viewIcon));
  }
}

// OFFSET: 0x00B99CE0
// FUNCTION: fn_19615
std::vector<unsigned char> CNavigationManager::WriteReplayState() {
  std::vector<unsigned char> output;
  const auto append = [&output](const std::vector<unsigned char>& record) { output.insert(output.end(), record.begin(), record.end()); };

  append(BuildReplayTaggedRecord(8000, nullptr, 0));
  append(BuildReplayTaggedRecord(8002, &this->unknown_127C, 1));
  append(BuildReplayTaggedRecord(8003, &this->unknown_127D, 1));
  append(BuildReplayTaggedRecord(8004, &this->unknown_127E, 1));
  append(BuildReplayTaggedRecord(8005, &this->unknown_1280, 4));

  for (std::size_t i = 0; i < this->nodes.size(); ++i) {
    CNavi_Link* node = this->nodes[i];
    std::vector<char> buffer;
    if (node != nullptr) {
      const int type = node->GetType();
      const int index = node->GetIndex();
      NaviCell cell;
      node->GetCell(&cell);
      std::string name = node->GetName();
      std::string title = node->GetTitle();
      std::string map = node->GetInfo()->GetName();
      std::string linkedMap = node->GetLinkedName();
      const NaviCell linkedCell = node->GetLinkedCell();

      buffer.resize(linkedMap.size() + 0x20 + title.size() + name.size() + map.size(), '\0');

      NaviObjectRecord* record = reinterpret_cast<NaviObjectRecord*>(buffer.data());
      record->type = type;
      record->index = index;
      record->cellX = cell.x;
      record->cellY = cell.y;

      const std::size_t nameLength = static_cast<std::int16_t>(name.size());
      record->nameLength = static_cast<std::int16_t>(name.size());
      std::memcpy(buffer.data() + 0x12, name.c_str(), nameLength);

      std::size_t offset = nameLength + 0x12;
      const std::size_t titleLength = static_cast<std::int16_t>(title.size());
      *reinterpret_cast<std::int16_t*>(buffer.data() + offset) = static_cast<std::int16_t>(title.size());
      offset += 2;
      std::memcpy(buffer.data() + offset, title.c_str(), titleLength);
      offset += titleLength;

      const std::size_t mapLength = static_cast<std::int16_t>(map.size());
      *reinterpret_cast<std::int16_t*>(buffer.data() + offset) = static_cast<std::int16_t>(map.size());
      offset += 2;
      std::memcpy(buffer.data() + offset, map.c_str(), mapLength);
      offset += mapLength;

      *reinterpret_cast<int*>(buffer.data() + offset) = linkedCell.x;
      *reinterpret_cast<int*>(buffer.data() + offset + 4) = linkedCell.y;
      *reinterpret_cast<std::int16_t*>(buffer.data() + offset + 8) = static_cast<std::int16_t>(linkedMap.size());
      offset += 10;
      std::memcpy(buffer.data() + offset, linkedMap.c_str(), static_cast<std::int16_t>(linkedMap.size()));
    }
    append(BuildReplayTaggedRecord(8022, buffer.data(), static_cast<std::uint32_t>(buffer.size())));
  }

  append(BuildReplayTaggedRecord(8009, &this->unknown_124C, 4));
  append(BuildReplayTaggedRecord(8010, &this->unknown_1250, 4));
  append(BuildReplayTaggedRecord(8011, &this->unknown_1254, 1));
  append(BuildReplayTaggedRecord(8013, &this->unknown_125C, 4));
  append(BuildReplayTaggedRecord(8014, &this->unknown_1260, 4));
  append(BuildReplayTaggedRecord(8016, &this->unknown_1284, 4));
  append(BuildReplayTaggedRecord(8017, &this->unknown_117C, 1));
  append(BuildReplayTaggedRecord(8018, &this->routeResult, 4));
  append(BuildReplayTaggedRecord(8019, &this->unknown_115C, 1));
  append(BuildReplayTaggedRecord(8020, &this->selectedNode, 4));
  append(BuildReplayTaggedRecord(8023, &this->unknown_1248, 1));
  append(BuildReplayTaggedRecord(8012, &this->unknown_1258, 4));
  if (!this->unknown_1264.empty()) {
    append(BuildReplayTaggedRecord(8015, this->unknown_1264.c_str(), static_cast<std::uint32_t>(this->unknown_1264.size())));
  }

  std::vector<char> target;
  for (std::size_t i = 0; i < this->states.size(); ++i) {
    target = this->WriteTarget(this->states[i]);
    if (!target.empty()) {
      append(BuildReplayTaggedRecord(8006, target.data(), static_cast<std::uint32_t>(target.size())));
    }
  }

  target = this->WriteTarget(this->currentTarget);
  if (!target.empty()) {
    append(BuildReplayTaggedRecord(8007, target.data(), static_cast<std::uint32_t>(target.size())));
  }

  target = this->WriteTarget(this->savedTarget);
  if (!target.empty()) {
    append(BuildReplayTaggedRecord(8008, target.data(), static_cast<std::uint32_t>(target.size())));
  }

  const int count = static_cast<int>(this->movementRoute.points.size());
  if (count > 1) {
    const int route[4] = {
      this->movementRoute.points[0].x, this->movementRoute.points[0].y, this->movementRoute.points[count - 1].x, this->movementRoute.points[count - 1].y
    };
    append(BuildReplayTaggedRecord(8021, route, sizeof(route)));
  }

  append(BuildReplayTaggedRecord(8001, nullptr, 0));
  return output;
}

// OFFSET: 0x00B9C020
// FUNCTION: fn_19618
float CNavigationManager::FitToSquare(int* offsetX, int* offsetY, int width, int height, float size) const {
  float scale;
  if (width > height) {
    scale = size / width;
    *offsetY = static_cast<int>((width - height) * 0.5f * scale);
    *offsetX = 0;
  } else if (width < height) {
    scale = size / height;
    *offsetX = static_cast<int>((height - width) * 0.5f * scale);
    *offsetY = 0;
  } else {
    *offsetY = 0;
    scale = size / width;
    *offsetX = 0;
  }
  return scale;
}

// OFFSET: 0x00B888F0
// FUNCTION: fn_19378
NaviCell* CNaviData::GetCell(NaviCell* out) {
  out->x = this->cellX;
  out->y = this->cellY;
  return out;
}

// OFFSET: 0x00B88920
// FUNCTION: fn_19380
CNaviInfo* CNaviData::GetInfo() {
  return this->info;
}

// OFFSET: 0x00B88A00
// FUNCTION: fn_45449
void CNaviData::Clear() {
  this->startLinks.clear();
  this->info = nullptr;
  this->cellX = 0;
  this->cellY = 0;
  this->range = 50000;
  this->endLinks.clear();

  for (NaviDataEntry& entry : this->entries) {
    entry.previous = -1;
    entry.link = nullptr;
    entry.cost = 0;
  }
  this->entries.clear();
}

// OFFSET: 0x00B8CD60
// FUNCTION: fn_19430
CNavi_Object::~CNavi_Object() {
  this->id = -1;
  this->type = 101;
  this->index = -1;
  this->name.clear();
  this->title.clear();
  this->cellX = 0;
  this->cellY = 0;
  this->info = nullptr;
  this->links.clear();
}

// OFFSET: 0x00B8CE30
// FUNCTION: fn_19431
void Nullsub36() {
}

// OFFSET: 0x00B98E40
// FUNCTION: fn_19610
bool CNavigationManager::SavedTargetText(char* out, int size) {
  if (out == nullptr || this->savedTarget.object == nullptr) {
    return false;
  }

  std::memset(out, 0, size);
  char text[0x104];
  std::memset(text, 0, sizeof(text));

  switch (this->savedTarget.type) {
  case 0: {
    CNaviInfo* info = static_cast<CNaviInfo*>(this->savedTarget.object);
    NaviCell cellY;
    NaviCell cellX;
    const int y = this->data.GetCell(&cellY)->y;
    const int x = this->data.GetCell(&cellX)->x;
    std::sprintf(text, "%s %d %d", info->GetName(), x, y);
    break;
  }
  case 1:
    std::sprintf(text, "%s", static_cast<CNaviInfo*>(this->savedTarget.object)->GetName());
    break;
  case 2: {
    CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
    CNaviInfo* info = object->GetInfo();
    NaviCell cellY;
    NaviCell cellX;
    const int y = object->GetCell(&cellY)->y;
    const int x = object->GetCell(&cellX)->x;
    std::sprintf(text, "%s %d %d", info->GetName(), x, y);
    break;
  }
  case 3:
    std::sprintf(text, "%s", static_cast<CNavi_Object*>(this->savedTarget.object)->GetInfo()->GetName());
    break;
  default:
    return false;
  }

  const int length = static_cast<int>(std::strlen(text));
  if (size > length) {
    std::memcpy(out, text, length);
  } else {
    char truncated[0x104];
    std::memset(truncated, 0, sizeof(truncated));
    std::memcpy(truncated, text, size - 4);
    std::sprintf(out, "%s...", truncated);
  }
  return true;
}
