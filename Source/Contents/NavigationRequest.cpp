#include "Contents/NavigationManager.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/World/WorldScene.h"
#include "Client/Session.h"
#include "Resource/ResourceMgr.h"

#include <cstring>

// OFFSET: 0x00B8DF10
// FUNCTION: fn_19461
std::uint16_t CNavi_Object::GetFlag() const {
  return this->flag;
}

// OFFSET: 0x00B8E600
// FUNCTION: fn_19474
const char* CNavi_Object::GetTitle() const {
  return this->title.c_str();
}

// OFFSET: 0x00B8E620
// FUNCTION: fn_19476
const char* CNaviInfo::GetTitle() const {
  return this->title.c_str();
}

// OFFSET: 0x00B8E630
// FUNCTION: fn_19477
const char* CNavi_Object::GetName() const {
  return this->name.c_str();
}

// OFFSET: 0x00B8E7F0
// FUNCTION: fn_19484
NaviCell* CNavi_Object::GetCell(NaviCell* out) const {
  out->x = this->cellX;
  out->y = this->cellY;
  return out;
}

// OFFSET: 0x00B8E810
// FUNCTION: fn_19485
const char* CNaviInfo::GetName() const {
  return this->name.c_str();
}

// OFFSET: 0x00B8E840
// FUNCTION: fn_19488
int CNavi_Object::GetIndex() const {
  if (this->type == 300 || this->type == 301) {
    return this->shortIndex;
  }
  return this->index;
}

// OFFSET: 0x00B8E8B0
// FUNCTION: fn_19493
int CNavi_Object::GetType() const {
  return this->type;
}

// OFFSET: 0x00B8E8D0
// FUNCTION: fn_19495
int CNavi_Object::GetId() const {
  return this->id;
}

// OFFSET: 0x00B8E910
// FUNCTION: fn_19498
CNaviInfo* CNavi_Object::GetInfo() const {
  return this->info;
}

// OFFSET: 0x00B89940
// FUNCTION: fn_19387
bool CNaviData::SetDestination(CNaviInfo* info, int cellX, int cellY) {
  if (info == nullptr) {
    return false;
  }
  this->info = info;
  this->cellX = cellX;
  this->cellY = cellY;
  return true;
}

// OFFSET: 0x00BA1260
// FUNCTION: fn_19638
bool CNavigationManager::UpdateGround() {
  CGameMode* mode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  if (mode == nullptr || mode->scene == nullptr || mode->scene->terrain == nullptr) {
    return false;
  }
  this->ground = mode->scene->ground;
  this->pathFinder.attr = mode->scene->terrain;
  return true;
}

// OFFSET: 0x00B97200
// FUNCTION: fn_19598
int CNavigationManager::RequestRoute(int cellX, int cellY) {
  this->selectedNode = -1;
  CNaviInfo* info = this->FindObject(g_session.m_navigationPlayerMap);
  if (info == nullptr) {
    const std::string resolved = ResourceMgr::Instance().aliases.ResolveMapName(g_session.m_navigationPlayerMap);
    info = this->FindObject(resolved.c_str());
    if (info == nullptr) {
      return -93;
    }
  }

  CNaviInfo* target = nullptr;
  int destX = cellX;
  int destY = cellY;
  const int type = this->savedTarget.type;
  if (type == 3) {
    if (info == static_cast<CNavi_Object*>(this->savedTarget.object)->GetInfo()) {
      return 2;
    }
    return -99;
  }
  if (type == 1) {
    if (info == this->savedTarget.object) {
      return 3;
    }
    return -99;
  }
  if (type == 0) {
    if (cellX < 0 || cellY < 0) {
      return -97;
    }
    target = static_cast<CNaviInfo*>(this->savedTarget.object);
  } else if (type == 2) {
    CNavi_Object* object = static_cast<CNavi_Object*>(this->savedTarget.object);
    target = object->GetInfo();
    NaviCell cell;
    const NaviCell* result = object->GetCell(&cell);
    destX = result->x;
    destY = result->y;
  }
  if (info != target) {
    return -99;
  }

  int startX = 0;
  int startY = 0;
  CGameMode* mode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  if (mode == nullptr) {
    return -95;
  }
  mode->PlayerCell(&startX, &startY);
  this->UpdateGround();
  if (!this->BuildRoute(startX, startY, destX, destY, false)) {
    return -99;
  }
  this->data.SetDestination(target, destX, destY);
  this->unknown_117C = true;
  this->unknown_115C = true;
  this->unknown_12B4 = true;
  this->unknown_1258 = 2;
  return 9;
}

