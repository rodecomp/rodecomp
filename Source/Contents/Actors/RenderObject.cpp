#include "Contents/Actors/RenderObject.h"

#include "Utility/Math/ScalarConversion.h"

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
CRenderObject::CRenderObject(ResourceMgr& resources, JobNames& names) : render(resources, names) {
}

// OFFSET: 0x00CA8F90
// FUNCTION: fn_20998
CRenderObject::~CRenderObject() = default;

// OFFSET: 0x00CAFDC0
// FUNCTION: fn_21089
bool CRenderObject::Update() {
  return false;
}

// OFFSET: 0x00CBF920
// FUNCTION: fn_21115
void CRenderObject::SendObjectMessage(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CC2D40
// FUNCTION: fn_21144
void CRenderObject::SetScreenBounds(const float*, float, float) {
}

// OFFSET: 0x00CC3DC0
// FUNCTION: fn_21154
void CRenderObject::SetScreenPosition(float x, float y) {
  this->render.screenX = TruncateToInt(x);
  this->render.screenY = TruncateToInt(y);
}
