#include "Contents/Models/ModelActor.h"

#include <cstring>
#include <windows.h>

namespace {
  // INLINED
  float Radians(float degrees) {
    return (degrees / 180.0f) * 3.141592f;
  }
}

// OFFSET: 0x00AB59A0
// FUNCTION: fn_17461
C3dActor::C3dActor()
    : position{}, rotation{}, scale{1, 1, 1}, offset{}, dirty(1), oriented{}, corners{}, minimum{}, maximum{}, enabled(1), renderFrame(0), occlusionEnabled(0),
      occluded(0), alpha(255), commandMode(0), color(0xFFFFFFFFu), playback(2), speed(0), shade(0) {
}

// OFFSET: 0x00AB6420
// FUNCTION: fn_17470
C3dActor::~C3dActor() {
  for (ModelVolume*& volume : this->volumes) {
    delete volume;
    volume = nullptr;
  }
}

// OFFSET: 0x00AB5BB0
// FUNCTION: fn_17462
C3dActorV1::C3dActorV1() : root(nullptr), frame(0), animationLength(0), lastTick(0) {
}

// INLINED
C3dActorV1::~C3dActorV1() {
  delete this->root;
  this->root = nullptr;
}

// OFFSET: 0x00AB5C30
// FUNCTION: fn_17463
C3dActorV2::C3dActorV2() : framesPerSecond(0), duration(0), startTick(GetTickCount()) {
}

// INLINED
C3dActorV2::~C3dActorV2() {
  for (C3dNodeV2*& root : this->roots) {
    delete root;
    root = nullptr;
  }
}

// OFFSET: 0x00AB6E90
// FUNCTION: fn_17479
void C3dActorV1::Assign(const C3dModelRes& resource) {
  this->shade = resource.shade;
  this->animationLength = static_cast<const C3dModelResV1&>(resource).animationLength;
  this->root = new C3dNodeV1;
  this->root->model = this;
  this->root->Initialize(*resource.nodes.front());
  this->Animate(0);
  this->UpdateBounds();
}

// OFFSET: 0x00AB6F50
// FUNCTION: fn_17480
void C3dActorV2::Assign(const C3dModelRes& resource) {
  const auto& modern = static_cast<const C3dModelResV2&>(resource);
  this->shade = resource.shade;
  this->framesPerSecond = modern.framesPerSecond;
  const float inverse = 1.0f / modern.framesPerSecond;
  const int milliseconds = 1000 * modern.animationLength;
  this->duration = static_cast<float>(milliseconds) * inverse;

  for (const std::string& name : resource.rootNames) {
    auto* root = new C3dNodeV2;
    root->model = this;
    root->Initialize(*resource.FindNode(name.c_str()));
    this->roots.push_back(root);
  }

  this->Animate(0);
  this->UpdateBounds();
}

// OFFSET: 0x00ABB890
// FUNCTION: fn_17516
void C3dActor::UpdateTransform() {
  this->transform.SetTranslation(this->offset[0], this->offset[1], this->offset[2]);
  this->transform.Scale(this->scale[0], this->scale[1], this->scale[2]);
  this->transform.RotateY(Radians(this->rotation[1]));
  this->transform.RotateX(Radians(this->rotation[0]));
  this->transform.RotateZ(Radians(this->rotation[2]));
  this->transform.Translate(this->position);
  this->inverseTransform.Inverse(this->transform);
  this->dirty = 0;
}

// OFFSET: 0x00ABB970
// FUNCTION: fn_17517
void C3dActorV2::UpdateTransform() {
  this->transform.Identity();
  this->transform.values[4] = -1.0f;
  this->transform.Scale(this->scale[0], this->scale[1], this->scale[2]);
  this->transform.RotateY(Radians(this->rotation[1]));
  this->transform.RotateX(Radians(this->rotation[0]));
  this->transform.RotateZ(Radians(this->rotation[2]));
  this->transform.Translate(this->position);
  this->inverseTransform.Inverse(this->transform);
  this->dirty = 0;
}

// OFFSET: 0x00ABBA60
// FUNCTION: fn_17519
void C3dActorV1::Prepare(ModelLightingContext& context) {
  if (this->dirty == 1) {
    this->UpdateTransform();
  }

  const float* previousPosition = context.position;
  const std::uint32_t* previousColor = context.color;
  const int* previousShade = context.shade;

  context.position = this->position;
  context.color = &this->color;
  context.shade = &this->shade;
  this->root->Prepare(this->transform.values, context);
  context.position = previousPosition;
  context.color = previousColor;
  context.shade = previousShade;
}

