#ifndef RAGNAROK_SOURCE_RESOURCE_MODELS_GRANNYMODELRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_MODELS_GRANNYMODELRESOURCE_H

#include "Resource/Models/ModelResource.h"

#include <cstddef>
#include <cstdint>
#include <windows.h>

class CTexture;
struct AffineMatrix;

struct granny_animation;
struct granny_control;
struct granny_file;
struct granny_local_pose;
struct granny_material;
struct granny_mesh_binding;
struct granny_mesh_deformer;
struct granny_model_instance;
struct granny_world_pose;

struct granny_data_type_definition {
  /* 0000 */ std::int32_t type;
  /* 0004 */ const char* name;
  /* 0008 */ granny_data_type_definition* referenceType;
  /* 000C */ std::int32_t arrayWidth;
  /* 0010 */ std::int32_t extra[3];
  /* 001C */ std::uint32_t ignored;
};

struct granny_pixel_layout {
  /* 0000 */ std::int32_t bytesPerPixel;
  /* 0004 */ std::int32_t shiftForComponent[4];
  /* 0014 */ std::int32_t bitsForComponent[4];
};

struct granny_texture {
  /* 0000 */ const char* fromFileName;
  /* 0004 */ std::int32_t textureType;
  /* 0008 */ std::int32_t width;
  /* 000C */ std::int32_t height;
};

struct granny_skeleton {
  /* 0000 */ const char* name;
  /* 0004 */ std::int32_t boneCount;
};

struct granny_material_binding {
  /* 0000 */ granny_material* material;
};

struct granny_mesh {
  /* 0000 */ unsigned char unknown_0000[0x14];
  /* 0014 */ std::int32_t materialBindingCount;
  /* 0018 */ granny_material_binding* materialBindings;
};

struct granny_model_mesh_binding {
  /* 0000 */ granny_mesh* mesh;
};

struct granny_model {
  /* 0000 */ const char* name;
  /* 0004 */ granny_skeleton* skeleton;
  /* 0008 */ unsigned char unknown_0008[0x44];
  /* 004C */ std::int32_t meshBindingCount;
  /* 0050 */ granny_model_mesh_binding* meshBindings;
};

struct granny_file_info {
  /* 0000 */ unsigned char unknown_0000[0x10];
  /* 0010 */ std::int32_t textureCount;
  /* 0014 */ unsigned char unknown_0014[0x2C];
  /* 0040 */ std::int32_t modelCount;
  /* 0044 */ granny_model** models;
  /* 0048 */ unsigned char unknown_0048[8];
  /* 0050 */ std::int32_t animationCount;
  /* 0054 */ granny_animation** animations;
};

extern "C" {
__declspec(dllimport) extern const granny_pixel_layout GrannyRGBA8888PixelFormat;
__declspec(dllimport) extern granny_data_type_definition GrannyPNT332VertexType[];

__declspec(dllimport) void WINAPI GrannySetControlActive(granny_control* control, bool active);
__declspec(dllimport) void WINAPI GrannyFreeControl(granny_control* control);
__declspec(dllimport) bool WINAPI GrannyTextureHasAlpha(const granny_texture* texture);
__declspec(dllimport) void WINAPI GrannyCopyTextureImage(
  const granny_texture* texture, std::int32_t imageIndex, std::int32_t mipIndex, const granny_pixel_layout* layout, std::int32_t destWidth,
  std::int32_t destHeight, std::int32_t destStride, void* pixels
);
__declspec(dllimport) granny_model_instance* WINAPI GrannyInstantiateModel(const granny_model* model);
__declspec(dllimport) granny_world_pose* WINAPI GrannyNewWorldPose(std::int32_t boneCount);
__declspec(dllimport) granny_skeleton* WINAPI GrannyGetSourceSkeleton(const granny_model_instance* instance);
__declspec(dllimport) granny_mesh_binding* WINAPI GrannyNewMeshBinding(const granny_mesh* mesh, const granny_skeleton* from, const granny_skeleton* to);
__declspec(dllimport) void WINAPI GrannyFreeMeshBinding(granny_mesh_binding* binding);
__declspec(dllimport) std::int32_t WINAPI GrannyGetMeshIndexCount(const granny_mesh* mesh);
__declspec(dllimport) void WINAPI GrannyCopyMeshIndices(const granny_mesh* mesh, std::int32_t bytesPerIndex, void* indices);
__declspec(dllimport) std::int32_t WINAPI GrannyGetMeshVertexCount(const granny_mesh* mesh);
__declspec(dllimport) bool WINAPI GrannyMeshIsRigid(const granny_mesh* mesh);
__declspec(dllimport) granny_data_type_definition* WINAPI GrannyGetMeshVertexType(const granny_mesh* mesh);
__declspec(dllimport) granny_mesh_deformer* WINAPI GrannyNewMeshDeformer(
  const granny_data_type_definition* inputLayout, const granny_data_type_definition* outputLayout, std::int32_t deformationType
);
__declspec(dllimport) void WINAPI GrannyCopyMeshVertices(const granny_mesh* mesh, const granny_data_type_definition* vertexType, void* vertices);
__declspec(dllimport) granny_texture* WINAPI GrannyGetMaterialTextureByType(const granny_material* material, std::int32_t type);
__declspec(dllimport) bool WINAPI GrannyVersionsMatch_(std::int32_t major, std::int32_t minor, std::int32_t customization, std::int32_t build);
__declspec(dllimport) granny_file* WINAPI GrannyReadEntireFileFromMemory(std::int32_t size, void* memory);
__declspec(dllimport) granny_file_info* WINAPI GrannyGetFileInfo(granny_file* file);
__declspec(dllimport) void WINAPI GrannyFreeFileSection(granny_file* file, std::int32_t section);
__declspec(dllimport) granny_local_pose* WINAPI GrannyNewLocalPose(std::int32_t boneCount);
__declspec(dllimport) void WINAPI GrannyFreeMeshDeformer(granny_mesh_deformer* deformer);
__declspec(dllimport) void WINAPI GrannyFreeModelInstance(granny_model_instance* instance);
__declspec(dllimport) void WINAPI GrannyFreeWorldPose(granny_world_pose* pose);
__declspec(dllimport) granny_control* WINAPI GrannyPlayControlledAnimation(float startTime, const granny_animation* animation, granny_model_instance* instance);
__declspec(dllimport) void WINAPI GrannySetControlLoopCount(granny_control* control, std::int32_t loopCount);
}

