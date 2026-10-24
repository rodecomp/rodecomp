#include "Render/RenderCommandPool.h"
#include "Render/RenderQueues.h"
#include "Render/RenderSystemBase.h"
#include "UI/CooldownDrawing.h"

#include <bit>

namespace {
  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }
  int Subtract(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) - static_cast<unsigned>(right));
  }
  void Submit(
    std::vector<RenderCommand*>& commands,
    RenderQueues& queues,
    bool depthAlpha,
    float depthStart,
    float depthEnd
  ) {
    for (RenderCommand* command : commands) {
      queues.Submit(command, 513, depthAlpha, depthStart, depthEnd);
    }
    commands.clear();
  }
}

// OFFSET: 0x00CA66B0
// FUNCTION: fn_20964
void CooldownDrawing::Draw(
  int centerX,
  int centerY,
  int radiusX,
  int radiusY,
  CooldownState& value,
  int limit,
  int offsetX,
  int offsetY,
  const CooldownClock& clock,
  RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  if (value.x <= 0 || value.y <= 0 || (value.color & 0xFF000000u) == 0) {
    return;
  }
  RenderVertex* vertices = value.vertices;
  const float top = static_cast<float>(Subtract(centerY, radiusY));
  const float bottom = static_cast<float>(Add(centerY, radiusY));
  vertices[1].x = vertices[23].x = static_cast<float>(centerX);
  vertices[1].y = vertices[23].y = top;
  vertices[11].x = vertices[13].x = static_cast<float>(centerX);
  vertices[11].y = vertices[13].y = bottom;
  for (unsigned i = 0; i < 24; ++i) {
    RenderVertex& vertex = vertices[i];
    if (i % 3 == 0) {
      vertex.x = static_cast<float>(centerX);
      vertex.y = static_cast<float>(centerY);
    }
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = value.color;
    vertex.u = vertex.v = 0;
  }
  vertices[5].y = vertices[7].y = vertices[17].y = vertices[19].y = static_cast<float>(centerY);
  vertices[2].y = vertices[4].y = vertices[20].y = vertices[22].y = top;
  vertices[8].y = vertices[10].y = vertices[14].y = vertices[16].y = bottom;
  const float right = static_cast<float>(Add(centerX, radiusX));
  const float left = static_cast<float>(Subtract(centerX, radiusX));
  const bool reversed = value.reverse != 0;
  for (unsigned i : {2u, 4u, 5u, 7u, 8u, 10u}) {
    vertices[i].x = reversed ? right : left;
  }
  for (unsigned i : {14u, 16u, 17u, 19u, 20u, 22u}) {
    vertices[i].x = reversed ? left : right;
  }

  value.AddEdge(0, vertices[reversed ? 1 : 23].x, radiusX, 1);
  value.AddEdge(1, vertices[reversed ? 4 : 20].y, radiusY, 1);
  value.AddEdge(2, vertices[reversed ? 7 : 17].y, radiusY, 1);
  value.AddEdge(3, vertices[reversed ? 10 : 14].x, radiusX, 0);
  value.AddEdge(4, vertices[reversed ? 13 : 11].x, radiusX, 0);
  value.AddEdge(5, vertices[reversed ? 16 : 8].y, radiusY, 0);
  value.AddEdge(6, vertices[reversed ? 19 : 5].y, radiusY, 0);
  value.AddEdge(7, vertices[reversed ? 22 : 2].x, radiusX, 1);
  if (reversed) {
    this->DrawReverse(value, clock, pool, system);
    return;
  }
  if (value.duration == 0) {
    return;
  }
  // The destination is chosen from the old segment, before Update changes it.
  const unsigned previous = value.segment;
  if (previous < 8) {
    const float position = value.Update(clock);
    RenderVertex& vertex = vertices[23 - 3 * previous];
    if (previous == 0 || previous == 3 || previous == 4 || previous == 7) {
      vertex.x = position;
    } else {
      vertex.y = position;
    }
  }
  if ((limit != 0 ? limit : 6) <= value.y) {
    return;
  }
  RenderCommand* command = pool.Acquire();
  command->texture = system.BackBufferTexture();
  command->vertices = vertices;
  command->vertexCount = value.vertexCount;
  for (RenderVertex& vertex : value.vertices) {
    vertex.x = static_cast<float>(Add(Add(offsetX, value.x), 12)) + vertex.x;
    vertex.y = static_cast<float>(Add(Add(offsetY, value.y), 12)) + vertex.y;
  }
  this->normal.push_back(command);
}

// OFFSET: 0x00CA6C00
// FUNCTION: fn_20965
void CooldownDrawing::DrawReverse(
  CooldownState& value,
  const CooldownClock& clock,
  RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  if (value.duration == 0) {
    return;
  }
  const unsigned previous = value.segment;
  if (previous < 8) {
    const float position = value.Update(clock);
    RenderVertex& vertex = value.vertices[2 + 3 * previous];
    if (previous == 0 || previous == 3 || previous == 4 || previous == 7) {
      vertex.x = position;
    } else {
      vertex.y = position;
    }
    if (previous < 7 && value.segment != previous) {
      value.vertices[5 + 3 * previous] = value.vertices[4 + 3 * previous];
      vertex = value.vertices[4 + 3 * previous];
    }
  }
  RenderCommand* command = pool.Acquire();
  command->texture = system.BackBufferTexture();
  command->vertices = value.vertices;
  command->vertexCount = value.vertexCount;
  for (RenderVertex& vertex : value.vertices) {
    vertex.x = static_cast<float>(value.x) + vertex.x;
    vertex.y = static_cast<float>(value.y) + vertex.y;
  }
  this->reverse.push_back(command);
}

// OFFSET: 0x00CA6670
// FUNCTION: fn_20963
void CooldownDrawing::SubmitNormal(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd) {
  Submit(this->normal, queues, depthAlpha, depthStart, depthEnd);
}

// OFFSET: 0x00CA6630
// FUNCTION: fn_20962
void CooldownDrawing::SubmitReverse(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd) {
  Submit(this->reverse, queues, depthAlpha, depthStart, depthEnd);
}
