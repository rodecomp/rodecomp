#include "Contents/TableMng.h"

#include <windows.h>

#include <utility>

// OFFSET: 0x00CA3400

CTableMng::~CTableMng() {
  this->Clear();
}

// OFFSET: 0x00CA3630

// OFFSET: 0x00CA3640

// OFFSET: 0x00CA3690
// FUNCTION: fn_20930
bool CTableMng::Init() {
  return true;
}

// OFFSET: 0x00CA3FD0

// OFFSET: 0x00CA4330

// OFFSET: 0x00CA43B0

// OFFSET: 0x00CA4410
// FUNCTION: fn_20937
void CTableMng::Release() {
  this->Clear();
}
