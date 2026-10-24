#ifndef RAGNAROK_SOURCE_CONTENTS_MODELANIMATION_H
#define RAGNAROK_SOURCE_CONTENTS_MODELANIMATION_H

#include "Resource/ModelResource.h"
#include "Utility/AffineMatrix.h"

#include <span>

// Outputs deliberately retain the original per-track write contracts. Legacy
// multi-key samplers leave output unchanged at/after their final key.
void SampleLegacyPosition(std::span<const ModelPositionKey> keys, int frame, AffineMatrix& output);
void SampleLegacyRotation(std::span<const ModelRotationKey> keys, int frame, AffineMatrix& output);
void SampleLegacyScale(std::span<const ModelScaleKey> keys, int frame, AffineMatrix& output);
void SampleModelScale(std::span<const ModelRotationKey> keys, float frame, AffineMatrix& output);
void SampleModelPosition(
  std::span<const ModelRotationKey> keys,
  float frame,
  const AffineMatrix& base,
  AffineMatrix& output
);
void SampleModelRotation(
  std::span<const ModelRotationKey> keys,
  float frame,
  const AffineMatrix& base,
  AffineMatrix& output
);
void QuaternionMatrix(const ModelRotationKey& key, AffineMatrix& output);
void InterpolateModelRotation(
  float amount,
  const ModelRotationKey& first,
  const ModelRotationKey& second,
  int spins,
  ModelRotationKey& output
);

#endif
