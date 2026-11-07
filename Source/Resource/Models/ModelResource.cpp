#include "Client/Application/ErrorDialog.h"
#include "Client/Session.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Models/AnimatedModelResource.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <list>
#include <string>

// GLOBAL: gl_24708
char g_modelPath[MAX_PATH];
// GLOBAL: gl_24734
char g_animationPath[MAX_PATH];
// GLOBAL: gl_24707
unsigned g_animationJobs[10] = {1288, 722, 1324, 1000, 1000, 1000, 1000, 1286, 1285, 1287};
// GLOBAL: gl_24706
const char* g_animationNames[5] = {"stand", "move", "attack", "dead", "damage"};

struct ShadeVector {
  ShadeVector() : x(0.0f), y(0.0f), z(0.0f) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

static_assert(sizeof(ShadeVector) == 12);

// OFFSET: 0x00756D60
// FUNCTION: fn_07997
AnimatedModelRecord::~AnimatedModelRecord() {
  delete[] this->unknown_0084;
}

// OFFSET: 0x00755630
// FUNCTION: fn_07985
ModelNode::ModelNode() : model(nullptr), parent(nullptr), alpha(255) {
}

// OFFSET: 0x007568E0
// FUNCTION: fn_07996
ModelNode::~ModelNode() {
  for (std::vector<CTexture*>::iterator it = this->textures.begin(); it != this->textures.end(); ++it) {
    if (*it) {
      (*it)->Release();
      g_texMgr.Remove(*it);
    }
  }

  this->textures.clear();
  this->mesh.reset();
  this->children.clear();
}

// INLINED
__forceinline ModelNodeV1::ModelNodeV1() = default;

// INLINED
ModelNodeV1::~ModelNodeV1() = default;

// INLINED
ModelNodeV2::ModelNodeV2() = default;

// INLINED
ModelNodeV2::~ModelNodeV2() = default;

// OFFSET: 0x007553E0
// FUNCTION: fn_07982
__declspec(noinline) C3dModelRes::C3dModelRes() : major(0), shade(0), alpha(255) {
}

// OFFSET: 0x007567B0
// FUNCTION: fn_07995
C3dModelRes::~C3dModelRes() = default;

// OFFSET: 0x00755560
// FUNCTION: fn_07983
C3dModelResV1::C3dModelResV1() : animationLength(0) {
}

// INLINED
C3dModelResV1::~C3dModelResV1() = default;

// OFFSET: 0x00750A70
// FUNCTION: fn_07949
C3dModelResV1* C3dModelResV1::Create() {
  return new C3dModelResV1;
}

// OFFSET: 0x007593C0
// FUNCTION: fn_08012
__declspec(noinline) ModelNode* C3dModelRes::FindNode(const char* name) const {
  for (std::list<ModelNode*>::const_iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
    if ((*it)->name.compare(name) == 0) {
      return *it;
    }
  }

  return nullptr;
}

// OFFSET: 0x00750950
// FUNCTION: fn_07948
void C3dModelRes::Unload() {
  for (ModelNode* node : this->nodes) {
    delete node;
  }

  this->nodes.clear();
  this->rootNames.clear();
  this->meshes.clear();

  for (ModelVolume* volume : this->volumes) {
    delete volume;
  }

  this->volumes.clear();
  this->movieNames.clear();
}

// INLINED
__forceinline void C3dModelResV1::ReadNode(CFile& file, unsigned minor, const std::vector<std::string>& textureNames) {
  ModelNodeV1* node = new ModelNodeV1;
  node->model = this;
  node->alpha = this->alpha;
  char field[40]{};
  file.Read(field, sizeof(field));
  node->name = field;
  file.Read(field, sizeof(field));
  node->parentName = field;
  int textureCount = 0;
  file.Read(&textureCount, 4);

  for (int index = 0; index < textureCount; ++index) {
    int textureIndex = 0;
    file.Read(&textureIndex, 4);
    CTexture* texture = g_texMgr.Load(textureNames[textureIndex].c_str(), 0, false, true, false);
    node->textures.push_back(texture);

    if (texture) {
      texture->Retain();
    }
  }

  float matrix[12];
  file.Read(matrix, sizeof(matrix));
  file.Read(node->position, sizeof(node->position));
  file.Read(&node->angle, 4);
  file.Read(node->axis, sizeof(node->axis));
  file.Read(node->scale, sizeof(node->scale));
  node->mesh = std::make_unique<ModelMesh>();
  ModelMesh& mesh = *node->mesh;
  int vertexCount = 0;
  file.Read(&vertexCount, 4);
  mesh.vertexCount = vertexCount;
  auto* vertices = new std::array<float, 3>[vertexCount]{};
  file.Read(vertices, static_cast<unsigned>(vertexCount) * 12u);
  mesh.vertices = vertices;

  for (int index = 0; index < vertexCount; ++index) {
    auto& vertex = vertices[index];
    const float y = ((vertex[0] * matrix[1] + vertex[1] * matrix[4]) + matrix[7] * vertex[2]) + matrix[10];
    const float z = ((vertex[0] * matrix[2] + vertex[1] * matrix[5]) + matrix[8] * vertex[2]) + matrix[11];
    vertex[0] = ((vertex[0] * matrix[0] + vertex[1] * matrix[3]) + vertex[2] * matrix[6]) + matrix[9];
    vertex[1] = y;
    vertex[2] = z;
  }

  int textureVertexCount = 0;
  file.Read(&textureVertexCount, 4);
  mesh.textureVertexCount = textureVertexCount;
  ModelTextureVertex* textureVertices = new ModelTextureVertex[textureVertexCount];

  if (minor == 1) {
    for (int index = 0; index < textureVertexCount; ++index) {
      float uv[2];
      file.Read(uv, sizeof(uv));
      textureVertices[index] = {0xFFFFFFFFu, uv[0], uv[1]};
    }
  } else {
    file.Read(textureVertices, static_cast<unsigned>(textureVertexCount) * 12u);
  }

  mesh.textureVertices = textureVertices;
  int faceCount = 0;
  file.Read(&faceCount, 4);
  mesh.faceCount = faceCount;
  ModelFace* faces = new ModelFace[faceCount];

  if (minor == 1) {
    for (int index = 0; index < faceCount; ++index) {
      ModelFace& face = faces[index];
      unsigned char record[20];
      file.Read(record, sizeof(record));
      std::memcpy(face.vertices, record, sizeof(face.vertices));
      std::memcpy(face.textureVertices, record + 6, sizeof(face.textureVertices));
      std::memcpy(&face.texture, record + 12, sizeof(face.texture));
      std::memcpy(&face.twoSided, record + 16, sizeof(face.twoSided));

      if (face.smoothing.empty()) {
        face.smoothing.push_back(0);
      }
    }
  } else {
    for (int index = 0; index < faceCount; ++index) {
      ModelFace& face = faces[index];
      unsigned char record[24];
      file.Read(record, sizeof(record));
      std::memcpy(face.vertices, record, sizeof(face.vertices));
      std::memcpy(face.textureVertices, record + 6, sizeof(face.textureVertices));
      std::memcpy(&face.texture, record + 12, sizeof(face.texture));
      std::memcpy(&face.twoSided, record + 16, sizeof(face.twoSided));
      std::uint32_t group;
      std::memcpy(&group, record + 20, sizeof(group));

      if (face.smoothing.empty()) {
        face.smoothing.push_back(group);
      }
    }
  }

  mesh.faces = faces;
  // INLINED
  std::sort(mesh.faces, mesh.faces + mesh.faceCount, [](const ModelFace& a, const ModelFace& b) { return a.texture < b.texture; });
  mesh.CalculateNormals();
  int rotationCount;
  file.Read(&rotationCount, 4);

  for (int index = 0; index < rotationCount; ++index) {
    ModelRotationKey key;
    file.Read(&key, sizeof(key));
    node->rotationKeys.push_back(key);
  }

  this->nodes.push_back(node);
  this->meshes[node->name] = node->mesh.get();
}

// OFFSET: 0x00750B10
// FUNCTION: fn_07950
bool C3dModelResV1::Load(const char* name) {
  CFile file;

  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  char magic[5]{};
  file.Read(magic, 4);

  if (std::strcmp(magic, "GRSM") != 0) {
    constexpr char message[] = "모델파일이 아닙니다. 형식을 다시한번 확인해주세요.";
    ShowErrorDialog(GetRenderSystemHolder()->system, message, 0);
    return false;
  }

  unsigned char major = 0;
  file.Read(&major, 1);
  this->major = major;
  unsigned char minor = 0;
  file.Read(&minor, 1);

  if (major != 1 || minor > 5) {
    constexpr char message[] = "지원되지 않는 버젼입니다. 내용물이 깨져있을 수 있습니다";
    ShowErrorDialog(GetRenderSystemHolder()->system, message, 0);
    return false;
  }

  file.Read(&this->animationLength, 4);
  file.Read(&this->shade, 4);

  if (minor < 4) {
    this->alpha = 255;
  } else {
    file.Read(&this->alpha, 1);
  }

  unsigned char reserved[16]{};
  file.Read(reserved, sizeof(reserved));
  std::vector<std::string> textureNames;
  int textureCount = 0;
  file.Read(&textureCount, 4);
  char textureName[40]{};

  for (int index = 0; index < textureCount; ++index) {
    file.Read(textureName, sizeof(textureName));
    textureNames.push_back(textureName);

    if (std::strstr(textureName, ".bik")) {
      this->movieNames.push_back(textureName);
    }
  }

  char rootName[40]{};
  file.Read(rootName, sizeof(rootName));
  int nodeCount = 0;
  file.Read(&nodeCount, 4);

  for (int index = 0; index < nodeCount; ++index) {
    this->ReadNode(file, minor, textureNames);
  }

  for (ModelNode* node : this->nodes) {
    if (!node->parentName.empty()) {
      node->parent = this->FindNode(node->parentName.c_str());
      node->parent->children.push_back(node);
    }
  }

  this->rootNames.push_back(rootName);
  int volumeCount = 0;
  file.Read(&volumeCount, 4);

  if (major == 1) {
    if (minor >= 3) {
      for (int index = 0; index < volumeCount; ++index) {
        auto* volume = new ModelVolume;
        std::memset(volume, 0, 132);
        unsigned char record[40]{};
        file.Read(record, sizeof(record));
        std::memcpy(&volume->flag, record + 36, 4);
        std::memcpy(volume, record, 36);

        if (volume->flag == 0) {
          volume->size[0] = volume->size[0] + 3.0f;
          volume->size[2] = volume->size[2] + 3.0f;
        }

        this->volumes.push_back(volume);
      }
    } else if (minor == 2) {
      for (int index = 0; index < volumeCount; ++index) {
        auto* volume = new ModelVolume;
        std::memset(volume, 0, 132);
        unsigned char record[36]{};
        file.Read(record, sizeof(record));
        volume->flag = 0;
        std::memcpy(volume, record, 36);
        this->volumes.push_back(volume);
      }
    }
  }

  this->sourceName = name;
  file.Close();

  return true;
}

// OFFSET: 0x007577E0
// FUNCTION: fn_08006
void C3dGrannyModelRes::RebindMeshes(const C3dGrannyModelRes& target) {
  GrannyModelInstance* instance = this->instance;
  for (int index = 0; index < this->instance->meshCount; ++index) {
    GrannyMeshEntry& entry = instance->meshes[index];
    GrannyFreeMeshBinding(entry.binding);
    entry.binding = GrannyNewMeshBinding(entry.mesh, this->skeleton, target.skeleton);
  }
}

// OFFSET: 0x00758E40
// FUNCTION: fn_08009
void C3dGrannyModelRes::InstantiateModel(GrannyModelInstance* target, const granny_model* model) {
  const int boneCount = model->skeleton->boneCount;
  if (this->maxBoneCount < boneCount) {
    this->maxBoneCount = boneCount;
  }

  target->instance = GrannyInstantiateModel(model);
  target->worldPose = GrannyNewWorldPose(boneCount);
  target->meshCount = model->meshBindingCount;
  target->meshes = new GrannyMeshEntry[target->meshCount];

  for (int index = 0; index < target->meshCount; ++index) {
    GrannyMeshEntry& entry = target->meshes[index];
    granny_mesh* mesh = model->meshBindings[index].mesh;
    entry.mesh = mesh;
    granny_skeleton* skeleton = GrannyGetSourceSkeleton(target->instance);
    entry.binding = GrannyNewMeshBinding(mesh, skeleton, skeleton);

    const unsigned int indexCount = GrannyGetMeshIndexCount(mesh);
    entry.indices = new std::uint16_t[indexCount];
    if (indexCount != 0) {
      GrannyCopyMeshIndices(mesh, 2, entry.indices);
    }

    const int vertexBytes = GrannyGetMeshVertexCount(mesh) * 32;
    entry.vertices = nullptr;
    if (GrannyMeshIsRigid(mesh)) {
      entry.vertices = new unsigned char[vertexBytes];
      GrannyCopyMeshVertices(mesh, GrannyPNT332VertexType, entry.vertices);
      entry.deformer = nullptr;
    } else {
      if (this->deformBufferSize < vertexBytes) {
        this->deformBufferSize = vertexBytes;
      }

      entry.deformer = GrannyNewMeshDeformer(GrannyGetMeshVertexType(mesh), GrannyPNT332VertexType, 2);
      if (!entry.deformer) {
        ShowErrorDialog(GetRenderSystemHolder()->system, "Granny didn't find a matching deformer for the vertex format used by mesh", 0);
      }
    }

    entry.materialCount = mesh->materialBindingCount;
    entry.textures = new GrannyTextureEntry*[entry.materialCount];

    for (int material = 0; material < entry.materialCount; ++material) {
      GrannyTextureEntry* found = nullptr;
      const granny_texture* texture = GrannyGetMaterialTextureByType(mesh->materialBindings[material].material, 2);
      if (texture) {
        for (int textureIndex = 0; textureIndex < this->textureCount; ++textureIndex) {
          if (std::strcmp(this->textures[textureIndex].name, texture->fromFileName) == 0) {
            found = &this->textures[textureIndex];
            break;
          }
        }
      }

      entry.textures[material] = found;
    }
  }
}

// OFFSET: 0x00759090
// FUNCTION: fn_08010
CTexture* C3dGrannyModelRes::CreateTexture(const granny_texture* texture, int) {
  const bool hasAlpha = GrannyTextureHasAlpha(texture);
  const int width = texture->width;
  const int height = texture->height;
  unsigned char* source = new unsigned char[height * width * 4];
  unsigned char* pixels = new unsigned char[height * width * 4];
  GrannyCopyTextureImage(texture, 0, 0, GrannyRGBA8888PixelFormat, width, height, width * 4, source);

  int format;
  int offset = 0;
  if (hasAlpha) {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        const unsigned char red = source[offset + 0];
        const unsigned char green = source[offset + 1];
        const unsigned char blue = source[offset + 2];
        const unsigned char alpha = source[offset + 3];
        pixels[offset + 0] = blue;
        pixels[offset + 1] = green;
        pixels[offset + 2] = red;
        pixels[offset + 3] = alpha;
        offset += 4;
      }
    }

