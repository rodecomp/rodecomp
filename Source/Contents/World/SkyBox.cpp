#include "Render/Primitives/RenderPrimitivePool.h"
#include <new>
#include "Contents/World/SkyBox.h"

#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Models/ModelActor.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/ModelResource.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdio>

namespace {
struct SkyLightVector : vector3d {
  // INLINED
  SkyLightVector(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  // INLINED
  operator const float*() const {
    return &this->x;
  }
};

struct PrimitivePoolAccess : RenderPrimitivePool {
  // INLINED
  inline RenderPrimitive* AcquireLocal() {
    if (this->m_commands.end() == this->m_current) {
      this->m_commands.emplace_back();
      this->m_current = this->m_commands.end();
      return &this->m_commands.back();
    }

    RenderPrimitive* command = &*this->m_current;
    ++this->m_current;
    new (command) RenderPrimitive;

    return command;
  }
};

struct SkyVertex : RenderVertex {
  // INLINED
  SkyVertex() {
    this->x = 0.0f;
    this->y = 0.0f;
    this->z = 0.0f;
    this->rhw = 0.0f;
    this->diffuse = 0;
    this->specular = 0xFF000000u;
    this->u = 0.0f;
    this->v = 0.0f;
  }
};
}

// GLOBAL: gl_25991
float g_skyBoxHeight = 1200.0f;

// OFFSET: 0x0075DC00
// FUNCTION: fn_08044
CLensFlare::CLensFlare() : m_position{0.0f, 0.0f, 0.0f} {
}

// OFFSET: 0x0075DC50
// FUNCTION: fn_08045
CSkyCube::CSkyCube() {
  this->m_vertices[0].x = -1250.0f;
  this->m_vertices[0].y = -450.0f;
  this->m_vertices[0].z = 1250.0f;
  this->m_vertices[0].diffuse = 0xFFFFFFFF;
  this->m_vertices[1].x = 1250.0f;
  this->m_vertices[1].y = -450.0f;
  this->m_vertices[1].z = 1250.0f;
  this->m_vertices[1].diffuse = 0xFFFFFFFF;
  this->m_vertices[2].x = -1250.0f;
  this->m_vertices[2].y = 650.0f;
  this->m_vertices[2].z = 1250.0f;
  this->m_vertices[2].diffuse = 0xFFFFFFFF;
  this->m_vertices[3].x = 1250.0f;
  this->m_vertices[3].y = 650.0f;
  this->m_vertices[3].z = 1250.0f;
  this->m_vertices[3].diffuse = 0xFFFFFFFF;
  this->m_vertices[4].x = -1250.0f;
  this->m_vertices[4].y = -450.0f;
  this->m_vertices[4].z = -1250.0f;
  this->m_vertices[4].diffuse = 0xFFFFFFFF;
  this->m_vertices[5].x = 1250.0f;
  this->m_vertices[5].y = -450.0f;
  this->m_vertices[5].z = -1250.0f;
  this->m_vertices[5].diffuse = 0xFFFFFFFF;
  this->m_vertices[6].x = -1250.0f;
  this->m_vertices[6].y = 650.0f;
  this->m_vertices[6].z = -1250.0f;
  this->m_vertices[6].diffuse = 0xFFFFFFFF;
  this->m_vertices[7].x = 1250.0f;
  this->m_vertices[7].y = 650.0f;
  this->m_vertices[7].z = -1250.0f;
  this->m_vertices[7].diffuse = 0xFFFFFFFF;
  this->m_loopRotationY = 0;
  this->m_vertices[8].x = -1250.0f;
  this->m_vertices[8].y = -350.0f;
  this->m_vertices[8].z = 1240.0f;
  this->m_vertices[9].x = 1250.0f;
  this->m_vertices[9].y = 100.0f;
  this->m_vertices[9].z = 1250.0f;

  this->m_flare.Init();
}

// OFFSET: 0x0075DE20
// FUNCTION: fn_45841
CSkyBox::CSkyBox() {
  this->m_model = ResourceMgr::Instance().Load("skybox.rsm");

  if (static_cast<C3dModelRes*>(this->m_model)->major == 1) {
    this->m_actor = new C3dActorV1;
  } else if (static_cast<C3dModelRes*>(this->m_model)->major == 2) {
    this->m_actor = new C3dActorV2;
  }

  this->m_actor->SetScale({30.0f, 30.0f, 30.0f});
  this->m_actor->SetRotation({0.0f, 0.0f, 0.0f});
  this->m_loopRotationX = 0;
  this->m_actor->SetSpeed(0.1f);
  this->m_actor->Assign(*static_cast<C3dModelRes*>(this->m_model));

  this->m_actor->Prepare(SkyLightVector(0.0f, 1.0f, 0.0f), SkyLightVector(1.0f, 1.0f, 1.0f), SkyLightVector(1.0f, 1.0f, 1.0f));
  this->m_actor->Animate(0.0);
  this->m_flare.Init();
}

// OFFSET: 0x0075DFB0
// FUNCTION: fn_08046
FlareElement::FlareElement() {
  vector3d zero = {0.0f, 0.0f, 0.0f};
  this->position = zero;
}

// OFFSET: 0x0075DFD0
// FUNCTION: fn_08047
SkyBoxVertex::SkyBoxVertex() {
}

// OFFSET: 0x0075DFE0
// FUNCTION: fn_08048
SkyBoxVertex::~SkyBoxVertex() {
}

// OFFSET: 0x0075DFF0
// FUNCTION: fn_08049
CSkyBox::~CSkyBox() {
  if (this->m_actor) {
    delete this->m_actor;
    this->m_actor = nullptr;
  }
}

// INLINED
static __forceinline void DrawSkyFace(const char* textureName, const SkyCubeVertex* projected, int first, int second, int third, int fourth) {
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->SetPosition(0, &projected[first].x);
  primitive->SetPosition(1, &projected[second].x);
  primitive->SetPosition(2, &projected[third].x);
  primitive->SetPosition(3, &projected[fourth].x);
  primitive->vertices[0].u = 0.0f;
  primitive->vertices[0].v = 0.0f;
  primitive->vertices[1].u = 1.0f;
  primitive->vertices[1].v = 0.0f;
  primitive->vertices[2].u = 0.0f;
  primitive->vertices[2].v = 1.0f;
  primitive->vertices[3].u = 1.0f;
  primitive->vertices[3].v = 1.0f;
  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  primitive->texture = g_texMgr.Load(textureName, 0, false, true, false);
  primitive->vertices[0].diffuse = projected[first].diffuse;
  primitive->vertices[1].diffuse = projected[second].diffuse;
  primitive->vertices[2].diffuse = projected[third].diffuse;
  primitive->vertices[3].diffuse = projected[fourth].diffuse;
  g_renderer->Submit(primitive, 3);
}

// OFFSET: 0x0075FA20
// FUNCTION: fn_45842
void CSkyBox::Render(const matrix& camera, const vector3d& position, float rotation, float, vector3d eye, vector3d target) {
  vector3d actorPosition = position;
  actorPosition.y += g_skyBoxHeight;
  this->m_actor->SetPosition(actorPosition);
  this->m_actor->Update();
  this->m_actor->Draw(camera.values, true);

  SkyVertex points[2];
  SkyVertex projected[2];

  matrix transform;
  transform.Identity();
  transform.AppendYRotation(rotation / 180.0f * 3.141592f);
  transform.AppendTranslate(&position.x);

  matrix combined;
  combined.Compose(transform, camera);

  points[0].x = -1250.0f;
  points[0].y = -250.0f;
  points[0].z = 1240.0f;
  points[1].x = 1250.0f;
  points[1].y = 100.0f;
  points[1].z = 1250.0f;

  vector3d point;
  SkyVertex* output = projected;
  for (const SkyVertex& input : points) {
    point.x = input.x;
    point.y = input.y;
    point.z = input.z;
    g_renderer->Project(&point.x, combined.values, *output++);
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->vertices[0].x = projected[0].x;
  primitive->vertices[0].y = projected[0].y;
  primitive->vertices[0].z = projected[0].z;
  primitive->vertices[0].rhw = projected[0].rhw;
  primitive->vertices[1].x = projected[1].x;
  primitive->vertices[1].y = projected[0].y;
  primitive->vertices[1].z = projected[0].z;
  primitive->vertices[1].rhw = projected[0].rhw;
  primitive->vertices[2].x = projected[0].x;
  primitive->vertices[2].y = projected[1].y;
  primitive->vertices[2].z = projected[0].z;
  primitive->vertices[2].rhw = projected[0].rhw;
  primitive->vertices[3].x = projected[1].x;
  primitive->vertices[3].y = projected[1].y;
  primitive->vertices[3].z = projected[0].z;
  primitive->vertices[3].rhw = projected[0].rhw;
  primitive->vertices[0].u = 0.0f;
  primitive->vertices[0].v = 0.0f;
  primitive->vertices[1].u = 1.0f;
  primitive->vertices[1].v = 0.0f;
  primitive->vertices[2].u = 0.0f;
  primitive->vertices[2].v = 1.0f;
  primitive->vertices[3].u = 1.0f;
  primitive->vertices[3].v = 1.0f;
  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  primitive->texture = nullptr;
  primitive->vertices[0].diffuse = 0x00FFFFFF;
  primitive->vertices[1].diffuse = 0x00FFFFFF;
  primitive->vertices[2].diffuse = 0xFFFFFFFF;
  primitive->vertices[3].diffuse = 0xFFFFFFFF;
  g_renderer->Submit(primitive, 5);

  this->m_flare.Update(0.0f, eye, target);
  this->m_flare.Render(camera.values, eye, target);
}

// OFFSET: 0x0075F080
// FUNCTION: fn_46310
void CSkyCube::Render(const matrix& camera, const vector3d& position, float rotation, float height, vector3d eye, vector3d target) {
  SkyCubeVertex projected[10];

  matrix transform;
  transform.Identity();
  this->m_loopRotationY += 0.1f;
  transform.AppendYRotation(this->m_loopRotationY / 180.0f * 3.141592f);
  transform.AppendTranslate(&position.x);

  matrix extraTransform;
  extraTransform.Identity();
  extraTransform.AppendYRotation(rotation / 180.0f * 3.141592f);
  extraTransform.AppendTranslate(&position.x);

  matrix combined;
  combined.Compose(transform, camera);
  matrix extraCombined;
  extraCombined.Compose(extraTransform, camera);

  for (int index = 0; index < 8; ++index) {
    vector3d point{this->m_vertices[index].x, this->m_vertices[index].y, this->m_vertices[index].z};
    g_renderer->Project(&point.x, combined.values, projected[index]);
    projected[index].diffuse = this->m_vertices[index].diffuse;
  }
  for (int index = 8; index < 10; ++index) {
    vector3d point{this->m_vertices[index].x, this->m_vertices[index].y, this->m_vertices[index].z};
    g_renderer->Project(&point.x, extraCombined.values, projected[index]);
    projected[index].diffuse = this->m_vertices[index].diffuse;
  }

  if (this->m_loopRotationY >= 360.0f) {
    this->m_loopRotationY -= 360.0f;
  }

  rotation -= this->m_loopRotationY;
  if (rotation >= 360.0f) {
    rotation -= 360.0f;
  }
  if (rotation < 0.0f) {
    rotation += 360.0f;
  }

  if (rotation > 270.0f || rotation < 90.0f) {
    DrawSkyFace("effect\\skybox_back.bmp", projected, 0, 1, 2, 3);
  }
  if (rotation >= 90.0f && rotation < 270.0f) {
    DrawSkyFace("effect\\skybox_front.bmp", projected, 4, 5, 6, 7);
  }
  if (rotation >= 180.0f) {
    DrawSkyFace("effect\\skybox_left.bmp", projected, 0, 2, 4, 6);
  } else {
    DrawSkyFace("effect\\skybox_right.bmp", projected, 1, 3, 5, 7);
  }
  if (height > -11.0f) {
    DrawSkyFace("effect\\skybox_top.bmp", projected, 0, 1, 4, 5);
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->vertices[0].x = projected[8].x;
  primitive->vertices[0].y = projected[8].y;
  primitive->vertices[0].z = projected[8].z;
  primitive->vertices[0].rhw = projected[8].rhw;
  primitive->vertices[1].x = projected[9].x;
  primitive->vertices[1].y = projected[8].y;
  primitive->vertices[1].z = projected[8].z;
  primitive->vertices[1].rhw = projected[8].rhw;
  primitive->vertices[2].x = projected[8].x;
  primitive->vertices[2].y = projected[9].y;
  primitive->vertices[2].z = projected[8].z;
  primitive->vertices[2].rhw = projected[8].rhw;
  primitive->vertices[3].x = projected[9].x;
  primitive->vertices[3].y = projected[9].y;
  primitive->vertices[3].z = projected[8].z;
  primitive->vertices[3].rhw = projected[8].rhw;
  primitive->vertices[0].u = 0.0f;
  primitive->vertices[0].v = 0.0f;
  primitive->vertices[1].u = 1.0f;
  primitive->vertices[1].v = 0.0f;
  primitive->vertices[2].u = 0.0f;
  primitive->vertices[2].v = 1.0f;
  primitive->vertices[3].u = 1.0f;
  primitive->vertices[3].v = 1.0f;
  primitive->sourceBlend = 5;
  primitive->destinationBlend = 6;
  primitive->texture = nullptr;
  primitive->vertices[0].diffuse = 0x00FFFFFF;
  primitive->vertices[1].diffuse = 0x00FFFFFF;
  primitive->vertices[2].diffuse = 0xFFFFFFFF;
  primitive->vertices[3].diffuse = 0xFFFFFFFF;
  g_renderer->Submit(primitive, 3);

  this->m_flare.Update(0.0f, eye, target);
  this->m_flare.Render(camera.values, eye, target);
}

// OFFSET: 0x0075FE30
// FUNCTION: fn_45903
FlareElement CLensFlare::MakeElement(int type, float distance, float size, float red, float green, float blue) const {
  FlareElement element;
  element.type = type;
  element.distance = distance;
  element.size = size;
  element.red = red;
  element.green = green;
  element.blue = blue;
  return element;
}

// OFFSET: 0x0075E530
// FUNCTION: fn_08050
void CLensFlare::Init() {
  this->m_elements[0] = this->MakeElement(2, 181.8f, 0.45f, 0.0f, 0.0f, 1.0f);
  this->m_elements[1] = this->MakeElement(1, 98.1f, 0.3f, 0.0f, 1.0f, 0.0f);
  this->m_elements[2] = this->MakeElement(1, 95.4f, 0.67f, 1.0f, 0.0f, 0.0f);
  this->m_elements[3] = this->MakeElement(1, 92.700005f, 0.06f, 0.6f, 0.0f, 0.0f);
  this->m_elements[4] = this->MakeElement(3, 90.0f, 0.15f, 0.4f, 0.0f, 0.0f);
  this->m_elements[5] = this->MakeElement(1, 4.5f, 0.7f, 0.3f, 0.0f, 0.0f);
  this->m_elements[6] = this->MakeElement(3, 1.8000001f, 0.07f, 0.3f, 0.0f, 0.0f);
  this->m_elements[7] = this->MakeElement(0, 0.0f, 0.06f, 0.3f, 0.0f, 0.0f);
  this->m_elements[8] = this->MakeElement(4, -2.25f, 0.15f, 0.5f, 0.0f, 0.0f);
  this->m_elements[9] = this->MakeElement(4, -3.6000001f, 0.03f, 0.6f, 0.0f, 0.0f);
  this->m_elements[10] = this->MakeElement(4, -5.4f, 0.06f, 0.4f, 0.0f, 0.0f);
  this->m_elements[11] = this->MakeElement(4, -90.0f, 0.04f, 0.2f, 0.0f, 0.0f);

  for (int i = 0; i < 12; ++i) {
    const FlareElement& element = this->m_elements[i];
    const std::uint32_t red = static_cast<int>(element.red * 255.0f) & 0xFF;
    const std::uint32_t green = static_cast<int>(element.green * 255.0f) & 0xFF;
    const std::uint32_t blue = static_cast<int>(element.blue * 255.0f) & 0xFF;
    this->m_colors[i] = ((((0xFF00 + red) << 8) + green) << 8) + blue;
  }
}

// OFFSET: 0x0075EA20
// FUNCTION: fn_08051
__declspec(noinline) void CLensFlare::Update(float unused, vector3d eye, vector3d target) {
  this->m_position = g_weatherEffect.GetWindDirection();
  this->m_position.x += eye.x;
  this->m_position.z += eye.z;

  const vector3d origin = eye;
  vector3d view;
  view = vector3d{target.x - eye.x, target.y - eye.y, target.z - eye.z};
  float inverse = 1.0f / SquareRoot(view.y * view.y + view.x * view.x + view.z * view.z);
  view *= inverse;

  vector3d light;
  light.x = this->m_position.x - target.x;
  light.y = this->m_position.y - target.y;
  light.z = this->m_position.z - target.z;
  inverse = 1.0f / SquareRoot(light.y * light.y + light.x * light.x + light.z * light.z);
  light.x = inverse * light.x;
  light.y = inverse * light.y;
  light.z = inverse * light.z;

  vector3d center;
  center.x = view.x * 1.01f + origin.x;
  center.y = view.y * 1.01f + origin.y;
  center.z = view.z * 1.01f + origin.z;

  inverse = 1.0f / SquareRoot(view.y * view.y + view.x * view.x + view.z * view.z);
  const float viewX = view.x * inverse;
  const float viewY = view.y * inverse;
  const float difference = viewX - viewY;
  const float sum = viewY + viewX;
  const float scale = SquareRoot(sum * sum + difference * difference);

  for (int i = 0; i < 12; ++i) {
    FlareElement& element = this->m_elements[i];
    element.position.y = element.distance * light.y + center.y;
    element.position.x = element.distance * light.x + center.x;
    element.position.z = element.distance * light.z + center.z;
    element.scaledSize = scale * element.size;
  }
}

// OFFSET: 0x0075ECC0
// FUNCTION: fn_08052
void CLensFlare::Render(const float* matrix, vector3d eye, vector3d target) {
  for (int i = 0; i < 12; ++i) {
    FlareElement& element = this->m_elements[i];
    std::sprintf(this->m_textureName, "effect\\flare%d.bmp", element.type);

    const vector3d position = element.position;
    vector3d view;
    view.x = target.x - eye.x;
    view.y = target.y - eye.y;
    view.z = target.z - eye.z;
    const float inversePosition = 1.0f / SquareRoot(position.y * position.y + position.x * position.x + position.z * position.z);
    const float inverseView = 1.0f / SquareRoot(view.y * view.y + view.x * view.x + view.z * view.z);
    const float dot = inverseView * view.y * (position.y * inversePosition) + inverseView * view.x * (position.x * inversePosition) +
                      inverseView * view.z * (position.z * inversePosition);
    if (dot < 0.0f) {
      RenderPrimitive* primitive = static_cast<PrimitivePoolAccess&>(g_renderer->primitives).AcquireLocal();
      RenderVertex* vertices = primitive->vertices;
      g_renderer->Project(&element.position.x, matrix, vertices[0]);
      vertices[3] = vertices[0];
      vertices[2] = vertices[0];
      vertices[1] = vertices[0];
      vertices[0].x = vertices[0].x - element.size * 290.0f;
      vertices[0].y = vertices[0].y - element.size * 290.0f;
      vertices[1].x = element.size * 290.0f + vertices[1].x;
      vertices[1].y = vertices[1].y - element.size * 290.0f;
      vertices[2].x = vertices[2].x - element.size * 290.0f;
      vertices[2].y = element.size * 290.0f + vertices[2].y;
      vertices[3].x = element.size * 290.0f + vertices[3].x;
      vertices[3].y = element.size * 290.0f + vertices[3].y;

      primitive->vertices[0].u = 0.0f;
      primitive->vertices[0].v = 0.0f;
      primitive->vertices[1].u = 1.0f;
      primitive->vertices[1].v = 0.0f;
      primitive->vertices[2].u = 0.0f;
      primitive->vertices[2].v = 1.0f;
      primitive->vertices[3].u = 1.0f;
      primitive->vertices[3].v = 1.0f;
      primitive->sourceBlend = 2;
      primitive->destinationBlend = 2;
      primitive->texture = g_texMgr.Load(this->m_textureName, 0, false, true, false);
      primitive->vertices[0].diffuse = this->m_colors[i];
      primitive->vertices[1].diffuse = this->m_colors[i];
      primitive->vertices[2].diffuse = this->m_colors[i];
      primitive->vertices[3].diffuse = this->m_colors[i];
      g_renderer->Submit(primitive, 5);
    }
  }
}
