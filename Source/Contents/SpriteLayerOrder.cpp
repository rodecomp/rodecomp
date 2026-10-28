#include "Contents/SpriteLayerOrder.h"
#include "Resource/ImfResource.h"

// OFFSET: 0x00DAAB00
// FUNCTION: fn_22575
int GetSpriteLayerOrder(CImfRes* imfRes, int layer, int baseAction, int action, int motion) {
  switch (action - baseAction) {
  case 0:
  case 1:
  case 6:
  case 7:
    switch (8 - layer) {
    case 0:
      return imfRes->GetLayer(1, action, motion);
    case 1:
      return imfRes->GetLayer(0, action, motion);
    case 2:
      return 2;
    case 3:
      return 3;
    case 4:
      return 4;
    case 5:
      return 8;
    case 6:
      return 5;
    case 7:
      return 6;
    case 8:
      return 7;
    }
    break;
  case 2:
  case 3:
  case 4:
  case 5:
    switch (8 - layer) {
    case 0:
      return 7;
    case 1:
      return imfRes->GetLayer(1, action, motion);
    case 2:
      return imfRes->GetLayer(0, action, motion);
    case 3:
      return 2;
    case 4:
      return 3;
    case 5:
      return 4;
    case 6:
      return 8;
    case 7:
      return 5;
    case 8:
      return 6;
    }
    break;
  }

  return 9;
}
