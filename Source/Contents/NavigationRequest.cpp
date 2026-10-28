#include "Contents/NavigationManager.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/World/WorldScene.h"

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

// OFFSET: 0x00B8E640

// OFFSET: 0x00B8E6F0

// OFFSET: 0x00B8E750
// FUNCTION: fn_19480
std::vector<CNavi_Object*>* CNaviInfo::Npcs() {
  return &this->npcs;
}
