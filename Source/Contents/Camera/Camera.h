#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERA_H

#include "Contents/Camera/CameraFrame.h"
#include "Contents/Camera/CameraState.h"

#include <cstddef>

struct WorldScene;

extern float g_cameraDistance;

class CView {
public:
  /* 0000 */ void* vtable;
  /* 0004 */ unsigned char unknown_0004[0x1C];
  /* 0020 */ CameraState camera;
  /* 0050 */ unsigned char unknown_0050[0x30];
  /* 0080 */ CameraFrame frame;
  /* 00F8 */ unsigned char unknown_00F8[0xB8];
  /* 01B0 */ WorldScene* scene;
  /* 01B4 */ unsigned char unknown_01B4[4];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CView, camera) == 0x0020);
static_assert(offsetof(CView, frame) == 0x0080);
static_assert(offsetof(CView, scene) == 0x01B0);
static_assert(sizeof(CView) == 0x1B8);
#endif

#endif
