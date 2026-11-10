#include "Contents/Actors/CharacterRender.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Render/SpriteGeometry.h"
#include "Script/LuaScript.h"

constexpr float robeTopLayerThreshold = 320.0f;

// GLOBAL: gl_24739
float g_cameraDistance = 300.0f;

// OFFSET: 0x0080DAA0
// FUNCTION: fn_10103
void CCharacterRender::OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const {
  switch (layer) {
  case 2:
    if (g_cameraDistance < robeTopLayerThreshold) {
      quad.bottomDepth = bottomDepth + 0.000027f;
      quad.topDepth = topDepth + 0.000027f;
    } else {
      quad.bottomDepth = bottomDepth;
      quad.topDepth = topDepth;
    }
    break;
  case 3:
    if (g_cameraDistance < robeTopLayerThreshold) {
      quad.bottomDepth = bottomDepth + 0.00004f;
      quad.topDepth = 0.0001f + topDepth;
    } else {
      quad.bottomDepth = bottomDepth;
      quad.topDepth = 0.000045f + topDepth;
    }
    break;
  case 4:
    if (g_cameraDistance < robeTopLayerThreshold) {
      topDepth += 0.00003f;
      bottomDepth += 0.00003f;
    }
    quad.bottomDepth = bottomDepth;
    quad.topDepth = topDepth;
    break;
  }
}

// OFFSET: 0x0080E4D0
// FUNCTION: fn_10107
void CCharacterRender::OffsetAccessoryLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const {
  switch (layer) {
  case 1:
    if (this->upperHeadgear != 1095) {
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = bottomDepth + 0.000025f;
        quad.topDepth = topDepth + 0.000025f;
      } else {
        quad.bottomDepth = bottomDepth;
        quad.topDepth = topDepth;
      }
    } else if (g_cameraDistance < 32.0f) {
      quad.bottomDepth = bottomDepth - 0.000008f;
      quad.topDepth = topDepth - 0.000008f;
    } else {
      quad.bottomDepth = bottomDepth - 0.0000005f;
      quad.topDepth = topDepth - 0.0000005f;
    }
    break;
  case 3:
    if (g_cameraDistance < robeTopLayerThreshold) {
      quad.bottomDepth = bottomDepth + 0.00004f;
      quad.topDepth = 0.0001f + topDepth;
    } else {
      quad.bottomDepth = bottomDepth;
      quad.topDepth = 0.000045f + topDepth;
    }
    break;
  case 4:
    if (this->upperHeadgear == 1485) {
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = 0.00035f + bottomDepth;
        quad.topDepth = 0.0006f + topDepth;
      } else {
        quad.bottomDepth = 0.00001f + bottomDepth;
        quad.topDepth = 0.00015f + topDepth;
      }
    } else {
      switch (this->middleHeadgear) {
      case 1318:
        if (g_cameraDistance < robeTopLayerThreshold) {
          quad.bottomDepth = 0.00025f + bottomDepth;
          quad.topDepth = 0.0001f + topDepth;
        } else {
          quad.bottomDepth = 0.00005f + bottomDepth;
          quad.topDepth = 0.00005f + topDepth;
        }
        break;
      case 1354:
        if (g_cameraDistance < robeTopLayerThreshold) {
          quad.bottomDepth = bottomDepth + 0.00009f;
          quad.topDepth = topDepth - 0.00005f;
        } else {
          quad.bottomDepth = bottomDepth;
          quad.topDepth = topDepth;
        }
        break;
      case 1363:
        if (g_cameraDistance < robeTopLayerThreshold) {
          quad.bottomDepth = bottomDepth + 0.000027f + 0.0004f;
          quad.topDepth = topDepth + 0.000027f + 0.0007f;
        } else {
          quad.bottomDepth = bottomDepth + 0.00007f;
          quad.topDepth = topDepth + 0.00017f;
        }
        break;
      case 1582:
        if (g_cameraDistance < robeTopLayerThreshold) {
          quad.bottomDepth = bottomDepth + 0.0002f;
          quad.topDepth = topDepth + 0.0002f;
        } else {
          quad.bottomDepth = bottomDepth + 0.00004f;
          quad.topDepth = topDepth + 0.00006f;
        }
        break;
      default:
        if (g_cameraDistance < robeTopLayerThreshold) {
          quad.bottomDepth = bottomDepth + 0.00003f;
          quad.topDepth = topDepth + 0.00003f;
        } else {
          quad.bottomDepth = bottomDepth;
          quad.topDepth = topDepth;
        }
        break;
      }
    }
    break;
  case 2:
    switch (this->lowerHeadgear) {
    case 1041:
    case 1358:
    case 1478:
    case 1499:
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = 0.0008f + bottomDepth;
        quad.topDepth = 0.0008f + topDepth;
      } else {
        quad.bottomDepth = 0.00019f + bottomDepth;
        quad.topDepth = 0.00019f + topDepth;
      }
      break;
    case 1235:
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = 0.0008f + bottomDepth;
        quad.topDepth = 0.0008f + topDepth;
      } else {
        quad.bottomDepth = 0.00013f + bottomDepth;
        quad.topDepth = 0.00013f + topDepth;
      }
      break;
    case 1488:
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = bottomDepth + 0.0008f;
        quad.topDepth = topDepth + 0.0008f;
      } else {
        quad.bottomDepth = bottomDepth + 0.00019f;
        quad.topDepth = topDepth + 0.00019f;
      }
      break;
    default:
      if (g_cameraDistance < robeTopLayerThreshold) {
        quad.bottomDepth = bottomDepth + 0.000027f;
        quad.topDepth = topDepth + 0.000027f;
      } else {
        quad.bottomDepth = bottomDepth;
        quad.topDepth = topDepth;
      }
      break;
    }
    break;
  default:
    quad.bottomDepth = bottomDepth;
    quad.topDepth = topDepth;
    break;
  }

  bool enabled = false;
  double nearOffset = 0.0;
  double farOffset = 0.0;
  g_session.m_hotkeyScript->Call("OffsetItemPos_GetUppingBottomItem", "d>bll", this->lowerHeadgear, &enabled, &nearOffset, &farOffset);

  if (layer == 2 && enabled) {
    if (nearOffset == 0.0) {
      nearOffset = 0.00025;
    }
    if (farOffset == 0.0) {
      farOffset = 0.0001;
    }

    const double offset = g_cameraDistance < robeTopLayerThreshold ? nearOffset : farOffset;
    quad.bottomDepth = static_cast<float>(static_cast<double>(bottomDepth) + offset);
    quad.topDepth = static_cast<float>(static_cast<double>(topDepth) + offset);
  }
}
