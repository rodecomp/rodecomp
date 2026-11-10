#include "Contents/Models/ModelNode.h"
#include "Utility/Math/ScalarConversion.h"

#include <cstring>

// OFFSET: 0x00AB5CE0
// FUNCTION: fn_17464
C3dNode::C3dNode() : model(nullptr), parent(nullptr), mesh(nullptr), alpha(1.0f), overrideColors(0) {
}

// OFFSET: 0x00AB66A0
// FUNCTION: fn_17471
C3dNode::~C3dNode() {
  for (C3dNode* child : this->children) {
    delete child;
  }

  this->children.clear();
  this->textures.clear();
  this->faceState.clear();
  this->faceValues.clear();
}

// OFFSET: 0x00AB5DE0
// FUNCTION: fn_17465
C3dNodeV1::C3dNodeV1() = default;

C3dNodeV1::~C3dNodeV1() = default;

// OFFSET: 0x00AB5EA0
// FUNCTION: fn_17466
C3dNodeV2::C3dNodeV2() = default;

C3dNodeV2::~C3dNodeV2() = default;

// OFFSET: 0x00AB7040
// FUNCTION: fn_17481
void C3dNode::Initialize(const ModelNode&) {
}

// OFFSET: 0x00AB9110
// FUNCTION: fn_17506
void C3dNode::Animate(double) {
}

// OFFSET: 0x00AB7D30
// FUNCTION: fn_17498
void C3dNode::Draw(const float*, unsigned char, bool, unsigned char) {
}

// OFFSET: 0x00AB7050
// FUNCTION: fn_17482
void C3dNodeV1::Initialize(const ModelNode& base) {
  const auto& resource = static_cast<const ModelNodeV1&>(base);
  this->name = resource.name;
  this->mesh = resource.mesh.get();
  this->textures = resource.textures;
  this->alpha = static_cast<float>(resource.alpha) / 255.0f;

  if (this->scaleKeys.empty()) {
    this->scaleKeys.push_back({0, {resource.scale[0], resource.scale[1], resource.scale[2]}, {0, 0, 0, 1}});
  }

  this->rotationKeys = resource.rotationKeys;

  if (this->rotationKeys.empty()) {
    const float halfAngle = resource.angle * 0.5f;
    const float sine = ClientSin(halfAngle);
    this->rotationKeys.push_back({0, {sine * resource.axis[0], sine * resource.axis[1], sine * resource.axis[2], ClientCos(halfAngle)}});
  }

  this->positionKeys = resource.legacyPositionKeys;

  if (this->positionKeys.empty()) {
    this->positionKeys.push_back({0, {resource.position[0], resource.position[1], resource.position[2]}});
  }

  for (const ModelNode* resourceChild : resource.children) {
    auto* child = new C3dNodeV1;
    this->children.push_back(child);
    child->model = this->model;
    child->parent = this;
    child->Initialize(*resourceChild);
  }

  this->faceState.resize(this->mesh->faceCount);
  this->faceValues.resize(this->mesh->faceCount * 9u);
}

// OFFSET: 0x00AB73F0
// FUNCTION: fn_17484
void C3dNodeV2::Initialize(const ModelNode& base) {
  const auto& resource = static_cast<const ModelNodeV2&>(base);
  this->name = resource.name;
  this->mesh = resource.mesh.get();
  std::memcpy(this->resourceTransform.values, resource.transform, sizeof(resource.transform));

  if (this->parent) {
    const matrix world = this->resourceTransform;
    const matrix parentWorld = static_cast<C3dNodeV2*>(this->parent)->resourceTransform;
    matrix inverse;
    inverse.MakeInverse(parentWorld);
    this->localTransform.Compose(world, inverse);
  } else {
    this->localTransform = this->resourceTransform;
  }

  this->scaleKeys = resource.scaleKeys;
  this->rotationKeys = resource.rotationKeys;
  this->positionKeys = resource.positionKeys;
  this->textureKeys = resource.textureKeys;

  for (const ModelNode* resourceChild : resource.children) {
    auto* child = new C3dNodeV2;
    child->parent = this;
    child->model = this->model;
    child->Initialize(*resourceChild);
    this->children.push_back(child);
  }

  this->alpha = static_cast<float>(resource.alpha) * 0.00390625f;
  this->textures.insert(this->textures.end(), resource.textures.begin(), resource.textures.end());
  this->alpha = static_cast<float>(resource.alpha) / 255.0f;
  this->faceState.resize(this->mesh->faceCount);
  this->faceValues.resize(this->mesh->faceCount * 9u);
}

// OFFSET: 0x00AB9120
// FUNCTION: fn_17507
void C3dNodeV1::Animate(double frame) {
  const int selected = static_cast<int>(frame);
  matrix position;
  matrix scale;
  matrix rotation;
  std::memcpy(position.values, g_identityMatrix, sizeof(g_identityMatrix));
  std::memcpy(scale.values, g_identityMatrix, sizeof(g_identityMatrix));
  std::memcpy(rotation.values, g_identityMatrix, sizeof(g_identityMatrix));
  this->positionKeys.Sample(selected, position);
  this->rotationKeys.Sample(selected, rotation);
  this->scaleKeys.Sample(selected, scale);
  matrix combined;
  combined.Compose(scale, rotation);
  this->transform.Compose(combined, position);

  for (auto it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->Animate(frame);
  }
}

// OFFSET: 0x00AB9210
// FUNCTION: fn_17508
void C3dNodeV2::Animate(double frame) {
  std::memcpy(this->transform.values, g_identityMatrix, sizeof(g_identityMatrix));
  const float selected = static_cast<float>(frame);
  this->scaleKeys.Sample(selected, this->transform);
  this->rotationKeys.Sample(selected, this->localTransform, this->transform);
  this->positionKeys.Sample(selected, this->localTransform, this->transform);
  this->textureTransforms.clear();

  for (auto it = this->textureKeys.begin(); it != this->textureKeys.end(); ++it) {
    SampleModelTexture(it->second, selected, this->textureTransforms[it->first]);
  }

  for (auto it = this->children.begin(); it != this->children.end(); ++it) {
    (*it)->Animate(frame);
  }
}
