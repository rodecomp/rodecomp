#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H

#include "Contents/Camera/CameraFrame.h"
#include "Contents/Camera/CameraState.h"
#include "Contents/World/WorldModelView.h"

#include <cstddef>

struct CWorld;

extern float g_cameraDistance;
extern float g_cameraMinDistance;
extern float g_cameraBaseDistance;
extern float g_cameraPitch;
extern float g_indoorCameraPitch;
extern float g_indoorCameraDistance;
extern float g_outdoorCameraPitch;
extern float g_outdoorCameraDistance;

class CView {
public:
  CView();

  /* 0000 */ void* vtable;
  /* 0004 */ unsigned char unknown_0004[0xC];
  /* 0010 */ unsigned char unknown_0010;
  /* 0011 */ unsigned char unknown_0011[3];
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ unsigned char unknown_001C[4];
  /* 0020 */ CameraState camera;
  /* 0050 */ float unknown_0050[3];
  /* 005C */ unsigned char unknown_005C[0xC];
  /* 0068 */ float unknown_0068[3];
  /* 0074 */ unsigned char unknown_0074[0xC];
  /* 0080 */ CameraFrame frame;
  /* 00F8 */ WorldViewSpatial spatial;
  /* 01B0 */ CWorld* scene;
  /* 01B4 */ void* unknown_01B4;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CView, camera) == 0x0020);
static_assert(offsetof(CView, frame) == 0x0080);
static_assert(offsetof(CView, spatial) == 0x00F8);
static_assert(offsetof(CView, scene) == 0x01B0);
static_assert(sizeof(CView) == 0x1B8);
#endif

#endif
