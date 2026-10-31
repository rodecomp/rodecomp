#include "Client/Mode/CMode.h"
#include "Contents/Configuration/ServiceConfiguration.h"

// OFFSET: 0x00AD5290
// FUNCTION: fn_17819
int SetServiceFlags(int first, int second) {
  const int serviceType = g_serviceType;
  g_serviceFlag05 = first != 0;
  g_serviceFlag06 = second != 0;
  return serviceType;
}

// OFFSET: 0x00AD53B0
// FUNCTION: fn_17821
void* __fastcall sub_AD53B0(void* self) {
  *reinterpret_cast<int*>(self) = 1;
  return self;
}

// OFFSET: 0x00AD53C0
// FUNCTION: fn_17822
CMode::~CMode() {
}
