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
// FUNCTION: fn_18811
std::string EmitterInfoKey(std::string effect) {
  std::string key = "_" + effect + "_emitterInfo";
  const std::size_t marker = key.find("@");
  if (marker != std::string::npos) {
    key.erase(marker, 1);
  }

  return key;
}

// OFFSET: 0x00B3C880
// FUNCTION: fn_18812
vector2d RandomDiscPoint(float x, float y, long long radius) {
  const double turn = rand() / 32767.0;
  const double distance = SquareRoot(static_cast<float>(rand() / 32767.0)) * static_cast<float>(radius);
  vector2d point;
  const double angle = (turn + turn) * 3.141592653589;
  point.x = static_cast<float>(x + ClientCos(static_cast<float>(angle)) * distance);
  point.y = static_cast<float>(y + ClientSin(static_cast<float>(angle)) * distance);
  return point;
}

// OFFSET: 0x00B3CBA0
// FUNCTION: fn_18813
CEZ2STRParticle::CEZ2STRParticle(vector3d position, int flags) : CParticle(position, flags) {
  this->Clear();
}

// OFFSET: 0x00B3CF20

// OFFSET: 0x00B3CFC0
// FUNCTION: fn_18815
void CEZ2STRParticle::Slot01() {
  this->m_res = CResPtr<CEZeffectRes>();
}

// OFFSET: 0x00B3D030
// FUNCTION: fn_18816
std::string CEZ2STRParticle::ResolvePath(const std::string& name) {
  std::smatch match;
  const bool matched = std::regex_match(name, match, std::regex("^(.+\\\\)(\\w+\\.str)$"));
  if (matched && match.size() > 2) {
    std::string path = match[1].str() + "min_" + match[2].str();
    if (ResourceMgr::Instance().Exists(path) == true) {
      return path;
    }
  }

  return name;
}

// OFFSET: 0x00B3D540
// FUNCTION: fn_18817
void CEZ2STRParticle::Clear() {
  this->Reset();
  this->m_res = CResPtr<CEZeffectRes>();
  this->m_data = nullptr;
  this->m_finishedClips = 0;
  this->m_frame = 0;
  this->m_loop = false;
  std::memset(this->m_cursors, 0, sizeof(this->m_cursors));
  std::memset(this->m_transforms, 0, sizeof(this->m_transforms));
  this->m_endFrame = 0;
  this->m_counting = false;
  this->m_lifeTime = 0;
  this->m_repeat = 0;
  this->m_delay = 0;
  this->m_angle = 0;
  this->m_pitch = 0;
  this->m_yaw = 0;
  this->m_scale = 1.0f;
  this->m_alpha = 1.0f;
  this->m_windowId = -1;
  this->m_attachToWindow = false;
  this->m_visible = true;
  this->m_windowOffset.x = 0;
  this->m_windowOffset.y = 0;
  this->m_windowOffset.z = 0;
  this->m_shiftX = 0;
  this->m_shiftY = 0;
  this->m_lift = 0;
  this->m_followActor = false;
  this->m_flipX = false;
  this->m_flipY = false;
  this->m_hold = false;
  this->m_screenShiftY = 0;
  this->m_screenShiftX = 0;
  this->m_groundHeight = -1000.0f;
}

// OFFSET: 0x00B3D6F0
// FUNCTION: fn_18818
void CEZ2STRParticle::Restart() {
  CEZeffectRes* resource = this->m_res;
  if (!resource) {
    return;
  }

  this->m_finishedClips = 0;
  this->m_frame = 0;
  this->m_data = reinterpret_cast<EZeffectData*>(&resource->maximumFrame);
  this->m_clipLimit = resource->clipCount;
  if (resource->clips[0].frameCount == 0) {
    this->m_clipLimit = resource->clipCount - 1;
  }

  for (int i = 0; i < this->m_data->clipCount; ++i) {
    this->m_cursors[i] = 0;
  }

  unsigned char* bytes = reinterpret_cast<unsigned char*>(this->m_transforms);
  for (int count = sizeof(this->m_transforms); count != 0; --count) {
    *bytes++ = 0;
  }
}

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
// FUNCTION: fn_18820
void CEZ2STRParticle::Load(const std::string& name) {
  if (g_serviceOptions.Get(229)) {
    this->Open(this->ResolvePath(name));
  } else {
    this->Open(name);
  }
}

