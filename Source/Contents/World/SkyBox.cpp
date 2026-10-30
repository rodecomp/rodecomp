#include "Contents/World/SkyBox.h"

#include "Contents/MapSkyMgr.h"
#include "Contents/Models/ModelActor.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdio>

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
  this->m_unknown_0140 = 0;
  this->m_vertices[8].x = -1250.0f;
  this->m_vertices[8].y = -350.0f;
  this->m_vertices[8].z = 1240.0f;
  this->m_vertices[9].x = 1250.0f;
  this->m_vertices[9].y = 100.0f;
  this->m_vertices[9].z = 1250.0f;

  this->m_flare.Init();
}

// OFFSET: 0x0075DFB0
// FUNCTION: fn_08046
FlareElement::FlareElement() : position{0.0f, 0.0f, 0.0f} {
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

// OFFSET: 0x0075E530
// FUNCTION: fn_08050
void CLensFlare::Init() {
  this->m_elements[0] = FlareElement(2, 181.8f, 0.45f, 0.0f, 0.0f, 1.0f);
  this->m_elements[1] = FlareElement(1, 98.1f, 0.3f, 0.0f, 1.0f, 0.0f);
  this->m_elements[2] = FlareElement(1, 95.4f, 0.67f, 1.0f, 0.0f, 0.0f);
  this->m_elements[3] = FlareElement(1, 92.700005f, 0.06f, 0.6f, 0.0f, 0.0f);
  this->m_elements[4] = FlareElement(3, 90.0f, 0.15f, 0.4f, 0.0f, 0.0f);
  this->m_elements[5] = FlareElement(1, 4.5f, 0.7f, 0.3f, 0.0f, 0.0f);
  this->m_elements[6] = FlareElement(3, 1.8000001f, 0.07f, 0.3f, 0.0f, 0.0f);
  this->m_elements[7] = FlareElement(0, 0.0f, 0.06f, 0.3f, 0.0f, 0.0f);
  this->m_elements[8] = FlareElement(4, -2.25f, 0.15f, 0.5f, 0.0f, 0.0f);
  this->m_elements[9] = FlareElement(4, -3.6000001f, 0.03f, 0.6f, 0.0f, 0.0f);
  this->m_elements[10] = FlareElement(4, -5.4f, 0.06f, 0.4f, 0.0f, 0.0f);
  this->m_elements[11] = FlareElement(4, -90.0f, 0.04f, 0.2f, 0.0f, 0.0f);

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
void CLensFlare::Update(const float* matrix, vector3d eye, vector3d target) {
  this->m_position = g_weatherEffect.GetWindDirection();
  this->m_position.x += eye.x;
  this->m_position.z += eye.z;

  const vector3d origin = eye;
  vector3d view;
  view.x = target.x - eye.x;
  view.y = target.y - eye.y;
  view.z = target.z - eye.z;
  float inverse = 1.0f / SquareRoot(view.y * view.y + view.x * view.x + view.z * view.z);
  view.x *= inverse;
  view.y *= inverse;
  view.z *= inverse;

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
    const float inversePosition = 1.0f / SquareRoot(position.x * position.x + position.y * position.y + position.z * position.z);
    const float inverseView = 1.0f / SquareRoot(view.y * view.y + view.x * view.x + view.z * view.z);
    const float dot = inverseView * view.y * (position.y * inversePosition) + inverseView * view.x * (position.x * inversePosition) +
                      inverseView * view.z * (position.z * inversePosition);
    if (dot < 0.0f) {
      RenderPrimitive* primitive = g_renderer->primitives.Acquire();
      RenderVertex* vertices = primitive->vertices;
      g_renderer->viewport.Projection().Project(&element.position.x, matrix, vertices[0]);
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
      primitive->unknown_0018 = 2;
      primitive->unknown_001C = 2;
      primitive->texture = g_texMgr.Load(this->m_textureName, 0, false, true, false);
      primitive->vertices[0].diffuse = this->m_colors[i];
      primitive->vertices[1].diffuse = this->m_colors[i];
      primitive->vertices[2].diffuse = this->m_colors[i];
      primitive->vertices[3].diffuse = this->m_colors[i];
      g_renderer->queues.Submit(primitive, 5, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
    }
  }
}
