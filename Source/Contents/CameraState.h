#ifndef RAGNAROK_SOURCE_CONTENTS_CAMERASTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CAMERASTATE_H

struct ServiceOptions;

// Established CView +0x20..+0x4F state. Reset changes only original fields.
struct CameraState {
  float target[3];
  float pitch;
  float yaw;
  float distance;
  float desiredTarget[3];
  float desiredPitch;
  float desiredYaw;
  float desiredDistance;

  void Reset();
  void AddDistance(float delta);
  void AddPitch(float delta);
  void AddYaw(float delta);
  void Smooth(
    const ServiceOptions& options,
    const float& rendererScale,
    const float& projectionScale,
    const float& baseDistance,
    struct CameraProjectionOutputs& output
  );
};

struct CameraProjectionOutputs {
  float& tangent;
  float& cotangent;
  float& scaleY;
  float& scaleX;
};

// CView +0x04..+0x1F. Time comparison uses unsigned start+duration directly.
struct CameraShake {
  float horizontal;
  float distance;
  float pitch;
  unsigned unknown_0010;
  int enabled;
  unsigned start;
  unsigned duration;

  void Apply(CameraState& camera, int& frameLatch);
  void Start(int enabled, unsigned duration, float horizontal, float distance, float pitch);
  void SetAmplitudes(float horizontal, float distance, float pitch);
};

static_assert(sizeof(CameraState) == 48);

#endif