// OFFSET: 0x00B3DA60
// FUNCTION: fn_18821
bool CEZ2STRParticle::Process(float x, float y, float z) {
  if (!this->m_res) {
    return false;
  }

  if (this->m_delay > 0) {
    this->m_delay -= 16;
    return true;
  }

  const int lifeTime = this->m_lifeTime;
  if (lifeTime != 0) {
    this->m_counting = true;
    if (this->m_frame - lifeTime >= 0) {
      this->life = -1.0f;
      return false;
    }
  } else if (this->m_counting == true && this->m_frame - lifeTime >= 0) {
    this->life = -1.0f;
    return false;
  }

  if (this->m_finishedClips >= this->m_clipLimit) {
    if (this->m_hold) {
      this->UpdatePosition(x, y, z);
      return true;
    }

    if (!this->m_loop && this->m_frame - this->m_endFrame >= 0 && this->m_frame - lifeTime >= 0) {
      if (this->m_repeat <= 0) {
        this->life = -1.0f;
        return false;
      }

      --this->m_repeat;
    }

    this->Restart();
  }

  this->UpdatePosition(x, y, z);

  for (int i = 0; i < this->m_data->clipCount; ++i) {
    this->m_active[i] = false;
    EZeffectClip* clip = &this->m_data->clips[i];
    if (!clip || !clip->frameCount || this->m_cursors[i] == -1) {
      continue;
    }

    const EZeffectKeyFrame* key = &clip->frames[this->m_cursors[i]];
    const int elapsed = this->m_frame - key->frame;
    if (elapsed < 0) {
      continue;
    }

    bool advance;
    if (clip->frameCount == 1) {
      advance = key->frame == this->m_frame;
    } else if (this->m_cursors[i] + 1 < clip->frameCount) {
      advance = elapsed != 0 && key[1].frame == this->m_frame;
    } else {
      advance = key->frame == this->m_frame;
    }

    if (advance) {
      const int cursor = this->m_cursors[i] + 1;
      this->m_cursors[i] = cursor;
      if (cursor < clip->frameCount - 1) {
        ++key;
      } else {
        this->m_cursors[i] = -1;
        ++this->m_finishedClips;
      }
    }

    EZeffectTransform& transform = this->m_transforms[i];
    if (key->type == 0) {
      transform = key->transform;
      if (this->m_cursors[i] != -1 && clip->frameCount > 1) {
        const int next = this->m_cursors[i] + 1;
        if (next < clip->frameCount && key[1].frame == key->frame) {
          this->m_cursors[i] = next;
        }
      }
    } else {
      transform.x += key->transform.x;
      transform.y += key->transform.y;
      transform.u += key->transform.u;
      transform.v += key->transform.v;
      transform.us += key->transform.us;
      transform.vs += key->transform.vs;
      transform.u2 += key->transform.u2;
      transform.v2 += key->transform.v2;
      transform.us2 += key->transform.us2;
      transform.vs2 += key->transform.vs2;
      transform.ax[0] += key->transform.ax[0];
      transform.ax[1] += key->transform.ax[1];
      transform.ax[2] += key->transform.ax[2];
      transform.ax[3] += key->transform.ax[3];
      transform.ay[0] += key->transform.ay[0];
      transform.ay[1] += key->transform.ay[1];
      transform.ay[2] += key->transform.ay[2];
      transform.ay[3] += key->transform.ay[3];
      transform.crR += key->transform.crR;
      transform.crG += key->transform.crG;
      transform.crB += key->transform.crB;
      transform.crA += key->transform.crA;
      transform.rz += key->transform.rz;

      switch (key->transform.anitype) {
      case 1:
        transform.aniframe += key->transform.aniframe;
        break;
      case 2:
        transform.aniframe += key->transform.anidelta;
        if (transform.aniframe >= static_cast<float>(clip->textureCount)) {
          transform.aniframe = static_cast<float>(clip->textureCount) - 1.0f;
        }
        break;
      case 3: {
        const float frame = key->transform.anidelta + transform.aniframe;
        transform.aniframe = frame;
        const float count = static_cast<float>(clip->textureCount);
        if (frame >= count) {
          transform.aniframe = frame - static_cast<float>(static_cast<int>(frame / count)) * count;
        }
        break;
      }
      case 4: {
        float frame = transform.aniframe - key->transform.anidelta;
        transform.aniframe = frame;
        if (frame < 0.0f) {
          const float count = static_cast<float>(clip->textureCount);
          frame -= count * 0.0f;
          if (frame < 0.0f) {
            frame += count;
          }
          transform.aniframe = frame;
        }
        break;
      }
      case 5: {
        const int last = clip->textureCount - 1;
        const int frame = static_cast<int>(static_cast<float>(elapsed) * key->transform.anidelta + transform.aniframe);
        const int bounce = frame / last;
        if (bounce & 1) {
          transform.aniframe = static_cast<float>((bounce + 1) * last - frame);
        } else {
          transform.aniframe = static_cast<float>(frame - bounce * last);
        }
        break;
      }
      }
    }

    this->m_active[i] = true;
  }

  ++this->m_frame;
  if (this->m_counting == true) {
    this->m_lifeTime -= 16;
  }

  return true;
}

