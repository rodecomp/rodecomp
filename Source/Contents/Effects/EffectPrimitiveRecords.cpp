#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObjectState.h"

#include "Utility/Math/ScalarConversion.h"
#include <cstring>

// OFFSET: 0x00BAE670
// FUNCTION: fn_19768
void EffectPrimitiveState::InitializeRecords(const RenderObjectState& render) {
  for (int index = 0; index < this->unknown_01F4; ++index) {
    EffectPrimitiveRecord& record = this->records[index];
    record.position = {render.position[0], render.position[1], render.position[2]};
    std::memcpy(record.corners, this->corners, sizeof(record.corners));
    record.unknown_003C = this->unknown_0348;
    record.scale = this->scale;
    record.angle = this->unknown_030C;
    std::memcpy(record.matrix, this->matrix1, sizeof(record.matrix));
    record.alpha = this->alpha;
    record.red = this->red;
    record.green = this->green;
    record.blue = this->blue;

    const int alpha = TruncateToInt(this->alpha);
    record.color = static_cast<unsigned char>(this->blue) +
                   ((static_cast<unsigned char>(this->green) + ((static_cast<unsigned char>(this->red) + (static_cast<std::uint32_t>(alpha) << 8)) << 8)) << 8);
  }
}
