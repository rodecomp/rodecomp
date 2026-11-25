#include "Render/Primitives/RenderCommandPool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Cooldowns/CooldownOwner.h"

namespace {
  // INLINED
  __forceinline void Submit(std::vector<RenderCommand*>& commands) {
    for (auto it = commands.begin(); it != commands.end(); ++it) {
      static_cast<CRendererDX9*>(g_renderer)->Submit(*it, 0x201);
    }

    commands.clear();
  }

  // INLINED
  static void QueueReverse(CooldownOwner& owner, RenderCommand* command) {
    owner.m_reverse.emplace_back(command);
  }
}

// OFFSET: 0x00CA66B0
// FUNCTION: fn_20964
void CooldownOwner::Draw(
  int centerX, int centerY, int radiusX, int radiusY, CooldownState& value, int limit, int offsetX, int offsetY
) {
  if (value.x <= 0 || value.y <= 0) {
    return;
  }

  const std::uint32_t color = value.color;
  if ((color & 0xFF000000u) == 0) {
    return;
  }

  const float top = static_cast<float>((centerY - radiusY));
  const float bottom = static_cast<float>((centerY + radiusY));

  value.vertices[1].x = static_cast<float>(centerX);
  value.vertices[23].x = static_cast<float>(centerX);
  value.vertices[1].y = top;
  value.vertices[23].y = top;
  value.vertices[11].x = static_cast<float>(centerX);
  value.vertices[13].x = static_cast<float>(centerX);
  value.vertices[11].y = bottom;
  value.vertices[13].y = bottom;

  for (int i = 0; i < 24; ++i) {
    if (i % 3 == 0) {
      value.vertices[i].x = static_cast<float>(centerX);
      value.vertices[i].y = static_cast<float>(centerY);
    }

    value.vertices[i].z = 0.000001f;
    value.vertices[i].rhw = 0.999999f;
    value.vertices[i].u = 0;
    value.vertices[i].v = 0;
    value.vertices[i].diffuse = color;
  }

  const int reverse = value.reverse;
  const float right = static_cast<float>((centerX + radiusX));
  const float left = static_cast<float>((centerX - radiusX));
  value.vertices[5].y = static_cast<float>(centerY);
  value.vertices[7].y = static_cast<float>(centerY);
  value.vertices[2].y = top;
  value.vertices[4].y = top;
  value.vertices[8].y = bottom;
  value.vertices[10].y = bottom;
  value.vertices[14].y = bottom;
  value.vertices[16].y = bottom;
  value.vertices[17].y = static_cast<float>(centerY);
  value.vertices[19].y = static_cast<float>(centerY);
  value.vertices[20].y = top;
  value.vertices[22].y = top;
  if (reverse == 0) {
    value.vertices[2].x = left;
    value.vertices[4].x = left;
    value.vertices[5].x = left;
    value.vertices[7].x = left;
    value.vertices[8].x = left;
    value.vertices[10].x = left;
    value.vertices[14].x = right;
    value.vertices[16].x = right;
    value.vertices[17].x = right;
    value.vertices[19].x = right;
    value.vertices[20].x = right;
    value.vertices[22].x = right;
    value.AddEdge(0, value.vertices[23].x, radiusX, 1);
    value.AddEdge(1, value.vertices[20].y, radiusY, 1);
    value.AddEdge(2, value.vertices[17].y, radiusY, 1);
    value.AddEdge(3, value.vertices[14].x, radiusX, 0);
    value.AddEdge(4, value.vertices[11].x, radiusX, 0);
    value.AddEdge(5, value.vertices[8].y, radiusY, 0);
    value.AddEdge(6, value.vertices[5].y, radiusY, 0);
    value.AddEdge(7, value.vertices[2].x, radiusX, 1);
  } else {
    value.vertices[2].x = right;
    value.vertices[4].x = right;
    value.vertices[5].x = right;
    value.vertices[7].x = right;
    value.vertices[8].x = right;
    value.vertices[10].x = right;
    value.vertices[14].x = left;
    value.vertices[16].x = left;
    value.vertices[17].x = left;
    value.vertices[19].x = left;
    value.vertices[20].x = left;
    value.vertices[22].x = left;
    value.AddEdge(0, value.vertices[1].x, radiusX, 1);
    value.AddEdge(1, value.vertices[4].y, radiusY, 1);
    value.AddEdge(2, value.vertices[7].y, radiusY, 1);
    value.AddEdge(3, value.vertices[10].x, radiusX, 0);
    value.AddEdge(4, value.vertices[13].x, radiusX, 0);
    value.AddEdge(5, value.vertices[16].y, radiusY, 0);
    value.AddEdge(6, value.vertices[19].y, radiusY, 0);
    value.AddEdge(7, value.vertices[22].x, radiusX, 1);
    this->DrawReverse(value);
    return;
  }

  if (value.duration == 0) {
    return;
  }

  switch (value.segment) {
  case 0:
    value.vertices[23].x = value.Update();
    break;
  case 1:
    value.vertices[20].y = value.Update();
    break;
  case 2:
    value.vertices[17].y = value.Update();
    break;
  case 3:
    value.vertices[14].x = value.Update();
    break;
  case 4:
    value.vertices[11].x = value.Update();
    break;
  case 5:
    value.vertices[8].y = value.Update();
    break;
  case 6:
    value.vertices[5].y = value.Update();
    break;
  case 7:
    value.vertices[2].x = value.Update();
    break;
  }

  if ((limit != 0 ? limit : 6) <= value.y) {
    return;
  }

  RenderCommand* command = g_renderer->AcquireCommand();
  command->texture = g_texMgr.Default();
  command->vertices = value.vertices;
  command->vertexCount = value.vertexCount;

  for (int i = 0; i < 24; ++i) {
    value.vertices[i].x += static_cast<float>(value.x + 12 + offsetX);
    value.vertices[i].y += static_cast<float>(value.y + 12 + offsetY);
  }

  this->m_normal.push_back(command);
}

