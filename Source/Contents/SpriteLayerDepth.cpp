#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Render/SpriteGeometry.h"
#include "Script/LuaScript.h"

constexpr float robeTopLayerThreshold = 320.0f;

// GLOBAL: gl_24739
float g_cameraDistance = 300.0f;

// OFFSET: 0x0080DAA0
// FUNCTION: fn_10103
void SpriteDrawingPolicy::OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const {
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
      bottomDepth += 0.00003f;
      topDepth += 0.00003f;
    }
    quad.bottomDepth = bottomDepth;
    quad.topDepth = topDepth;
    break;
  }
}

// OFFSET: 0x0080E4D0
// FUNCTION: fn_10107
void AccessoryLayerDepth::OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const {
  float bottom = bottomDepth;
  float top = topDepth;

  switch (layer) {
  case 1:
    if (this->topAccessory == 1095) {
      if (g_cameraDistance < 32.0f) {
        bottom = bottomDepth - 0.000008f;
        top = topDepth - 0.000008f;
      } else {
        bottom = bottomDepth - 0.0000005f;
        top = topDepth - 0.0000005f;
      }
    } else if (g_cameraDistance < robeTopLayerThreshold) {
      bottom = bottomDepth + 0.000025f;
      top = topDepth + 0.000025f;
    }
    break;
  case 2:
    switch (this->bottomAccessory) {
    case 1041:
    case 1358:
    case 1478:
    case 1499:
      if (g_cameraDistance < robeTopLayerThreshold) {
        bottom = 0.0008f + bottomDepth;
        top = 0.0008f + topDepth;
      } else {
        bottom = 0.00019f + bottomDepth;
        top = 0.00019f + topDepth;
      }
      break;
    case 1235:
      if (g_cameraDistance < robeTopLayerThreshold) {
        bottom = 0.0008f + bottomDepth;
        top = 0.0008f + topDepth;
      } else {
        bottom = 0.00013f + bottomDepth;
        top = 0.00013f + topDepth;
      }
      break;
    case 1488:
      if (g_cameraDistance < robeTopLayerThreshold) {
        bottom = bottomDepth + 0.0008f;
        top = topDepth + 0.0008f;
      } else {
        bottom = bottomDepth + 0.00019f;
        top = topDepth + 0.00019f;
      }
      break;
    default:
      if (g_cameraDistance < robeTopLayerThreshold) {
        bottom = bottomDepth + 0.000027f;
        top = topDepth + 0.000027f;
      }
      break;
    }
    break;
  case 3:
    if (g_cameraDistance < robeTopLayerThreshold) {
      bottom = bottomDepth + 0.00004f;
      top = 0.0001f + topDepth;
    } else {
      top = 0.000045f + topDepth;
    }
    break;
  case 4:
    if (this->topAccessory == 1485) {
      if (g_cameraDistance < robeTopLayerThreshold) {
        bottom = 0.00035f + bottomDepth;
        top = 0.0006f + topDepth;
      } else {
        bottom = 0.00001f + bottomDepth;
        top = 0.00015f + topDepth;
      }
    } else {
      switch (this->middleAccessory) {
      case 1318:
        if (g_cameraDistance < robeTopLayerThreshold) {
          bottom = 0.00025f + bottomDepth;
          top = 0.0001f + topDepth;
        } else {
          bottom = 0.00005f + bottomDepth;
          top = 0.00005f + topDepth;
        }
        break;
      case 1354:
        if (g_cameraDistance < robeTopLayerThreshold) {
          bottom = bottomDepth + 0.00009f;
          top = topDepth - 0.00005f;
        }
        break;
      case 1363:
        if (g_cameraDistance < robeTopLayerThreshold) {
          bottom = bottomDepth + 0.000027f + 0.0004f;
          top = topDepth + 0.000027f + 0.0007f;
        } else {
          bottom = bottomDepth + 0.00007f;
          top = topDepth + 0.00017f;
        }
        break;
      case 1582:
        if (g_cameraDistance < robeTopLayerThreshold) {
          bottom = bottomDepth + 0.0002f;
          top = topDepth + 0.0002f;
        } else {
          bottom = bottomDepth + 0.00004f;
          top = topDepth + 0.00006f;
        }
        break;
      default:
        if (g_cameraDistance < robeTopLayerThreshold) {
          bottom = bottomDepth + 0.00003f;
          top = topDepth + 0.00003f;
        }
        break;
      }
    }
    break;
  }

  quad.bottomDepth = bottom;
  quad.topDepth = top;

  bool enabled = false;
  double nearOffset = 0.0;
  double farOffset = 0.0;
  g_session.m_hotkeyScript->Call("OffsetItemPos_GetUppingBottomItem", "d>bll", this->bottomAccessory, &enabled, &nearOffset, &farOffset);

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
