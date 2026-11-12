#include "Contents/Navigation/NavigationManager.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"

#include "Contents/Tables/MessageTable.h"
#include "Utility/Text/FormatString.h"
#include "Client/Application/Registry.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Resource/ResourceMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/UIWindow.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Navigation/UINavigationWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/CashShop/UIPayConfirmWnd.h"
#include "Client/Application/RegistryKey.h"

#include <cstdint>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"

namespace {
  // GLOBAL: gl_25136
  int g_locationFrame = 1;
  // GLOBAL: gl_25137
  DWORD g_locationTick;
}

// OFFSET: 0x00B99890
// FUNCTION: fn_19614
void CNavigationManager::DrawRoute(const float* matrix) {
  if (!this->drawRoute || this->movementRoute.points.empty() || this->ground == nullptr) {
    return;
  }

  int playerX = 0;
  int playerY = 0;
  if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
    mode->PlayerCell(&playerX, &playerY);
  }

  int visible = 0;
  const int maxY = playerY + 10;
  const int minY = playerY - 10;
  const int minX = playerX - 10;
  const int maxX = playerX + 10;
  const PathCell& last = this->movementRoute.points.back();
  const int lastX = last.x;
  const int lastY = last.y;

  if (minX < lastX && maxX > lastX && minY < lastY && maxY > lastY) {
    visible = 1;
    std::uint32_t color;
    if (this->drawDestination && std::strcmp(g_session.m_navigationPlayerMap, this->data.GetInfo()->GetName()) == 0) {
      color = 0xEEFFFFFF;
    } else {
      color = 0xEE64AAFF;
    }

    const DWORD now = timeGetTime();
    if (static_cast<int>(now - g_locationTick) >= 0x96) {
      g_locationTick = now;
      if (++g_locationFrame > 4) {
        g_locationFrame = 1;
      }
    }

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode && mode->scene && mode->scene->ground) {
      GroundGeometry* ground = mode->scene->ground;
      ground->DrawTerrainOverlay(
        matrix, lastX, lastY, color,
        g_texMgr.Load(FormatOwnedString("유저인터페이스\\navigation_interface3\\location%d.tga", g_locationFrame).c_str(), 0, false, true, false)
      );
    }
  }

  const int blink = static_cast<int>(timeGetTime() % 1000 / 100);
  auto& points = this->movementRoute.points;

  if (points.size() - 1 != 0) {
    for (unsigned index = 0; index < points.size() - 1; index += 2) {
      const int x = points[index].x;
      const int y = points[index].y;
      if (minX < x && maxX > x && minY < y && maxY > y) {
        ++visible;
        CTexture* texture = g_texMgr.Load(this->labels[points[index + 1].metadata].c_str(), 0, false, true, false);
        constexpr float uv[8] = {0, 0, 1, 0, 0, 1, 1, 1};
        const std::uint32_t color = blink == static_cast<int>(index) % 10 ? 0xEEFFFFFF : 0x99FFFFFF;
        this->ground->DrawCellDecal(matrix, x, y, color, *texture, uv);
      }
    }
  }

  if (visible == 0 && !points.empty()) {
    this->BuildRoute(playerX, playerY, points.back().x, points.back().y, true);
  }
}

// OFFSET: 0x00B88800
// FUNCTION: fn_19376
__declspec(noinline) CNaviInfo* CNavigationManager::FindObject(const char* name) const {
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
    const unsigned int id = key;
    for (auto it = this->objects.begin(); it != this->objects.end(); ++it) {
      if ((*it)->HasId(id)) {
        return *it;
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

  for (auto it = this->objects.begin(); it != this->objects.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->objects.clear();

  for (auto it = this->destinations.begin(); it != this->destinations.end(); ++it) {
    if (*it != nullptr) {
      delete *it;
    }
    *it = nullptr;
  }
  this->destinations.clear();
}

// OFFSET: 0x00B8CF70
// FUNCTION: fn_19432
__declspec(noinline) bool CNavi_Object::HasId(const unsigned int& id) const {
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
__declspec(noinline) int CNaviInfo::GetCellX() const {
  return this->cellX;
}

// OFFSET: 0x00B97750
// FUNCTION: fn_19600
bool CNavigationManager::BuildRoute(int startX, int startY, int destX, int destY, bool collectCells) {
  this->movementRoute.points.erase(this->movementRoute.points.begin(), this->movementRoute.points.end());
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
    const std::string resolved = ResourceMgr::Instance().ResolveMapName(g_session.m_navigationPlayerMap);
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

  if (g_windowMgr.navigationWnd != nullptr) {
    g_windowMgr.navigationWnd->Invalidate();
  }
  return found;
}

#pragma pack(push, 1)
struct PrivateAirshipRequestPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ char mapName[16];
  /* 0012 */ int itemId;
};
#pragma pack(pop)

static_assert(sizeof(PrivateAirshipRequestPacket) == 0x16);

// OFFSET: 0x00B97980
// FUNCTION: fn_19601
void CNavigationManager::RequestPrivateAirship(std::string mapName, int itemId) {
  if (mapName.size() == 0) {
    return;
  }

  if (itemId == 0x1AFD) {
    UIPayConfirmWnd* window = new UIPayConfirmWnd();
    window->Configure(LookupMessage(MSI_ENCHANT_POSSIBLE), 0x1AFD);
    window->SendMsg(nullptr, 0x9C, 1, 0, 0, 0);
    window->SetId(0x16B);
    g_windowMgr.AddRendered(window);
    if (g_windowMgr.RunModal(window) != 187) {
      return;
    }
  }

  PrivateAirshipRequestPacket packet;
  packet.id = ID_CZ_REQ_PRIVATE_AIRPLANE_CALL;
  strcpy_s(packet.mapName, sizeof(packet.mapName), mapName.c_str());
  packet.itemId = itemId;
  GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
}

// OFFSET: 0x00B97E90
// FUNCTION: fn_19603
NaviCell* CNavigationManager::MapCellToView(NaviCell* out, const char* mapName, int cellX, int cellY, float size) {
  CNaviInfo* info = this->FindObject(mapName);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().ResolveMapName(mapName);
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
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "[%s] -> %s %d %d", LookupMessage(MSI_RODEX_BUTTON_DELETE_SELECTED), info->GetName(), x, y);
    } else {
      StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "[%s] %s", LookupMessage(MSI_RODEX_BUTTON_DELETE_SELECTED), info->GetName());
    }
  }

  this->routeText.assign(buffer, std::strlen(buffer));
  return this->routeText;
}