// OFFSET: 0x00ABBAA0
// FUNCTION: fn_17520
void C3dActorV2::Prepare(ModelLightingContext& context) {
  if (this->dirty == 1) {
    this->UpdateTransform();
  }

  const float* previousPosition = context.position;
  const std::uint32_t* previousColor = context.color;
  const int* previousShade = context.shade;

  context.position = this->position;
  context.color = &this->color;
  context.shade = &this->shade;

  for (C3dNodeV2* root : this->roots) {
    root->Prepare(this->transform.values, context);
  }

  context.position = previousPosition;
  context.color = previousColor;
  context.shade = previousShade;
}

// OFFSET: 0x00AB9070
// FUNCTION: fn_17504
void C3dActorV1::Animate(double frame) {
  this->root->Animate(frame);
  this->dirty = 1;
}

// OFFSET: 0x00AB90A0
// FUNCTION: fn_17505
void C3dActorV2::Animate(double frame) {
  const float selected = static_cast<float>(static_cast<double>(this->framesPerSecond) * frame / 1000.0);

  for (C3dNodeV2* root : this->roots) {
    root->Animate(static_cast<double>(selected));
  }

  this->dirty = 1;
}

// OFFSET: 0x00AB9010
// FUNCTION: fn_17501
void C3dActor::SetSpeed(float speed) {
  this->speed = speed;

  if (speed != 0.0f) {
    this->speed = speed / 3.0f;
  }
}

// OFFSET: 0x00AB9040
// FUNCTION: fn_17502
void C3dActorV2::SetSpeed(float speed) {
  this->speed = speed;
}

// OFFSET: 0x00AB7860
// FUNCTION: fn_17489
C3dNode* C3dActorV1::Find(const char* name) {
  return this->root->Find(name);
}

// OFFSET: 0x00AB7870
// FUNCTION: fn_17490
C3dNode* C3dActorV2::Find(const char* name) {
  for (C3dNodeV2* root : this->roots) {
    if (C3dNode* found = root->Find(name)) {
      return found;
    }
  }

  return nullptr;
}

// OFFSET: 0x00AB94D0
// FUNCTION: fn_17510
void C3dActor::CopyVolumes(const std::list<ModelVolume*>& source) {
  for (const ModelVolume* volume : source) {
    this->volumes.push_back(new ModelVolume(*volume));
  }
}

// INLINED
bool C3dActor::Reversed() const {
  bool result = this->scale[0] >= 0.0f;

  if (this->scale[1] >= 0.0f) {
    result = this->scale[0] < 0.0f;
  }

  return this->scale[2] < 0.0f ? !result : result;
}

// INLINED
void C3dActor::Fade() {
  if (this->occluded == 1) {
    if (this->alpha > 90) {
      this->alpha = static_cast<unsigned char>(this->alpha - 5);
    } else {
      this->occluded = 0;
    }
  } else if (this->alpha != 255) {
    this->alpha = static_cast<unsigned char>(this->alpha + 5);
  }
}

// OFFSET: 0x00AB7B50
// FUNCTION: fn_17496
void C3dActorV1::Draw(const float* matrix, bool clip, ModelRenderContext& context) {
  if (this->dirty == 1) {
    this->UpdateTransform();
  }

  AffineMatrix camera;
  std::memcpy(camera.values, matrix, sizeof(camera.values));
  AffineMatrix combined;
  combined.Compose(this->transform, camera);
  this->root->Draw(combined.values, this->Reversed(), clip, this->commandMode, context);

  if (this->occlusionEnabled == 1) {
    this->Fade();
  }
}

// OFFSET: 0x00AB7C40
// FUNCTION: fn_17497
void C3dActorV2::Draw(const float* matrix, bool clip, ModelRenderContext& context) {
  if (this->dirty == 1) {
    this->UpdateTransform();
  }

  AffineMatrix camera;
  std::memcpy(camera.values, matrix, sizeof(camera.values));
  AffineMatrix combined;
  combined.Compose(this->transform, camera);
  const bool reversed = this->Reversed();

  for (C3dNodeV2* root : this->roots) {
    root->Draw(combined.values, reversed, clip, this->commandMode, context);

    if (this->occlusionEnabled == 1) {
      this->Fade();
    }
  }
}
