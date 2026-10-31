#include "Contents/NavigationManager.h"

#include "Contents/MessageTable.h"
#include "Utility/Text/FormatString.h"
#include "Client/Registry.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Resource/ResourceMgr.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/UIWindow.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/RegistryKey.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>

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
  subKey += "\\Navigation";
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

// OFFSET: 0x00B9C0D0
// FUNCTION: fn_19619
std::vector<unsigned char> CNavigationManager::WriteStates() {
  std::vector<unsigned char> buffer;
  if (this->states.empty()) {
    return buffer;
  }

  buffer.resize(4, 0);
  std::size_t offset = 4;
  *reinterpret_cast<int*>(buffer.data()) = static_cast<int>(this->states.size());
  for (auto it = this->states.begin(); it != this->states.end(); ++it) {
    CNaviInfo* info = static_cast<CNaviInfo*>(it->object);
    if (info == nullptr) {
      continue;
    }
    buffer.resize(it->name.size() + 0x24 + offset, 0);
    *reinterpret_cast<int*>(buffer.data() + offset) = it->type;
    offset += 4;
    std::memcpy(buffer.data() + offset, info->GetName(), info->GetNameLength());
    *reinterpret_cast<int*>(buffer.data() + offset + 0x14) = it->x;
    *reinterpret_cast<int*>(buffer.data() + offset + 0x18) = it->y;
    const int length = static_cast<int>(it->name.size());
    *reinterpret_cast<int*>(buffer.data() + offset + 0x1C) = length;
    offset += 0x20;
    if (length > 0) {
      std::memcpy(buffer.data() + offset, it->name.c_str(), length);
      offset += length;
    }
  }
  return buffer;
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

// OFFSET: 0x00B887B0
// FUNCTION: fn_19374
int NaviDataEntry::GetPrevious() const {
  return this->previous;
}

// OFFSET: 0x00B887C0
// FUNCTION: fn_19375
int NaviDataEntry::GetCost() const {
  return this->cost;
}

// OFFSET: 0x00B890E0
// FUNCTION: fn_19384
bool CNaviData::CollectLinksTo(CNaviInfo* target, std::vector<CNavi_Link*>* out) {
  out->clear();
  for (auto info = this->destinations.begin(); info != this->destinations.end(); ++info) {
    for (auto link = (*info)->LinksBegin(); link != (*info)->LinksEnd(); ++link) {
      if ((*link)->GetLinkedInfo() == target) {
        out->push_back(*link);
      }
    }
  }
  for (auto link = this->objects.begin(); link != this->objects.end(); ++link) {
    if ((*link)->GetLinkedInfo() == target) {
      out->push_back(*link);
    }
  }
  return out->size() != 0;
}

// OFFSET: 0x00B89980
// FUNCTION: fn_19389
int CNaviData::ObjectCount() const {
  return static_cast<int>(this->objects.size());
}

// OFFSET: 0x00B88A60
// FUNCTION: fn_19382
void NaviDataEntry::Init() {
  this->previous = -1;
  this->link = nullptr;
  this->cost = 0;
}

// OFFSET: 0x00B88AB0
// FUNCTION: fn_19383
bool CNaviData::Search(std::string query, int type, std::vector<NavigationTarget>& out) {
  if (query.size() == 0) {
    return false;
  }
  std::vector<std::string> tokens;
  std::vector<char> text(query.begin(), query.end());
  bool result = false;
  text.push_back(0);
  std::vector<int> spaces;
  spaces.push_back(-1);
  for (unsigned int i = 0; i < text.size(); ++i) {
    if (text[i] == ' ') {
      spaces.push_back(i);
    }
  }
  spaces.push_back(static_cast<int>(text.size()) - 1);
  for (auto it = spaces.begin(); it != spaces.end() - 1;) {
    int start = *it + 1;
    ++it;
    if (start < *it) {
      std::string token(&text[0] + start, &text[0] + *it);
      tokens.push_back(token);
    }
  }
  if (tokens.empty()) {
    result = false;
  } else {
    int kind = this->ParseSearchKind(tokens);
    out.clear();
    for (CNaviInfo* info : this->destinations) {
      if (kind == 2) {
        info->CollectTargets(type, out);
      } else if (kind == 3) {
        info->CollectMobsInRange(std::atoi(tokens[0].c_str()), std::atoi(tokens[1].c_str()), out);
      } else if (kind == 4) {
        if (tokens.size() == 2) {
          info->CollectMobsByTitle(tokens[1].c_str(), out);
        }
      } else {
        info->Search(type, tokens, out);
      }
    }
    result = true;
  }
  return result;
}

// OFFSET: 0x00B891C0
// FUNCTION: fn_19385
int CNaviData::ParseSearchKind(std::vector<std::string>& tokens) {
  if (std::strncmp(tokens[0].c_str(), LookupMessage(0x912), std::strlen(LookupMessage(0x912))) != 0) {
    return 1;
  }
  std::size_t prefixLength = std::strlen(LookupMessage(0x913));
  if (prefixLength < tokens[0].size() && std::strncmp(tokens[0].c_str(), LookupMessage(0x913), prefixLength) == 0) {
    std::string token(tokens[0]);
    const char* separator = std::strstr(token.c_str(), LookupMessage(0x914));
    std::string minimum;
    std::string maximum;
    if (separator == nullptr) {
      minimum.assign(token.c_str() + prefixLength, token.c_str() + token.size());
      maximum = minimum;
    } else {
      minimum.assign(token.c_str() + prefixLength, separator);
      maximum.assign(separator + std::strlen(LookupMessage(0x914)), token.c_str() + token.size());
      if (maximum.size() == 0) {
        maximum = "500";
      }
    }
    int low = std::atoi(minimum.c_str());
    int high = std::atoi(maximum.c_str());
    int kind;
    if (low < 1 || high < 1 || low > high) {
      kind = 0;
    } else {
      tokens.clear();
      tokens.push_back(minimum);
      tokens.push_back(maximum);
      kind = 3;
    }
    return kind;
  }
  if (std::strncmp(tokens[0].c_str(), LookupMessage(0x915), std::strlen(LookupMessage(0x915))) == 0) {
    return 2;
  }
  return std::strncmp(tokens[0].c_str(), "$$DB", 4) == 0 ? 4 : 0;
}

// OFFSET: 0x00B896E0
// FUNCTION: fn_19386
CNavi_Object* CNaviData::FindMobByTitle(std::string title) {
  std::vector<NavigationTarget> targets;
  for (CNaviInfo* info : this->destinations) {
    info->CollectMobsByTitle(title, targets);
    if (targets.begin() != targets.end()) {
      return static_cast<CNavi_Object*>(targets.front().object);
    }
  }
  return nullptr;
}

// OFFSET: 0x00B9C6B0
// FUNCTION: fn_19621
void CNavigationManager::RefreshSearch() {
  if (this->data.Search(this->unknown_1264, this->unknown_1260, this->states) && g_windowMgr.window_4F174) {
    static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->FillRouteGroups();
  }
}

// OFFSET: 0x00B9C740
// FUNCTION: fn_19623
int CNavigationManager::AddWrappedRouteLines(int kind, int x, int y, const char* text) {
  std::string bracketed = "[";
  bracketed += text;

  wchar_t wide[100];
  std::memset(wide, 0, sizeof(wide));
  MultiByteToWideChar(CP_ACP, 0, bracketed.c_str(), -1, wide, static_cast<int>(bracketed.size()));
  const int length = static_cast<int>(wcsnlen(wide, 100));

  char line[40] = {};
  const int count = length / 12;
  for (int index = 0; index <= count; ++index) {
    const int limit = 11 * (index + 1);
    const int end = length <= limit ? length : limit;
    std::wstring part(wide + 11 * index, wide + end);
    WideCharToMultiByte(CP_ACP, 0, part.c_str(), -1, line, 40, nullptr, nullptr);
    NaviRouteLine routeLine(kind, x, y + 14 * index, line, 0xDCDCDC, 0xCB6B54);
    this->routeLines.push_back(routeLine);
  }

  this->routeLines.back().text += "]";
  return count + 1;
}

// OFFSET: 0x00B9DB50
// FUNCTION: fn_19624
bool CNavigationManager::SelectState(int index) {
  if (this->states.empty() || this->states[index].object == nullptr) {
    return false;
  }

  this->currentTarget = this->states[index];
  return this->LoadTargetPreview();
}

// OFFSET: 0x00B9DBD0
// FUNCTION: fn_19625
bool CNavigationManager::SelectNodeState() {
  const int path = this->unknown_12C4;
  const int node = this->unknown_12C8;
  if (path >= 0 && static_cast<unsigned int>(path) < this->nodePaths.size() && node >= 0 &&
      static_cast<unsigned int>(node) < this->nodePaths[path].size() && this->nodePaths[path][node] != -1) {
    return this->SelectState(this->nodePaths[path][node]);
  }

  this->currentTarget = NavigationTarget();
  this->LoadTargetPreview();
  this->ClearRouteNodes();
  if (this->data.GetInfo() && !this->data.GetInfo()->IsSearchable(this->data.GetInfo()->GetName())) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4195)), 0x0A0AFF, 0, 0, 0);
  }
  return false;
}

