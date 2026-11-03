#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H

#include "Contents/Camera/CameraFrame.h"
#include "Contents/Camera/CameraState.h"
#include "Contents/World/WorldModelView.h"

#include <cstddef>

struct CWorld;
class CSkyBox;

extern float g_cameraDistance;
extern float g_cameraMinDistance;
extern float g_cameraBaseDistance;
extern float g_projectionScale;
extern float g_cameraPitch;
extern float g_indoorCameraPitch;
extern float g_indoorCameraDistance;
extern float g_outdoorCameraPitch;
extern float g_outdoorCameraDistance;

// VTABLE: vt_01610
class CView {
public:
  CView();
  virtual ~CView();
  virtual void Reset();
  virtual void Slot2();
  virtual void Render();
  virtual void Update();

  /* 0004 */ CameraShake shake;
  /* 0020 */ CameraState camera;
  /* 0050 */ float unknown_0050[3];
  /* 005C */ unsigned char unknown_005C[0xC];
  /* 0068 */ float unknown_0068[3];
  /* 0074 */ unsigned char unknown_0074[0xC];
  /* 0080 */ CameraFrame frame;
  /* 00F8 */ WorldViewSpatial spatial;
  /* 01B0 */ CWorld* scene;
  /* 01B4 */ CSkyBox* unknown_01B4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CView, camera) == 0x0020);
static_assert(offsetof(CView, frame) == 0x0080);
static_assert(offsetof(CView, spatial) == 0x00F8);
static_assert(offsetof(CView, scene) == 0x01B0);
static_assert(sizeof(CView) == 0x1B8);
#endif

#endif
