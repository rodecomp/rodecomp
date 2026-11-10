#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Particles/SpriteEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/SpriteParticle.h"
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
void CSpriteEffect::Open() {
  CSpriteParticle* particle = static_cast<CSpriteParticle*>(CreateSpriteParticle(reinterpret_cast<const vector3d*>(this->position), 0x200));
  if (!particle) {
    return;
  }

  particle->effectId = this->id;
  particle->columnIndex = 1;
  particle->rowIndex = 0;
  particle->groupIndex = 1;
  this->SetupParticle(particle);
  particle->actorId = this->actor;
  this->particles.push_back(particle);
}

// OFFSET: 0x00B78BC0
// FUNCTION: fn_19198
void CSpriteEffect::SetupParticle(CSpriteParticle* particle) {
  if (!particle) {
    return;
  }

  particle->renderOptions |= 0x200;
  particle->frameDelay = 2.0f;
  particle->alpha = 255;
  this->effectLifetime = 100;

  switch (this->id) {
  case EF_GROUND_EXPLOSION:
    particle->SetSprite("이팩트\\m_ef03.spr");
    particle->SetAction("이팩트\\m_ef03.act");
    particle->frameDelay = 4.0f;
    particle->scale = 1.5f;
    break;
  case EF_BAKURETSU_HADOU:
    particle->SetSprite("이팩트\\bakuretsu_hadou\\bakuretsu_hadou.spr");
    particle->SetAction("이팩트\\bakuretsu_hadou\\bakuretsu_hadou.act");
    particle->renderOptions |= 0x200;
    particle->depthOffset = 0.0003f;
    particle->flags = 0x101;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  case EF_POPCORN:
    particle->SetSprite("이팩트\\EF_POPCORN\\EF_POPCORN.spr");
    particle->SetAction("이팩트\\EF_POPCORN\\EF_POPCORN.act");
    particle->loopMode = 1;
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 8.0f;
    break;
  case EF_IMMUNE_PROPERTY_NOTHING:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_NOTHING.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_NOTHING.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_WATER:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WATER.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WATER.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_GROUND:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_GROUND.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_GROUND.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_FIRE:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_FIRE.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_FIRE.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_WIND:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WIND.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_WIND.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_POISON:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_POISON.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_POISON.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_SAINT:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_SAINT.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_SAINT.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_DARKNESS:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_DARKNESS.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_DARKNESS.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_TELEKINESIS:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_TELEKINESIS.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_TELEKINESIS.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_IMMUNE_PROPERTY_UNDEAD:
    particle->SetSprite("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_UNDEAD.spr");
    particle->SetAction("이팩트\\EF_IMMUNE_PROPERTY\\EF_IMMUNE_PROPERTY_UNDEAD.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    break;
  case EF_RANDOM_OPT_YELLOW:
    particle->SetSprite("이팩트\\random_opt_spr\\yellow.spr");
    particle->SetAction("이팩트\\random_opt_spr\\yellow.act");
    particle->loopMode = 0;
    particle->frameDelay = 4.0f;
    break;
  case EF_UNK_1211:
  case EF_SUBJECT_AURA_1212:
  case EF_SUBJECT_AURA_1213:
    particle->SetSprite("이팩트\\subject_aura\\subject_aura.spr");
    particle->SetAction("이팩트\\subject_aura\\subject_aura.act");
    particle->frameDelay = 8.0f;
    particle->loopMode = 0;
    particle->depthOffset = 0.0003f;
    particle->flags = 0x101;
    if (this->id == EF_SUBJECT_AURA_1212) {
      particle->animation = 0;
    } else if (this->id == EF_SUBJECT_AURA_1213) {
      particle->animation = 1;
    } else if (this->id == EF_UNK_1211) {
      particle->animation = 2;
    }
    break;
  case EF_BOOK_OF_DIMENSION:
    particle->SetSprite("이팩트\\book_of_dimension\\book_of_dimension.spr");
    particle->SetAction("이팩트\\book_of_dimension\\book_of_dimension.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_BOOK_OF_CREATINGSTAR:
    particle->SetSprite("이팩트\\book_of_creatingstar\\book_of_creatingstar.spr");
    particle->SetAction("이팩트\\book_of_creatingstar\\book_of_creatingstar.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_CURSE_EXPLOSION:
    particle->SetSprite("이팩트\\curse_explosion\\curse_explosion.spr");
    particle->SetAction("이팩트\\curse_explosion\\curse_explosion.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_SOUL_REAPER:
    particle->SetSprite("이팩트\\soul_reaper\\soul_reaper.spr");
    particle->SetAction("이팩트\\soul_reaper\\soul_reaper.act");
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_SOUL_SHADOW:
    particle->SetSprite("이팩트\\soul_shadow\\soul_shadow.spr");
    particle->SetAction("이팩트\\soul_shadow\\soul_shadow.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_SOUL_FAIRY:
    particle->SetSprite("이팩트\\soul_fairy\\soul_fairy.spr");
    particle->SetAction("이팩트\\soul_fairy\\soul_fairy.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_SOUL_FALCON:
    particle->SetSprite("이팩트\\soul_falcon\\soul_falcon.spr");
    particle->SetAction("이팩트\\soul_falcon\\soul_falcon.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_SOUL_EXPLOSION:
    particle->SetSprite("이팩트\\soul_explosion\\soul_explosion.spr");
    particle->SetAction("이팩트\\soul_explosion\\soul_explosion.act");
    particle->depthOffset = 0.0003f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 1;
    particle->flags = 0x101;
    break;
  case EF_VALKYRIE_WING:
    particle->SetSprite("이팩트\\valkyrie_wing\\valkyrie_wing.spr");
    particle->SetAction("이팩트\\valkyrie_wing\\valkyrie_wing.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 6.0f;
    particle->flags = 0x101;
    break;
  case EF_C_AMDARAIS_EFFECT:
    particle->SetSprite("이팩트\\c_amdarais_effect.spr");
    particle->SetAction("이팩트\\c_amdarais_effect.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case EF_BLACKBUBBLE_SMALL:
    particle->SetSprite("이팩트\\black_bubble\\small\\blackbubble_small.spr");
    particle->SetAction("이팩트\\black_bubble\\small\\blackbubble_small.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case EF_BLACKBUBBLE_MIDIUM:
    particle->SetSprite("이팩트\\black_bubble\\midium\\blackbubble_midium.spr");
    particle->SetAction("이팩트\\black_bubble\\midium\\blackbubble_midium.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case EF_BLACKBUBBLE_LARGE:
    particle->SetSprite("이팩트\\black_bubble\\large\\blackbubble_large.spr");
    particle->SetAction("이팩트\\black_bubble\\large\\blackbubble_large.act");
    particle->depthOffset = 0.0001f;
    particle->frameDelay = 6.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case EF_SERPENT_SHADOW:
    particle->SetSprite("이팩트\\Serpent_Shadow\\Serpent_Shadow.spr");
    particle->SetAction("이팩트\\Serpent_Shadow\\Serpent_Shadow.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->flags = 0xD;
    break;
  case EF_VIOLEN_HIT:
    particle->SetSprite("몬스터\\violen_hit.spr");
    particle->SetAction("몬스터\\violen_hit.act");
    particle->loopMode = 1;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    break;
  case EF_C_AURA_OF_GHOST_S:
    particle->SetSprite("이팩트\\C_Aura_Of_Ghost_S\\C_Aura_Of_Ghost_S.spr");
    particle->SetAction("이팩트\\C_Aura_Of_Ghost_S\\C_Aura_Of_Ghost_S.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    break;
  case EF_C_GUIDE_OF_DEAD_TEXT:
    particle->SetSprite("이팩트\\C_Guide_Of_Dead_Text\\C_Guide_Of_Dead_Text.spr");
    particle->SetAction("이팩트\\C_Guide_Of_Dead_Text\\C_Guide_Of_Dead_Text.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 12.0f;
    break;
  case EF_C_MEDJED_TEXT:
    particle->SetSprite("이팩트\\C_Medjed_Text\\C_Medjed_Text.spr");
    particle->SetAction("이팩트\\C_Medjed_Text\\C_Medjed_Text.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 12.0f;
    break;
  case EF_ATQUE_POENITENTIA:
    particle->SetSprite("이팩트\\Atque_Poenitentia\\Atque_Poenitentia.spr");
    particle->SetAction("이팩트\\Atque_Poenitentia\\Atque_Poenitentia.act");
    particle->loopMode = 0;
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->flags = 0xD;
    break;
  case EF_PERM_FROST_OBLIVION:
    particle->SetSprite("이팩트\\Perm_Frost_Oblivion\\Perm_Frost_Oblivion.spr");
    particle->SetAction("이팩트\\Perm_Frost_Oblivion\\Perm_Frost_Oblivion.act");
    particle->depthOffset = 0.001f;
    particle->frameDelay = 4.0f;
    particle->loopMode = 0;
    particle->flags = 1;
    break;
  case EF_S_BEELZEBUB_WING:
    particle->SetSprite("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.spr");
    particle->SetAction("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.act");
    particle->flags = 1;
    particle->depthOffset = 0.0015f;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  case EF_UNK_2443: {
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
void CSpriteEffect::Render() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    if (particle->elapsedFrames > 0) {
      particle->Draw(1);
    }
  }
}

// INLINED
static __forceinline bool IsParticleActive(const CSpriteParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// OFFSET: 0x00B79520
// FUNCTION: fn_19199
bool CSpriteEffect::IsAlive() {
  switch (this->id) {
  case EF_GROUND_EXPLOSION:
    return !this->IsFinished();
  case EF_RANDOM_OPT_YELLOW: {
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
  case EF_BAKURETSU_HADOU:
  case EF_IMMUNE_PROPERTY_NOTHING:
  case EF_IMMUNE_PROPERTY_WATER:
  case EF_IMMUNE_PROPERTY_GROUND:
  case EF_IMMUNE_PROPERTY_FIRE:
  case EF_IMMUNE_PROPERTY_WIND:
  case EF_IMMUNE_PROPERTY_POISON:
  case EF_IMMUNE_PROPERTY_SAINT:
  case EF_IMMUNE_PROPERTY_DARKNESS:
  case EF_IMMUNE_PROPERTY_TELEKINESIS:
  case EF_IMMUNE_PROPERTY_UNDEAD:
  case EF_UNK_1211:
  case EF_SUBJECT_AURA_1212:
  case EF_SUBJECT_AURA_1213:
  case EF_VALKYRIE_WING:
  case EF_C_AMDARAIS_EFFECT:
  case EF_BLACKBUBBLE_SMALL:
  case EF_BLACKBUBBLE_MIDIUM:
  case EF_BLACKBUBBLE_LARGE:
  case EF_SERPENT_SHADOW:
  case EF_C_AURA_OF_GHOST_S:
  case EF_ATQUE_POENITENTIA:
  case EF_PERM_FROST_OBLIVION:
  case EF_C_GUIDE_OF_DEAD_TEXT:
  case EF_C_MEDJED_TEXT:
  case EF_UNK_2443:
  case EF_S_BEELZEBUB_WING:
    return this->ActorExists();
  }

  for (CSpriteParticle* particle : this->particles) {
    if (IsParticleActive(particle)) {
      return true;
    }
  }

  return false;
}

// INLINED
static void SetOffsetPosition(vector3d& position, const float* origin, const vector3d& offset) {
  const float x = origin[0] + offset.x;
  const float y = origin[1] + offset.y;
  const float z = origin[2] + offset.z;
  position.x = x;
  position.y = y;
  position.z = z;
}

// OFFSET: 0x00B79780
// FUNCTION: fn_19200
void CSpriteEffect::OnProcess() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    ++particle->elapsedFrames;

    switch (this->id) {
    case EF_BAKURETSU_HADOU: {
      int height = 8;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = static_cast<int>(actor->GetNameOffset() + 8.0f);
      }

      const vector3d offset = {0.0f, -static_cast<float>(height), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_POPCORN:
    case EF_RANDOM_OPT_YELLOW: {
      const vector3d offset = {0.0f, -10.0f, 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_IMMUNE_PROPERTY_NOTHING:
    case EF_IMMUNE_PROPERTY_WATER:
    case EF_IMMUNE_PROPERTY_GROUND:
    case EF_IMMUNE_PROPERTY_FIRE:
    case EF_IMMUNE_PROPERTY_WIND:
    case EF_IMMUNE_PROPERTY_POISON:
    case EF_IMMUNE_PROPERTY_SAINT:
    case EF_IMMUNE_PROPERTY_DARKNESS:
    case EF_IMMUNE_PROPERTY_TELEKINESIS:
    case EF_IMMUNE_PROPERTY_UNDEAD: {
      int height = 25;
      if (CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor))) {
        height = static_cast<int>(actor->GetNameOffset() + 25.0f);
      }

      const vector3d offset = {0.0f, -static_cast<float>(height), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_UNK_1211:
    case EF_SUBJECT_AURA_1212:
    case EF_SUBJECT_AURA_1213: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset() + 8.0f);
      const vector3d offset = {0.0f, static_cast<float>(-height), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_VALKYRIE_WING: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset() + 3.0f);
      const vector3d offset = {0.0f, static_cast<float>(-height), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_SERPENT_SHADOW:
    case EF_C_AURA_OF_GHOST_S: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const int height = static_cast<int>(actor->GetNameOffset());
      const vector3d offset = {0.0f, static_cast<float>(-height), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_ATQUE_POENITENTIA: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const float height = actor->GetNameOffset();
      const float lift = g_session.IsDoramJob(actor->GetEffectiveJob(-1)) ? height + 13.7f : height + 17.0f;
      const vector3d offset = {0.0f, -lift, 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_PERM_FROST_OBLIVION: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const float lift = g_session.IsDoramJob(actor->GetEffectiveJob(-1)) ? 2.2f : 0.4f;
      const vector3d offset = {0.0f, -lift, 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_C_GUIDE_OF_DEAD_TEXT:
    case EF_C_MEDJED_TEXT: {
      CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
      if (!actor) {
        break;
      }

      const vector3d offset = {0.0f, -actor->GetNameOffset(), 0.0f};
      SetOffsetPosition(particle->position, this->position, offset);
      break;
    }
    case EF_UNK_2443:
      particle->applyActorExtent = 1;
      particle->applyActorLift = 1;
      particle->screenOffsetY = 0.0f;
      particle->screenOffsetX = 0.0f;
      particle->position.x = this->position[0];
      particle->position.y = this->position[1];
      particle->position.z = this->position[2];
      break;
    case EF_S_BEELZEBUB_WING:
      particle->applyActorExtent = 1;
      particle->applyActorLift = 1;
      particle->screenOffsetY = -105.0f;
      particle->screenOffsetX = 0.0f;
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
