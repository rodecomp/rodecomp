#include "Contents/Effects/SpriteEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Contents/World/World.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdint>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  std::uint8_t m_text[Length];
  bool m_encrypted;
};

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

// OFFSET: 0x00B78840
// FUNCTION: fn_19194
CSpriteEffect::CSpriteEffect() : CEffect(-1) {
}

// OFFSET: 0x00B78B00
// FUNCTION: fn_19196
void CSpriteEffect::Close() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    FreeParticle(*it);
  }

  this->particles.clear();
}

// OFFSET: 0x00B78B40
// FUNCTION: fn_19197
void CSpriteEffect::Slot01() {
  CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x200));
  if (!particle) {
    return;
  }

  particle->unknown_0004 = this->id;
  particle->unknown_000C = 1;
  particle->unknown_0010 = 0;
  particle->unknown_0008 = 1;
  this->SetupParticle(particle);
  particle->unknown_02F4 = this->actor;
  this->particles.push_back(particle);
}

// OFFSET: 0x00B78BC0
// FUNCTION: fn_19198
void CSpriteEffect::SetupParticle(CSpriteParticle* particle) {
  if (!particle) {
    return;
  }

  particle->unknown_00EC |= 0x200;
  particle->frameDelay = 2.0f;
  particle->alpha = 255;
  this->unknown_002C = 100;

  switch (this->id) {
  case 0x3DE:
    particle->SetSprite("이팩트\\m_ef03.spr");
    particle->SetAction("이팩트\\m_ef03.act");
    particle->frameDelay = 4.0f;
    particle->scale = 1.5f;
    break;
  case 0x46A:
    particle->SetSprite("이팩트\\bakuretsu_hadou\\bakuretsu_hadou.spr");
    particle->SetAction("이팩트\\bakuretsu_hadou\\bakuretsu_hadou.act");
    particle->unknown_00EC |= 0x200;
    particle->depthOffset = 0.0003f;
    particle->flags = 0x101;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  case 0x47B:
    particle->SetSprite("이팩트\\EF_POPCORN\\EF_POPCORN.spr");
    particle->SetAction("이팩트\\EF_POPCORN\\EF_POPCORN.act");
    particle->loopMode = 1;
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 8.0f;
    break;
  case 0x47F:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_NOTHING.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_NOTHING.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x480:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WATER.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WATER.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x481:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_GROUND.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_GROUND.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x482:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_FIRE.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_FIRE.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x483:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WIND.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WIND.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x484:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_POISON.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_POISON.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x485:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_SAINT.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_SAINT.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x486:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_DARKNESS.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_DARKNESS.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x487:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_TELEKINESIS.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_TELEKINESIS.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x488:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_UNDEAD.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_UNDEAD.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case 0x4A8:
    particle->SetSprite("이팩트\\random_opt_spr\\yellow.spr");
    particle->SetAction("이팩트\\random_opt_spr\\yellow.act");
    particle->loopMode = 0;
    particle->frameDelay = 4.0f;
    break;
  case 0x4BB:
  case 0x4BC:
  case 0x4BD:
    particle->SetSprite("이팩트\\subject_aura\\subject_aura.spr");
    particle->SetAction("이팩트\\subject_aura\\subject_aura.act");
    particle->frameDelay = 8.0f;
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    particle->flags = 0x101;
    if (this->id == 0x4BC) {
      particle->animation = 0;
    } else if (this->id == 0x4BD) {
      particle->animation = 1;
    } else if (this->id == 0x4BB) {
      particle->animation = 2;
    }
    break;
  case 0x4CF:
    particle->SetSprite("이팩트\\book_of_dimension\\book_of_dimension.spr");
    particle->SetAction("이팩트\\book_of_dimension\\book_of_dimension.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D0:
    particle->SetSprite("이팩트\\book_of_creatingstar\\book_of_creatingstar.spr");
    particle->SetAction("이팩트\\book_of_creatingstar\\book_of_creatingstar.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D1:
    particle->SetSprite("이팩트\\curse_explosion\\curse_explosion.spr");
    particle->SetAction("이팩트\\curse_explosion\\curse_explosion.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D2:
    particle->SetSprite("이팩트\\soul_reaper\\soul_reaper.spr");
    particle->SetAction("이팩트\\soul_reaper\\soul_reaper.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D3:
    particle->SetSprite("이팩트\\soul_shadow\\soul_shadow.spr");
    particle->SetAction("이팩트\\soul_shadow\\soul_shadow.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D4:
    particle->SetSprite("이팩트\\soul_fairy\\soul_fairy.spr");
    particle->SetAction("이팩트\\soul_fairy\\soul_fairy.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4D5:
    particle->SetSprite("이팩트\\soul_falcon\\soul_falcon.spr");
    particle->SetAction("이팩트\\soul_falcon\\soul_falcon.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x4DA:
    particle->SetSprite("이팩트\\soul_explosion\\soul_explosion.spr");
    particle->SetAction("이팩트\\soul_explosion\\soul_explosion.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case 0x561:
    particle->SetSprite("이팩트\\valkyrie_wing\\valkyrie_wing.spr");
    particle->SetAction("이팩트\\valkyrie_wing\\valkyrie_wing.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 6.0f;
    particle->flags = 0x101;
    break;
  case 0x58B:
    particle->SetSprite("이팩트\\c_amdarais_effect.spr");
    particle->SetAction("이팩트\\c_amdarais_effect.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case 0x651:
    particle->SetSprite("이팩트\\black_bubble\\small\\blackbubble_small.spr");
    particle->SetAction("이팩트\\black_bubble\\small\\blackbubble_small.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case 0x652:
    particle->SetSprite("이팩트\\black_bubble\\midium\\blackbubble_midium.spr");
    particle->SetAction("이팩트\\black_bubble\\midium\\blackbubble_midium.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case 0x653:
    particle->SetSprite("이팩트\\black_bubble\\large\\blackbubble_large.spr");
    particle->SetAction("이팩트\\black_bubble\\large\\blackbubble_large.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case 0x95A:
    particle->SetSprite("이팩트\\Serpent_Shadow\\Serpent_Shadow.spr");
    particle->SetAction("이팩트\\Serpent_Shadow\\Serpent_Shadow.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->flags = 0xD;
    break;
  case 0x974:
    particle->SetSprite("몬스터\\violen_hit.spr");
    particle->SetAction("몬스터\\violen_hit.act");
    particle->loopMode = 1;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    break;
  case 0x978:
    particle->SetSprite("이팩트\\C_Aura_Of_Ghost_S\\C_Aura_Of_Ghost_S.spr");
    particle->SetAction("이팩트\\C_Aura_Of_Ghost_S\\C_Aura_Of_Ghost_S.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    break;
  case 0x97E:
    particle->SetSprite("이팩트\\C_Guide_Of_Dead_Text\\C_Guide_Of_Dead_Text.spr");
    particle->SetAction("이팩트\\C_Guide_Of_Dead_Text\\C_Guide_Of_Dead_Text.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 12.0f;
    break;
  case 0x97F:
    particle->SetSprite("이팩트\\C_Medjed_Text\\C_Medjed_Text.spr");
    particle->SetAction("이팩트\\C_Medjed_Text\\C_Medjed_Text.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 12.0f;
    break;
  case 0x97C:
    particle->SetSprite("이팩트\\Atque_Poenitentia\\Atque_Poenitentia.spr");
    particle->SetAction("이팩트\\Atque_Poenitentia\\Atque_Poenitentia.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->flags = 0xD;
    break;
  case 0x97D:
    particle->SetSprite("이팩트\\Perm_Frost_Oblivion\\Perm_Frost_Oblivion.spr");
    particle->SetAction("이팩트\\Perm_Frost_Oblivion\\Perm_Frost_Oblivion.act");
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case 0x99A:
    particle->SetSprite("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.spr");
    particle->SetAction("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.act");
    particle->flags = 1;
    particle->depthOffset = 0.0015f;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  case 0x98B: {
    const char spriteSource[] = {'\x99', '\x4B', '\xC5', '\xAC', '\x93', '\x65', '\x7B', '\x98', '\x06', '\xB5', '\x33', '\x4F', '\x60',
                                 '\x99', '\x48', '\xA8', '\x1F', '\xE2', '\x70', '\x09', '\x34', '\xE5', '\x54', '\xAB', '\x2B', '\x87'};
    particle->SetSprite(DecryptName<0xDB27CB557D038759ULL>(spriteSource));

    const char actionSource[] = {'\x4D', '\x71', '\x7D', '\xB2', '\x71', '\x97', '\xE7', '\x12', '\xD2', '\x8F', '\x8B', '\x51', '\x82',
                                 '\x6B', '\xD4', '\x22', '\xCB', '\xD8', '\xC8', '\x17', '\xD6', '\x17', '\xDA', '\x32', '\xF9', '\xBD'};
    particle->SetAction(DecryptName<0x51BB39B763BBBD8DULL>(actionSource));
    particle->flags = 1;
    particle->depthOffset = 0.0015f;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  }
  }
}

// OFFSET: 0x00B79C40
// FUNCTION: fn_19201
void CSpriteEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    if (particle->unknown_00E8 > 0) {
      particle->Draw(1);
    }
  }
}

// OFFSET: 0x00B79520
// FUNCTION: fn_19199
bool CSpriteEffect::Slot06() {
  switch (this->id) {
  case 0x3DE:
    return !this->IsFinished();
  case 0x4A8: {
    if (this->actor == 0) {
      return false;
    }

    if (!g_modeMgr.GetGameMode()) {
      return false;
    }

    CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
    if (!scene) {
      return false;
    }

    if (!scene->GetItemByAID(this->actor)) {
      return false;
    }

    return true;
  }
  case 0x46A:
  case 0x47F:
  case 0x480:
  case 0x481:
  case 0x482:
  case 0x483:
  case 0x484:
  case 0x485:
  case 0x486:
  case 0x487:
  case 0x488:
  case 0x4BB:
  case 0x4BC:
  case 0x4BD:
  case 0x561:
  case 0x58B:
  case 0x651:
  case 0x652:
  case 0x653:
  case 0x95A:
  case 0x978:
  case 0x97C:
  case 0x97D:
  case 0x97E:
  case 0x97F:
  case 0x98B:
  case 0x99A:
    return this->ActorExists();
  }

  for (CSpriteParticle* particle : this->particles) {
    if (particle->life == 0.0f) {
      return true;
    }

    if (particle->life * 60.0f > static_cast<float>(particle->unknown_00E8)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00B79780
// FUNCTION: fn_19200
void CSpriteEffect::Slot03() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    ++particle->unknown_00E8;

    switch (this->id) {
    case 0x46A: {
      int height = 8;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = static_cast<int>(actor->GetNameOffset() + 8.0f);
      }

      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - static_cast<float>(height);
      particle->position.z = this->position[2];
      break;
    }
    case 0x47B:
    case 0x4A8:
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - 10.0f;
      particle->position.z = this->position[2];
      break;
    case 0x47F:
    case 0x480:
    case 0x481:
    case 0x482:
    case 0x483:
    case 0x484:
    case 0x485:
    case 0x486:
    case 0x487:
    case 0x488: {
      int height = 25;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = static_cast<int>(actor->GetNameOffset() + 25.0f);
      }

      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - static_cast<float>(height);
      particle->position.z = this->position[2];
      break;
    }
    case 0x4BB:
    case 0x4BC:
    case 0x4BD: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset() + 8.0f);
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - static_cast<float>(height);
      particle->position.z = this->position[2];
      break;
    }
    case 0x561: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset() + 3.0f);
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - static_cast<float>(height);
      particle->position.z = this->position[2];
      break;
    }
    case 0x95A:
    case 0x978: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset());
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - static_cast<float>(height);
      particle->position.z = this->position[2];
      break;
    }
    case 0x97C: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const float height = actor->GetNameOffset();
      const float offset = g_session.IsDoramJob(actor->GetEffectiveJob(-1)) ? height + 13.7f : height + 17.0f;
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - offset;
      particle->position.z = this->position[2];
      break;
    }
    case 0x97D: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const float offset = g_session.IsDoramJob(actor->GetEffectiveJob(-1)) ? 2.2f : 0.4f;
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - offset;
      particle->position.z = this->position[2];
      break;
    }
    case 0x97E:
    case 0x97F: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const float height = actor->GetNameOffset();
      particle->position.x = this->position[0];
      particle->position.y = this->position[1] - height;
      particle->position.z = this->position[2];
      break;
    }
    case 0x98B:
      particle->unknown_02BC = 1;
      particle->unknown_02BD = 1;
      particle->unknown_02C4 = 0.0f;
      particle->unknown_02C0 = 0.0f;
      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
      break;
    case 0x99A:
      particle->unknown_02BC = 1;
      particle->unknown_02BD = 1;
      particle->unknown_02C4 = -105.0f;
      particle->unknown_02C0 = 0.0f;
      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
      break;
    default:
      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
      break;
    }
  }
}