// OFFSET: 0x00B3E010
// FUNCTION: fn_18822
void CEZ2STRParticle::UpdatePosition(float x, float y, float z) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  RenderVertex screen = {0, 0, 0, 0, 0, 0xFF000000, 0, 0};
  g_renderer->viewport.Projection().Project(&this->position.x, mode->camera->frame.view.values, screen);
  this->m_screen.x = screen.x + x;
  this->m_screen.y = screen.y + y;
  this->m_screen.z = screen.z + z;

  if (this->flags == 0x205 || this->flags == 0x225) {
    this->m_screen.x = static_cast<float>(g_renderer->viewport.centerX) + this->m_windowOffset.x;
    this->m_screen.y =
      (static_cast<float>(g_renderer->viewport.centerY) - static_cast<float>(g_renderer->viewport.centerY) * 0.2f) + this->m_windowOffset.y;
    if (this->m_attachToWindow == true) {
      UIWindow* window = g_windowMgr.LookupWindow(this->m_windowId);
      if (window) {
        this->m_visible = true;
        this->m_screen.x = static_cast<float>(window->geometry.x) + this->m_windowOffset.x;
        this->m_screen.y = static_cast<float>(window->geometry.y) + this->m_windowOffset.y;
        return;
      }

      this->m_visible = false;
    }
  }
}

// OFFSET: 0x00B3E170
// FUNCTION: fn_18823
void CEZ2STRParticle::DrawBillboards(const matrix& view) {
  if (!this->m_data || !this->m_visible) {
    return;
  }

  for (int i = 1; i < this->m_data->clipCount; ++i) {
    if (this->m_active[i]) {
      this->RenderBillboard(&this->m_data->clips[i], &this->m_transforms[i], view, i);
    }
  }
}

// OFFSET: 0x00B3E1F0
// FUNCTION: fn_18824
void CEZ2STRParticle::DrawScreen(const matrix& view) {
  if (!this->m_data || !this->m_visible) {
    return;
  }

  for (int i = 1; i < this->m_data->clipCount; ++i) {
    if (this->m_active[i]) {
      this->RenderScreen(&this->m_data->clips[i], &this->m_transforms[i], view, i);
    }
  }
}

// OFFSET: 0x00B3E270
// FUNCTION: fn_18825
void CEZ2STRParticle::RenderBillboard(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  SetClipCorners(vertices, *transform);

  CTexture* texture = ClipTexture(clip, *transform, this->m_texturePrefix);
  const float scaleU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
  const float scaleV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());

  matrix world;
  world.MakeZRotation(ToRadians(static_cast<float>(transform->rz / 2.8444444444444)));
  vector3d offset;
  offset.x = transform->x - 320.0f;
  offset.z = 0;
  offset.y = transform->y - 240.0f;
  world.AppendTranslate(&offset.x);
  const float scale = this->m_scale;
  const float scaleY = this->m_flipY ? scale * -1.0f : scale;
  const float scaleX = this->m_flipX ? scale * -1.0f : scale;
  world.AppendScale(scaleX, scaleY, 1.0f);
  world.AppendZRotation(ToRadians(this->m_angle));
  world.AppendXRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch));
  world.AppendYRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw));
  world.AppendTranslate(&this->position.x);

  matrix combined;
  combined.Compose(world, view);
  for (int i = 0; i < 4; ++i) {
    vector3d point = {vertices[i].x, vertices[i].y, vertices[i].z};
    g_renderer->viewport.Projection().Project(&point.x, combined.values, vertices[i]);
    vertices[i].rhw = this->depthOffset + vertices[i].rhw;
    vertices[i].z = RhwToDepth(vertices[i].rhw);
    vertices[i].u = scaleU * vertices[i].u;
    vertices[i].v = scaleV * vertices[i].v;
    vertices[i].specular = 0xFFFFFFFF;
    vertices[i].diffuse = ClipColor(*transform, this->m_alpha);
  }

  primitive->texture = texture;
  primitive->unknown_0018 = transform->srcalpha;
  primitive->unknown_001C = transform->destalpha;
  primitive->unknown_0020 = transform->mtpreset;
  this->flags |= 0x20;
  this->submit(primitive, this->flags);
}

