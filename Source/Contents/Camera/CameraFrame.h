#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERAFRAME_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERAFRAME_H

#include "Contents/Camera/CameraState.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstddef>
#include "Utility/Math/Vector3d.h"

class CRenderer;

struct CameraFrame {
  /* 0080 */ vector3d eye{0.0f, 0.0f, 0.0f};
  /* 008C */ vector3d up{0.0f, 0.0f, 0.0f};
  /* 0098 */ matrix view;
  /* 00C8 */ matrix inverse;

  void Forward(const CameraState& camera, float* output) const;
};


static_assert(sizeof(CameraFrame) == 120);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CameraFrame, up) + 0x80 == 0x8C);
static_assert(offsetof(CameraFrame, view) + 0x80 == 0x98);
static_assert(offsetof(CameraFrame, inverse) + 0x80 == 0xC8);
#endif

#endif