// OFFSET: 0x00B98780
// FUNCTION: fn_19609
__declspec(noinline) void CNavigationManager::ShowRouteResult(int result, int mode) {
  std::string message;
  switch (result) {
  case -100:
    message = LookupMessage(MSI_NAVIGATION_RS_FAILTARGET);
    break;
  case -99:
    message = LookupMessage(MSI_NAVIGATION_RS_FAILROUTE);
    break;
  case -98:
    message = LookupMessage(MSI_NAVIGATION_RS_FAILPLAYER);
    break;
  case -97:
    message = LookupMessage(MSI_NAVIGATION_RS_NODATA);
    break;
  case -96:
    message = LookupMessage(MSI_NAVIGATION_RS_MAPNOTNAVIGATION);
    break;
  case -95:
    message = LookupMessage(MSI_NAVIGATION_RS_FAILTARGETROUTE);
    break;
  case -94:
    message = LookupMessage(MSI_NAVIGATION_RS_SUCCESS);
    break;
  case -93:
    message = LookupMessage(MSI_NAVIGATION_RS_START);
    break;
  case -92:
    message = LookupMessage(MSI_NAVIGATION_RS_MOBTARGETMAP);
    break;
  case 0:
    message = LookupMessage(MSI_NAVIGATION_RS_TARGETMAP);
    break;
  case 1:
    message = LookupMessage(MSI_NAVIGATION_RS_SCROLL);
    break;
  case 2:
    message = LookupMessage(MSI_NAVIGATION_RS_ROUTE_SUCCESS);
    break;
  case 3:
    message = LookupMessage(MSI_NAVIGATION_RS_MAPTMAP_SUCCESS);
    break;
  case 4:
    message = LookupMessage(MSI_NAVIGATION_RS_NPCTMAP_SUCCESS);
    break;
  case 5:
    message = LookupMessage(MSI_NAVIGATION_RS_MOBTMAP_SUCCESS);
    break;
  case 6:
    message = LookupMessage(MSI_NAVIGATION_RS_POSTMAP_SUCCESS);
    break;
  case 7:
    message = LookupMessage(MSI_NAVIGATION_RS_SAMETARGET);
    break;
  case 8:
    message = LookupMessage(MSI_NAVIGATION_RS_HERE);
    break;
  case 10:
    message = LookupMessage(MSI_NAVIGATION_PRINT_S);
    break;
  case 9:
    message = LookupMessage(MSI_NAVIGATION_PRINT_GO_S);
    break;
  case 11:
    message = LookupMessage(MSI_NAVIGATION_PRINT_MAP_S);
    break;
  case -90:
    message = LookupMessage(4692);
    break;
  default:
    message = "NoMsg.";
    break;
  }

  if (mode == 0 || mode == 1) {
    char buffer[250];
    std::memset(buffer, 0, sizeof(buffer));
    std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_PRINT_SELECTMAP_S), message.c_str());
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);

    if (result == 5 && this->selectedNode != -1 && this->nodes.size() > static_cast<unsigned>(this->selectedNode)) {
      const int type = this->nodes[this->selectedNode]->GetType();
      if (type == 201 || type == 202 || type == 203 || type == 204) {
        std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_PRINT_PLANE), this->nodes[this->selectedNode]->GetName());
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);

        CNaviInfo* info = static_cast<CNavi_Link*>(this->nodes[this->selectedNode])->GetLinkedInfo();
        if (info != nullptr) {
          char text[0x104];
          std::memset(text, 0, sizeof(text));
          std::sprintf(text, " %s[ %s ] ", info->GetTitle(), info->GetName());
          std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_PRINT_WARP), text);
        }
      } else if (type == 205) {
        std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_PRINT_PLANE), this->nodes[this->selectedNode]->GetName());
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);
        std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_ZZ));
      } else {
        std::sprintf(buffer, LookupMessage(MSI_NAVIGATION_ZZLV));
      }
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(buffer), 0x62B0FF, 0, 0, 0);
    }
  }

  if (mode == 0 || mode == 2) {
    g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    if (mode == 4) {
      const int count = static_cast<int>(g_navigationManager.nodes.size());
      CNavi_Object* node = count == 0 ? nullptr : g_navigationManager.nodes[0];
      std::string text(LookupMessage(MSI_NAVIGATION_ZZLIMIT));
      text += node->GetName();
      text += " ";
      text += static_cast<CNavi_Link*>(node)->GetLinkedName();
      g_windowMgr.Confirm(text.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
    }
  }
}