    format = 1;
  } else {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        const unsigned char red = source[offset + 0];
        const unsigned char green = source[offset + 1];
        const unsigned char blue = source[offset + 2];
        pixels[offset + 0] = blue;
        pixels[offset + 1] = green;
        pixels[offset + 2] = red;
        pixels[offset + 3] = 0xFF;
        offset += 4;
      }
    }

    format = 0;
  }

  CTexture* result = g_texMgr.Upload(width, height, reinterpret_cast<const std::uint32_t*>(pixels), format, 0, false, false, false);
  delete[] pixels;
  delete[] source;
  return result;
}

// OFFSET: 0x00759330
// FUNCTION: fn_08011
void C3dGrannyModelRes::FreeControl(granny_control* control) {
  if (control) {
    GrannySetControlActive(control, false);
    GrannyFreeControl(control);
  }
}

// OFFSET: 0x007598B0
// FUNCTION: fn_08019
granny_file* GrannyAnimationLibrary::ReadAnimationFile(const char* name, const char* suffix) {
  std::string path;
  CFile file;
  std::sprintf(g_animationPath, "%s%s.gr2", name, suffix);

  if (!file.Open(g_animationPath, 0, true, false)) {
    ShowErrorDialog(GetRenderSystemHolder()->system, (std::string("Cannot find File : ") + std::string(g_animationPath)).c_str(), 0);
    return nullptr;
  }

  granny_file* result = GrannyReadEntireFileFromMemory(file.GetSize(), file.GetData());
  file.Close();
  return result;
}

