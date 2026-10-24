#ifndef RAGNAROK_SOURCE_UI_COOLDOWNDRAWING_H
#define RAGNAROK_SOURCE_UI_COOLDOWNDRAWING_H

#include "UI/CooldownState.h"

#include <vector>

class RenderCommandPool;
class CRenderSystemBase;
struct RenderQueues;

// Original cooldown owner's two transient command lists (+4 and +16).
// Commands borrow CooldownState::vertices until the renderer consumes them.
struct CooldownDrawing {
  std::vector<RenderCommand*> normal;
  std::vector<RenderCommand*> reverse;

  void Draw(
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
  );
  void SubmitNormal(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd);
  void SubmitReverse(RenderQueues& queues, bool depthAlpha, float depthStart, float depthEnd);

private:
  void
  DrawReverse(CooldownState& value, const CooldownClock& clock, RenderCommandPool& pool, CRenderSystemBase& system);
};

#endif