// OFFSET: 0x00B8E350
// FUNCTION: fn_19466
std::vector<CNavi_Link*>::iterator CNaviInfo::LinksBegin() {
  return this->links.begin();
}

// OFFSET: 0x00B8E360
// FUNCTION: fn_19467
std::vector<CNavi_Link*>::iterator CNaviInfo::LinksEnd() {
  return this->links.end();
}

// OFFSET: 0x00B8E760
// FUNCTION: fn_19481
bool CNaviInfo::IsVisited() const {
  return this->visited;
}

// OFFSET: 0x00B90410
// FUNCTION: fn_19509
void CNaviInfo::SetVisited(bool visited) {
  if (this->type == 5001) {
    this->visited = visited;
  }
}

// OFFSET: 0x00B8DF20
// FUNCTION: fn_19462
int CNaviInfo::GetCellY() const {
  return this->cellY;
}

// OFFSET: 0x00B8DF30
// FUNCTION: fn_19463
int CNaviInfo::GetIndex() const {
  return this->index;
}

// OFFSET: 0x00B8DF40
// FUNCTION: fn_19464
CNavi_Link* CNaviInfo::FindLink(unsigned int key, bool byId) {
  if (byId) {
    for (CNavi_Link* link : this->links) {
      if (static_cast<unsigned int>(link->id) == key) {
        return link;
      }
    }
    return nullptr;
  }
  if (this->links.empty() || key >= this->links.size()) {
    return nullptr;
  }
  return this->links[key];
}

// OFFSET: 0x00B8E640
// FUNCTION: fn_19478
CNavi_Object* CNaviInfo::FindNpc(unsigned int key, bool byId) {
  if (byId) {
    for (CNavi_Object* npc : this->npcs) {
      if (static_cast<unsigned int>(npc->id) == key) {
        return npc;
      }
    }
    return nullptr;
  }
  if (this->npcs.empty() || key >= this->npcs.size()) {
    return nullptr;
  }
  return this->npcs[key];
}

// OFFSET: 0x00B8E6F0
// FUNCTION: fn_19479
CNavi_Object* CNaviInfo::FindNpcAt(int cellX, int cellY) {
  for (CNavi_Object* npc : this->npcs) {
    if (npc->cellX == cellX && npc->cellY == cellY) {
      return npc;
    }
  }
  return nullptr;
}

// OFFSET: 0x00B8E750
// FUNCTION: fn_19480
std::vector<CNavi_Object*>* CNaviInfo::Npcs() {
  return &this->npcs;
}

// OFFSET: 0x00B8E010
// FUNCTION: fn_19465
CNavi_Link* CNaviInfo::FindMatchingLink(CNavi_Link link) {
  const CNavi_Link target = link;
  for (CNavi_Link* candidate : this->links) {
    if (candidate->cellX == target.cellX && candidate->cellY == target.cellY && candidate->unknown_0080 == target.unknown_0080 &&
        candidate->unknown_0084 == target.unknown_0084 && candidate->info != nullptr &&
        std::strcmp(candidate->info->name.c_str(), target.info->name.c_str()) == 0 && candidate->linkedInfo != nullptr &&
        std::strcmp(candidate->linkedInfo->name.c_str(), target.linkedInfo->name.c_str()) == 0) {
      return candidate;
    }
  }
  return nullptr;
}
