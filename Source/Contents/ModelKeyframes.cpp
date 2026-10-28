#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <vector>

struct ModelRotationKeys : std::vector<ModelRotationKey> {
  void Add(int frame, float x, float y, float z, float angle);
};

struct ModelScaleKeys : std::vector<ModelScaleKey> {
  void Add(int frame, float scaleX, float scaleY, float scaleZ, float x, float y, float z, float angle);
};

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
