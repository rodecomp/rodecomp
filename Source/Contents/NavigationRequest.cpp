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

// OFFSET: 0x00BA1260
