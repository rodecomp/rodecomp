#include "UI/Window/UIRegisterMgr.h"

// OFFSET: 0x00A970A0
// FUNCTION: fn_17199
bool CUIRegisterMgr::sub_A970A0(int key) {
  return this->entries.find(key) != this->entries.end();
}
