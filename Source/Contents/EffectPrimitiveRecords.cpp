#include "Contents/EffectPrimitiveState.h"
#include "Contents/RenderObjectState.h"

#include <bit>
#include <cstring>
#include <emmintrin.h>

// OFFSET: 0x00BAE670
// FUNCTION: fn_19768
void EffectPrimitiveState::InitializeRecords(const RenderObjectState& render) {
  for (int index = 0; index < this->unknown_01F4; ++index) {
    EffectPrimitiveRecord& record = this->records[index];
    record.position = {render.position[0], render.position[1], render.position[2]};
    std::memcpy(record.corners, this->corners, sizeof(record.corners));
    record.unknown_003C = this->unknown_0348;
    record.scale = this->scale;
    record.angle = std::bit_cast<float>(this->unknown_030C);
    std::memcpy(record.matrix, this->matrix1, sizeof(record.matrix));
    record.alpha = this->unknown_0324;
    record.red = this->red;
    record.green = this->green;
    record.blue = this->blue;

    const int alpha = _mm_cvttss_si32(_mm_set_ss(this->unknown_0324));
    record.color = static_cast<unsigned char>(this->blue) +
                   ((static_cast<unsigned char>(this->green) +
                     ((static_cast<unsigned char>(this->red) + (static_cast<std::uint32_t>(alpha) << 8)) << 8))
                    << 8);
  }
}
