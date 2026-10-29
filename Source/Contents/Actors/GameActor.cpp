#include "Contents/Actors/GameActor.h"

#include "Client/Session.h"
#include "Resource/Sprites/ActionResource.h"

#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CA83A0
// FUNCTION: fn_20988
CGameActor::CGameActor()
    : unknown_0214(), unknown_02F0(0), unknown_02F4(0), unknown_0318(), unknown_0330(), unknown_0348(), unknown_0360(),
      unknown_03F0(), unknown_0404(0), unknown_0408(0) {
  this->ResetState();
}

// OFFSET: 0x00CAF4F0
// FUNCTION: fn_21084
void CGameActor::ResetState() {
  this->unknown_037C[11] = 0;
  this->unknown_037C[2] = 0;
  this->unknown_037C[3] = 0;
  this->unknown_037C[1] = 0;
  this->unknown_037C[0] = 0;
  this->unknown_037C[6] = 0;
  this->unknown_037C[4] = 0;
  this->unknown_037C[5] = 0;
  this->unknown_037C[9] = 0;
  this->unknown_037C[7] = 0;
  this->unknown_037C[8] = 0;
  this->unknown_037C[10] = 0;
  this->unknown_037C[12] = 0;
  this->render.position.x = 0.0f;
  this->render.position.y = 0.0f;
  this->render.position.z = 0.0f;
  this->render.angle = 0.0f;
  this->render.animation = 0;
  this->render.previousAnimation = 0;
  this->render.curMotion = 0;
  this->render.unknown_0070 = 0;
  this->render.unknown_006C = 0;
  this->render.speed = 1.0f;
  this->render.unknown_00A4Bytes[0] = 1;
  this->unknown_0214.x = 0.0f;
  this->unknown_0214.y = 0.0f;
  this->unknown_0214.z = 0.0f;
  this->unknown_0240 = 0;
  this->render.accountId = 0xFFFFFFFF;
  this->unknown_0258 = g_session.GetServerTime();
  this->unknown_0244 = timeGetTime();
  this->render.screenX = 0;
  this->render.screenY = 0;
  this->unknown_0264[0] = 0;
  this->unknown_0264[1] = 0;
  this->unknown_0264[2] = 0;
  this->unknown_0264[3] = 0;
  this->unknown_0288 = 0;
  this->unknown_0290 = 0;
  this->render.isMotionFinished = false;
  this->hitTargetAid = 0;
  this->unknown_0378 = -1.0f;
  this->unknown_023C = -1;
  this->render.unknown_00A4Bytes[1] = 1;
  this->render.unknown_008C = timeGetTime();
  this->unknown_027C = 0;
  this->unknown_0280 = timeGetTime() - 5000;
  this->unknown_0284 = timeGetTime() - 10000;
  this->unknown_02D8 = 3600;
  this->render.color = 0xFFFFFFFF;
  this->render.visible = 1;
  this->render.unknown_00A2 = 0;
  this->unknown_02B0 = 0;
  this->bodyState = 0;
  this->stateFlags = 0;
  this->Slot47(0);
  this->unknown_02C4 = 0;
  this->unknown_02C8 = 0;
  this->unknown_02DC = 0;
  this->unknown_02B8 = 0;
  this->unknown_02B4 = 0;
  this->unknown_0210 = 0;
  this->unknown_0178 = 0;
  this->unknown_017C = 0;
  this->unknown_0180 = 0;
  std::memset(this->unknown_0190, 0, sizeof(this->unknown_0190));
  this->guildId = 0;
  this->unknown_0278 = 0;
  this->unknown_02E0 = 0;
  this->unknown_0188 = 0;
  this->unknown_018C = 0;
  this->unknown_0238 = 0;
  this->unknown_0184 = 0;
  this->unknown_0250 = 0;
  this->unknown_030C = 0;
  this->unknown_0310 = 0;
  this->unknown_0314 = 4;
  this->unknown_0264[4] = 0;
  this->unknown_03B8[0] = -1;
  this->unknown_03B8[1] = -1;
  this->unknown_03B8[2] = -1;
  this->unknown_03C4 = -1;
  this->unknown_03C8 = 0;
  this->charmType = 0;
  this->unknown_03D0 = 0;
  this->unknown_0158[0] = 0;
  this->unknown_0158[1] = 0;
  this->unknown_0158[2] = 0;
  this->unknown_0158[3] = 0;
  this->unknown_0158[4] = 0;
  this->unknown_0158[5] = 0;
  this->unknown_02F8 = 0;
  this->unknown_02FC = 0;
  this->unknown_0300 = 0;
  this->unknown_0304 = 0;
  this->unknown_0305 = 0;
  this->unknown_0308 = 0;
  this->unknown_03DC = 0;
  this->unknown_03E0 = 0;
  this->unknown_02EC = 0;
  this->unknown_03D4 = 0;
  this->unknown_03D8 = 0;
  this->unknown_03E8 = 0;
  this->unknown_03EC = 0;
}

// OFFSET: 0x00CB0A90
// FUNCTION: fn_21095
void CGameActor::UpdateMotionFrame() {
  const int frames = static_cast<int>(this->render.action->FrameCount(this->render.previousAnimation));
  this->render.curMotion = static_cast<int>(this->unknown_0240 * 0.37f * 4.0f / this->render.speed) % frames;
}