// OFFSET: 0x007594B0
// FUNCTION: fn_08013
__declspec(noinline) void GrannyAnimationLibrary::FreeInstance() {
  if (GrannyModelInstance* instance = this->instance) {
    for (int index = 0; index < instance->meshCount; ++index) {
      GrannyMeshEntry& entry = instance->meshes[index];
      GrannyFreeMeshBinding(entry.binding);
      GrannyFreeMeshDeformer(entry.deformer);
      delete entry.indices;
    }

    GrannyFreeModelInstance(instance->instance);
    GrannyFreeWorldPose(instance->worldPose);
    delete[] instance->meshes;
  }

  delete this->instance;
  this->instance = nullptr;
}

// OFFSET: 0x00759550
// FUNCTION: fn_08014
void C3dModelRes::ForEachRootName(std::function<void(const std::string&)> callback) const {
  std::for_each(this->rootNames.begin(), this->rootNames.end(), callback);
}

// OFFSET: 0x007597A0
// FUNCTION: fn_08015
int C3dGrannyModelRes::GetTriangleCount(int mesh) const {
  return GrannyGetMeshIndexCount(this->instance->meshes[mesh].mesh) / 3;
}

// OFFSET: 0x007597E0
// FUNCTION: fn_08016
unsigned char* C3dGrannyModelRes::GetRigidVertices(int mesh) const {
  const GrannyMeshEntry& entry = this->instance->meshes[mesh];

  if (GrannyMeshIsRigid(entry.mesh)) {
    return entry.vertices;
  }

  return nullptr;
}

