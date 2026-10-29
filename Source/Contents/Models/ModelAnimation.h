#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H

#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <span>

void SampleLegacyPosition(std::span<const ModelPositionKey> keys, int frame, matrix& output);
void SampleLegacyRotation(std::span<const ModelRotationKey> keys, int frame, matrix& output);
void SampleLegacyScale(std::span<const ModelScaleKey> keys, int frame, matrix& output);
void SampleModelScale(std::span<const ModelRotationKey> keys, float frame, matrix& output);
void SampleModelPosition(std::span<const ModelRotationKey> keys, float frame, const matrix& base, matrix& output);
void SampleModelRotation(std::span<const ModelRotationKey> keys, float frame, const matrix& base, matrix& output);
void QuaternionMatrix(const ModelRotationKey& key, matrix& output);
void InterpolateModelRotation(float amount, const ModelRotationKey& first, const ModelRotationKey& second, int spins, ModelRotationKey& output);

#endif