#pragma pack(push, 1)
struct NavigationDestinationRecord {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::int16_t length;
  /* 0006 */ char mapName[30];
  /* 0024 */ int x;
  /* 0028 */ int y;
};
#pragma pack(pop)

static_assert(sizeof(NavigationDestinationRecord) == 0x2C);

// OFFSET: 0x00B9E290
// FUNCTION: fn_19628
bool CNavigationManager::SetDestinationMap(std::string mapName, int x, int y) {
  CNaviInfo* info = this->FindObject(mapName.c_str());
  if (!info) {
    return false;
  }

  {
    std::string name = mapName;
    if (Replay::Instance().state == 2) {
      char packet[100] = {};
      NavigationDestinationRecord record;
      record.header = 0x039008AE;
      record.length = sizeof(record);
      std::memset(record.mapName, 0, sizeof(record.mapName));
      std::memcpy(record.mapName, name.c_str(), name.size());
      record.x = x;
      record.y = y;
      std::memcpy(packet, &record, sizeof(record));

      Replay& replay = Replay::Instance();
      ReplayPacketRecorder recorder = {
        g_packetLengths,
        replay.baseFrame,
        replay.started,
        replay.frame,
        replay.header.key0,
        replay.header.key1,
        replay.header.options.recordRestricted,
        replay.file,
        replay.fileOffset,
        replay.records,
        replay.cipherIndex,
        replay.cipherBase,
        replay.cipherKey,
      };
      recorder.Record(static_cast<std::uint16_t>(record.length), packet, 0x29, 0);
    }
  }

  char text[50];
  std::memset(text, 0, sizeof(text));
  this->currentTarget.object = info;
  if (x == 1000) {
    this->currentTarget.type = 1;
    this->currentTarget.x = -1;
    this->currentTarget.y = -1;
    std::sprintf(text, "%s", info->GetName());
  } else {
    this->currentTarget.x = x;
    this->currentTarget.type = 0;
    this->currentTarget.y = y;
    std::sprintf(text, "%s %d,%d", info->GetName(), x, y);
  }
  this->currentTarget.name = text;

  this->unknown_125C = 0;
  this->unknown_1254 = false;
  this->unknown_1248 = false;
  if (this->unknown_1258 != 2) {
    this->unknown_1258 = 1;
  }

  this->LoadTargetPreview();
  g_windowMgr.EnsureWindow(203);
  if (g_windowMgr.window_4F174) {
    UIWindow* window = g_windowMgr.window_4F174;
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
    static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->RefreshLayout();
    static_cast<UINavigationV4Wnd*>(g_windowMgr.window_4F174)->LayoutWithDetails();
    g_windowMgr.window_4F174->Invalidate();
  }

  if (g_windowMgr.modal) {
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapSelection();
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapLevels();
  }
  return true;
}

