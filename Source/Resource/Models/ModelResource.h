#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_MODELRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_MODELRESOURCE_H

#include "Resource/Models/ModelMesh.h"
#include "Resource/Resource.h"

#include <list>
#include <map>
#include <memory>
#include <string>

class ResourceBytes;
class TextureCache;
struct ResourceByteReader;
class C3dModelRes;

struct ModelPositionKey {
  int frame;
  float position[3];
};

struct ModelScaleKey {
  int frame;
  float scale[3];
  float rotation[4];
};

struct ModelRotationKey {
  int frame;
  float rotation[4];
};

struct ModelTextureKey {
  /* 0000 */ int frame;
  /* 0004 */ float value;
};

using ModelTextureChannels = std::map<int, std::vector<ModelTextureKey>>;

static_assert(sizeof(ModelTextureKey) == 8);

struct ModelVolume {
  float size[3];
  float position[3];
  float rotation[3];
  float corners[8][3];
  unsigned char unknown_0084[96];
  int flag;
};

class ModelNode {
public:
  explicit ModelNode(TextureCache& textures);
  ~ModelNode();
  ModelNode(const ModelNode&) = delete;
  ModelNode& operator=(const ModelNode&) = delete;

  std::string name;
  std::string parentName;
  C3dModelRes* model;
  ModelNode* parent;
  std::list<ModelNode*> children;
  std::vector<class CTexture*> textures;
  std::unique_ptr<ModelMesh> mesh;
  unsigned char alpha;
  unsigned char unknown_0055[3];
  float position[3];
  float scale[3];
  float angle;
  float axis[3];
  float transform[12];
  std::vector<ModelPositionKey> legacyPositionKeys;
  std::vector<ModelRotationKey> rotationKeys;
  std::vector<ModelRotationKey> scaleKeys;
  std::vector<ModelRotationKey> positionKeys;
  std::map<int, ModelTextureChannels> textureKeys;

private:
  TextureCache& m_textures;
};

struct ModelResourceContext {
  ResourceBytes& file;
  TextureCache& textures;
  CRenderSystemBase*& renderSystem;
};

class C3dModelRes : public CRes {
public:
  explicit C3dModelRes(ModelResourceContext& context);
  ~C3dModelRes() override;
  void Unload() override;
  ModelNode* FindNode(const char* name) const;
  void RegisterMovies(class BinkMovies& movies, const float* position) const;

  int major;
  std::list<ModelNode*> nodes;
  std::list<std::string> rootNames;
  std::map<std::string, ModelMesh*> meshes;
  std::list<ModelVolume*> volumes;
  std::string sourceName;
  int shade;
  unsigned char alpha;
  std::list<std::string> movieNames;

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
  void ReadNode(ResourceByteReader& input, unsigned minor, const std::vector<std::string>& textureNames);
};

static_assert(sizeof(ModelPositionKey) == 16);
static_assert(sizeof(ModelScaleKey) == 32);
static_assert(sizeof(ModelRotationKey) == 20);
static_assert(sizeof(ModelVolume) == 232);

#endif
