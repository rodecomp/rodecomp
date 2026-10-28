#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELNODE_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELNODE_H

#include "Contents/Models/ModelAnimation.h"
#include "Contents/Models/ModelRendering.h"

#include <array>

class C3dActor;

struct ModelTextureTransform {
  /* 0000 */ float values[5] = {0, 0, 1, 1, 0};
};

void SampleModelTexture(const ModelTextureChannels& channels, float frame, ModelTextureTransform& output);

class C3dNode {
public:
  C3dNode();
  virtual ~C3dNode();
  C3dNode(const C3dNode&) = delete;
  C3dNode& operator=(const C3dNode&) = delete;

  virtual void Initialize(const ModelNode& resource);
  virtual void Animate(double frame);
  virtual void Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context);
  C3dNode* Find(const char* name);
  void OverrideColors();
  void Prepare(const float* matrix, ModelLightingContext& context);
  void SampleGroundColors(const float* matrix, const float* position, const class GroundGeometry& ground);
  void AccumulateBounds(float* bounds, const float* matrix);

  /* 0004 */ std::string name;
  /* 001C */ C3dActor* model;
  /* 0020 */ C3dNode* parent;
  /* 0024 */ std::list<C3dNode*> children;
  /* 002C */ ModelMesh* mesh;
  /* 0030 */ AffineMatrix transform;
  /* 0060 */ std::vector<CTexture*> textures;
  /* 006C */ float alpha;
  /* 0070 */ std::vector<ModelFaceState> faceState;
  /* 007C */ std::vector<int> faceValues;
  /* 0088 */ unsigned char unknown_0088;
};

class C3dNodeV1 : public C3dNode {
public:
  C3dNodeV1();
  ~C3dNodeV1() override;
  void Initialize(const ModelNode& resource) override;
  void Animate(double frame) override;
  void Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context) override;

  /* 008C */ std::vector<ModelScaleKey> scaleKeys;
  /* 0098 */ std::vector<ModelRotationKey> rotationKeys;
  /* 00A4 */ std::vector<ModelPositionKey> positionKeys;
};

class C3dNodeV2 : public C3dNode {
public:
  C3dNodeV2();
  ~C3dNodeV2() override;
  void Initialize(const ModelNode& resource) override;
  void Animate(double frame) override;
  void Draw(const float* matrix, unsigned char reversed, bool clip, unsigned char commandMode, ModelRenderContext& context) override;

  /* 008C */ AffineMatrix resourceTransform;
  /* 00BC */ AffineMatrix localTransform;
  /* 00EC */ std::vector<ModelRotationKey> scaleKeys;
  /* 00F8 */ std::vector<ModelRotationKey> rotationKeys;
  /* 0104 */ std::vector<ModelRotationKey> positionKeys;
  /* 0110 */ std::map<int, ModelTextureChannels> textureKeys;
  /* 0118 */ std::map<int, ModelTextureTransform> textureTransforms;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dNode) == 140);
static_assert(sizeof(C3dNodeV1) == 176);
static_assert(sizeof(C3dNodeV2) == 288);
static_assert(sizeof(ModelTextureTransform) == 20);
static_assert(offsetof(C3dNode, mesh) == 0x2C);
static_assert(offsetof(C3dNode, transform) == 0x30);
static_assert(offsetof(C3dNode, faceState) == 0x70);
static_assert(offsetof(C3dNodeV1, scaleKeys) == 0x8C);
static_assert(offsetof(C3dNodeV2, resourceTransform) == 0x8C);
static_assert(offsetof(C3dNodeV2, textureTransforms) == 0x118);
#endif

#endif