// OFFSET: 0x00759820
// FUNCTION: fn_08017
void GrannyAnimationLibrary::Initialize() {
}

// OFFSET: 0x00759830
// FUNCTION: fn_08018
bool C3dModelRes::Load(const char*) {
  return false;
}

// OFFSET: 0x00757390
// FUNCTION: fn_08005
bool GrannyAnimationLibrary::Load(const char* name, int job, int action) {
  bool result = false;

  if (!GrannyVersionsMatch_(2, 1, 0, 5)) {
    ShowErrorDialog(GetRenderSystemHolder()->system, "doesn't match the .h file used during compilation", 0);
    return false;
  }

  this->modelCount = 0;
  this->maxBoneCount = 0;
  this->textureCount = 0;
  this->unknown_0170 = 0;
  CFile file;

  if (!file.Open(name, 0, true, false)) {
    return false;
  }

  this->file = GrannyReadEntireFileFromMemory(file.GetSize(), file.GetData());
  file.Close();

  if (!this->file) {
    return false;
  }

  granny_file_info* info = GrannyGetFileInfo(this->file);
  std::strcpy(this->fileName, name);

  if (info) {
    this->textureCount = info->textureCount;
    this->modelCount = info->modelCount;

    if (this->instance) {
      this->FreeInstance();
    }

    GrannyModelInstance* instance = new GrannyModelInstance;
    this->instance = instance;
    granny_model* model = info->models[0];
    const int boneCount = model->skeleton->boneCount;

    if (this->maxBoneCount < boneCount) {
      this->maxBoneCount = boneCount;
    }

    instance->instance = GrannyInstantiateModel(model);
    instance->worldPose = GrannyNewWorldPose(boneCount);
    instance->meshCount = model->meshBindingCount;
    instance->meshes = new GrannyMeshEntry[instance->meshCount];

    for (int index = 0; index < instance->meshCount; ++index) {
      GrannyMeshEntry& entry = instance->meshes[index];
      granny_mesh* mesh = model->meshBindings[index].mesh;
      entry.mesh = mesh;
      granny_skeleton* skeleton = GrannyGetSourceSkeleton(instance->instance);
      entry.binding = GrannyNewMeshBinding(mesh, skeleton, skeleton);

      const unsigned int indexCount = GrannyGetMeshIndexCount(mesh);
      entry.indices = new std::uint16_t[indexCount];
      if (indexCount != 0) {
        GrannyCopyMeshIndices(mesh, 2, entry.indices);
      }

      const int vertexBytes = GrannyGetMeshVertexCount(mesh) * 32;
      if (GrannyMeshIsRigid(mesh)) {
        entry.deformer = nullptr;
      } else {
        if (this->deformBufferSize < vertexBytes) {
          this->deformBufferSize = vertexBytes;
        }

        entry.deformer = GrannyNewMeshDeformer(GrannyGetMeshVertexType(mesh), GrannyPNT332VertexType, 2);
        if (!entry.deformer) {
          ShowErrorDialog(GetRenderSystemHolder()->system, "Granny didn't find a matching deformer for the vertex format used by mesh", 0);
        }
      }
    }

    if (info->animationCount != 0) {
      granny_animation* animation = nullptr;

      if (char* extension = std::strchr(this->fileName, '.')) {
        *extension = '\0';
      }

      if (granny_file* animationFile = this->ReadAnimationFile(this->fileName, "")) {
        if (granny_file_info* animationInfo = GrannyGetFileInfo(animationFile)) {
          if (!instance->instance || animationInfo->animationCount != 1) {
            ShowErrorDialog(GetRenderSystemHolder()->system, "This sample only supports single-animation files", 0);
            this->animations[job][action] = nullptr;
            return false;
          }

          animation = animationInfo->animations[0];
        }
      }

      this->animations[job][action] = animation;

      if (!animation) {
        return false;
      }
    }

    this->localPose = GrannyNewLocalPose(this->maxBoneCount);

    if (this->deformBufferSize != 0) {
      this->unknown_0170 = 4;
    }

    result = true;
  } else {
    ShowErrorDialog(GetRenderSystemHolder()->system, "Unable to access file data .gr2", 0);
  }

  GrannyFreeFileSection(this->file, 1);
  GrannyFreeFileSection(this->file, 2);
  GrannyFreeFileSection(this->file, 4);
  GrannyFreeFileSection(this->file, 5);
  this->unknown_0000 = 1920.0f;
  return result;
}