// OFFSET: 0x00B9FBB0
// FUNCTION: fn_19632
bool CNavigationManager::LoadTargetPreview() {
  this->unknown_128C = -1;
  this->unknown_1290 = -1;
  this->unknown_12A4 = nullptr;
  this->unknown_12A0 = nullptr;
  this->unknown_1288 = nullptr;
  if (!this->currentTarget.object || !this->BuildTargetRouteLines()) {
    return false;
  }

  const int type = this->currentTarget.type;
  void* object = this->currentTarget.object;
  this->unknown_1284 = 1;

  int mapType;
  const char* mapName;
  if (type == 0 || type == 1) {
    if (!object) {
      return false;
    }
    CNaviInfo* info = static_cast<CNaviInfo*>(object);
    mapType = info->GetType();
    mapName = info->GetName();
  } else if (type == 6) {
    if (!object) {
      return false;
    }
    CNavi_Object* target = static_cast<CNavi_Object*>(object);
    if (target->GetType() == 400) {
      mapName = target->GetName();
      mapType = 5004;
    } else {
      mapType = target->GetInfo()->GetType();
      mapName = target->GetInfo()->GetName();
    }
  } else {
    this->unknown_12A4 = nullptr;
    this->unknown_12A0 = nullptr;
    if (!object) {
      return false;
    }
    CNavi_Object* target = static_cast<CNavi_Object*>(object);
    mapType = target->GetInfo()->GetType();
    mapName = target->GetInfo()->GetName();
    if (target->GetIndex() >= 0) {
      char spriteName[128];
      char actionName[128];
      std::memset(spriteName, 0, sizeof(spriteName));
      std::memset(actionName, 0, sizeof(actionName));
      bool found = false;
      if (this->currentTarget.type == 2) {
        std::sprintf(spriteName, "npc\\%s.spr", g_session.GetJobName(target->GetIndex(), -1));
        std::sprintf(actionName, "npc\\%s.act", g_session.GetJobName(target->GetIndex(), -1));
        if (ResourceMgr::Instance().Exists(spriteName) && ResourceMgr::Instance().Exists(actionName)) {
          found = true;
        } else {
          std::sprintf(spriteName, "몬스터\\%s.spr", g_session.GetJobName(target->GetIndex(), -1));
          std::sprintf(actionName, "몬스터\\%s.act", g_session.GetJobName(target->GetIndex(), -1));
          found = ResourceMgr::Instance().Exists(spriteName) && ResourceMgr::Instance().Exists(actionName);
        }
      } else if (this->currentTarget.type == 3) {
        std::sprintf(spriteName, "몬스터\\%s.spr", g_session.GetJobName(target->GetIndex(), -1));
        std::sprintf(actionName, "몬스터\\%s.act", g_session.GetJobName(target->GetIndex(), -1));
        if (ResourceMgr::Instance().Exists(spriteName) && ResourceMgr::Instance().Exists(actionName)) {
          found = true;
        } else {
          std::sprintf(spriteName, "npc\\%s.spr", g_session.GetJobName(target->GetIndex(), -1));
          std::sprintf(actionName, "npc\\%s.act", g_session.GetJobName(target->GetIndex(), -1));
          found = ResourceMgr::Instance().Exists(spriteName) && ResourceMgr::Instance().Exists(actionName);
        }
      }
      if (found) {
        this->unknown_12A4 = dynamic_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));
        this->unknown_12A0 = dynamic_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
      }
    }
    this->unknown_1284 = 2;
  }

  this->unknown_1288 = nullptr;
  if (mapType == 5004) {
    this->unknown_1288 = g_texMgr.Load("유저인터페이스\\collection\\불의주문서.bmp", 0, false, true, false);
  } else if (mapType != 5001 && mapType != 5005) {
    this->unknown_1288 = g_texMgr.Load("유저인터페이스\\navigation_interface\\Interior.bmp", 0, false, true, false);
  } else {
    char path[60];
    std::memset(path, 0, sizeof(path));
    std::sprintf(path, "유저인터페이스\\map\\%s.bmp", mapName);
    this->unknown_1288 = g_texMgr.Load(path, 0, false, true, false);
    if (this->unknown_1288) {
      const int targetType = this->currentTarget.type;
      if (targetType == 2 || targetType == 6) {
        CNavi_Object* target = static_cast<CNavi_Object*>(this->currentTarget.object);
        NaviCell cellY;
        NaviCell cellX;
        const int cellRow = target->GetCell(&cellY)->y;
        const int cellColumn = target->GetCell(&cellX)->x;
        CNaviInfo* info = target->GetInfo();
        int viewX = -1;
        int viewY = -1;
        if (info) {
          int offsetX;
          int offsetY;
          const float scale = this->FitToSquare(&offsetX, &offsetY, info->GetCellX(), info->GetCellY(), 126.0f);
          viewX = static_cast<int>(cellColumn * scale + offsetX);
          viewY = static_cast<int>(126.0f - (cellRow * scale + offsetY));
        }
        this->unknown_1290 = viewY;
        this->unknown_128C = viewX;
      } else if (targetType == 0) {
        int cellColumn = this->currentTarget.x;
        int cellRow;
        CNaviInfo* info;
        if (cellColumn < 0) {
          NaviCell cellY;
          NaviCell cellX;
          cellRow = this->data.GetCell(&cellY)->y;
          cellColumn = this->data.GetCell(&cellX)->x;
          info = this->data.GetInfo();
        } else {
          cellRow = this->currentTarget.y;
          info = static_cast<CNaviInfo*>(this->currentTarget.object);
        }
        int viewX = -1;
        int viewY = -1;
        if (info) {
          int offsetX;
          int offsetY;
          const float scale = this->FitToSquare(&offsetX, &offsetY, info->GetCellX(), info->GetCellY(), 126.0f);
          viewX = static_cast<int>(cellColumn * scale + offsetX);
          viewY = static_cast<int>(126.0f - (cellRow * scale + offsetY));
        }
        this->unknown_1290 = viewY;
        this->unknown_128C = viewX;
      }
    }
  }

  if (!this->unknown_1288 || this->unknown_1288->Width() == 1) {
    this->unknown_1288 = g_texMgr.Load("유저인터페이스\\navigation_interface\\NoImage.bmp", 0, false, true, false);
  }
  return true;
}