// OFFSET: 0x00B3E6E0
// FUNCTION: fn_18826
void CEZ2STRParticle::RenderScreen(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  SetClipCorners(vertices, *transform);

  float screenX;
  float screenY;
  float rhw;
  g_renderer->ProjectViewport(&this->position.x, view.values, &screenX, &screenY, &rhw);
  const float sine = ClientSin(ToRadians(static_cast<float>(transform->rz / 2.8444444444444)));
  const float cosine = ClientCos(ToRadians(static_cast<float>(transform->rz / 2.8444444444444)));
  float scale = g_renderer->ScaleDistance(rhw);
  if (this->m_followActor == true) {
    if (CRenderObject* actor = FindRenderObject(this->m_actorId)) {
      actor->ScaleByFadePulse(scale);
    }
  }

  if (this->m_attachToWindow == true) {
    switch (this->m_windowId) {
    case 361:
    case 10022:
    case 10023:
    case 10026:
      scale = 1.0f;
      break;
    }
  }

  for (int i = 0; i < 4; ++i) {
    const float rotated = vertices[i].x * sine + vertices[i].y * cosine;
    vertices[i].x = ((vertices[i].x * cosine - vertices[i].y * sine) + (transform->x - 320.0f)) * scale + this->m_screen.x;
    if (IsScreenShakeActive()) {
      const float shake = screenY - this->m_screen.y;
      vertices[i].y = (this->m_screen.y - ((transform->y - 240.0f) + rotated) * scale) + (shake + shake);
    } else {
      vertices[i].y = ((transform->y - 240.0f) + rotated) * scale + this->m_screen.y;
    }
  }

  for (int i = 0; i < 4; ++i) {
    float x = (vertices[i].x - screenX) * this->m_scale;
    float y = (vertices[i].y - screenY) * this->m_scale;
    if (this->m_flipX) {
      x *= -1.0f;
    }
    if (this->m_flipY) {
      y *= -1.0f;
    }
    vertices[i].x = x + screenX;
    vertices[i].y = y + screenY;
  }

  if (this->m_angle > 0.0f || this->m_pitch > 0.0f || this->m_yaw > 0.0f) {
    matrix rotation;
    rotation.Identity();
    if (this->m_angle > 0.0f) {
      rotation.AppendZRotation(ToRadians(this->m_angle));
    }
    if (this->m_pitch > 0.0f) {
      rotation.AppendXRotation(ToRadians(this->m_pitch));
    }
    if (this->m_yaw > 0.0f) {
      rotation.AppendYRotation(ToRadians(this->m_yaw));
    }

    const float* values = rotation.values;
    for (int i = 0; i < 4; ++i) {
      const float x = vertices[i].x - screenX;
      const float y = vertices[i].y - screenY;
      vertices[i].x = values[0] * x + values[3] * y + values[6] * 0.0f + values[9] + screenX;
      vertices[i].y = values[1] * x + values[4] * y + values[7] * 0.0f + values[10] + screenY;
    }
  }

  for (long long i = 0; i < 4; ++i) {
    vertices[i].x = this->m_shiftX + vertices[i].x;
    vertices[i].y = this->m_shiftY + vertices[i].y;
  }

  for (long long i = 0; i < 4; ++i) {
    vertices[i].y -= static_cast<float>(this->m_lift);
  }

  for (long long i = 0; i < 4; ++i) {
    vertices[i].x += this->m_screenShiftX;
    vertices[i].y -= this->m_screenShiftY;
  }

  CTexture* texture = ClipTexture(clip, *transform, this->m_texturePrefix);
  const float depth = static_cast<float>(index) * 1e-05f;
  const float scaleU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
  const float scaleV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
  for (int i = 0; i < 4; ++i) {
    vertices[i].u = scaleU * vertices[i].u;
    vertices[i].v = scaleV * vertices[i].v;
    vertices[i].specular = 0xFFFFFFFF;
    vertices[i].z = 3.102122e-05f;
    vertices[i].diffuse = ClipColor(*transform, this->m_alpha);
    vertices[i].rhw = rhw - depth;
  }

  primitive->texture = texture;
  primitive->unknown_0018 = transform->srcalpha;
  primitive->unknown_001C = transform->destalpha;
  primitive->unknown_0020 = transform->mtpreset;
  this->flags |= 0x20;
  this->submit(primitive, this->flags);
}