// OFFSET: 0x00CA6C00
// FUNCTION: fn_20965
void CooldownOwner::DrawReverse(CooldownState& value) {
  if (value.duration == 0) {
    return;
  }

  switch (value.segment) {
  case 0:
    value.vertices[2].x = value.Update();
    if (value.segment != 0) {
      value.vertices[5] = value.vertices[4];
      value.vertices[2] = value.vertices[4];
    }
    break;
  case 1:
    value.vertices[5].y = value.Update();
    if (value.segment != 1) {
      value.vertices[8] = value.vertices[7];
      value.vertices[5] = value.vertices[7];
    }
    break;
  case 2:
    value.vertices[8].y = value.Update();
    if (value.segment != 2) {
      value.vertices[11] = value.vertices[10];
      value.vertices[8] = value.vertices[10];
    }
    break;
  case 3:
    value.vertices[11].x = value.Update();
    if (value.segment != 3) {
      value.vertices[14] = value.vertices[13];
      value.vertices[11] = value.vertices[13];
    }
    break;
  case 4:
    value.vertices[14].x = value.Update();
    if (value.segment != 4) {
      value.vertices[17] = value.vertices[16];
      value.vertices[14] = value.vertices[16];
    }
    break;
  case 5:
    value.vertices[17].y = value.Update();
    if (value.segment != 5) {
      value.vertices[20] = value.vertices[19];
      value.vertices[17] = value.vertices[19];
    }
    break;
  case 6:
    value.vertices[20].y = value.Update();
    if (value.segment != 6) {
      value.vertices[23] = value.vertices[22];
      value.vertices[20] = value.vertices[22];
    }
    break;
  case 7:
    value.vertices[23].x = value.Update();
    break;
  }

  RenderCommand* command = g_renderer->AcquireCommand();
  command->texture = g_texMgr.Default();
  command->vertices = value.vertices;
  command->vertexCount = value.vertexCount;

  for (int i = 0; i < 24; ++i) {
    value.vertices[i].x += static_cast<float>(value.x);
    value.vertices[i].y += static_cast<float>(value.y);
  }

  QueueReverse(*this, command);
}

// OFFSET: 0x00CA6670
// FUNCTION: fn_20963
void CooldownOwner::SubmitNormal() {
  Submit(this->m_normal);
}

// OFFSET: 0x00CA6630
// FUNCTION: fn_20962
void CooldownOwner::SubmitReverse() {
  Submit(this->m_reverse);
}
