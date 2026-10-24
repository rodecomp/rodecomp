#include "Contents/ModelNode.h"

#include <bit>

// OFFSET: 0x00540EA0
// FUNCTION: fn_01727
void SampleModelTexture(const ModelTextureChannels& channels, float frame, ModelTextureTransform& output) {
  for (int channel = 0; channel < 5; ++channel) {
    const auto found = channels.find(channel);

    if (found == channels.end()) {
      continue;
    }

    const auto& keys = found->second;
    const int last = std::bit_cast<int>(keys.back()[0]);

    if (frame >= static_cast<float>(last)) {
      output.values[channel] = std::bit_cast<float>(keys.back()[1]);
      continue;
    }

    unsigned next = 1;

    while (next < keys.size() && !(static_cast<float>(std::bit_cast<int>(keys[next][0])) > frame)) {
      ++next;
    }

    const int previous = std::bit_cast<int>(keys[next - 1][0]);
    const int difference = std::bit_cast<int>(keys[next][0] - keys[next - 1][0]);
    const float amount = (frame - static_cast<float>(previous)) / static_cast<float>(difference);
    const float first = (1.0f - amount) * std::bit_cast<float>(keys[next - 1][1]);
    const float second = amount * std::bit_cast<float>(keys[next][1]);
    output.values[channel] = first + second;
  }
}
