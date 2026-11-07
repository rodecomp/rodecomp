#ifndef RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDRENDERING_H
#define RAGNAROK_SOURCE_CONTENTS_GROUND_GROUNDRENDERING_H

#include "Render/Renderer.h"

// INLINED
inline const float& Minimum4(const float& a, const float& b, const float& c, const float& d) {
  const float& first = !(a > b) ? a : b;
  const float& second = !(c > d) ? c : d;
  return first > second ? second : first;
}

// INLINED
inline const float& Maximum4(const float& a, const float& b, const float& c, const float& d) {
  const float& first = !(b > a) ? a : b;
  const float& second = !(d > c) ? c : d;
  return second > first ? second : first;
}

// INLINED
__forceinline bool GroundQuadVisible(const RenderVertex* vertices) {
  const float minimumX = Minimum4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x);

  if (minimumX >= static_cast<float>(g_renderer->viewport.width)) {
    return false;
  }

  const float maximumX = Maximum4(vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x);

  if (0.0f > maximumX) {
    return false;
  }

  const float minimumY = Minimum4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y);

  if (minimumY >= static_cast<float>(g_renderer->viewport.height)) {
    return false;
  }

  const float maximumY = Maximum4(vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y);

  if (0.0f > maximumY) {
    return false;
  }

  for (unsigned corner = 0; corner < 4; ++corner) {
    if (0.0f > vertices[corner].rhw) {
      return false;
    }
  }

  return true;
}

#endif
