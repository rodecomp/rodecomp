#include "Contents/Sprites/SpriteLayerOrder.h"
#include "Contents/Actors/ActorEnums.h"
#include "Resource/Sprites/ImfResource.h"

// OFFSET: 0x00DAAB00
// FUNCTION: fn_22575
int GetSpriteLayerOrder(CImfRes* imfRes, int layer, int baseAction, int action, int motion) {
  const int order = 8 - layer;
  switch (action - baseAction) {
  case SPRITE_DIR_DOWN:
  case SPRITE_DIR_DOWN_LEFT:
  case SPRITE_DIR_RIGHT:
  case SPRITE_DIR_DOWN_RIGHT:
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
  case SPRITE_DIR_LEFT:
  case SPRITE_DIR_UP_LEFT:
  case SPRITE_DIR_UP:
  case SPRITE_DIR_UP_RIGHT:
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
