#include "Contents/NavigationManager.h"

// OFFSET: 0x00B8D8D0
// FUNCTION: fn_19436
CNavi_Link::~CNavi_Link() {
  this->type = 200;
  this->linkedInfo = nullptr;
  this->linkedLinks.clear();
}

// OFFSET: 0x00B8E5F0
// FUNCTION: fn_19473
const char* CNavi_Link::GetTitle() const {
  return this->title.c_str();
}

// OFFSET: 0x00B8E610
// FUNCTION: fn_19475
const char* CNavi_Link::GetName() const {
  return this->name.c_str();
}

// OFFSET: 0x00B8E7D0
// FUNCTION: fn_19483
NaviCell* CNavi_Link::GetCell(NaviCell* out) const {
  out->x = this->cellX;
  out->y = this->cellY;
  return out;
}

// OFFSET: 0x00B8E830
// FUNCTION: fn_19487
int CNavi_Link::GetIndex() const {
  return this->index;
}

// OFFSET: 0x00B8E8A0
// FUNCTION: fn_19492
int CNavi_Link::GetType() const {
  return this->type;
}

// OFFSET: 0x00B8E8C0
// FUNCTION: fn_19494
int CNavi_Link::GetId() const {
  return this->id;
}

// OFFSET: 0x00B8E8F0
// FUNCTION: fn_19497
CNaviInfo* CNavi_Link::GetInfo() const {
  if (this->type == 400) {
    return g_navigationManager.GetLinkedPlayerInfo();
  }
  return this->info;
}

// OFFSET: 0x00B8DB20
// FUNCTION: fn_19444
std::vector<NaviLink>::iterator CNavi_Link::LinkedBegin() {
  return this->linkedLinks.begin();
}

// OFFSET: 0x00B8DE60
// FUNCTION: fn_19456
std::vector<NaviLink>::iterator CNavi_Link::LinkedEnd() {
  return this->linkedLinks.end();
}

// OFFSET: 0x00B8E860
// FUNCTION: fn_19489
CNaviInfo* CNavi_Link::GetLinkedInfo() const {
  return this->linkedInfo;
}

// OFFSET: 0x00B90400
// FUNCTION: fn_19508
void CNavi_Link::SetDepth(int depth) {
  this->depth = depth;
}

// OFFSET: 0x00B8D9A0
// FUNCTION: fn_19438
bool CNaviInfo::AddNpc(CNavi_Object* npc) {
  if (npc == nullptr) {
    return false;
  }

  this->npcs.push_back(npc);
  return true;
}

// OFFSET: 0x00B8D9E0
// FUNCTION: fn_19439
bool CNaviInfo::AddLink(CNavi_Link* link) {
  if (link == nullptr) {
    return false;
  }

  this->links.push_back(link);
  return true;
}

// OFFSET: 0x00B8DA20
// FUNCTION: fn_19440
bool CNaviInfo::AddMob(CNavi_Object* mob) {
  if (mob == nullptr) {
    return false;
  }

  this->mobs.push_back(mob);
  return true;
}

// OFFSET: 0x00B8DA60
// FUNCTION: fn_19441
void CNavi_Link::AddLinkedLink(const NaviLink& link) {
  this->linkedLinks.push_back(link);
}

// OFFSET: 0x00B8DAA0
// FUNCTION: fn_19442
void CNavi_Link::AddLink(const NaviLink& link) {
  this->links.push_back(link);
}

// OFFSET: 0x00B8DAE0
// FUNCTION: fn_19443
void CNavi_Object::AddLink(const NaviLink& link) {
  this->links.push_back(link);
}

// OFFSET: 0x00B8DB40
// FUNCTION: fn_19445
std::vector<NaviLink>::iterator CNavi_Link::LinksBegin() {
  return this->links.begin();
}

// OFFSET: 0x00B8DB50
// FUNCTION: fn_19446
std::vector<NaviLink>::iterator CNavi_Object::LinksBegin() {
  return this->links.begin();
}

