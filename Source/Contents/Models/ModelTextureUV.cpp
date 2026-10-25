#include "Contents/Models/ModelNode.h"

namespace {
  float Rotated(float sine, float delta, float cosineProduct, bool subtract) {
    const double product = static_cast<double>(sine) * delta;
    return static_cast<float>(subtract ? cosineProduct - product : product + cosineProduct);
  }
}

// ORIGINAL_SITE: 0x00AB8856 in fn_17500 (fragment)
void TransformModelUV(float& u, float& v, const ModelTextureTransform& transform) {
  const float cosineU = ClientCos(transform.values[4]);
  const float productU = cosineU * (u - 0.5f);
  const float sineU = ClientSin(transform.values[4]);
  const float rotatedU = Rotated(sineU, v - 0.5f, productU, true);
  const float resultU = (rotatedU * transform.values[2] + 0.5f) + transform.values[0];
  const float cosineV = ClientCos(transform.values[4]);
  const float productV = cosineV * (v - 0.5f);
  const float sineV = ClientSin(transform.values[4]);
  const float rotatedV = Rotated(sineV, u - 0.5f, productV, false);
  const float resultV = (rotatedV * transform.values[3] + 0.5f) + transform.values[1];

  u = resultU;
  v = resultV;
}
