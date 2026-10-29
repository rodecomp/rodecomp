#include "Contents/Effects/Particle.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>
#include <cstring>
#include <regex>

// INLINED
static __forceinline float ToRadians(float degrees) {
  return (degrees / 180.0f) * 3.141592f;
}

// INLINED
static __forceinline void SetClipCorners(RenderVertex* vertices, const EZeffectTransform& transform) {
  if (transform.ax[1] > transform.ax[0]) {
    vertices[0].x = transform.ax[3];
    vertices[1].x = transform.ax[0];
    vertices[2].x = transform.ax[2];
    vertices[3].x = transform.ax[1];
    vertices[1].u = vertices[0].u = transform.u;
    vertices[3].u = vertices[2].u = transform.us + transform.u;
  } else {
    vertices[0].x = transform.ax[2];
    vertices[1].x = transform.ax[1];
    vertices[2].x = transform.ax[3];
    vertices[3].x = transform.ax[0];
    vertices[1].u = vertices[0].u = transform.us + transform.u;
    vertices[3].u = vertices[2].u = transform.u;
  }

  if (transform.ay[2] > transform.ay[1]) {
    vertices[0].y = transform.ay[3];
    vertices[1].y = transform.ay[0];
    vertices[2].y = transform.ay[2];
    vertices[3].y = transform.ay[1];
    vertices[3].v = vertices[1].v = transform.v;
    vertices[2].v = vertices[0].v = transform.vs + transform.v;
  } else {
    vertices[0].y = transform.ay[0];
    vertices[1].y = transform.ay[3];
    vertices[2].y = transform.ay[1];
    vertices[3].y = transform.ay[2];
    vertices[2].v = vertices[0].v = transform.v;
    vertices[3].v = vertices[1].v = transform.vs + transform.v;
  }
}

// INLINED
static __forceinline std::uint32_t ClipColor(const EZeffectTransform& transform, float alpha) {
  return ((((static_cast<int>(transform.crA * alpha) << 8) + static_cast<unsigned char>(transform.crR)) << 8) + static_cast<unsigned char>(transform.crG)) << 8 |
         static_cast<unsigned char>(transform.crB);
}

// INLINED
static __forceinline CTexture* ClipTexture(EZeffectClip* clip, const EZeffectTransform& transform, const std::string& prefix) {
  const int frame = static_cast<int>(transform.aniframe);
  if (prefix.size() == 0) {
    return clip->Texture(frame < 0 ? 0 : frame);
  }

  return clip->Texture(frame < 0 ? 0 : frame, prefix.c_str());
}

// OFFSET: 0x00B3C440
// FUNCTION: fn_18809
float RandomRange(float minimum, float maximum) {
  if (minimum == maximum) {
    return minimum;
  }

  const int low = static_cast<int>(minimum * 10.0f);
  const int value = rand();
  const int span = std::abs(low + static_cast<int>(maximum * -10.0f)) + 1;
  return static_cast<float>(value % span + low) * 0.1f;
}

// OFFSET: 0x00B3C4B0
// FUNCTION: fn_18810
std::string EmitterKey(std::string effect, std::string emitter) {
  std::string key = "_" + effect + "_" + emitter;
  const std::size_t marker = key.find("@");
  if (marker != std::string::npos) {
    key.erase(marker, 1);
  }

  return key;
}

// OFFSET: 0x00B3C6F0

// OFFSET: 0x00B3C880

// OFFSET: 0x00B3CBA0

// OFFSET: 0x00B3CF20

// OFFSET: 0x00B3CFC0

// OFFSET: 0x00B3D030

// OFFSET: 0x00B3D540

// OFFSET: 0x00B3D6F0

// OFFSET: 0x00B3D7A0
// FUNCTION: fn_18819
void CEZ2STRParticle::Open(std::string name) {
  this->m_res = CResPtr<CEZeffectRes>(static_cast<CEZeffectRes*>(ResourceMgr::Instance().Load(name)));
  this->m_name = name;

  char directory[256];
  std::memset(directory, 0, sizeof(directory) - 1);
  _splitpath(name.c_str(), nullptr, directory, nullptr, nullptr);
  this->m_texturePrefix = std::string(directory);
  this->Restart();
}

// OFFSET: 0x00B3D9D0

// OFFSET: 0x00B3DA60

// OFFSET: 0x00B3E010

// OFFSET: 0x00B3E170

// OFFSET: 0x00B3E1F0

// OFFSET: 0x00B3E270

// OFFSET: 0x00B3E6E0

// OFFSET: 0x00B3F050

// OFFSET: 0x00B3F550

// OFFSET: 0x00B3F5D0

// OFFSET: 0x00B3F630

// OFFSET: 0x00B3F950
// FUNCTION: fn_18831
CParticle::~CParticle() {
}

// OFFSET: 0x00B3F990

// OFFSET: 0x00B3FBF0
// FUNCTION: fn_18834
void ParticleUnused420() {
}

// OFFSET: 0x00B3FC00
// FUNCTION: fn_18835
void ParticleUnused421() {
}

// OFFSET: 0x00B3FC10
// FUNCTION: fn_18836
void CParticle::SetQuad(RenderVertex* vertices, float top, float bottom, float left, float right) {
  const std::uint32_t color = ((this->alpha << 8 | this->red) << 8 | this->green) << 8 | this->blue;
  vertices[0].diffuse = color;
  vertices[1].diffuse = color;
  vertices[2].diffuse = color;
  vertices[3].diffuse = color;

  vertices[0].v = top;
  vertices[1].v = top;
  vertices[0].u = left;
  vertices[1].u = right;
  vertices[2].u = left;
  vertices[2].v = bottom;
  vertices[3].u = right;
  vertices[3].v = bottom;
}

// OFFSET: 0x00B3FC80
// FUNCTION: fn_18837
void InitParticleQuad(RenderVertex* vertices) {
  vertices[0].x = -1.0f;
  vertices[0].y = 1.0f;
  vertices[0].z = 1.0f;
  vertices[1].x = 1.0f;
  vertices[1].y = 1.0f;
  vertices[1].z = 1.0f;
  vertices[2].x = -1.0f;
  vertices[2].y = -1.0f;
  vertices[2].z = 1.0f;
  vertices[3].x = 1.0f;
  vertices[3].y = -1.0f;
  vertices[3].z = 1.0f;
}

// OFFSET: 0x00B3FCE0

// OFFSET: 0x00B3FD10

// OFFSET: 0x00B40100
// FUNCTION: fn_18840
void ParticleUnused422() {
}

// OFFSET: 0x00B40110
// FUNCTION: fn_18841
void ParticleUnused423() {
}

// OFFSET: 0x00B40120
// FUNCTION: fn_18842
void CParticle::SetColor(short redValue, short greenValue, short blueValue) {
  if (redValue != -1) {
    this->red = redValue;
  }
  if (greenValue != -1) {
    this->green = greenValue;
  }
  if (blueValue != -1) {
    this->blue = blueValue;
  }
}

// OFFSET: 0x00B40160
// FUNCTION: fn_18843
void CParticle::SetTexture(const char* name) {
  this->texture = g_texMgr.Load(name, 0, false, true, false);
}

// OFFSET: 0x00B40190
// FUNCTION: fn_18844
void CParticle::SetTexture(std::string name) {
  this->texture = g_texMgr.Load(name.c_str(), 0, false, true, false);
}

// OFFSET: 0x00B40210
