#ifndef RAGNAROK_SOURCE_CONTENTS_MODELACTOR_H
#define RAGNAROK_SOURCE_CONTENTS_MODELACTOR_H

#include "Contents/ModelNode.h"
#include "Contents/ModelSpatial.h"

// RTTI C3dActor, C3dActorV1 and C3dActorV2. These are model actors, not
// CGameObject sprite actors. Runtime nodes borrow this owner and its resources.
class C3dActor {
public:
  C3dActor();
  virtual ~C3dActor();
  C3dActor(const C3dActor&) = delete;
  C3dActor& operator=(const C3dActor&) = delete;

  virtual void Assign(const C3dModelRes& resource) = 0;
  virtual void UpdateTransform();
  virtual void UpdateBounds() = 0;
  virtual void Prepare(ModelLightingContext& context) = 0;
  virtual void Animate(double frame) = 0;
  virtual void Update() = 0;
  virtual void Draw(const float* matrix, bool clip, ModelRenderContext& context) = 0;
  virtual C3dNode* Find(const char* name) = 0;
  virtual void MarkOccluded(const float* ray);
  virtual void SetSpeed(float speed);
  void CopyVolumes(const std::list<ModelVolume*>& source);

  /* 0004 */ float position[3];
  /* 0010 */ float rotation[3];
  /* 001C */ float scale[3];
  /* 0028 */ float offset[3];
  /* 0034 */ AffineMatrix transform;
  /* 0064 */ AffineMatrix inverseTransform;
  /* 0094 */ unsigned char dirty;
  unsigned char unknown_0095[3];
  /* 0098 */ OrientedBounds oriented;
  /* 00D4 */ float corners[8][3];
  /* 0134 */ float minimum[3];
  /* 0140 */ float maximum[3];
  /* 014C */ unsigned char enabled;
  unsigned char unknown_014D[3];
  /* 0150 */ unsigned renderFrame;
  /* 0154 */ unsigned char occlusionEnabled;
  /* 0155 */ unsigned char occluded;
  /* 0156 */ unsigned char alpha;
  /* 0157 */ unsigned char commandMode;
  /* 0158 */ std::uint32_t color;
  /* 015C */ std::list<ModelVolume*> volumes;
  /* 0164 */ int playback;
  /* 0168 */ float speed;
  /* 016C */ int shade;

protected:
  void FinishBounds(const float* bounds, bool modern);
  void Fade();
  bool Reversed() const;
};

class C3dActorV1 : public C3dActor {
public:
  C3dActorV1();
  ~C3dActorV1() override;
  void Assign(const C3dModelRes& resource) override;
  void UpdateBounds() override;
  void Prepare(ModelLightingContext& context) override;
  void Animate(double frame) override;
  void Update() override;
  void Draw(const float* matrix, bool clip, ModelRenderContext& context) override;
  C3dNode* Find(const char* name) override;

  /* 0170 */ C3dNodeV1* root;
  /* 0174 */ int frame;
  /* 0178 */ int animationLength;
  /* 017C */ unsigned lastTick;
};

class C3dActorV2 : public C3dActor {
public:
  C3dActorV2();
  ~C3dActorV2() override;
  void Assign(const C3dModelRes& resource) override;
  void UpdateTransform() override;
  void UpdateBounds() override;
  void Prepare(ModelLightingContext& context) override;
  void Animate(double frame) override;
  void Update() override;
  void Draw(const float* matrix, bool clip, ModelRenderContext& context) override;
  C3dNode* Find(const char* name) override;
  void SetSpeed(float speed) override;

  /* 0170 */ std::list<C3dNodeV2*> roots;
  /* 0178 */ float framesPerSecond;
  /* 017C */ unsigned char unknown_017C[4];
  /* 0180 */ double duration;
  /* 0188 */ unsigned startTick;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dActor) == 368);
static_assert(sizeof(C3dActorV1) == 384);
static_assert(sizeof(C3dActorV2) == 400);
#endif

#endif
