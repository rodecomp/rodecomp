#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"

// OFFSET: 0x007D4840
// FUNCTION: fn_09582
FuncDrawWNDChar::~FuncDrawWNDChar() = default;

// OFFSET: 0x007E4FB0
// FUNCTION: fn_09729
bool FuncDrawWNDChar::DrawOnTop() {
  int job = this->m_job;
  bool drawOnTop = true;

  if (g_session.m_newRobeDrawOnTop == 1) {
    if (this->m_flags == 1) {
      job = g_session.GetRidingJob(job);
    }

    int layer = job;
    if (g_session.m_hotkeyScript->Call("_New_DrawOnTop", "ddddd>bd", this->m_robe, this->m_sex, job, this->m_action, this->m_motion, &drawOnTop, &layer) == 1) {
      return drawOnTop;
    }
  } else if (g_session.m_hotkeyScript->Call("DrawOnTop", "ddddd>b", this->m_robe, this->m_sex, job, this->m_action, this->m_motion, &drawOnTop) == 1) {
    return drawOnTop;
  }

  return true;
}
