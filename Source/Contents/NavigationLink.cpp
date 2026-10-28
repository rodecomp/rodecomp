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

// OFFSET: 0x00B8D9E0

// OFFSET: 0x00B8DA20

// OFFSET: 0x00B8DA60

// OFFSET: 0x00B8DAA0

// OFFSET: 0x00B8DAE0

// OFFSET: 0x00B8DB40

// OFFSET: 0x00B8DB50

// OFFSET: 0x00B8DBB0

// OFFSET: 0x00B8DC00

// OFFSET: 0x00B8DC50

// OFFSET: 0x00B8DC90

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

// OFFSET: 0x00B8DE20

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