// OFFSET: 0x00757270
// FUNCTION: fn_08004
granny_control* C3dGrannyModelRes::PlayAnimation(unsigned action, granny_model_instance* instance, granny_control* control, int loopCount) {
  if (!control) {
    const unsigned job = this->job;
    GrannyAnimationLibrary* library = this->animations;
    granny_animation* const& animation = library->animations[job][action];

    if (!animation) {
      g_modelPath[0] = '\0';

      if (job > 9 || action > 4) {
        return nullptr;
      }

      if (action == 0) {
        std::sprintf(g_modelPath, "model\\3dmob\\%s", g_session.GetJobName(g_animationJobs[job], -1));
      } else {
        std::sprintf(g_modelPath, "model\\3dmob_bone\\%d_%s.gr2", job, g_animationNames[action]);
      }

      if (!g_modelPath[0] || !ResourceExists(g_modelPath, 0)) {
        return nullptr;
      }

      if (job != 1) {
        library->Load(g_modelPath, job, action);
      }

      if (!animation) {
        return nullptr;
      }
    }

    granny_control* result = GrannyPlayControlledAnimation(0.0f, animation, instance);
    if (result) {
      GrannySetControlLoopCount(result, loopCount);
      return result;
    }

    return nullptr;
  }

  GrannySetControlActive(control, true);
  return control;
}

