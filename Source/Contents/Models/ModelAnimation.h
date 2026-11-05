#ifndef RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H
#define RAGNAROK_SOURCE_CONTENTS_MODELS_MODELANIMATION_H

#include "Resource/Models/ModelResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <vector>

void SampleLegacyPosition(const std::vector<ModelPositionKey>& keys, int frame, matrix& output);
void SampleLegacyRotation(const std::vector<ModelRotationKey>& keys, int frame, matrix& output);
void SampleLegacyScale(const std::vector<ModelScaleKey>& keys, int frame, matrix& output);
void SampleModelScale(const std::vector<ModelRotationKey>& keys, float frame, matrix& output);
void SampleModelPosition(const std::vector<ModelRotationKey>& keys, float frame, const matrix& base, matrix& output);
void SampleModelRotation(const std::vector<ModelRotationKey>& keys, float frame, const matrix& base, matrix& output);
void QuaternionMatrix(const ModelRotationKey& key, matrix& output);
void InterpolateModelRotation(float amount, const ModelRotationKey& first, const ModelRotationKey& second, int spins, ModelRotationKey& output);

#endif
