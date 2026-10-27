#include "Contents/Models/ModelNode.h"
#include "File/File.h"

// OFFSET: 0x00540EA0
// FUNCTION: fn_01727
void SampleModelTexture(const ModelTextureChannels& channels, float frame, ModelTextureTransform& output) {
  for (int channel = 0; channel < 5; ++channel) {
    const auto found = channels.find(channel);

    if (found == channels.end()) {
      continue;
    }

    const auto& keys = found->second;
    const int last = keys.back().frame;

    if (frame >= static_cast<float>(last)) {
      output.values[channel] = keys.back().value;
      continue;
    }

    unsigned next = 1;

    while (next < keys.size() && !(static_cast<float>(keys[next].frame) > frame)) {
      ++next;
    }

    const int previous = keys[next - 1].frame;
    const int difference = keys[next].frame - keys[next - 1].frame;
    const float amount = (frame - static_cast<float>(previous)) / static_cast<float>(difference);
    const float first = (1.0f - amount) * keys[next - 1].value;
    const float second = amount * keys[next].value;
    output.values[channel] = first + second;
  }
}

struct ModelKeyList1 : std::vector<ModelRotationKey> {
  void Read(CFile& file);
};

// OFFSET: 0x00541160
// FUNCTION: fn_01728
void ModelKeyList1::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key{};
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

struct ModelKeyList2 : std::vector<ModelRotationKey> {
  void Read(CFile& file);
};

// OFFSET: 0x005411E0
// FUNCTION: fn_01729
void ModelKeyList2::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key{};
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

struct ModelKeyList3 : std::vector<ModelRotationKey> {
  void Read(CFile& file);
};

// OFFSET: 0x00541250

struct ModelKeyList4 : std::vector<ModelRotationKey> {
  void Read(CFile& file);
};

// OFFSET: 0x00541350
// FUNCTION: fn_01731
void ModelKeyList4::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    ModelRotationKey key{};
    file.Read(&key, sizeof(key));
    this->push_back(key);
  }
}

struct ModelTextureChannelMap : ModelTextureChannels {
  void Read(CFile& file);
};

// OFFSET: 0x005413D0
// FUNCTION: fn_01732
void ModelTextureChannelMap::Read(CFile& file) {
  int count = 0;
  file.Read(&count, sizeof(count));
  for (int i = 0; i < count; ++i) {
    int id = 0;
    int keyCount = 0;
    file.Read(&id, sizeof(id));
    file.Read(&keyCount, sizeof(keyCount));
    for (int k = 0; k < keyCount; ++k) {
      ModelTextureKey key{};
      file.Read(&key, sizeof(key));
      (*this)[id].push_back(key);
    }
  }
}
