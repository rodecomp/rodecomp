#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_ANIMATEDMODELRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_ANIMATEDMODELRESOURCE_H

#include "Resource/Models/ModelResource.h"

struct AnimatedModelFace {
  /* 0000 */ std::uint16_t vertices[3];
  /* 0006 */ std::uint16_t textureVertices[3];
  /* 000C */ std::uint32_t unknown_000C;
  /* 0010 */ std::uint32_t unknown_0010;
  /* 0014 */ int smoothGroup;
};

struct AnimatedModelFrame {
  AnimatedModelFrame();
  ~AnimatedModelFrame();
  AnimatedModelFrame(const AnimatedModelFrame&) = delete;
  AnimatedModelFrame& operator=(const AnimatedModelFrame&) = delete;

  /* 0000 */ std::array<float, 3>* vertices;
  /* 0004 */ ModelTextureVertex* textureVertices;
};

struct AnimatedModelRecord {
  ~AnimatedModelRecord();

  /* 0000 */ unsigned char unknown_0000[0x84];
  /* 0084 */ std::string* unknown_0084;
};

class C3dAnimModelRes;

struct AnimatedModelNode {
  explicit AnimatedModelNode(TextureCache& textures);
  ~AnimatedModelNode();
  AnimatedModelNode(const AnimatedModelNode&) = delete;
  AnimatedModelNode& operator=(const AnimatedModelNode&) = delete;
  void Shade(const float* matrix, int frame, const float* light, const float* diffuse, const float* ambient, int shadeType, int alpha);

  /* 0000 */ std::string name;
  /* 0018 */ std::string parentName;
  /* 0030 */ C3dAnimModelRes* model;
  /* 0034 */ AnimatedModelNode* parent;
  /* 0038 */ std::list<AnimatedModelNode*> children;
  /* 0040 */ int vertexCount;
  /* 0044 */ int textureVertexCount;
  /* 0048 */ int faceCount;
  /* 004C */ int textureCount;
  /* 0050 */ std::array<float, 3>* faceNormals;
  /* 0054 */ AnimatedModelFrame* frames;
  /* 0058 */ AnimatedModelFace* faces;
  /* 005C */ CTexture** textures;
  /* 0060 */ int sharedTextureVertices;

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

  /* 0110 */ int unknown_0110;
  /* 0114 */ AnimatedModelRecord* unknown_0114;
  /* 0118 */ std::list<AnimatedModelNode*> nodes;
  /* 0120 */ std::list<std::string> rootNames;
  /* 0128 */ std::string sourceName;
  /* 0140 */ int shade;
  /* 0144 */ int frameCount;
  /* 0148 */ int animationLength;
  /* 014C */ float speed;
  /* 0150 */ std::uint32_t unknown_0150;
  /* 0154 */ float unknown_0154;
  /* 0158 */ float unknown_0158;
  /* 015C */ float unknown_015C;
  /* 0160 */ std::uint32_t alpha;

private:
  ModelResourceContext* m_context;
};

static_assert(sizeof(AnimatedModelFace) == 24);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AnimatedModelFrame) == 8);
static_assert(offsetof(AnimatedModelFrame, textureVertices) == 0x04);
static_assert(offsetof(AnimatedModelNode, vertexCount) == 0x40);
static_assert(offsetof(AnimatedModelNode, textureCount) == 0x4C);
static_assert(offsetof(AnimatedModelNode, faceNormals) == 0x50);
static_assert(offsetof(AnimatedModelNode, frames) == 0x54);
static_assert(offsetof(AnimatedModelNode, faces) == 0x58);
static_assert(offsetof(AnimatedModelNode, textures) == 0x5C);
static_assert(offsetof(AnimatedModelNode, sharedTextureVertices) == 0x60);
#endif

#endif
