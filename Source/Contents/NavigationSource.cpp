#include "Contents/NavigationManager.h"
#include "Client/Session.h"
#include "Resource/ResourceMgr.h"

#include <cstdio>
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
