#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERASTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERA_CAMERASTATE_H

struct ServiceOptions;

struct CameraState {
  /* 0020 */ float target[3];
  /* 002C */ float pitch;
  /* 0030 */ float yaw;
  /* 0034 */ float distance;
  /* 0038 */ float desiredTarget[3];
  /* 0044 */ float desiredPitch;
  /* 0048 */ float desiredYaw;
  /* 004C */ float desiredDistance;

  // INLINED
  void Reset() {
    this->yaw = this->desiredYaw = 0;
    this->distance = this->desiredDistance = 300;
    this->pitch = this->desiredPitch = -45;

    for (unsigned axis = 0; axis < 3; ++axis) {
      this->target[axis] = this->desiredTarget[axis] = 0;
    }
  }
  void AddDistance(float delta);
  void AddPitch(float delta);
  void AddYaw(float delta);
  void Smooth();
};

struct CameraShake {
  /* 0004 */ float horizontal;
  /* 0008 */ float distance;
  /* 000C */ float pitch;
  /* 0010 */ unsigned unknown_0010;
  /* 0014 */ int enabled;
  /* 0018 */ unsigned start;
  /* 001C */ unsigned duration;
};

static_assert(sizeof(CameraState) == 48);
static_assert(sizeof(CameraShake) == 28);

#endif