// OFFSET: 0x00B8D270
// FUNCTION: fn_19434
__declspec(noinline) bool CNaviInfo::HasName(const char* name) const {
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
    this->selectedNodeText.assign(buffer, std::strlen(buffer));
    return this->selectedNodeText;
  }

  if (this->viewMode == 2) {
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
      return std::string(buffer);
    }

    CNavi_Object* node = this->nodes[this->selectedNode];
    NaviCell cellY;
    NaviCell cellX;
    const int y = node->GetCell(&cellY)->y;
    const int x = node->GetCell(&cellX)->x;
    std::sprintf(buffer, "%s %d %d", node->GetInfo()->GetName(), x, y);
    this->destinationText.assign(buffer, std::strlen(buffer));
    return this->destinationText;
  }

  if (this->viewMode == 2) {
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

  if (this->viewMode == 2) {
    NaviCell cell;
    this->data.GetCell(&cell);
    CNaviInfo* info = this->data.info;
    if (info != nullptr) {
      this->destinationTitle.assign(info->GetTitle());
    }
    return this->destinationTitle;
  }

  this->destinationTitle.assign(buffer, std::strlen(buffer));
  return this->destinationTitle;
}

#pragma pack(push, 1)
struct NavigationRouteRecord {
  /* 0000 */ std::uint32_t header;
  /* 0004 */ std::int16_t length;
  /* 0006 */ char mapName[30];
  /* 0024 */ int type;
  /* 0028 */ unsigned char mode;
  /* 0029 */ bool flag;
  /* 002A */ int x;
  /* 002E */ int y;
  /* 0032 */ int id;
};
#pragma pack(pop)

static_assert(sizeof(NavigationRouteRecord) == 0x36);

// INLINED
static bool IsSameInstanceMap(std::string player, std::string target) {
  std::size_t position = player.rfind("@");
  if (position != std::string::npos && position >= 4) {
    player.erase(position - 4, 3);
  }
  position = target.rfind("@");
  if (position != std::string::npos && position >= 4) {
    target.erase(position - 4, 3);
  }
  return player == target;
}

// OFFSET: 0x00B99030
// FUNCTION: fn_19611
bool CNavigationManager::StartNavigation(std::string name, int type, unsigned char mode, bool flag, int x, int y, int id) {
  if (type < 0 || type > 4 || name.size() == 0) {
    return false;
  }

  const char* playerMap = g_session.m_navigationPlayerMap;
  if (std::strchr(playerMap, '@')) {
    if (!IsSameInstanceMap(playerMap, name)) {
      this->ShowRouteResult(-90, 1);
      return false;
    }
  }

  CNaviInfo* info = this->FindObject(name.c_str());
  if (!info) {
    const std::string resolved = ResourceMgr::Instance().ResolveMapName(name.c_str());
    info = this->FindObject(resolved.c_str());
    if (!info) {
      return false;
    }
  }

  NavigationTarget target(type, nullptr);
  std::string text = info->GetName();
  if (type == 1) {
    target.object = info;
  } else if (type == 2) {
    CNavi_Object* npc = info->FindNpcAt(x, y);
    if (!npc) {
      this->ShowRouteResult(-94, 1);
      return false;
    }
    text.push_back(' ');
    text += npc->GetName();
    target.object = npc;
  } else if (type == 3) {
    CNavi_Object* mob = info->FindMobByIndex(id);
    if (!mob) {
      this->ShowRouteResult(-94, 1);
      return false;
    }
    text.push_back(' ');
    text += mob->GetName();
    target.object = mob;
  } else if (type == 0) {
    target.object = info;
  } else {
    return false;
  }

  this->currentTarget = this->savedTarget = target;
  this->searchText = text;
  this->hideWindow = flag;
  this->allowWarps = (mode >> 2) & 1;
  this->allowGates = mode & 1;
  this->includeObjects = (mode >> 1) & 1;
  if (this->UpdateSearchResults()) {
    this->selectedPath = 0;
    this->selectedPathNode = 0;
  }

  const int result = this->Navigate(x, y);
  this->ShowRouteResult(result, 1);
  if (result < 0) {
    return false;
  }

  if (g_windowMgr.modal) {
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapSelection();
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapLevels();
    reinterpret_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapMarks();
  }

  this->displayMode = 1;
  if (g_windowMgr.navigationWnd) {
    this->hideWindow = false;
  }
  if (!this->hideWindow) {
    if (g_windowMgr.navigationWnd) {
      static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->RefreshLayout();
    } else {
      g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
    }
  }

  {
    std::string recordName = name;
    if (Replay::Instance().state == 2) {
      char packet[400] = {};
      NavigationRouteRecord record = {};
      record.header = 0x038E08AE;
      record.length = sizeof(record);
      std::memcpy(record.mapName, recordName.c_str(), recordName.size());
      record.type = type;
      record.mode = mode;
      record.flag = flag;
      record.x = x;
      record.y = y;
      record.id = id;
      std::memcpy(packet, &record, sizeof(record));

      Replay::Instance().Record(static_cast<std::uint16_t>(record.length), packet, 0x29, 0);
    }
  }
  return true;
}

