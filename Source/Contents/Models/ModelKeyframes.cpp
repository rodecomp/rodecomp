#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <vector>

struct ModelQuaternionKey {
  /* 0000 */ int frame;
  /* 0004 */ float rotation[4];

  ModelQuaternionKey();
};

static_assert(sizeof(ModelQuaternionKey) == 20);

struct ModelVectorKey {
  /* 0000 */ int frame;
  /* 0004 */ float value[4];
};

static_assert(sizeof(ModelVectorKey) == 20);

struct ModelRotationKeys : std::vector<ModelRotationKey> {
  void Add(int frame, float x, float y, float z, float angle);
};

struct ModelScaleKeys : std::vector<ModelScaleKey> {
  void Add(int frame, float scaleX, float scaleY, float scaleZ, float x, float y, float z, float angle);
};

struct ModelPositionKeysV2 : std::vector<ModelVectorKey> {
  ModelPositionKeysV2& operator=(const ModelPositionKeysV2& other);
};

struct ModelQuaternionKeys : std::vector<ModelQuaternionKey> {
  void operator=(const ModelQuaternionKeys& other);
};

struct ModelScaleKeysV2 : std::vector<ModelVectorKey> {
  ModelScaleKeysV2& operator=(const ModelScaleKeysV2& other);
};

// OFFSET: 0x0053FB40
// FUNCTION: fn_01711
ModelQuaternionKey::ModelQuaternionKey() : frame(0), rotation{0.0f, 0.0f, 0.0f, 1.0f} {
}

// OFFSET: 0x0053FFE0
// FUNCTION: fn_01716
void ModelRotationKeys::Add(int frame, float x, float y, float z, float angle) {
  angle *= 0.5f;
  const float sine = ClientSin(angle);
  ModelRotationKey key;
  key.frame = frame;
  key.rotation[0] = sine * x;
  key.rotation[1] = sine * y;
  key.rotation[2] = sine * z;
  key.rotation[3] = ClientCos(angle);
  this->push_back(key);
}

// OFFSET: 0x00540120
// FUNCTION: fn_01717
void ModelScaleKeys::Add(int frame, float scaleX, float scaleY, float scaleZ, float x, float y, float z, float angle) {
  angle *= 0.5f;
  const float sine = ClientSin(angle);
  ModelScaleKey key;
  key.frame = frame;
  key.scale[0] = scaleX;
  key.scale[1] = scaleY;
  key.scale[2] = scaleZ;
  key.rotation[0] = sine * x;
  key.rotation[1] = sine * y;
  key.rotation[2] = sine * z;
  key.rotation[3] = ClientCos(angle);
  this->push_back(key);
}

// OFFSET: 0x005402D0
// FUNCTION: fn_01718
ModelPositionKeysV2& ModelPositionKeysV2::operator=(const ModelPositionKeysV2& other) {
  return static_cast<ModelPositionKeysV2&>(std::vector<ModelVectorKey>::operator=(other));
}

// OFFSET: 0x005402E0
// FUNCTION: fn_01719
void ModelQuaternionKeys::operator=(const ModelQuaternionKeys& other) {
  this->clear();
  std::vector<ModelQuaternionKey>::operator=(other);
}

// OFFSET: 0x00540360
// FUNCTION: fn_01720
ModelScaleKeysV2& ModelScaleKeysV2::operator=(const ModelScaleKeysV2& other) {
  return static_cast<ModelScaleKeysV2&>(std::vector<ModelVectorKey>::operator=(other));
}
