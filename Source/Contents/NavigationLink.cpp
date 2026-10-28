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
