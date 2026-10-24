#ifndef RAGNAROK_SOURCE_UTILITY_RAYTRIANGLE_H
#define RAGNAROK_SOURCE_UTILITY_RAYTRIANGLE_H

// Six floats: origin xyz followed by direction xyz. Outputs are written only
// as each original rejection stage is passed; signed distance is not clipped.
bool IntersectRayTriangle(
  const float* ray,
  const float* first,
  const float* second,
  const float* third,
  float& u,
  float& v,
  float& distance
);

#endif
