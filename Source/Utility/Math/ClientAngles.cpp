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

namespace {
  struct AngleTables {
    float radians[361];
    float negativeRadians[361];
    float sine[361];
    float negativeSine[361];
    float cosine[361];
    float negativeCosine[361];

    // INLINED
    AngleTables() {
      for (int index = 0; index <= 360; ++index) {
        const float angle = (static_cast<float>(index) / 180.0f) * 3.141592f;
        radians[index] = angle;
        sine[index] = ClientSin(angle);
        cosine[index] = ClientCos(angle);
        negativeRadians[index] = -angle;
        negativeSine[index] = -sine[index];
        negativeCosine[index] = -cosine[index];
      }
    }
  };

  const AngleTables tables;

  // INLINED
  int Wrapped(int degrees) {
    return degrees > 360 || degrees < -360 ? degrees % 360 : degrees;
  }
}

// OFFSET: 0x00554420
// FUNCTION: fn_02024
float ClientRadians(int degrees) {
  const int index = Wrapped(degrees);
  return tables.radians[index >= 0 ? index : 361 + index];
}

// OFFSET: 0x00554470
// FUNCTION: fn_02025
float ClientSine(int degrees) {
  const int index = Wrapped(degrees);
  return tables.sine[index >= 0 ? index : 361 + index];
}

// OFFSET: 0x005543D0
// FUNCTION: fn_02023
float ClientCosine(int degrees) {
  const int index = Wrapped(degrees);
  return tables.cosine[index >= 0 ? index : 361 + index];
}
