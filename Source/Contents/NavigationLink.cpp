#include "Contents/NavigationManager.h"

// OFFSET: 0x00B8D8D0

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
