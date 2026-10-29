#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERAFRAME_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERAFRAME_H

#include "Contents/Camera/CameraState.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstddef>

struct TerrainHeight;
class CRenderer;

struct CameraFrame {
  /* 0080 */ float eye[3];
  /* 008C */ float up[3];
  /* 0098 */ matrix view;
  /* 00C8 */ matrix inverse;

  void Build(const CameraState& camera, const TerrainHeight& terrain);
  void Refresh(const CameraState& camera, const TerrainHeight& terrain, CRenderer& renderer);
  void Forward(const CameraState& camera, float* output) const;
  void AlternateView(const CameraState& camera, float yaw, const TerrainHeight& terrain, matrix& output) const;
};

void ResetCamera(CameraState& camera, CameraFrame& frame);

void UpdateCameraFrame(
  CameraState& camera, CameraShake& shake, int& frameLatch, const float* desiredTarget, const ServiceOptions& options, const float& rendererScale,
  const float& projectionScale, const float& baseDistance, CameraProjectionOutputs& outputs, const TerrainHeight& terrain, CameraFrame& frame,
  CRenderer& renderer
);

static_assert(sizeof(CameraFrame) == 120);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CameraFrame, up) + 0x80 == 0x8C);
static_assert(offsetof(CameraFrame, view) + 0x80 == 0x98);
static_assert(offsetof(CameraFrame, inverse) + 0x80 == 0xC8);
#endif

#endif