// OFFSET: 0x00B995D0
// FUNCTION: fn_19612
bool CNavigationManager::SearchNavigation(std::string name, int type, bool flag) {
  if (static_cast<unsigned int>(type) <= 4) {
    this->searchText = name;
    this->hideWindow = flag;
    this->searchType = type;
    g_navigationManager.UpdateSearchResults();

    if (!this->hideWindow) {
      if (g_windowMgr.navigationWnd == nullptr) {
        g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
      } else {
        g_windowMgr.navigationWnd->SendMsg(nullptr, UM_COMMAND, ID_BASE, 0, 0, 0);
        if (g_windowMgr.modal != nullptr) {
          static_cast<UINavigationWnd*>(g_windowMgr.modal)->RefreshWorldMapLevels();
        }
      }

      g_windowMgr.Promote(g_windowMgr.navigationWnd);
    }

    return true;
  }

  return false;
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

namespace {
#pragma pack(push, 1)
struct ReplayTaggedRecordHeader {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ std::uint32_t bytes;
};
#pragma pack(pop)

// INLINED
__forceinline std::vector<unsigned char> MakeReplayTaggedRecord(std::uint16_t id, const void* data, std::uint32_t bytes) {
  std::vector<unsigned char> record(bytes + sizeof(ReplayTaggedRecordHeader), 0);
  ReplayTaggedRecordHeader* header = reinterpret_cast<ReplayTaggedRecordHeader*>(record.data());
  header->id = id;
  header->bytes = bytes;
  if (bytes != 0) {
    std::memcpy(record.data() + sizeof(ReplayTaggedRecordHeader), data, bytes);
  }
  return record;
}

// INLINED
__forceinline std::vector<char> WriteLink(CNavi_Link* link) {
  std::vector<char> buffer;
  if (link != nullptr) {
    const int type = link->GetType();
    const int index = link->GetIndex();
    NaviCell cell;
    link->GetCell(&cell);
    std::string name = link->GetName();
    std::string title = link->GetTitle();
    std::string map = link->GetInfo()->GetName();
    std::string linkedMap = link->GetLinkedName();
    const NaviCell linkedCell = link->GetLinkedCell();

    buffer.resize(linkedMap.size() + 0x20 + title.size() + name.size() + map.size(), '\0');

    reinterpret_cast<NaviObjectRecord*>(buffer.data())->type = type;
    reinterpret_cast<NaviObjectRecord*>(buffer.data())->index = index;
    std::memcpy(buffer.data() + 8, &cell, sizeof(cell));

    std::int16_t length = static_cast<std::int16_t>(name.size());
    reinterpret_cast<NaviObjectRecord*>(buffer.data())->nameLength = length;
    std::memcpy(buffer.data() + 0x12, name.c_str(), length);
    std::size_t offset = length + 0x12;
    length = static_cast<std::int16_t>(title.size());
    *reinterpret_cast<std::int16_t*>(buffer.data() + offset) = length;
    offset += 2;
    std::memcpy(buffer.data() + offset, title.c_str(), length);
    offset += length;
    length = static_cast<std::int16_t>(map.size());
    *reinterpret_cast<std::int16_t*>(buffer.data() + offset) = length;
    offset += 2;
    std::memcpy(buffer.data() + offset, map.c_str(), length);
    offset += length;
    std::memcpy(buffer.data() + offset, &linkedCell, sizeof(linkedCell));
    length = static_cast<std::int16_t>(linkedMap.size());
    *reinterpret_cast<std::int16_t*>(buffer.data() + offset + 8) = length;
    offset += 10;
    std::memcpy(buffer.data() + offset, linkedMap.c_str(), length);
  }
  return buffer;
}
}

// OFFSET: 0x00B99CE0
// FUNCTION: fn_19615
std::vector<unsigned char> CNavigationManager::WriteReplayState() {
  std::vector<unsigned char> output;
  const auto append = [&output](const std::vector<unsigned char>& record) { output.insert(output.end(), record.begin(), record.end()); };

  append(MakeReplayTaggedRecord(8000, nullptr, 0));
  append(MakeReplayTaggedRecord(8002, &this->allowGates, 1));
  append(MakeReplayTaggedRecord(8003, &this->includeObjects, 1));
  append(MakeReplayTaggedRecord(8004, &this->allowWarps, 1));
  append(MakeReplayTaggedRecord(8005, &this->routeSide, 4));

  for (std::size_t i = 0; i < this->nodes.size(); ++i) {
    const std::vector<char> buffer = WriteLink(this->nodes[i]);
    append(MakeReplayTaggedRecord(8022, buffer.data(), static_cast<std::uint32_t>(buffer.size())));
  }

  append(BuildReplayTaggedRecord(8009, &this->savedCellX, 4));
  append(BuildReplayTaggedRecord(8010, &this->savedCellY, 4));
  append(BuildReplayTaggedRecord(8011, &this->expandedMap, 1));
  append(BuildReplayTaggedRecord(8013, &this->displayMode, 4));
  append(BuildReplayTaggedRecord(8014, &this->searchType, 4));
  append(BuildReplayTaggedRecord(8016, &this->previewType, 4));
  append(BuildReplayTaggedRecord(8017, &this->drawRoute, 1));
  append(BuildReplayTaggedRecord(8018, &this->routeResult, 4));
  append(BuildReplayTaggedRecord(8019, &this->routeFlag, 1));
  append(BuildReplayTaggedRecord(8020, &this->selectedNode, 4));
  append(BuildReplayTaggedRecord(8023, &this->hideWindow, 1));
  append(BuildReplayTaggedRecord(8012, &this->viewMode, 4));
  if (!this->searchText.empty()) {
    append(BuildReplayTaggedRecord(8015, this->searchText.c_str(), static_cast<std::uint32_t>(this->searchText.size())));
  }

  std::vector<char> target;
  for (std::size_t i = 0; i < this->states.size(); ++i) {
    target = this->WriteTarget(this->states[i]);
    if (target.size() != 0) {
      append(MakeReplayTaggedRecord(8006, target.data(), static_cast<std::uint32_t>(target.size())));
    }
  }

  target = this->WriteTarget(this->currentTarget);
  if (target.size() != 0) {
    append(BuildReplayTaggedRecord(8007, target.data(), static_cast<std::uint32_t>(target.size())));
  }

  target = this->WriteTarget(this->savedTarget);
  if (target.size() != 0) {
    append(BuildReplayTaggedRecord(8008, target.data(), static_cast<std::uint32_t>(target.size())));
  }

  const int count = static_cast<int>(this->movementRoute.points.size());
  if (count > 1) {
    NaviCell route[2];
    route[0].x = this->movementRoute.points[0].x;
    route[0].y = this->movementRoute.points[0].y;
    route[1].x = this->movementRoute.points[count - 1].x;
    route[1].y = this->movementRoute.points[count - 1].y;
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
    scale = size / width;
    *offsetY = 0;
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
__declspec(noinline) NaviCell* CNaviData::GetCell(NaviCell* out) {
  *out = NaviCell{this->cellX, this->cellY};
  return out;
}

// OFFSET: 0x00B88920
// FUNCTION: fn_19380
__declspec(noinline) CNaviInfo* CNaviData::GetInfo() {
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
  this->links.erase(this->links.begin(), this->links.end());
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

  if (this->savedTarget.type == 2) {
    CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
    CNaviInfo* info = object->GetInfo();
    NaviCell cellY;
    NaviCell cellX;
    std::sprintf(text, "%s %d %d", info->GetName(), object->GetCell(&cellX)->x, object->GetCell(&cellY)->y);
  } else if (this->savedTarget.type == 3) {
    std::sprintf(text, "%s", static_cast<CNavi_Object*>(this->savedTarget.object)->GetInfo()->GetName());
  } else if (this->savedTarget.type == 1) {
    std::sprintf(text, "%s", static_cast<CNaviInfo*>(this->savedTarget.object)->GetName());
  } else if (this->savedTarget.type == 0) {
    CNaviInfo* info = static_cast<CNaviInfo*>(this->savedTarget.object);
    NaviCell cellY;
    NaviCell cellX;
    const int y = this->data.GetCell(&cellY)->y;
    const int x = this->data.GetCell(&cellX)->x;
    std::sprintf(text, "%s %d %d", info->GetName(), x, y);
  } else {
    return false;
  }

  if (size <= static_cast<int>(std::strlen(text))) {
    char truncated[0x104];
    std::memset(truncated, 0, sizeof(truncated));
    std::memcpy(truncated, text, size - 4);
    std::sprintf(out, "%s...", truncated);
    return true;
  }

  std::memcpy(out, text, std::strlen(text));
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
__declspec(noinline) bool CNaviData::CollectLinksTo(CNaviInfo* target, std::vector<CNavi_Link*>* out) {
  out->clear();
  for (auto info = this->destinations.begin(); info != this->destinations.end(); ++info) {
    std::vector<CNavi_Link*>::iterator link;
    for (link = (*info)->LinksBegin(); link != (*info)->LinksEnd(); ++link) {
      if (target == (*link)->GetLinkedInfo()) {
        out->push_back(*link);
      }
    }
  }
  for (auto& link : this->objects) {
    if (target == link->GetLinkedInfo()) {
      out->push_back(link);
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
__declspec(noinline) bool CNaviData::Search(std::string query, int type, std::vector<NavigationTarget>& out) {
  if (query.size() == 0) {
    return false;
  }
  std::vector<std::string> tokens;
  std::vector<char> text(query.begin(), query.end());
  bool result = false;
  text.push_back(0);
  std::vector<int> spaces;
  spaces.push_back(-1);
  for (int i = 0; i < text.size(); ++i) {
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
    for (auto& info : this->destinations) {
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
  if (std::strncmp(tokens[0].c_str(), LookupMessage(MSI_NAVIGATION_ZZALL), std::strlen(LookupMessage(MSI_NAVIGATION_ZZALL))) == 0) {
    std::size_t prefixLength = std::strlen(LookupMessage(MSI_SHOP_ITEM_BAY_TITLE));
    if (prefixLength < tokens[0].size() && std::strncmp(tokens[0].c_str(), LookupMessage(MSI_SHOP_ITEM_BAY_TITLE), prefixLength) == 0) {
      std::string token(tokens[0]);
      const char* separator = std::strstr(token.c_str(), LookupMessage(MSI_SHOP_ITEM_BAY_MAG1));
      std::string minimum;
      std::string maximum;
      if (separator == nullptr) {
        minimum.assign(token.c_str() + prefixLength, token.c_str() + token.size());
        maximum = minimum;
      } else {
        minimum.assign(token.c_str() + prefixLength, separator);
        maximum.assign(separator + std::strlen(LookupMessage(MSI_SHOP_ITEM_BAY_MAG1)), token.c_str() + token.size());
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
    if (std::strncmp(tokens[0].c_str(), LookupMessage(MSI_SHOP_ITEM_BAY_MAG2), std::strlen(LookupMessage(MSI_SHOP_ITEM_BAY_MAG2))) == 0) {
      return 2;
    }
    return std::strncmp(tokens[0].c_str(), "$$DB", 4) == 0 ? 4 : 0;
  }
  return 1;
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
  if (this->data.Search(this->searchText, this->searchType, this->states) && g_windowMgr.navigationWnd) {
    static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->FillRouteGroups();
  }
}

// OFFSET: 0x00B9C5C0
// FUNCTION: fn_19620
__declspec(noinline) bool CNavigationManager::UpdateSearchResults() {
  this->displayMode = 0;
  if (!this->data.Search(this->searchText, 0, this->states)) {
    return false;
  }

  std::sort(this->states.begin(), this->states.end(), [](const NavigationTarget& left, const NavigationTarget& right) { return left > right; });
  this->BuildNodePaths();
  this->SelectNodePath(this->searchText);
  return true;
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
  const int path = this->selectedPath;
  const int node = this->selectedPathNode;
  if (path >= 0 && static_cast<unsigned int>(path) < this->nodePaths.size() && node >= 0 &&
      static_cast<unsigned int>(node) < this->nodePaths[path].size() && this->nodePaths[path][node] != -1) {
    return this->SelectState(this->nodePaths[path][node]);
  }

  this->currentTarget = NavigationTarget();
  this->LoadTargetPreview();
  this->ClearRouteNodes();
  if (this->data.GetInfo() && !this->data.GetInfo()->IsSearchable(this->data.GetInfo()->GetName())) {
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_RUNESYSTEM_REWARD_NOTMET)), 0x0A0AFF, 0, 0, 0);
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

      Replay::Instance().Record(static_cast<std::uint16_t>(record.length), packet, 0x29, 0);
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

  this->displayMode = 0;
  this->expandedMap = false;
  this->hideWindow = false;
  if (this->viewMode != 2) {
    this->viewMode = 1;
  }

  this->LoadTargetPreview();
  g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  if (g_windowMgr.navigationWnd) {
    UIWindow* window = g_windowMgr.navigationWnd;
    window->geometry.targetAlpha = 255;
    window->geometry.alphaStart = timeGetTime();
    static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->RefreshLayout();
    static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->LayoutWithDetails();
    g_windowMgr.navigationWnd->Invalidate();
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
  this->previewX = -1;
  this->previewY = -1;
  this->previewSprite = nullptr;
  this->previewAction = nullptr;
  this->previewTexture = nullptr;
  if (!this->currentTarget.object || !this->BuildTargetRouteLines()) {
    return false;
  }

  const int type = this->currentTarget.type;
  void* object = this->currentTarget.object;
  this->previewType = 1;

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
    this->previewSprite = nullptr;
    this->previewAction = nullptr;
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
        this->previewSprite = dynamic_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));
        this->previewAction = dynamic_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));
      }
    }
    this->previewType = 2;
  }

  this->previewTexture = nullptr;
  if (mapType == 5004) {
    this->previewTexture = g_texMgr.Load("유저인터페이스\\collection\\불의주문서.bmp", 0, false, true, false);
  } else if (mapType != 5001 && mapType != 5005) {
    this->previewTexture = g_texMgr.Load("유저인터페이스\\navigation_interface\\Interior.bmp", 0, false, true, false);
  } else {
    char path[60];
    std::memset(path, 0, sizeof(path));
    std::sprintf(path, "유저인터페이스\\map\\%s.bmp", mapName);
    this->previewTexture = g_texMgr.Load(path, 0, false, true, false);
    if (this->previewTexture) {
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
        this->previewY = viewY;
        this->previewX = viewX;
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
        this->previewY = viewY;
        this->previewX = viewX;
      }
    }
  }

  if (!this->previewTexture || this->previewTexture->Width() == 1) {
    this->previewTexture = g_texMgr.Load("유저인터페이스\\navigation_interface\\NoImage.bmp", 0, false, true, false);
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
      this->routeLines.push_back(NaviRouteLine(2, 142, 149, LookupMessage(MSI_NAVIGATION_TARGET_D_D), 0xC8C8C8, 0xFF));
    }
  } else if (type == 0) {
    this->AddWrappedRouteLines(1, 141, 46, static_cast<CNaviInfo*>(object)->GetName());
    std::sprintf(text, "( %d %d )", this->currentTarget.x, this->currentTarget.y);
    this->routeLines.push_back(NaviRouteLine(1, 142, 149, text, 0xDCDCDC, 0xCB6B54));
  }
  return true;
}

// OFFSET: 0x00BA0C90
// FUNCTION: fn_19636
bool CNavigationManager::RouteToSavedTarget(int cellX, int cellY) {
  if (this->savedTarget.object == nullptr) {
    return false;
  }

  this->data.endLinks.clear();
  const int type = this->savedTarget.type;
  int x = -1;
  int y = -1;
  CNaviInfo* info;
  if (type == 2) {
    CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
    if (object->LinkCount() == 0) {
      return false;
    }
    info = object->GetInfo();
    NaviCell cell;
    const NaviCell* position = object->GetCell(&cell);
    x = position->x;
    y = position->y;
    this->data.endLinks = object->CopyLinks();
  } else {
    if (type == 3) {
      info = static_cast<CNavi_Object*>(this->savedTarget.object)->GetInfo();
    } else if (type == 1) {
      info = static_cast<CNaviInfo*>(this->savedTarget.object);
    } else if (type == 0) {
      info = static_cast<CNaviInfo*>(this->savedTarget.object);
      x = cellX;
      y = cellY;
      std::string name(info->GetName());
      name = ResourceMgr::Instance().ResolveMapName(name.c_str());
      if (!this->pathFinder.LoadAttr(name) || !this->pathFinder.Walkable(cellX, cellY)) {
        return false;
      }
    } else {
      return false;
    }

    std::vector<CNavi_Link*> links;
    if (!this->data.CollectLinksTo(info, &links)) {
      return false;
    }

    MovementRouteInterpolation route;
    int cost = 0;
    for (CNavi_Link* link : links) {
      if (this->savedTarget.type == 0) {
        this->pathFinder.Reset();
        cost = 0;
        if (link->GetLinkedCell().x == x && link->GetLinkedCell().y == y) {
          cost = 1;
        } else if (this->pathFinder.FindPath(1, link->GetLinkedCell().x, link->GetLinkedCell().y, 8, 8, x, y, 10, route, 15)) {
          cost = static_cast<int>(route.points.size());
        } else {
          continue;
        }
      }
      NaviLink entry;
      entry.link = link;
      entry.cost = cost;
      this->data.endLinks.push_back(entry);
    }
  }

  if (this->data.endLinks.empty()) {
    return false;
  }
  return this->data.SetDestination(info, x, y);
}

// OFFSET: 0x00BA10C0
// FUNCTION: fn_19637
bool CNavigationManager::SelectTarget(int nodeIndex) {
  this->currentTarget = this->savedTarget;
  if (nodeIndex >= 0 && static_cast<unsigned int>(nodeIndex) < this->nodes.size()) {
    this->currentTarget.type = 6;
    this->currentTarget.object = this->nodes[nodeIndex];
  }
  this->currentTarget = NavigationTarget(this->currentTarget);
  return this->LoadTargetPreview();
}

// OFFSET: 0x00BA12D0
// FUNCTION: fn_19641
void CNavigationManager::CancelRoute() {
  this->Reset(false);
  this->displayMode = 0;
  if (!this->hideWindow) {
    this->viewMode = 1;
    if (g_windowMgr.navigationWnd != nullptr) {
      static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->RefreshLayout();
    }
  }
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_NAVIGATION_RS_POSTMAP_SUCCESS)), 0x62B0FF, 0, 0, 0);
}

