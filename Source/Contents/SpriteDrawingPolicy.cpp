#include "Contents/SpriteDrawingPolicy.h"
#include "Script/LuaScript.h"

// OFFSET: 0x0080E3A0
// FUNCTION: fn_10106
bool SpriteDrawingPolicy::DrawOnTop() const {
  bool drawOnTop = true;

  if (this->newDrawOnTop == 1) {
    int unusedPriority = 1;
    this->script.Call(
      "_New_DrawOnTop", "ddddd>bd", this->argument00BC, this->argument0018, this->argument0010,
      this->argument0040, this->argument0044, &drawOnTop, &unusedPriority
    );
  } else {
    this->script.Call(
      "DrawOnTop", "ddddd>b", this->argument00BC, this->argument0018, this->argument0010,
      this->argument0040, this->argument0044, &drawOnTop
    );
  }

  return drawOnTop;
}

// OFFSET: 0x008124B0
// FUNCTION: fn_10111
bool SpriteDrawingPolicy::SetRobe(int robe) {
  this->argument00BC = robe;
  return this->script.CallTable("TB_Layer_Priority", "IsHelmRobe", "d>b", robe, &this->helmRobe);
}
