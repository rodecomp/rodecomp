#include "Contents/Sprites/SpriteLayerOrder.h"
#include "Resource/Sprites/ImfResource.h"

// OFFSET: 0x00DAAB00
// FUNCTION: fn_22575
int GetSpriteLayerOrder(CImfRes* imfRes, int layer, int baseAction, int action, int motion) {
  const int order = 8 - layer;
  switch (action - baseAction) {
  case 0:
  case 1:
  case 6:
  case 7:
    switch (order) {
    case 0:
      return imfRes->GetLayer(1, action, motion);
    case 1:
      return imfRes->GetLayer(0, action, motion);
    case 2:
      return SPRITE_LAYER_HEAD_LOW;
    case 3:
      return SPRITE_LAYER_HEAD_TOP;
    case 4:
      return SPRITE_LAYER_HEAD_MID;
    case 5:
      return SPRITE_LAYER_ROBE;
    case 6:
      return SPRITE_LAYER_WEAPON;
    case 7:
      return SPRITE_LAYER_WEAPON_EFFECT;
    case 8:
      return SPRITE_LAYER_SHIELD;
    }
    break;
  case 2:
  case 3:
  case 4:
  case 5:
    switch (order) {
    case 0:
      return SPRITE_LAYER_SHIELD;
    case 1:
      return imfRes->GetLayer(1, action, motion);
    case 2:
      return imfRes->GetLayer(0, action, motion);
    case 3:
      return SPRITE_LAYER_HEAD_LOW;
    case 4:
      return SPRITE_LAYER_HEAD_TOP;
    case 5:
      return SPRITE_LAYER_HEAD_MID;
    case 6:
      return SPRITE_LAYER_ROBE;
    case 7:
      return SPRITE_LAYER_WEAPON;
    case 8:
      return SPRITE_LAYER_WEAPON_EFFECT;
    }
    break;
  }

  return 9;
}