// OFFSET: 0x00B3F050
// FUNCTION: fn_18827
void CEZ2STRParticle::RenderGround(EZeffectClip* clip, EZeffectTransform* transform, const matrix& view, int index) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  SetClipCorners(vertices, *transform);

  CTexture* texture = ClipTexture(clip, *transform, this->m_texturePrefix);
  const float scaleU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
  const float scaleV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());

  matrix world;
  world.MakeZRotation(ToRadians(static_cast<float>(transform->rz / 2.8444444444444)));
  vector3d point;
  point.x = transform->x - 320.0f;
  point.z = 0;
  point.y = transform->y - 240.0f;
  world.AppendTranslate(&point.x);
  const float scale = this->m_scale;
  const float scaleY = this->m_flipY ? scale * -1.0f : scale;
  const float scaleX = this->m_flipX ? scale * -1.0f : scale;
  world.AppendScale(scaleX, scaleY, 1.0f);
  world.AppendXRotation(ToRadians(-90.0f));
  world.AppendYRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw + this->m_angle));
  world.AppendTranslate(&this->position.x);

  const float* values = world.values;
  for (int i = 0; i < 4; ++i) {
    const float x = vertices[i].x;
    const float y = vertices[i].y;
    const float z = vertices[i].z;
    point.x = x * values[0] + y * values[3] + z * values[6] + values[9];
    point.y = x * values[1] + y * values[4] + z * values[7] + values[10];
    point.z = x * values[2] + y * values[5] + z * values[8] + values[11];
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    point.y = mode->scene->terrain->Grid().PlaneHeight(point.x, point.z) - 2.0f;
    if (this->m_groundHeight != -1000.0f && point.y - this->m_groundHeight > 5.0f) {
      point.y = this->m_groundHeight;
    }
    this->m_groundHeight = point.y;

    g_renderer->viewport.Projection().Project(&point.x, view.values, vertices[i]);
    vertices[i].rhw = this->depthOffset + vertices[i].rhw;
    vertices[i].z = RhwToDepth(vertices[i].rhw);
    vertices[i].u = scaleU * vertices[i].u;
    vertices[i].v = scaleV * vertices[i].v;
    vertices[i].specular = 0xFFFFFFFF;
    vertices[i].diffuse = ClipColor(*transform, this->m_alpha);
  }

  primitive->texture = texture;
  primitive->unknown_0018 = transform->srcalpha;
  primitive->unknown_001C = transform->destalpha;
  primitive->unknown_0020 = transform->mtpreset;
  this->flags |= 0x20;
  this->submit(primitive, this->flags);
}

// OFFSET: 0x00B3F550
// FUNCTION: fn_18828
void CEZ2STRParticle::DrawGround(const matrix& view) {
  if (!this->m_data || !this->m_visible) {
    return;
  }

  for (int i = 1; i < this->m_data->clipCount; ++i) {
    if (this->m_active[i]) {
      this->RenderGround(&this->m_data->clips[i], &this->m_transforms[i], view, i);
    }
  }
}

// OFFSET: 0x00B3F5D0
// FUNCTION: fn_18829
void CEZ2STRParticle::Advance(float amount) {
  if (!this->m_res) {
    return;
  }

  for (int count = static_cast<int>(static_cast<float>(this->m_data->maximumFrame) * amount); count > 0; --count) {
    this->Process(0.0f, -80.0f, 0.0f);
  }
}

