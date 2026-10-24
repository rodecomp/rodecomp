#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERAFRAME_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERAFRAME_H

#include "Contents/CameraState.h"
#include "Utility/AffineMatrix.h"

struct TerrainHeight;
class CRenderer;

// CView camera outputs +0x80..+0xF7. up is established by reset, not rewritten
// on each frame. These matrices feed projection, picking and frustum collection.
struct CameraFrame {
  float eye[3];
  float up[3];
  AffineMatrix view;
  AffineMatrix inverse;

  void Build(const CameraState& camera, const TerrainHeight& terrain);
  void Refresh(const CameraState& camera, const TerrainHeight& terrain, CRenderer& renderer);
  void Forward(const CameraState& camera, float* output) const;
  void AlternateView(const CameraState& camera, float yaw, const TerrainHeight& terrain, AffineMatrix& output) const;
};

void ResetCamera(CameraState& camera, CameraFrame& frame);

void UpdateCameraFrame(
  CameraState& camera,
  CameraShake& shake,
  int& frameLatch,
  const float* desiredTarget,
  const ServiceOptions& options,
  const float& rendererScale,
  const float& projectionScale,
  const float& baseDistance,
  CameraProjectionOutputs& outputs,
  const TerrainHeight& terrain,
  CameraFrame& frame,
  CRenderer& renderer
);

static_assert(sizeof(CameraFrame) == 120);

#endif
