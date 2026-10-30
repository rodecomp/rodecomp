#include "Contents/Effects/Bash.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/Particle.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>

// OFFSET: 0x00B42270

// OFFSET: 0x00B422E0

// OFFSET: 0x00B42600
// FUNCTION: fn_18871
bool CBash::Slot06() {
  return !this->IsFinished();
}

// OFFSET: 0x00B42720

// OFFSET: 0x00B42C40
