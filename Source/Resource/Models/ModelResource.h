#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_MODELRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_MODELRESOURCE_H

#include "Resource/Models/ModelMesh.h"
#include "Resource/Resource.h"

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

class ResourceBytes;
class TextureCache;
struct JobNames;
struct ResourceByteReader;
class C3dModelRes;

struct ModelPositionKey {
  /* 0000 */ int frame;
  /* 0004 */ float position[3];
};

struct ModelScaleKey {
  /* 0000 */ int frame;
  /* 0004 */ float scale[3];
  /* 0010 */ float rotation[4];
};

struct ModelRotationKey {
  /* 0000 */ int frame;
  /* 0004 */ float rotation[4];
};

struct ModelTextureKey {
  /* 0000 */ int frame;
  /* 0004 */ float value;
};

using ModelTextureChannels = std::map<int, std::vector<ModelTextureKey>>;

static_assert(sizeof(ModelTextureKey) == 8);

struct ModelVolume {
  /* 0000 */ float size[3];
  /* 000C */ float position[3];
  /* 0018 */ float rotation[3];
  /* 0024 */ float corners[8][3];
  /* 0084 */ unsigned char unknown_0084[96];
  /* 00E4 */ int flag;
};

class ModelNode {
public:
  ModelNode();
  virtual ~ModelNode();
  ModelNode(const ModelNode&) = delete;
  ModelNode& operator=(const ModelNode&) = delete;

  /* 0004 */ std::string name;
  /* 001C */ std::string parentName;
  /* 0034 */ C3dModelRes* model;
  /* 0038 */ ModelNode* parent;
  /* 003C */ std::list<ModelNode*> children;
  /* 0044 */ std::vector<class CTexture*> textures;
  /* 0050 */ std::unique_ptr<ModelMesh> mesh;
  /* 0054 */ unsigned char alpha;
};

class ModelNodeV1 : public ModelNode {
public:
  ModelNodeV1();
  ~ModelNodeV1() override;

  /* 0058 */ float position[3];
  /* 0064 */ float axis[3];
  /* 0070 */ float angle;
  /* 0074 */ float scale[3];
  /* 0080 */ std::vector<ModelPositionKey> legacyPositionKeys;
  /* 008C */ std::vector<ModelRotationKey> rotationKeys;
};

class ModelNodeV2 : public ModelNode {
public:
  ModelNodeV2();
  ~ModelNodeV2() override;

  /* 0058 */ float transform[12];
  /* 0088 */ std::vector<ModelRotationKey> scaleKeys;
  /* 0094 */ std::vector<ModelRotationKey> rotationKeys;
  /* 00A0 */ std::vector<ModelRotationKey> positionKeys;
  /* 00AC */ std::map<int, ModelTextureChannels> textureKeys;
};

struct ModelResourceContext {
  ResourceBytes& file;
  TextureCache& textures;
  CRenderSystemBase*& renderSystem;
  JobNames& jobNames;
};

class C3dModelRes : public CRes {
  friend class ModelNode;
public:
  explicit C3dModelRes(ModelResourceContext& context);
  ~C3dModelRes() override;
  bool Load(const char* name) override;
  void Unload() override;
  ModelNode* FindNode(const char* name) const;
  void ForEachRootName(std::function<void(const std::string&)> callback) const;
  void RegisterMovies(class BinkMovies& movies, const float* position) const;

  /* 0110 */ int major;
  /* 0114 */ std::list<ModelNode*> nodes;
  /* 011C */ std::list<std::string> rootNames;
  /* 0124 */ std::map<std::string, ModelMesh*> meshes;
  /* 012C */ std::list<ModelVolume*> volumes;
  /* 0134 */ std::string sourceName;
  /* 014C */ int shade;
  /* 0150 */ unsigned char alpha;
  /* 0154 */ std::list<std::string> movieNames;

protected:
  ModelResourceContext* m_context;
};

class C3dModelResV1 : public C3dModelRes {
public:
  explicit C3dModelResV1(ModelResourceContext& context);
  ~C3dModelResV1() override;
  C3dModelResV1* Create() override;
  bool Load(const char* name) override;
  int animationLength;

private:
  void ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames);
};

class C3dModelResV2 : public C3dModelRes {
public:
  explicit C3dModelResV2(ModelResourceContext& context);
  ~C3dModelResV2() override;
  C3dModelResV2* Create() override;
  bool Load(const char* name) override;

  float framesPerSecond;
  int animationLength;

private:
  void ReadVersion(ResourceByteReader& input, unsigned minor);
  void ReadVersion1(ResourceByteReader& input);
  void ReadVersion2(ResourceByteReader& input);
  void ReadVersion3(ResourceByteReader& input);
};

static_assert(sizeof(ModelPositionKey) == 16);
static_assert(sizeof(ModelScaleKey) == 32);
static_assert(sizeof(ModelRotationKey) == 20);
static_assert(sizeof(ModelVolume) == 232);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ModelNode) == 0x58);
static_assert(sizeof(ModelNodeV1) == 0x98);
static_assert(sizeof(ModelNodeV2) == 0xB4);
#endif

#endif