// OFFSET: 0x00B97B90
// FUNCTION: fn_19602
__declspec(noinline) int CNavigationManager::Navigate(int cellX, int cellY) {
  int x = cellX;
  int y = cellY;
  if (this->savedTarget.object != nullptr) {
    const int type = this->savedTarget.type;
    CNaviInfo* info;
    if (type == 2) {
      CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
      info = object->GetInfo();
      NaviCell cell;
      const NaviCell* position = object->GetCell(&cell);
      x = position->x;
      y = position->y;
    } else if (type == 3 || type == 1 || type != 0) {
      goto request;
    } else {
      info = static_cast<CNaviInfo*>(this->savedTarget.object);
    }

    bool sameMap = std::strcmp(info->GetName(), g_session.m_navigationPlayerMap) == 0;
    if (!sameMap) {
      const std::string resolved = ResourceMgr::Instance().ResolveMapName(g_session.m_navigationPlayerMap);
      sameMap = std::strcmp(info->GetName(), resolved.c_str()) == 0;
    }

    if (sameMap) {
      if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
        int playerX;
        int playerY;
        mode->PlayerCell(&playerX, &playerY);
        if (x + 1 >= playerX && x - 1 <= playerX && y + 1 >= playerY && y - 1 <= playerY) {
          return 11;
        }
      }
    }
  }

