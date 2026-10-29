#include "Client/Session.h"
#include "Contents/Effects/Effect.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00B4E2D0
// FUNCTION: fn_18990
void CEffect::Slot29(int id) {
  this->id = id;
  g_session.m_hotkeyScript->Call("IsForceRenderEffect", "d>b", id, &this->forceRender);
}
