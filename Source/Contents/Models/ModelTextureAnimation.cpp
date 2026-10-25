#include "Contents/Models/ModelNode.h"

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
