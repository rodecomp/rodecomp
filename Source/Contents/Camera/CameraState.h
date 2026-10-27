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

  void Reset();
  void AddDistance(float delta);
  void AddPitch(float delta);
  void AddYaw(float delta);
  void Smooth(
    const ServiceOptions& options, const float& rendererScale, const float& projectionScale, const float& baseDistance, struct CameraProjectionOutputs& output
  );
};

struct CameraProjectionOutputs {
  float& tangent;
  float& cotangent;
  float& scaleY;
  float& scaleX;
};

struct CameraShake {
  /* 0004 */ float horizontal;
  /* 0008 */ float distance;
  /* 000C */ float pitch;
  /* 0010 */ unsigned unknown_0010;
  /* 0014 */ int enabled;
  /* 0018 */ unsigned start;
  /* 001C */ unsigned duration;

  void Apply(CameraState& camera, int& frameLatch);
  void Start(int enabled, unsigned duration, float horizontal, float distance, float pitch);
  void SetAmplitudes(float horizontal, float distance, float pitch);
};

static_assert(sizeof(CameraState) == 48);
static_assert(sizeof(CameraShake) == 28);

#endif