// OFFSET: 0x00758710
// FUNCTION: fn_08008
bool C3dGrannyModelRes::ProjectBounds(float* rect, const matrix& view, const matrix& world, const float* eye) const {
  matrix matrix;
  matrix.MakeZRotation(3.141592f);
  const float* m = matrix.values;
  const float x = this->position[1] * m[3] + this->position[0] * m[0] + this->position[2] * m[6] + m[9];
  const float y = this->position[0] * m[1] + this->position[1] * m[4] + this->position[2] * m[7] + m[10];
  const float z = this->position[0] * m[2] + this->position[1] * m[5] + this->position[2] * m[8] + m[11];
  matrix.Compose(world, view);

  const float screenX = y * m[3] + x * m[0] + z * m[6] + m[9];
  const float screenY = x * m[1] + y * m[4] + z * m[7] + m[10];
  const float depth = y * m[5] + x * m[2] + z * m[8] + m[11];
  const float deltaY = eye[1] - y;
  const float deltaX = eye[0] - x;
  const float deltaZ = eye[2] - z;
  const float distance = SquareRoot(deltaY * deltaY + deltaX * deltaX + deltaZ * deltaZ);
  const float slope = std::min(std::fabs(1.0f / distance * deltaY), 1.0f);
  const float height = this->unknown_03A4 - (this->unknown_03A4 - this->unknown_03A0) * slope;
  const float halfWidth = SquareRoot(this->unknown_03A0 * this->unknown_03A0 * 0.5f);
  const float halfHeight = SquareRoot(height * height * 0.5f);
  const float inverseDepth = 1.0f / depth;

  rect[4] = inverseDepth;
  rect[0] = (screenX - halfWidth) * g_renderer->viewport.scaleX * inverseDepth + static_cast<float>(g_renderer->viewport.clipCenterX);
  rect[1] = (screenY - halfHeight) * g_renderer->viewport.scaleY * inverseDepth + static_cast<float>(g_renderer->viewport.clipCenterY);
  rect[2] = (halfWidth + screenX) * g_renderer->viewport.scaleX * inverseDepth + static_cast<float>(g_renderer->viewport.clipCenterX);
  rect[3] = (halfHeight + screenY) * g_renderer->viewport.scaleY * inverseDepth + static_cast<float>(g_renderer->viewport.clipCenterY);

  const unsigned width = g_renderer->viewport.width;
  const unsigned screenHeight = g_renderer->viewport.height;
  return !(static_cast<unsigned>(rect[0]) > width && static_cast<unsigned>(rect[2]) > width && static_cast<unsigned>(rect[1]) > screenHeight &&
           static_cast<unsigned>(rect[3]) > screenHeight);
}

