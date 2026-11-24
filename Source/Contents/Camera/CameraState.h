#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERASTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERASTATE_H

#include "Utility/Math/Vector3d.h"
struct ServiceOptions;

struct CameraState {
  /* 0020 */ vector3d target{0.0f, 0.0f, 0.0f};
  /* 002C */ float pitch;
  /* 0030 */ float yaw;
  /* 0034 */ float distance;
  /* 0038 */ vector3d desiredTarget{0.0f, 0.0f, 0.0f};
  /* 0044 */ float desiredPitch;
  /* 0048 */ float desiredYaw;
  /* 004C */ float desiredDistance;

  // INLINED
  void Reset() {
    this->desiredYaw = this->yaw = 0;
    this->desiredDistance = this->distance = 300;
    this->desiredPitch = this->pitch = -45;

    this->desiredTarget = this->target = vector3d{0, 0, 0};
  }
};

struct CameraShake {
  /* 0004 */ float horizontal;
  /* 0008 */ float distance;
  /* 000C */ float pitch;
  /* 0010 */ unsigned resetFlags;
  /* 0014 */ int enabled;
  /* 0018 */ unsigned start;
  /* 001C */ unsigned duration;
};

static_assert(sizeof(CameraState) == 48);
static_assert(sizeof(CameraShake) == 28);

#endif
