#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H

#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <vector>

class LegacyPositionTrack : public std::vector<ModelPositionKey> {
public:
  using std::vector<ModelPositionKey>::operator=;
  void Sample(int frame, matrix& output) const;
};

class LegacyRotationTrack : public std::vector<ModelRotationKey> {
public:
  using std::vector<ModelRotationKey>::operator=;
  void Add(int frame, float x, float y, float z, float angle);
  void Sample(int frame, matrix& output) const;
};

class LegacyScaleTrack : public std::vector<ModelScaleKey> {
public:
  using std::vector<ModelScaleKey>::operator=;
  void Add(int frame, float scaleX, float scaleY, float scaleZ, float x, float y, float z, float angle);
  void Sample(int frame, matrix& output) const;
};

class ModelScaleTrack : public std::vector<ModelRotationKey> {
public:
  using std::vector<ModelRotationKey>::operator=;
  void Sample(float frame, matrix& output) const;
};

class ModelPositionTrack : public std::vector<ModelRotationKey> {
public:
  using std::vector<ModelRotationKey>::operator=;
  void Sample(float frame, const matrix& base, matrix& output) const;
};

class ModelRotationTrack : public std::vector<ModelRotationKey> {
public:
  using std::vector<ModelRotationKey>::operator=;
  void Sample(float frame, const matrix& base, matrix& output) const;
};
void QuaternionMatrix(const ModelRotationKey& key, matrix& output);

#endif
