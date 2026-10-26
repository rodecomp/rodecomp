#include "Contents/Actors/RenderObject.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// FUNCTION: fn_21007
float __stdcall CRenderObject::Heading(float x, float z, float targetX, float targetZ) {
  const float deltaX = targetX - x;
  const float deltaZ = targetZ - z;
  float angle = (static_cast<float>(std::atan2(static_cast<double>(deltaX), static_cast<double>(-deltaZ))) / 3.141592f) * 180.0f;
  if (angle >= 360.0f) {
    angle -= 360.0f;
  }

  if (angle < 0.0f) {
    angle += 360.0f;
  }

  return angle;
}

// FUNCTION: fn_21008
void CRenderObject::Face(float x, float z) {
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
  if (deltaX != 0.0f || deltaZ != 0.0f) {
    float angle = (static_cast<float>(std::atan2(static_cast<double>(deltaX), static_cast<double>(-deltaZ))) / 3.141592f) * 180.0f;
    this->render.angle = angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
      this->render.angle = angle;
    }

    if (angle < 0.0f) {
      this->render.angle = angle + 360.0f;
    }
  }
}

// FUNCTION: fn_21009
float CRenderObject::HorizontalDistance(float x, float z) const {
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
  return SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
}

// FUNCTION: fn_21010
float CRenderObject::Distance(float x, float y, float z) const {
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
  const float deltaY = y - this->render.position[1];
  const float horizontal = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  return SquareRoot(horizontal * horizontal + deltaY * deltaY);
}
