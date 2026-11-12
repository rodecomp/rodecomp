#include "Contents/Models/ModelNode.h"
#include "File/File.h"

// OFFSET: 0x00540EA0
// FUNCTION: fn_01727
void SampleModelTexture(ModelTextureChannels& channels, float frame, ModelTextureTransform& output) {
  float* targets[5] = {&output.values[0], &output.values[1], &output.values[2], &output.values[3], &output.values[4]};

  for (int channel = 0; channel < 5; ++channel) {
    if (channels.find(channel) == channels.end()) {
      continue;
    }

    std::vector<ModelTextureKey>& keys = channels[channel];
    unsigned next = 1;
    if (frame < static_cast<float>(keys.back().frame)) {
      const unsigned count = keys.size();
      for (; next < count; ++next) {
        if (frame < static_cast<float>(keys[next].frame)) {
          break;
        }
      }

      const int previous = keys[next - 1].frame;
      const float amount = (frame - static_cast<float>(previous)) / static_cast<float>(keys[next].frame - previous);
      *targets[channel] = (1.0f - amount) * keys[next - 1].value + amount * keys[next].value;
    } else {
      *targets[channel] = keys.back().value;
    }
  }
}

// OFFSET: 0x00541160
// FUNCTION: fn_01728
void ModelKeyList1::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key;
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

// OFFSET: 0x005411E0
// FUNCTION: fn_01729
void ModelKeyList2::Read(CFile& file) {
  int count;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key;
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

// OFFSET: 0x00541250
// FUNCTION: fn_01730
void ModelKeyList3::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key{0, {0.0f, 0.0f, 0.0f, 1.0f}};
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

// OFFSET: 0x00541350
// FUNCTION: fn_01731
void ModelKeyList4::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key;
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

// OFFSET: 0x005413D0
// FUNCTION: fn_01732
void ModelTextureChannelMap::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    int id = 0;
    file.Read(&id, sizeof(id));
    int keyCount = 0;
    file.Read(&keyCount, sizeof(keyCount));
    for (int k = 0; k < keyCount; ++k) {
      ModelTextureKey key;
      file.Read(&key, sizeof(key));
      (*this)[id].push_back(key);
    }
  }
}