// OFFSET: 0x00B3F630
// FUNCTION: fn_18830
CParticle::CParticle(vector3d position, int flags) : flags(flags), position(position) {
  for (RenderVertex& vertex : this->vertices) {
    vertex.x = 0;
    vertex.y = 0;
    vertex.z = 0;
    vertex.rhw = 0;
    vertex.diffuse = 0;
    vertex.specular = 0xFF000000;
    vertex.u = 0;
    vertex.v = 0;
  }

  for (vector3d& corner : this->unknown_0278) {
    corner.x = 0;
    corner.y = 0;
    corner.z = 0;
  }

  this->submit = SubmitParticlePrimitive;
  this->Reset();
}

// OFFSET: 0x00B3F950
// FUNCTION: fn_18831
CParticle::~CParticle() {
}

// OFFSET: 0x00B3F990
// FUNCTION: fn_18833
void CParticle::Reset() {
  this->red = 0xFF;
  this->green = 0xFF;
  this->blue = 0xFF;
  this->scale = 1.0f;
  this->unknown_00C0 = 0;
  this->texture = nullptr;
  this->alpha = 0xFF;
  this->unknown_00C4 = 0;
  this->life = 0;
  this->unknown_00C8 = 0;
  this->unknown_0014 = 0;
  this->unknown_00E8 = 0;
  this->unknown_00EC = 0;
  this->unknown_00D0 = 0;
  this->unknown_00D4 = 0;
  this->unknown_00D8 = 0;
  this->sourceBlend = 5;
  this->destinationBlend = 2;
  this->primitive = RenderPrimitive();
  this->transform.Identity();
  this->depthOffset = 0;

  for (int i = 0; i < 4; ++i) {
    this->vertices[i].x = 0;
    this->vertices[i].y = 0;
    this->vertices[i].z = 0;
    this->unknown_0278[i].x = 0;
    this->unknown_0278[i].y = 0;
    this->unknown_0278[i].z = 0;
  }

  InitParticleQuad(this->vertices);
  this->submit = SubmitParticlePrimitive;
}

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
// FUNCTION: fn_18838
void CParticle::Render(const ParticleQuad& quad) {
  this->Render(quad.billboard, quad.top, quad.bottom, quad.left, quad.right, quad.unused, quad.depth);
}

// OFFSET: 0x00B3FD10
// FUNCTION: fn_18839
void CParticle::Render(bool billboard, float top, float bottom, float left, float right, bool unused, bool depth) {
  bool shaped = false;
  for (const RenderVertex& vertex : this->vertices) {
    if (vertex.x != 0.0f || vertex.y != 0.0f || vertex.z != 0.0f) {
      shaped = true;
    }
  }

  if (!shaped) {
    return;
  }

  this->SetQuad(this->vertices, top, bottom, left, right);
  for (int i = 0; i < 4; ++i) {
    this->primitive.vertices[i] = this->vertices[i];
  }

  RenderVertex* output = this->primitive.vertices;
  const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const matrix* local;
  matrix placed;
  if (!billboard) {
    matrix translation;
    translation.Identity();
    translation.MakeTranslate(this->position.x, this->position.y, this->position.z);
    matrix rotation;
    rotation.Identity();
    rotation.AppendYRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw));
    matrix facing;
    facing.Compose(rotation, translation);
    placed.Compose(this->transform, facing);
    local = &placed;
  } else {
    local = &this->transform;
  }

  matrix world;
  world.Compose(*local, view);
  for (int i = 0; i < 4; ++i) {
    vector3d point = {output[i].x, output[i].y, output[i].z};
    g_renderer->viewport.Projection().Project(&point.x, world.values, output[i]);
    const float rhw = this->depthOffset + output[i].rhw;
    output[i].rhw = rhw;
    if (0.0f > rhw) {
      return;
    }

    float z = 0.0f;
    if (depth) {
      z = RhwToDepth(rhw);
    }
    output[i].z = z;
  }

  this->primitive.texture = this->texture;
  this->primitive.unknown_0018 = this->sourceBlend;
  this->primitive.unknown_001C = this->destinationBlend;
  g_renderer->queues.Submit(&this->primitive, static_cast<std::uint16_t>(this->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

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
// FUNCTION: fn_18846
void SubmitParticlePrimitive(RenderCommand* primitive, int flags) {
  g_renderer->queues.Submit(primitive, static_cast<std::uint16_t>(flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