// OFFSET: 0x00BA02F0
// FUNCTION: fn_19633
bool CNavigationManager::BuildTargetRouteLines() {
  this->ClearRouteNodes();
  void* object = this->currentTarget.object;
  if (!object) {
    return false;
  }

  char text[50];
  std::memset(text, 0, sizeof(text));
  const int type = this->currentTarget.type;
  if (type == 1) {
    this->AddWrappedRouteLines(1, 141, 46, static_cast<CNaviInfo*>(object)->GetTitle());
  } else if (type == 2) {
    CNavi_Object* npc = static_cast<CNavi_Object*>(object);
    this->AddWrappedRouteLines(2, 141, 46, npc->GetName());
    NaviCell cellY;
    NaviCell cellX;
    std::sprintf(text, "( %d %d )", npc->GetCell(&cellX)->x, npc->GetCell(&cellY)->y);
    this->routeLines.push_back(NaviRouteLine(2, 142, 149, text, 0xDCDCDC, 0xCB6B54));
  } else if (type == 3) {
    CNavi_Object* mob = static_cast<CNavi_Object*>(object);
    this->AddWrappedRouteLines(2, 141, 46, mob->GetName());
    NaviCell cell;
    std::sprintf(text, "Lv. %d", mob->GetCell(&cell)->x);
    this->routeLines.push_back(NaviRouteLine(2, 142, 77, text, 0xDCDCDC, 0xCB6B54));

    const int attributes = mob->GetCell(&cell)->y;
    const unsigned int property = static_cast<unsigned char>(attributes >> 16);
    const unsigned int propertyLevel = property / 20;
    const unsigned int element = property % 10;
    const unsigned int size = static_cast<unsigned char>(attributes);

    int message;
    if (element == 1) {
      message = 2263;
    } else if (element == 2) {
      message = 2264;
    } else if (element == 3) {
      message = 2265;
    } else if (element == 4) {
      message = 2266;
    } else if (element == 5) {
      message = 2267;
    } else if (element == 6) {
      message = 2268;
    } else if (element == 7) {
      message = 2269;
    } else if (element == 8) {
      message = 2270;
    } else {
      message = element == 9 ? 2271 : 2272;
    }
    std::sprintf(text, LookupMessage(message), propertyLevel);
    this->routeLines.push_back(NaviRouteLine(2, 142, 93, text, 0xDCDCDC, 0xCB6B54));

    if (size == 1) {
      message = 2276;
    } else if (size == 2) {
      message = 2277;
    } else if (size == 3) {
      message = 2278;
    } else if (size == 4) {
      message = 2279;
    } else if (size == 5) {
      message = 2280;
    } else if (size == 6) {
      message = 2281;
    } else if (size == 7) {
      message = 2282;
    } else if (size == 8) {
      message = 2283;
    } else {
      message = size == 9 ? 2284 : 2285;
    }
    std::sprintf(text, LookupMessage(message));
    this->routeLines.push_back(NaviRouteLine(2, 142, 109, text, 0xDCDCDC, 0xCB6B54));

    if ((attributes & 0xFF00) == 0x100) {
      message = 2273;
    } else {
      message = (attributes & 0xFF00) == 0x200 ? 2274 : 2275;
    }
    std::sprintf(text, LookupMessage(message));
    this->routeLines.push_back(NaviRouteLine(2, 142, 125, text, 0xDCDCDC, 0xCB6B54));

    if (mob->GetType() == 301) {
      this->routeLines.push_back(NaviRouteLine(2, 142, 149, LookupMessage(2230), 0xC8C8C8, 0xFF));
    }
  } else if (type == 0) {
    this->AddWrappedRouteLines(1, 141, 46, static_cast<CNaviInfo*>(object)->GetName());
    std::sprintf(text, "( %d %d )", this->currentTarget.x, this->currentTarget.y);
    this->routeLines.push_back(NaviRouteLine(1, 142, 149, text, 0xDCDCDC, 0xCB6B54));
  }
  return true;
}