// OFFSET: 0x00757850
// FUNCTION: fn_08007
void AnimatedModelNode::Shade(const float* matrix, int frame, const float* light, const float* diffuse, const float* ambient, int shadeType, int alpha) {
  // GLOBAL: gl_24735
  static ShadeVector faceNormals[30000];
  // GLOBAL: gl_24736
  static ShadeVector vertexNormals[30000];
  std::list<int> groups[32];
  auto* colors = reinterpret_cast<std::array<std::uint32_t, 3>*>(this->faceNormals);

  switch (shadeType) {
  case 0: {
    for (int index = 0; index < this->faceCount; ++index) {
      const std::uint32_t color = (alpha << 24) + 0xFFFFFF;
      colors[index][0] = color;
      colors[index][1] = color;
      colors[index][2] = color;
    }
    break;
  }
  case 1: {
    for (int index = 0; index < this->faceCount; ++index) {
      const AnimatedModelFace& face = this->faces[index];
      const std::array<float, 3>* vertices = this->frames[frame].vertices;
      const std::array<float, 3>& v0 = vertices[face.vertices[0]];
      const std::array<float, 3>& v1 = vertices[face.vertices[1]];
      const std::array<float, 3>& v2 = vertices[face.vertices[2]];
      const float x2 = v2[0] * matrix[0] + v2[1] * matrix[3] + v2[2] * matrix[6] + matrix[9];
      const float y2 = v2[0] * matrix[1] + v2[1] * matrix[4] + v2[2] * matrix[7] + matrix[10];
      const float z2 = v2[0] * matrix[2] + v2[1] * matrix[5] + v2[2] * matrix[8] + matrix[11];
      const float ax = x2 - (matrix[0] * v0[0] + v0[1] * matrix[3] + v0[2] * matrix[6] + matrix[9]);
      const float ay = y2 - (matrix[1] * v0[0] + matrix[4] * v0[1] + matrix[7] * v0[2] + matrix[10]);
      const float az = z2 - (matrix[2] * v0[0] + matrix[5] * v0[1] + matrix[8] * v0[2] + matrix[11]);
      const float bx = x2 - (v1[0] * matrix[0] + v1[1] * matrix[3] + v1[2] * matrix[6] + matrix[9]);
      const float by = y2 - (v1[0] * matrix[1] + v1[1] * matrix[4] + v1[2] * matrix[7] + matrix[10]);
      const float bz = z2 - (v1[0] * matrix[2] + v1[1] * matrix[5] + v1[2] * matrix[8] + matrix[11]);
      const float nx = by * az - bz * ay;
      const float ny = bz * ax - bx * az;
      const float nz = bx * ay - by * ax;
      const float inverse = 1.0f / SquareRoot(ny * ny + nx * nx + nz * nz);
      const float intensity = std::max(0.0f, nx * inverse * light[0] + ny * inverse * light[1] + nz * inverse * light[2]);
      const float red = std::min(intensity * diffuse[0] + ambient[0], 1.0f);
      const float green = std::min(intensity * diffuse[1] + ambient[1], 1.0f);
      const float blue = std::min(intensity * diffuse[2] + ambient[2], 1.0f);

      for (int corner = 0; corner < 3; ++corner) {
        const int source = this->sharedTextureVertices ? 0 : frame;
        const float scale = this->frames[source].textureVertices[face.textureVertices[corner]].shaded ? 128.0f : 255.0f;
        colors[index][corner] = ((((alpha << 8) | static_cast<int>(scale * red)) << 8 | static_cast<int>(scale * green)) << 8) | static_cast<int>(scale * blue);
      }
    }
    break;
  }
  case 2: {
    for (int index = 0; index < this->faceCount; ++index) {
      const AnimatedModelFace& face = this->faces[index];
      const std::array<float, 3>* vertices = this->frames[frame].vertices;
      const std::array<float, 3>& v0 = vertices[face.vertices[0]];
      const std::array<float, 3>& v1 = vertices[face.vertices[1]];
      const std::array<float, 3>& v2 = vertices[face.vertices[2]];
      const float x2 = v2[0] * matrix[0] + v2[1] * matrix[3] + v2[2] * matrix[6] + matrix[9];
      const float y2 = v2[0] * matrix[1] + v2[1] * matrix[4] + v2[2] * matrix[7] + matrix[10];
      const float z2 = v2[0] * matrix[2] + v2[1] * matrix[5] + v2[2] * matrix[8] + matrix[11];
      const float ax = x2 - (v0[0] * matrix[0] + v0[1] * matrix[3] + v0[2] * matrix[6] + matrix[9]);
      const float ay = y2 - (matrix[1] * v0[0] + matrix[4] * v0[1] + matrix[7] * v0[2] + matrix[10]);
      const float az = z2 - (matrix[2] * v0[0] + matrix[5] * v0[1] + matrix[8] * v0[2] + matrix[11]);
      const float bx = x2 - (v1[0] * matrix[0] + v1[1] * matrix[3] + v1[2] * matrix[6] + matrix[9]);
      const float by = y2 - (v1[0] * matrix[1] + v1[1] * matrix[4] + v1[2] * matrix[7] + matrix[10]);
      const float bz = z2 - (v1[0] * matrix[2] + v1[1] * matrix[5] + v1[2] * matrix[8] + matrix[11]);
      ShadeVector& normal = faceNormals[index];
      normal.x = by * az - bz * ay;
      normal.y = ax * bz - bx * az;
      normal.z = bx * ay - ax * by;
      const float inverse = 1.0f / SquareRoot(normal.y * normal.y + normal.x * normal.x + normal.z * normal.z);
      normal.x = inverse * normal.x;
      normal.y = inverse * normal.y;
      normal.z = inverse * normal.z;
      groups[face.smoothGroup].push_back(index);
    }

    for (int group = 0; group < 32; ++group) {
      if (groups[group].empty()) {
        continue;
      }

      for (int vertex = 0; vertex < this->vertexCount; ++vertex) {
        ShadeVector& normal = vertexNormals[vertex];
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;

        for (int index = 0; index < this->faceCount; ++index) {
          const AnimatedModelFace& face = this->faces[index];
          if (face.smoothGroup == group && (face.vertices[0] == vertex || face.vertices[1] == vertex || face.vertices[2] == vertex)) {
            normal.x = faceNormals[index].x + normal.x;
            normal.y = faceNormals[index].y + normal.y;
            normal.z = normal.z + faceNormals[index].z;
          }
        }

        if (SquareRoot(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z) != 0.0f) {
          const float inverse = 1.0f / SquareRoot(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
          normal.x = inverse * normal.x;
          normal.y = inverse * normal.y;
          normal.z = inverse * normal.z;
        }
      }

      for (int index : groups[group]) {
        const AnimatedModelFace& face = this->faces[index];

        for (int corner = 0; corner < 3; ++corner) {
          const ShadeVector& normal = vertexNormals[face.vertices[corner]];
          const int source = this->sharedTextureVertices ? 0 : frame;
          const float intensity = std::max(0.0f, normal.y * light[1] + light[0] * normal.x + normal.z * light[2]);
          const float red = std::min(intensity * diffuse[0] + ambient[0], 1.0f);
          const float green = std::min(intensity * diffuse[1] + ambient[1], 1.0f);
          const float blue = std::min(intensity * diffuse[2] + ambient[2], 1.0f);
          const float scale = this->frames[source].textureVertices[face.textureVertices[corner]].shaded ? 128.0f : 255.0f;
          colors[index][corner] = ((((alpha << 8) | static_cast<int>(scale * red)) << 8 | static_cast<int>(green * scale)) << 8) | static_cast<int>(blue * scale);
        }
      }
    }
    break;
  }
  default: {
    for (int index = 0; index < this->faceCount; ++index) {
      const std::uint32_t color = (alpha << 24) + 0xFFFFFF;
      colors[index][0] = color;
      colors[index][1] = color;
      colors[index][2] = color;
    }

    ShowErrorDialog(GetRenderSystemHolder()->system, "Invalid Shade Type", 0);
    break;
  }
  }

  for (AnimatedModelNode* child : this->children) {
    child->Shade(matrix, frame, light, diffuse, ambient, shadeType, alpha);
  }
}

// OFFSET: 0x0075CB30
// FUNCTION: fn_08025
const void* C3dGrannyModelRes::GetMeshRenderData(
  int mesh, CTexture** texture, std::uint16_t** indices, int* triangleCount, matrix* transform, granny_world_pose* pose
) {
  return this->GetMeshRenderData(this->instance, mesh, texture, indices, triangleCount, transform, pose);
}

// OFFSET: 0x0075CB60
// FUNCTION: fn_08026
const void* C3dGrannyModelRes::GetMeshRenderData(
  GrannyModelInstance* model, int mesh, CTexture** texture, std::uint16_t** indices, int* triangleCount, matrix* transform,
  granny_world_pose* pose
) {
  if (pose == nullptr) {
    pose = model->worldPose;
  }

  const float* matrices = GrannyControlModelsNext(pose);
  GrannyMeshEntry* meshes = model->meshes;
  const std::int32_t* boneIndices = GrannyGetMeshBindingToBoneIndices(meshes[mesh].binding);
  const std::int32_t vertexCount = GrannyGetMeshVertexCount(meshes[mesh].mesh);
  const granny_tri_material_group* groups = GrannyGetMeshTriangleGroups(meshes[mesh].mesh);
  GrannyTextureEntry* entry = meshes[mesh].textures[groups->materialIndex];
  *texture = entry != nullptr && meshes[mesh].materialCount > groups->materialIndex ? entry->texture : nullptr;

  if (GrannyMeshIsRigid(meshes[mesh].mesh)) {
    const float* composite = GrannyGetWorldPoseComposite4x4(model->worldPose, boneIndices[0]);
    transform->values[0] = composite[0];
    transform->values[1] = composite[1];
    transform->values[2] = composite[2];
    transform->values[3] = composite[4];
    transform->values[4] = composite[5];
    transform->values[5] = composite[6];
    transform->values[6] = composite[8];
    transform->values[7] = composite[9];
    transform->values[8] = composite[10];
    transform->values[9] = composite[12];
    transform->values[10] = composite[13];
    transform->values[11] = composite[14];

    const void* vertices = meshes[mesh].vertices;
    *indices = meshes[mesh].indices;
    *triangleCount = GrannyGetMeshIndexCount(meshes[mesh].mesh) / 3;
    if (std::strstr(meshes[mesh].mesh->name, "hair_pivot") != nullptr && this->unknown_03B0 != nullptr) {
      this->unknown_03B0->transform = *transform;
    }

    return vertices;
  }

  if (meshes[mesh].deformer != nullptr) {
    GrannyDeformVertices(meshes[mesh].deformer, boneIndices, matrices, vertexCount, GrannyGetMeshVertices(meshes[mesh].mesh), this->deformBuffer);
    GrannyGetMeshBytesPerIndex(meshes[mesh].mesh);
    GrannyGetMeshIndices(meshes[mesh].mesh);
    const void* vertices = this->deformBuffer;
    *indices = meshes[mesh].indices;
    *triangleCount = GrannyGetMeshIndexCount(meshes[mesh].mesh) / 3;
    std::memcpy(transform->values, g_identityMatrix, sizeof(g_identityMatrix));
    return vertices;
  }

  return nullptr;
}

// OFFSET: 0x0075CD90
// FUNCTION: fn_08027
bool C3dGrannyModelRes::FindHairPivot() {
  if (this->unknown_03B0 == nullptr) {
    return false;
  }

  for (int mesh = 0; mesh < this->instance->meshCount; ++mesh) {
    if (std::strstr(this->instance->meshes[mesh].mesh->name, "hair_pivot") != nullptr) {
      this->unknown_03B0->mesh = mesh;
      return true;
    }
  }

  return false;
}

// OFFSET: 0x0075CFC0
// FUNCTION: fn_08029
granny_world_pose* C3dGrannyModelRes::Animate(float seconds, granny_model_instance* instance) {
  GrannyModelInstance* model = this->instance;
  if (model == nullptr) {
    return nullptr;
  }

  granny_skeleton* skeleton = GrannyGetSourceSkeleton(instance);
  const std::int32_t boneCount = skeleton->boneCount;
  GrannySetModelClock(instance, seconds);
  GrannySampleModelAnimations(instance, 0, boneCount, static_cast<granny_local_pose*>(this->localPose));
  GrannyBuildWorldPose(skeleton, 0, boneCount, static_cast<granny_local_pose*>(this->localPose), nullptr, model->worldPose);
  GrannyFreeCompletedModelControls(instance);
  return model->worldPose;
}

// OFFSET: 0x0075CA20
// FUNCTION: fn_08024
void C3dGrannyModelRes::Release() {
  if (GrannyModelInstance* instance = this->instance) {
    for (int index = 0; index < instance->meshCount; ++index) {
      GrannyMeshEntry& entry = instance->meshes[index];
      GrannyFreeMeshBinding(entry.binding);
      GrannyFreeMeshDeformer(entry.deformer);
      delete[] entry.textures;
      delete entry.indices;
      if (entry.vertices != nullptr) {
        delete entry.vertices;
      }
    }

    GrannyFreeModelInstance(instance->instance);
    GrannyFreeWorldPose(instance->worldPose);
    delete[] instance->meshes;
  }

  for (int index = 0; index < this->textureCount; ++index) {
    if (this->textures[index].texture != nullptr) {
      delete this->textures[index].texture;
    }
  }

  delete[] this->textures;
  GrannyFreeLocalPose(static_cast<granny_local_pose*>(this->localPose));
  GrannyFreeFile(this->file);
  delete this->instance;
  this->instance = nullptr;
}