struct GrannyTextureEntry {
  /* 0000 */ const char* name;
  /* 0004 */ CTexture* texture;
};

struct GrannyMeshEntry {
  /* 0000 */ granny_mesh* mesh;
  /* 0004 */ granny_mesh_binding* binding;
  /* 0008 */ granny_mesh_deformer* deformer;
  /* 000C */ std::uint16_t* indices;
  /* 0010 */ unsigned char* vertices;
  /* 0014 */ int materialCount;
  /* 0018 */ GrannyTextureEntry** textures;
};

struct GrannyModelInstance {
  /* 0000 */ granny_model_instance* instance;
  /* 0004 */ granny_world_pose* worldPose;
  /* 0008 */ int meshCount;
  /* 000C */ GrannyMeshEntry* meshes;
};

class GrannyAnimationLibrary {
public:
  bool Load(const char* name, int job, int action);
  void Initialize();
  void FreeInstance();
  granny_file* ReadAnimationFile(const char* name, const char* suffix);

  /* 0000 */ float unknown_0000;
  /* 0004 */ unsigned char unknown_0004[0x14C];
  /* 0150 */ granny_file* file;
  /* 0154 */ int textureCount;
  /* 0158 */ GrannyTextureEntry* textures;
  /* 015C */ int modelCount;
  /* 0160 */ GrannyModelInstance* instance;
  /* 0164 */ int maxBoneCount;
  /* 0168 */ granny_local_pose* localPose;
  /* 016C */ int deformBufferSize;
  /* 0170 */ int unknown_0170;
  /* 0174 */ unsigned char unknown_0174[8];
  /* 017C */ char fileName[MAX_PATH];
  /* 0280 */ granny_animation* animations[100][20];
};

class C3dGrannyModelRes : public CRes {
public:
  void RebindMeshes(const C3dGrannyModelRes& target);
  void InstantiateModel(GrannyModelInstance* target, const granny_model* model);
  CTexture* CreateTexture(const granny_texture* texture, int option);
  void FreeControl(granny_control* control);
  granny_control* PlayAnimation(unsigned action, granny_model_instance* instance, granny_control* control, int loopCount);
  bool ProjectBounds(float* rect, const AffineMatrix& view, const AffineMatrix& world, const float* eye) const;
  int GetTriangleCount(int mesh) const;
  unsigned char* GetRigidVertices(int mesh) const;

  /* 0110 */ unsigned char unknown_0110[0x150];
  /* 0260 */ void* file;
  /* 0264 */ int textureCount;
  /* 0268 */ GrannyTextureEntry* textures;
  /* 026C */ int modelCount;
  /* 0270 */ GrannyModelInstance* instance;
  /* 0274 */ int maxBoneCount;
  /* 0278 */ void* localPose;
  /* 027C */ int deformBufferSize;
  /* 0280 */ int unknown_0280;
  /* 0284 */ unsigned char unknown_0284[8];
  /* 028C */ char fileName[MAX_PATH];
  /* 0390 */ const granny_model* model;
  /* 0394 */ float position[3];
  /* 03A0 */ float unknown_03A0;
  /* 03A4 */ float unknown_03A4;
  /* 03A8 */ GrannyAnimationLibrary* animations;
  /* 03AC */ unsigned job;
  /* 03B0 */ int unknown_03B0;
  /* 03B4 */ granny_skeleton* skeleton;

private:
  ModelResourceContext* m_context;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(granny_texture, width) == 0x08);
static_assert(offsetof(granny_mesh, materialBindings) == 0x18);
static_assert(offsetof(granny_model, meshBindingCount) == 0x4C);
static_assert(sizeof(GrannyMeshEntry) == 0x1C);
static_assert(sizeof(GrannyModelInstance) == 0x10);
static_assert(offsetof(C3dGrannyModelRes, file) == 0x260);
static_assert(offsetof(C3dGrannyModelRes, fileName) == 0x28C);
static_assert(offsetof(C3dGrannyModelRes, model) == 0x390);
static_assert(offsetof(C3dGrannyModelRes, skeleton) == 0x3B4);
static_assert(offsetof(granny_file_info, models) == 0x44);
static_assert(offsetof(granny_file_info, animations) == 0x54);
static_assert(offsetof(GrannyAnimationLibrary, file) == 0x150);
static_assert(offsetof(GrannyAnimationLibrary, fileName) == 0x17C);
static_assert(offsetof(GrannyAnimationLibrary, animations) == 0x280);
#endif

#endif
