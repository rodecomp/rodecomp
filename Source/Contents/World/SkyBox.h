#ifndef RAGNAROK_SOURCE_CONTENTS_WORLD_SKYBOX_H
#define RAGNAROK_SOURCE_CONTENTS_WORLD_SKYBOX_H

#include "Render/Primitives/RenderPrimitive.h"
#include "Utility/Math/Vector3d.h"

#include <cstddef>
#include <cstdint>

class C3dActor;
class CRes;

struct FlareElement {
  FlareElement();
  FlareElement(int type, float distance, float size, float red, float green, float blue)
    : type(type), distance(distance), size(size), red(red), green(green), blue(blue), position{0.0f, 0.0f, 0.0f} {
  }

  FlareElement& operator=(const FlareElement& other) {
    this->type = other.type;
    this->distance = other.distance;
    this->size = other.size;
    this->red = other.red;
    this->green = other.green;
    this->blue = other.blue;
    this->position.x = other.position.x;
    this->position.y = other.position.y;
    this->position.z = other.position.z;
    this->scaledSize = other.scaledSize;
    return *this;
  }

  /* 0000 */ int type;
  /* 0004 */ float distance;
  /* 0008 */ float size;
  /* 000C */ float red;
  /* 0010 */ float green;
  /* 0014 */ float blue;
  /* 0018 */ vector3d position;
  /* 0024 */ float scaledSize;
};

class CLensFlare {
public:
  CLensFlare();

  void Init();
  void Update(const float* matrix, vector3d eye, vector3d target);
  void Render(const float* matrix, vector3d eye, vector3d target);

  /* 0000 */ FlareElement m_elements[12];
  /* 01E0 */ std::uint32_t m_colors[12];
  /* 0210 */ char m_textureName[260];
  /* 0314 */ vector3d m_position;
};

struct SkyCubeVertex : RenderVertex {
  SkyCubeVertex() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->rhw = 0;
    this->diffuse = 0;
    this->specular = 0xFF000000;
    this->u = 0;
    this->v = 0;
  }
};

class CSkyCube {
public:
  CSkyCube();

  /* 0000 */ SkyCubeVertex m_vertices[10];
  /* 0140 */ int m_unknown_0140;
  /* 0144 */ CLensFlare m_flare;
};

struct SkyBoxVertex {
  SkyBoxVertex();
  ~SkyBoxVertex();

  /* 0000 */ RenderVertex vertex;
};

class CSkyBox {
public:
  ~CSkyBox();

  /* 0000 */ CRes* m_model;
  /* 0004 */ C3dActor* m_actor;
  /* 0008 */ int m_unknown_0008;
  /* 000C */ CLensFlare m_flare;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FlareElement) == 0x28);
static_assert(offsetof(FlareElement, position) == 0x18);
static_assert(sizeof(CLensFlare) == 0x320);
static_assert(offsetof(CLensFlare, m_colors) == 0x1E0);
static_assert(offsetof(CLensFlare, m_textureName) == 0x210);
static_assert(offsetof(CLensFlare, m_position) == 0x314);
static_assert(offsetof(CSkyCube, m_flare) == 0x144);
static_assert(sizeof(CSkyBox) == 0x32C);
#endif

#endif
