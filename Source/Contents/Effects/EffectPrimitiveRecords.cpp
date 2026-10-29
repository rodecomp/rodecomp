#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObjectState.h"

#include "Utility/Math/ScalarConversion.h"
#include <cstring>

// OFFSET: 0x00BAE670
// FUNCTION: fn_19768
void CEffectPrim::InitSegment() {
  for (int index = 0; index < this->state.numSegment; ++index) {
    EffectPrimitiveRecord& record = this->state.records[index];
    record.position = this->render.position;
    std::memcpy(record.corners, this->state.segPos, sizeof(record.corners));
    record.unknown_003C = this->state.radius;
    record.scale = this->state.size;
    record.angle = this->state.longitude;
    std::memcpy(record.matrix, this->state.transform, sizeof(record.matrix));
    record.alpha = this->state.alpha;
    record.red = this->state.red;
    record.green = this->state.green;
    record.blue = this->state.blue;

    const int alpha = TruncateToInt(this->state.alpha);
    record.color = static_cast<unsigned char>(this->state.blue) +
                   ((static_cast<unsigned char>(this->state.green) +
                     ((static_cast<unsigned char>(this->state.red) + (static_cast<std::uint32_t>(alpha) << 8)) << 8))
                    << 8);
  }
}
