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
// FUNCTION: fn_23356
bool CSession::IsRobeDrawOnTop(int action, int frame, bool riding) {
  bool drawOnTop = true;
  int job = this->m_job;

  if (this->m_newRobeDrawOnTop) {
    if (riding) {
      job = this->GetRidingJob(job);
    }

    int layer = 1;
    this->m_hotkeyScript->Call("_New_DrawOnTop", "ddddd>bd", this->m_robe, this->unknown_0E98, job, action, frame, &drawOnTop, &layer);
  } else {
    this->m_hotkeyScript->Call("DrawOnTop", "ddddd>b", this->m_robe, this->unknown_0E98, job, action, frame, &drawOnTop);
  }

  return drawOnTop;
}
