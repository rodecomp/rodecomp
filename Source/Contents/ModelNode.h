#ifndef RAGNAROK_SOURCE_CONTENTS_MODELNODE_H
#define RAGNAROK_SOURCE_CONTENTS_MODELNODE_H

#include "Contents/ModelAnimation.h"
#include "Contents/ModelRendering.h"

#include <array>

class C3dActor;

struct ModelTextureTransform {
  float values[5] = {0, 0, 1, 1, 0};
};

using ModelTextureChannels = std::map<int, std::vector<std::array<std::uint32_t, 2>>>;
void SampleModelTexture(const ModelTextureChannels& channels, float frame, ModelTextureTransform& output);

// Runtime C3dNode hierarchy. Meshes/textures and the owning model are borrowed;
// children are owned here. Resource nodes are copied at initialization, not kept
// as alternate live animation state. Rendering remains a separate node operation.
class C3dNode {
public:
  C3dNode();
  virtual ~C3dNode();
  C3dNode(const C3dNode&) = delete;
  C3dNode& operator=(const C3dNode&) = delete;

  virtual void Initialize(const ModelNode& resource) = 0;
  virtual void Animate(double frame) = 0;
  virtual void Draw(
    const float* matrix,
    unsigned char reversed,
    bool clip,
    unsigned char commandMode,
    ModelRenderContext& context
  ) = 0;
  C3dNode* Find(const char* name);
  void OverrideColors();
  void Prepare(const float* matrix, ModelLightingContext& context);
  void SampleGroundColors(const float* matrix, const float* position, const class GroundGeometry& ground);
  void AccumulateBounds(float* bounds, const float* matrix);

  std::string name;
  C3dActor* model;
  C3dNode* parent;
  std::list<C3dNode*> children;
  ModelMesh* mesh;
  AffineMatrix transform;
  std::vector<CTexture*> textures;
  float alpha;
  std::vector<ModelFaceState> faceState;
  std::vector<int> faceValues;
  unsigned char unknown_0088;
};

class C3dNodeV1 : public C3dNode {
public:
  C3dNodeV1();
  ~C3dNodeV1() override;
  void Initialize(const ModelNode& resource) override;
  void Animate(double frame) override;
  void Draw(
    const float* matrix,
    unsigned char reversed,
    bool clip,
    unsigned char commandMode,
    ModelRenderContext& context
  ) override;

  std::vector<ModelScaleKey> scaleKeys;
  std::vector<ModelRotationKey> rotationKeys;
  std::vector<ModelPositionKey> positionKeys;
};

class C3dNodeV2 : public C3dNode {
public:
  C3dNodeV2();
  ~C3dNodeV2() override;
  void Initialize(const ModelNode& resource) override;
  void Animate(double frame) override;
  void Draw(
    const float* matrix,
    unsigned char reversed,
    bool clip,
    unsigned char commandMode,
    ModelRenderContext& context
  ) override;

  AffineMatrix resourceTransform;
  AffineMatrix localTransform;
  std::vector<ModelRotationKey> scaleKeys;
  std::vector<ModelRotationKey> rotationKeys;
  std::vector<ModelRotationKey> positionKeys;
  std::map<int, ModelTextureChannels> textureKeys;
  std::map<int, ModelTextureTransform> textureTransforms;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(C3dNode) == 140);
static_assert(sizeof(C3dNodeV1) == 176);
static_assert(sizeof(C3dNodeV2) == 288);
#endif

#endif
