#include "Client/Session/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Script/LuaScript.h"

// OFFSET: 0x0080E3A0
// FUNCTION: fn_10106
bool CCharacterRender::DrawOnTop() const {
  bool drawOnTop = true;

  if (g_session.m_newRobeDrawOnTop == 1) {
    int unusedPriority = 1;
    g_session.m_hotkeyScript->Call(
      "_New_DrawOnTop", "ddddd>bd", this->robe, this->appearance[0], this->job, this->actionIndex, this->frameIndex, &drawOnTop,
      &unusedPriority
    );
  } else {
    g_session.m_hotkeyScript->Call("DrawOnTop", "ddddd>b", this->robe, this->appearance[0], this->job, this->actionIndex, this->frameIndex, &drawOnTop);
  }

  return drawOnTop;
}

// OFFSET: 0x008124B0
// FUNCTION: fn_10111
bool CCharacterRender::SetRobe(int robe) {
  this->robe = robe;
  return g_session.m_hotkeyScript->CallTable("TB_Layer_Priority", "IsHelmRobe", "d>b", robe, &this->isHelmRobe);
}
