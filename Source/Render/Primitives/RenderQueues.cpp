#include "Render/Primitives/DepthAlpha.h"
#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderQueues.h"

#include <algorithm>

// OFFSET: 0x00559CA0
// FUNCTION: fn_02073
void RenderQueues::Submit(RenderCommand* primitive, std::uint16_t flags, bool depthAlpha, float depthStart, float depthEnd) {
  if (depthAlpha) {
    primitive->ApplyDepthAlpha();
  }

  if (flags & 1) {
    const auto first = [&]() { return primitive->vertices[0].rhw; };
    const auto average = [&]() {
      const float second = primitive->vertices[1].rhw;
      return (first() - second) * 0.5f + second;
    };
    if (flags & 4) {
      if (flags & 8) {
        this->queue_0210.push_back(primitive);
      } else if (flags & 0x200) {
        this->queue_0234.push_back({average(), primitive});
      } else {
        this->queue_01EC.push_back(primitive);
      }
    } else if (flags & 2) {
      if (flags & 0x100) {
        this->queue_01C8.push_back({average(), primitive});
      } else if (flags & 0x200) {
        this->queue_01E0.push_back({average(), primitive});
      } else {
        this->queue_01D4.push_back({average(), primitive});
      }
    } else if (flags & 0x10) {
      this->queue_0228.push_back({average(), primitive});
    } else if (flags & 0x100) {
      this->queue_0180.push_back({average(), primitive});
    } else if (flags & 0x200) {
      if (flags & 8) {
        this->queue_021C.push_back({average(), primitive});
      } else {
        this->queue_0198.push_back({average(), primitive});
      }
    } else if (flags & 0x20) {
      this->queue_01BC.push_back({first(), primitive});
    } else if (static_cast<unsigned char>(primitive->depthFlags) == 1) {
      this->queue_018C.push_back({first(), primitive});
    } else {
      this->queue_01A4.push_back({first(), primitive});
    }
  } else if (flags & 0x800) {
    this->queue_0168.push_back(primitive);
  } else if (flags & 0x1000) {
    this->queue_0174.push_back(primitive);
  } else if (flags & 0x20) {
    this->queue_01B0.push_back(primitive);
  } else {
    this->queue_015C.push_back(primitive);
  }
}

// OFFSET: 0x00559F70
// FUNCTION: fn_02074
void RenderQueues::SubmitTriangle(RenderTriangle& triangle, std::uint16_t flags, RenderCommandPool& pool, bool depthAlpha, float depthStart, float depthEnd) {
  RenderCommand* command = pool.Acquire();
  command->vertices = triangle.vertices;
  command->vertexCount = 3;
  command->texture = triangle.texture;
  command->textureMode = 0;
  command->sourceBlend = triangle.sourceBlend;
  command->destinationBlend = triangle.destinationBlend;
  this->Submit(command, flags, depthAlpha, depthStart, depthEnd);
}
