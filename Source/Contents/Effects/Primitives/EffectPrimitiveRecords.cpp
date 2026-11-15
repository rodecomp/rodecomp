#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Actors/RenderObjectState.h"

#include <cstring>

// OFFSET: 0x00BAE670
// FUNCTION: fn_19768
void CEffectPrim::InitSegment() {
  for (int index = 0; index < this->state.numSegment; ++index) {
    EffectPrimitiveRecord& record = this->state.records[index];
    record.position = this->render.position;
    for (int corner = 0; corner < 4; ++corner) {
      record.corners[corner] = this->state.segPos[corner];
    }
    record.radius = this->state.radius;
    record.scale = this->state.size;
    record.angle = this->state.longitude;
    std::memcpy(record.matrix, this->state.transform.values, sizeof(record.matrix));
    record.alpha = this->state.alpha;
    record.red = this->state.red;
    record.green = this->state.green;
    record.blue = this->state.blue;

    const int alpha = static_cast<int>(this->state.alpha);
    record.color = static_cast<unsigned char>(this->state.blue) +
                   ((static_cast<unsigned char>(this->state.green) +
                     ((static_cast<unsigned char>(this->state.red) + (static_cast<std::uint32_t>(alpha) << 8)) << 8))
                    << 8);
  }
}