// OFFSET: 0x00B8DBB0
// FUNCTION: fn_19447
CNavi_Link* CNavi_Link::FindLinkAt(int x, int y) {
  for (std::vector<NaviLink>::iterator it = this->links.begin(); it != this->links.end(); ++it) {
    NaviCell cell;
    NaviCell* found = it->link->GetCell(&cell);
    if (found->x == x && found->y == y) {
      return it->link;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B8DC00
// FUNCTION: fn_19448
CNavi_Link* CNavi_Object::FindLinkAt(int x, int y) {
  for (std::vector<NaviLink>::iterator it = this->links.begin(); it != this->links.end(); ++it) {
    NaviCell cell;
    NaviCell* found = it->link->GetCell(&cell);
    if (found->x == x && found->y == y) {
      return it->link;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B8DC50
// FUNCTION: fn_19449
CNavi_Link* CNavi_Link::FindLinkTo(int x, int y) {
  for (std::vector<NaviLink>::iterator it = this->links.begin(); it != this->links.end(); ++it) {
    CNavi_Link* link = it->link;
    if (link->unknown_0080 == x && link->unknown_0084 == y) {
      return link;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B8DC90
// FUNCTION: fn_19450
CNavi_Link* CNavi_Object::FindLinkTo(int x, int y) {
  for (std::vector<NaviLink>::iterator it = this->links.begin(); it != this->links.end(); ++it) {
    CNavi_Link* link = it->link;
    if (link->unknown_0080 == x && link->unknown_0084 == y) {
      return link;
    }
  }

  return nullptr;
}

// OFFSET: 0x00B8DCD0
// FUNCTION: fn_19451
void CNavi_Link::ClearLinkedLinks() {
  this->linkedLinks.clear();
}

// OFFSET: 0x00B8DCE0
// FUNCTION: fn_19452
void CNavi_Link::ClearLinks() {
  this->links.clear();
}

// OFFSET: 0x00B8DCF0
// FUNCTION: fn_19453
void CNavi_Object::ClearLinks() {
  this->links.clear();
}

// OFFSET: 0x00B8DDE0
// FUNCTION: fn_19454
void CNavi_Link::EraseLink(std::vector<NaviLink>::iterator& it) {
  it = this->links.erase(it);
}

// OFFSET: 0x00B8DE20
// FUNCTION: fn_19455
void CNavi_Object::EraseLink(std::vector<NaviLink>::iterator& it) {
  it = this->links.erase(it);
}

// OFFSET: 0x00B8DE80
// FUNCTION: fn_19457
std::vector<NaviLink>::iterator CNavi_Link::LinksEnd() {
  return this->links.end();
}

// OFFSET: 0x00B8DE90
// FUNCTION: fn_19458
std::vector<NaviLink>::iterator CNavi_Object::LinksEnd() {
  return this->links.end();
}

// OFFSET: 0x00B8E870
// FUNCTION: fn_19490
const char* CNavi_Link::GetLinkedName() const {
  return this->linkedInfo->name.c_str();
}

// OFFSET: 0x00B8E880
// FUNCTION: fn_19491
NaviCell CNavi_Link::GetLinkedCell() const {
  NaviCell cell;
  cell.x = this->unknown_0080;
  cell.y = this->unknown_0084;
  return cell;
}

// OFFSET: 0x00B90470
// FUNCTION: fn_19510
void CNavi_Link::SetType(int type) {
  if (type >= 200 && type <= 206) {
    this->type = type;
  } else {
    this->type = 200;
  }
}

// OFFSET: 0x00B90490
// FUNCTION: fn_19511
void CNavi_Object::SetType(int type) {
  this->type = 101;
}

// OFFSET: 0x00B904A0
// FUNCTION: fn_19512
bool CNavi_Link::SetId(int id) {
  if (this->id != -1) {
    return false;
  }

  this->id = id;
  return true;
}

// OFFSET: 0x00B904C0
// FUNCTION: fn_19513
bool CNavi_Object::SetId(int id) {
  if (this->id != -1) {
    return false;
  }

  this->id = id;
  return true;
}

// OFFSET: 0x00B904E0
// FUNCTION: fn_19514
int CNaviInfo::LinkCount() const {
  return static_cast<int>(this->links.size());
}

// OFFSET: 0x00B904F0
// FUNCTION: fn_19515
int CNaviInfo::MobCount() const {
  return static_cast<int>(this->mobs.size());
}

// OFFSET: 0x00B90500
// FUNCTION: fn_19516
int CNaviInfo::NpcCount() const {
  return static_cast<int>(this->npcs.size());
}

// OFFSET: 0x00B90510
// FUNCTION: fn_19517
int CNavi_Link::LinkedLinkCount() const {
  return static_cast<int>(this->linkedLinks.size());
}

// OFFSET: 0x00B90520
// FUNCTION: fn_19518
int CNavi_Link::LinkCount() const {
  return static_cast<int>(this->links.size());
}

// OFFSET: 0x00B90530
// FUNCTION: fn_19519
int CNavi_Object::LinkCount() const {
  return static_cast<int>(this->links.size());
}
