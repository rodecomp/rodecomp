#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

#pragma pack(push, 1)
struct SoundPacket {
  /* 0000 */ std::uint16_t id;
  /* 0002 */ char name[24];
  /* 001A */ unsigned char act;
  /* 001B */ std::uint32_t term;
  /* 001F */ std::uint32_t npcId;
};
#pragma pack(pop)

static_assert(sizeof(SoundPacket) == 0x23);

// OFFSET: 0x00D42C20
// FUNCTION: fn_21830
void CGameMode::ReceiveSound(const unsigned char* data) {
  const SoundPacket* packet = reinterpret_cast<const SoundPacket*>(data);

  switch (packet->act) {
  case 0: {
    CRenderObject* actor = this->scene->GetGameActorByAID(packet->npcId);
    if (actor != nullptr) {
      CPlayer* player = this->scene->player;
      g_soundMgr->PlayWave(
        packet->name, player->render.position.x - actor->render.position.x, 0.0f, player->render.position.z - actor->render.position.z, 250, 40, 1.0f, 0
      );
    } else {
      g_soundMgr->PlayWave(packet->name, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;
  }
  case 1: {
    GameModeLoopSound sound;
    sound.name = packet->name;
    sound.term = packet->term;
    sound.nextTick = timeGetTime() + packet->term;
    sound.npcId = packet->npcId;
    this->loopSounds.push_back(sound);
    g_soundMgr->PlayWave(packet->name, 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  }
  case 2:
    for (std::vector<GameModeLoopSound>::iterator it = this->loopSounds.begin(); it != this->loopSounds.end(); ++it) {
      GameModeLoopSound sound = *it;
      if (std::strcmp(sound.name.c_str(), packet->name) == 0) {
        this->loopSounds.erase(it);
        break;
      }
    }
    break;
  }
}