request:
  if (this->RequestRoute(cellX, cellY) == -99) {
    if (!this->RouteToSavedTarget(cellX, cellY)) {
      return -97;
    }
    if (!this->CollectStartLinks()) {
      return -98;
    }
    const int type = this->savedTarget.type;
    this->routeResult =
      this->data.FindRoute(&this->nodes, type != 2 && type != 0, this->allowGates, this->includeObjects, this->allowWarps, this->routeSide);
    if (this->routeResult == 0) {
      return -96;
    }
  }
  return this->RouteFromPlayer();
}

#pragma pack(push, 1)
struct NavigationReplayHeader {
  /* 0000 */ std::int16_t tag;
  /* 0002 */ int size;

  NavigationReplayHeader() : tag(0), size(0) {
  }

  ~NavigationReplayHeader() {
    this->tag = 0;
    this->size = 0;
  }

  void Read(const unsigned char* source) {
    this->size = 0;
    this->tag = 0;
    std::memcpy(this, source, sizeof(*this));
  }
};
#pragma pack(pop)

static_assert(sizeof(NavigationReplayHeader) == 6);

// OFFSET: 0x00B9B080
// FUNCTION: fn_19616
int CNavigationManager::LoadReplayState(const std::vector<unsigned char>& data) {
  g_windowMgr.CloseWindow(WID_NAVIGATIONV4WND);
  this->Reset(true);
  if (data.empty()) {
    return -1;
  }

  std::vector<char> buffer;
  NavigationReplayHeader header;
  unsigned offset = 0;

  header.Read(data.data() + offset);
  offset += 6;
  if (8000 != header.tag) {
    offset = static_cast<unsigned>(-1);
  } else {
    while (data.size() > offset) {
      header.Read(data.data() + offset);
      offset += 6;

      switch (header.tag) {
      case 8006: {
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        NavigationTarget target = this->ReadTarget(buffer.data(), header.size);
        if (target.object != nullptr) {
          this->states.push_back(target);
        }
        break;
      }
      case 8007:
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        this->currentTarget = this->ReadTarget(buffer.data(), header.size);
        break;
      case 8008:
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        this->savedTarget = this->ReadTarget(buffer.data(), header.size);
        break;
      case 8009:
        std::memcpy(&this->savedCellX, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8010:
        std::memcpy(&this->savedCellY, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8011:
        std::memcpy(&this->expandedMap, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8012:
        std::memcpy(&this->viewMode, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8013:
        std::memcpy(&this->displayMode, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8014:
        std::memcpy(&this->searchType, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8015:
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        this->searchText = buffer.data();
        break;
      case 8016:
        std::memcpy(&this->previewType, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8017:
        std::memcpy(&this->drawRoute, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8018:
        std::memcpy(&this->routeResult, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8019:
        std::memcpy(&this->routeFlag, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8021: {
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        const int* route = reinterpret_cast<const int*>(buffer.data());
        int x = route[2];
        int y = route[3];
        if (this->savedTarget.object != nullptr) {
          const int type = this->savedTarget.type;
          CNaviInfo* info;
          bool found = true;
          if (type == 2) {
            CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
            info = object->GetInfo();
            NaviCell cell;
            const NaviCell* position = object->GetCell(&cell);
            x = position->x;
            y = position->y;
          } else if (type == 3) {
            info = static_cast<CNavi_Object*>(this->savedTarget.object)->GetInfo();
            x = -1;
            y = -1;
          } else if (type == 1) {
            x = -1;
            y = -1;
            info = static_cast<CNaviInfo*>(this->savedTarget.object);
          } else if (type == 0) {
            info = static_cast<CNaviInfo*>(this->savedTarget.object);
          } else {
            found = false;
          }
          if (found) {
            this->data.SetDestination(info, x, y);
          }
        }

        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        if (mode) {
          CWorld* scene = mode->scene;
          if (scene) {
            C3dAttr* terrain = scene->terrain;
            if (terrain) {
              this->ground = scene->ground;
              this->pathFinder.attr = terrain;
            }
          }
        }
        break;
      }
      case 8022: {
        buffer.resize(header.size + 1, 0);
        std::memcpy(buffer.data(), data.data() + offset, header.size);
        offset += header.size;
        CNavi_Link* node = this->ReadObject(buffer.data());
        if (node != nullptr) {
          this->nodes.push_back(node);
        }
        break;
      }
      case 8020:
        std::memcpy(&this->selectedNode, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8002:
        std::memcpy(&this->includeObjects, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8003:
        std::memcpy(&this->allowWarps, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8004:
        std::memcpy(&this->allowGates, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8005:
        std::memcpy(&this->routeSide, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8023:
        std::memcpy(&this->hideWindow, data.data() + offset, header.size);
        offset += header.size;
        break;
      case 8001:
        this->ResumeRoute();
        return static_cast<int>(offset);
      default:
        offset += header.size;
        break;
      }
    }
  }
  return static_cast<int>(offset);
}

// OFFSET: 0x00B9BFC0
// FUNCTION: fn_19617
void CNavigationManager::ResumeRoute() {
  if (this->viewMode == 1) {
    this->LoadTargetPreview();
    g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  } else if (this->viewMode == 2) {
    this->ShowRouteResult(this->RouteFromPlayer(), 1);
    if (!this->hideWindow) {
      g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
    }
  }
}
