#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Cooldowns/CooldownDrawing.h"

namespace {
  // INLINED
  __forceinline void Submit(std::vector<RenderCommand*>& commands) {
    for (auto it = commands.begin(); it != commands.end(); ++it) {
      static_cast<CRendererDX9*>(g_renderer)->Submit(*it, 0x201);
    }

    commands.clear();
  }
}

// OFFSET: 0x00CA66B0
// FUNCTION: fn_20964
void CooldownDrawing::Draw(
  int centerX, int centerY, int radiusX, int radiusY, CooldownState& value, int limit, int offsetX, int offsetY
) {
  if (value.x <= 0 || value.y <= 0 || (value.color & 0xFF000000u) == 0) {
    return;
  }

  RenderVertex* vertices = value.vertices;
  const float top = static_cast<float>((centerY - radiusY));
  const float bottom = static_cast<float>((centerY + radiusY));

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
  const float right = static_cast<float>((centerX + radiusX));
  const float left = static_cast<float>((centerX - radiusX));
  if (value.reverse == 0) {
    vertices[2].x = vertices[4].x = vertices[5].x = vertices[7].x = vertices[8].x = vertices[10].x = left;
    vertices[14].x = vertices[16].x = vertices[17].x = vertices[19].x = vertices[20].x = vertices[22].x = right;
    value.AddEdge(0, vertices[23].x, radiusX, 1);
    value.AddEdge(1, vertices[20].y, radiusY, 1);
    value.AddEdge(2, vertices[17].y, radiusY, 1);
    value.AddEdge(3, vertices[14].x, radiusX, 0);
    value.AddEdge(4, vertices[11].x, radiusX, 0);
    value.AddEdge(5, vertices[8].y, radiusY, 0);
    value.AddEdge(6, vertices[5].y, radiusY, 0);
    value.AddEdge(7, vertices[2].x, radiusX, 1);
  } else {
    vertices[2].x = vertices[4].x = vertices[5].x = vertices[7].x = vertices[8].x = vertices[10].x = right;
    vertices[14].x = vertices[16].x = vertices[17].x = vertices[19].x = vertices[20].x = vertices[22].x = left;
    value.AddEdge(0, vertices[1].x, radiusX, 1);
    value.AddEdge(1, vertices[4].y, radiusY, 1);
    value.AddEdge(2, vertices[7].y, radiusY, 1);
    value.AddEdge(3, vertices[10].x, radiusX, 0);
    value.AddEdge(4, vertices[13].x, radiusX, 0);
    value.AddEdge(5, vertices[16].y, radiusY, 0);
    value.AddEdge(6, vertices[19].y, radiusY, 0);
    value.AddEdge(7, vertices[22].x, radiusX, 1);
    this->DrawReverse(value);
    return;
  }

  if (value.duration == 0) {
    return;
  }

  switch (value.segment) {
  case 0:
    vertices[23].x = value.Update();
    break;
  case 1:
    vertices[20].y = value.Update();
    break;
  case 2:
    vertices[17].y = value.Update();
    break;
  case 3:
    vertices[14].x = value.Update();
    break;
  case 4:
    vertices[11].x = value.Update();
    break;
  case 5:
    vertices[8].y = value.Update();
    break;
  case 6:
    vertices[5].y = value.Update();
    break;
  case 7:
    vertices[2].x = value.Update();
    break;
  }

  if ((limit != 0 ? limit : 6) <= value.y) {
    return;
  }

  RenderCommand* command = g_renderer->commands.Acquire();
  command->texture = g_texMgr.Default();
  command->vertices = vertices;
  command->vertexCount = value.vertexCount;

  for (RenderVertex& vertex : value.vertices) {
    vertex.x = static_cast<float>(((offsetX + value.x) + 12)) + vertex.x;
    vertex.y = static_cast<float>(((offsetY + value.y) + 12)) + vertex.y;
  }

  this->normal.push_back(command);
}

// OFFSET: 0x00CA6C00
// FUNCTION: fn_20965
void CooldownDrawing::DrawReverse(CooldownState& value) {
  if (value.duration == 0) {
    return;
  }

  RenderVertex* vertices = value.vertices;
  switch (value.segment) {
  case 0:
    vertices[2].x = value.Update();
    if (value.segment != 0) {
      vertices[5] = vertices[4];
      vertices[2] = vertices[4];
    }
    break;
  case 1:
    vertices[5].y = value.Update();
    if (value.segment != 1) {
      vertices[8] = vertices[7];
      vertices[5] = vertices[7];
    }
    break;
  case 2:
    vertices[8].y = value.Update();
    if (value.segment != 2) {
      vertices[11] = vertices[10];
      vertices[8] = vertices[10];
    }
    break;
  case 3:
    vertices[11].x = value.Update();
    if (value.segment != 3) {
      vertices[14] = vertices[13];
      vertices[11] = vertices[13];
    }
    break;
  case 4:
    vertices[14].x = value.Update();
    if (value.segment != 4) {
      vertices[17] = vertices[16];
      vertices[14] = vertices[16];
    }
    break;
  case 5:
    vertices[17].y = value.Update();
    if (value.segment != 5) {
      vertices[20] = vertices[19];
      vertices[17] = vertices[19];
    }
    break;
  case 6:
    vertices[20].y = value.Update();
    if (value.segment != 6) {
      vertices[23] = vertices[22];
      vertices[20] = vertices[22];
    }
    break;
  case 7:
    vertices[23].x = value.Update();
    break;
  }

  RenderCommand* command = g_renderer->commands.Acquire();
  command->texture = g_texMgr.Default();
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
  Submit(this->normal);
}

// OFFSET: 0x00CA6630
// FUNCTION: fn_20962
void CooldownDrawing::SubmitReverse(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd) {
  Submit(this->reverse);
}
