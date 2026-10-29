#include "Client/Session.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00E31D30
// FUNCTION: fn_24176
bool CSession::IsTopLayerRobe(int robe) {
  if (robe == -1) {
    robe = this->m_robe;
  }

  bool isTopLayer = false;
  g_session.m_hotkeyScript->Call("IsTopLayer", "d>b", robe, &isTopLayer);
  return isTopLayer;
}

// OFFSET: 0x00DDB920
