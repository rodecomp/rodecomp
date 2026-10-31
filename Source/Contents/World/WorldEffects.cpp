#include "Contents/World/World.h"

#include "Client/Mode/CGameMode.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectMaker.h"
#include "Contents/Sound/SoundConfiguration.h"
#include "Contents/Sound/SoundMaker.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/WorldResource.h"

#include <string>

// GLOBAL: gl_25108
SoundConfiguration g_soundConfiguration;

// OFFSET: 0x00AC9040
// FUNCTION: fn_17682
void CWorld::LoadMapEffects() {
  std::string name = reinterpret_cast<const char*>(this->gameMode->SendMsg(9, 0, 0, 0, 0));
  C3dWorldRes* resource = static_cast<C3dWorldRes*>(ResourceMgr::Instance().Load(name.c_str()));

  for (auto it = resource->effects.begin(); it != resource->effects.end(); ++it) {
    WorldEffectRecord* record = *it;

    if (g_effectMgr->HasAllocator(record->id)) {
      g_effectMgr->Spawn(0, record->id, record->position[0], record->position[1], record->position[2], 0, -1, 0, 0);
    } else {
      CEffectMaker* effect = new CEffectMaker;
      const int address = reinterpret_cast<int>(record);
      effect->SendMsg(nullptr, 14, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->objects.push_back(effect);
    }
  }

  g_effectMgr->LoadMapEffectScript(true);

  if (g_soundConfiguration.mode != 0) {
    for (auto it = resource->sounds.begin(); it != resource->sounds.end(); ++it) {
      WorldSoundRecord* record = *it;
      CSoundMaker* sound = new CSoundMaker;
      const int address = reinterpret_cast<int>(record);
      sound->SendMsg(nullptr, 14, 0, address, address >> 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      this->objects.push_back(sound);
    }
  }
}
