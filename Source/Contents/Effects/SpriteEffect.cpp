#include "Contents/Effects/SpriteEffect.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/SpriteParticle.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstdint>

namespace {

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  explicit EncryptedName(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
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

}

// OFFSET: 0x00B78840
// FUNCTION: fn_19194
CSpriteEffect::CSpriteEffect() : CEffect(-1) {
}

// OFFSET: 0x00B78B00
// FUNCTION: fn_19196
void CSpriteEffect::Close() {
  for (CSpriteParticle* particle : this->particles) {
    FreeParticle(particle);
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
  case 0x98B: {
    const std::uint8_t spriteSource[] = {0x99, 0x4B, 0xC5, 0xAC, 0x93, 0x65, 0x7B, 0x98, 0x06, 0xB5, 0x33, 0x4F, 0x60,
                                         0x99, 0x48, 0xA8, 0x1F, 0xE2, 0x70, 0x09, 0x34, 0xE5, 0x54, 0xAB, 0x2B, 0x87};
    thread_local EncryptedName<sizeof(spriteSource), 0xDB27CB557D038759ULL> spriteName(spriteSource);
    particle->SetSprite(spriteName.Text());

    const std::uint8_t actionSource[] = {0x4D, 0x71, 0x7D, 0xB2, 0x71, 0x97, 0xE7, 0x12, 0xD2, 0x8F, 0x8B, 0x51, 0x82,
                                         0x6B, 0xD4, 0x22, 0xCB, 0xD8, 0xC8, 0x17, 0xD6, 0x17, 0xDA, 0x32, 0xF9, 0xBD};
    thread_local EncryptedName<sizeof(actionSource), 0x51BB39B763BBBD8DULL> actionName(actionSource);
    particle->SetAction(actionName.Text());
    particle->flags = 1;
    particle->depthOffset = 0.0015f;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  }
  case 0x99A:
    particle->SetSprite("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.spr");
    particle->SetAction("이팩트\\S_Beelzebub_Wing\\S_Beelzebub_Wing.act");
    particle->flags = 1;
    particle->depthOffset = 0.0015f;
    particle->loopMode = 0;
    particle->frameDelay = 8.0f;
    break;
  }
}

// OFFSET: 0x00B79C40
// FUNCTION: fn_19201
void CSpriteEffect::Slot04() {
  for (auto it = this->particles.begin(); it != this->particles.end(); ++it) {
    CSpriteParticle* particle = *it;
    if (particle->unknown_00E8 > 0) {
      particle->sub_B40F30(1);
    }
  }
}
