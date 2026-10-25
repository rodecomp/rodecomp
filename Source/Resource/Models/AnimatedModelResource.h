#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_ANIMATEDMODELRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_ANIMATEDMODELRESOURCE_H

#include "Resource/Models/ModelResource.h"

struct AnimatedModelFace {
  std::uint32_t values[6];
};

struct AnimatedModelFrame {
  std::vector<std::array<float, 3>> vertices;
  std::vector<ModelTextureVertex> textureVertices;
};

class C3dAnimModelRes;

struct AnimatedModelNode {
  explicit AnimatedModelNode(TextureCache& textures);
  ~AnimatedModelNode();
  AnimatedModelNode(const AnimatedModelNode&) = delete;
  AnimatedModelNode& operator=(const AnimatedModelNode&) = delete;

  std::string name;
  std::string parentName;
  C3dAnimModelRes* model;
  AnimatedModelNode* parent;
  std::list<AnimatedModelNode*> children;
  std::uint32_t unknown_0040;
  int vertexCount;
  int textureVertexCount;
  int faceCount;
  std::vector<AnimatedModelFace> faces;
  std::vector<std::array<float, 3>> faceNormals;
  std::vector<AnimatedModelFrame> frames;
  std::vector<CTexture*> textures;
  int sharedTextureVertices;

private:
  TextureCache& m_textures;
};

class C3dAnimModelRes : public CRes {
public:
  explicit C3dAnimModelRes(ModelResourceContext& context);
  ~C3dAnimModelRes() override;
  C3dAnimModelRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  std::list<AnimatedModelNode*> nodes;
  std::list<std::string> rootNames;
  std::string sourceName;
  int shade;
  int frameCount;
  int animationLength;
  float speed;
  std::uint32_t unknown_0150;
  float unknown_0154;
  float unknown_0158;
  float unknown_015C;
  std::uint32_t alpha;

private:
  ModelResourceContext* m_context;
};

static_assert(sizeof(AnimatedModelFace) == 24);

#endif
