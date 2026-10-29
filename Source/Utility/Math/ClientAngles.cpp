#include "Utility/Math/AffineMatrix.h"

#include <bit>
#include <cmath>

// INLINED
float ClientSin(float radians) {
  return std::sin(radians);
}

// INLINED
float ClientCos(float radians) {
  return std::cos(radians);
}

// GLOBAL: gl_24568
float g_radTable[722];

// GLOBAL: gl_24572
float g_sinTable[722];

// GLOBAL: gl_24574
float g_cosTable[722];

namespace {
  // INLINED
  int Wrapped(int degrees) {
    return degrees > 360 || degrees < -360 ? degrees % 360 : degrees;
  }
}

// OFFSET: 0x00554280
// FUNCTION: fn_02021
void CreateTrigonometricTable() {
  for (int index = 0; index <= 360; ++index) {
    const float angle = (static_cast<float>(index) / 180.0f) * 3.141592f;
    g_radTable[index] = angle;
    g_sinTable[index] = ClientSin(angle);
    g_cosTable[index] = ClientCos(angle);
    g_radTable[index + 361] = -angle;
    g_sinTable[index + 361] = -g_sinTable[index];
    g_cosTable[index + 361] = -g_cosTable[index];
  }
}

// OFFSET: 0x00554420
// FUNCTION: fn_02024
float GetRadian(int degrees) {
  const int index = Wrapped(degrees);
  return g_radTable[index >= 0 ? index : 361 + index];
}

// OFFSET: 0x00554470
// FUNCTION: fn_02025
float ClientSine(int degrees) {
  const int index = Wrapped(degrees);
  return g_sinTable[index >= 0 ? index : 361 + index];
}

// OFFSET: 0x005543D0
// FUNCTION: fn_02023
float ClientCosine(int degrees) {
  const int index = Wrapped(degrees);
  return g_cosTable[index >= 0 ? index : 361 + index];
}
