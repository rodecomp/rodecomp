#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/RagEffectInitialization.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Resource/EZeffectResource.h"
#include "Resource/ResourceMgr.h"

#include <bit>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// INLINED
static void SelectRagEffectResource(RagEffectState& effect, ServiceOptions& options, char* name) {
  name[0] = '\0';

  switch (effect.type) {
  case EF_COIN:
    std::sprintf(name, options.Get(229) != 0 ? "memor_min.str" : "Maemor.str");
    break;
  case EF_GLASSWALL:
  case EF_GLASSWALL2:
    std::sprintf(name, "SafetyWall.str");
    break;
  case EF_STONECURSE:
  case EF_PETRIFYATTACK:
    std::sprintf(name, "StoneCurse.str");
    break;
  case EF_FIREWALL:
    std::sprintf(name, "Firewall%d.str", std::rand() % 2 + 1);
    break;
  case EF_LIGHTBOLT:
    std::sprintf(name, "Lightning.str");
    break;
  case EF_THUNDERSTORM:
    std::sprintf(name, "ThunderStorm.str");
    break;
  case EF_SIGNUM:
    std::sprintf(name, "Cross.str");
    break;
  case EF_ANGELUS:
  case EF_WEWISH:
    std::sprintf(name, options.Get(229) != 0 ? "jong_mini.str" : "Angelus.str");
    break;
  case EF_FIREHIT:
  case EF_FIREHIT2:
    std::sprintf(name, "FireHit%d.str", std::rand() % 3 + 1);
    break;
  case EF_WINDHIT:
    std::sprintf(name, "WindHit%d.str", std::rand() % 3 + 1);
    break;
  case EF_ARROWSHOT:
    std::sprintf(name, "ArrowShot.str");
    break;
  case EF_INVENOM:
    std::sprintf(name, "Invenom.str");
    break;
  case EF_CURE:
    std::sprintf(name, options.Get(229) != 0 ? "cure_min.str" : "Cure.str");
    break;
  case EF_PROVOKE:
    std::sprintf(name, "Provoke.str");
    break;
  case EF_MVP:
    std::sprintf(name, "Mvp.str");
    break;
  case EF_SKIDTRAP:
    std::sprintf(name, "SkidTrap.str");
    break;
  case EF_BRANDISHSPEAR:
    std::sprintf(name, "Brandish.str");
    break;
  case EF_GLORIA:
    std::sprintf(name, options.Get(229) != 0 ? "gloria_min.str" : "Gloria.str");
    break;
  case EF_MAGNIFICAT:
    std::sprintf(name, options.Get(229) != 0 ? "magnificat_min.str" : "Magnificat.str");
    break;
  case EF_RESURRECTION:
    std::sprintf(name, options.Get(229) != 0 ? "resurrection_min.str" : "Resurrection.str");
    break;
  case EF_RECOVERY:
    std::sprintf(name, "Recovery.str");
    break;
  case EF_SANCTUARY:
    std::sprintf(name, "Sanctuary.str");
    break;
  case EF_IMPOSITIO:
    std::sprintf(name, "Impositio.str");
    break;
  case EF_LEXAETERNA:
    std::sprintf(name, options.Get(229) != 0 ? "lexaeterna_min.str" : "LexAeterna.str");
    break;
  case EF_ASPERSIO:
    std::sprintf(name, "Aspersio.str");
    break;
  case EF_LEXDIVINA:
    std::sprintf(name, "LexDivina.str");
    break;
  case EF_SUFFRAGIUM:
    std::sprintf(name, options.Get(229) != 0 ? "suffragium_min.str" : "Suffragium.str");
    break;
  case EF_STORMGUST:
    std::sprintf(name, "StormGust.str");
    break;
  case EF_LORD:
    std::sprintf(name, "Lord.str");
    break;
  case EF_BENEDICTIO:
    std::sprintf(name, "Benedictio.str");
    break;
  case EF_METEORSTORM:
    std::sprintf(name, "Meteor%d.str", std::rand() % 3 + 1);
    break;
  case EF_QUAGMIRE:
    std::sprintf(name, "Quagmire.str");
    break;
  case EF_FIREPILLAR:
    std::sprintf(name, "FirePillar.str");
    break;
  case EF_FIREPILLARBOMB:
    std::sprintf(name, "FirePillarBomb.str");
    break;
  case EF_REPAIRWEAPON:
    std::sprintf(name, "RepairWeapon.str");
    break;
  case EF_CRASHEARTH:
    std::sprintf(name, "CrashEarth.str");
    break;
  case EF_PERFECTION:
    std::sprintf(name, options.Get(229) != 0 ? "WeaponPerfection_min.str" : "WeaponPerfection.str");
    break;
  case EF_MAXPOWER:
    std::sprintf(name, options.Get(229) != 0 ? "maximize_min.str" : "MaximizePower.str");
    break;
  case EF_BLASTMINEBOMB:
    std::sprintf(name, "BlastMine.str");
    break;
  case EF_CLAYMORE:
    std::sprintf(name, "Claymore.str");
    break;
  case EF_FREEZING:
    std::sprintf(name, "Freezing.str");
    break;
  case EF_BUBBLE:
    std::sprintf(name, "Bubble%d.str", std::rand() % 4 + 1);
    break;
  case EF_GASPUSH:
    std::sprintf(name, "GasPush.str");
    break;
  case EF_SPRINGTRAP:
    std::sprintf(name, "Spring.str");
    break;
  case EF_KYRIE:
    std::sprintf(name, options.Get(229) != 0 ? "kyrie_min.str" : "Kyrie.str");
    break;
  case EF_MAGNUS:
    std::sprintf(name, "Magnus.str");
    break;
  case EF_VENOMDUST:
    std::sprintf(name, "VenomDust.str");
    break;
  case EF_POISONREACT:
    std::sprintf(name, "PoisonReact_1st.str");
    break;
  case EF_POISONREACT2:
    std::sprintf(name, "PoisonReact.str");
    break;
  case EF_SPLASHER:
    std::sprintf(name, "VenomSplasher.str");
    break;
  case EF_TWOHANDQUICKEN:
  case EF_SPEARQUICKEN:
  case EF_LKCONCENTRATION:
    std::sprintf(name, "TwoHand.str");
    break;
  case EF_AUTOCOUNTER:
    std::sprintf(name, "AutoCounter.str");
    break;
  case EF_FREEZE:
    std::sprintf(name, "Freeze.str");
    break;
  case EF_FREEZED:
    std::sprintf(name, "Freezed.str");
    break;
  case EF_ICECRASH:
    std::sprintf(name, "IceCrash.str");
    break;
  case EF_SLOWPOISON:
    std::sprintf(name, "slowp.str");
    break;
  case EF_SANDMAN:
    std::sprintf(name, "SandMan.str");
    break;
  case EF_PNEUMA:
    std::sprintf(name, "Pneuma%d.str", std::rand() % 3 + 1);
    break;
  case EF_SONICBLOW2:
    std::sprintf(name, "SonicBlow.str");
    break;
  case EF_BRANDISH2:
    std::sprintf(name, "Brandish2.str");
    break;
  case EF_SHOCKWAVE:
    std::sprintf(name, "ShockWave.str");
    break;
  case EF_SHOCKWAVEHIT:
    std::sprintf(name, "ShockWaveHit.str");
    break;
  case EF_EARTHHIT:
    std::sprintf(name, "EarthHit.str");
    break;
  case EF_PIERCESELF:
    std::sprintf(name, "Pierce.str");
    break;
  case EF_BOWLINGSELF:
    std::sprintf(name, "Bowling.str");
    break;
  case EF_SPEARSTABSELF:
    std::sprintf(name, "SpearStab.str");
    break;
  case EF_SPEARBMRSELF:
    std::sprintf(name, "SpearBoomerang.str");
    break;
  case EF_HOLYHIT:
    std::sprintf(name, "HolyHit.str");
    break;
  case EF_CONCENTRATION:
  case EF_CONCENTRATION2:
    std::sprintf(name, "Concentration.str");
    break;
  case EF_REFINEOK:
    std::sprintf(name, "bs_RefineSuccess.str");
    break;
  case EF_REFINEFAIL:
    std::sprintf(name, "bs_RefineFailed.str");
    break;
  case EF_JOBCHANGE:
    std::sprintf(name, "JobChange.str");
    break;
  case EF_LVUP:
    std::sprintf(name, "LevelUP.str");
    break;
  case EF_JOBLVUP:
  case EF_JOBLVUP50:
    std::sprintf(name, "JobLvUP.str");
    break;
  case EF_TAMINGSUCCESS:
    std::sprintf(name, "TamingSuccess.str");
    break;
  case EF_TAMINGFAILED:
    std::sprintf(name, "TamingFailed.str");
    break;
  case EF_ENERGYCOAT:
    std::sprintf(name, "EnergyCoat.str");
    break;
  case EF_CARTREVOLUTION:
    std::sprintf(name, "CartRevolution.str");
    break;
  case EF_MENTALBREAK:
    std::sprintf(name, "MentalBreak.str");
    break;
  case EF_MAGICALATTHIT:
    std::sprintf(name, "magical.str");
    break;
  case EF_SUI_EXPLOSION:
    std::sprintf(name, "sui_explosion.str");
    break;
  case EF_SUICIDE:
    std::sprintf(name, "suicide.str");
    break;
  case EF_COMBOATTACK1:
    std::sprintf(name, "yunta_1.str");
    break;
  case EF_COMBOATTACK2:
    std::sprintf(name, "yunta_2.str");
    break;
  case EF_COMBOATTACK3:
    std::sprintf(name, "yunta_3.str");
    break;
  case EF_COMBOATTACK4:
    std::sprintf(name, "yunta_4.str");
    break;
  case EF_COMBOATTACK5:
    std::sprintf(name, "yunta_5.str");
    break;
  case EF_GUIDEDATTACK:
    std::sprintf(name, "homing.str");
    break;
  case EF_POISONATTACK:
    std::sprintf(name, "poison.str");
    break;
  case EF_SILENCEATTACK:
    std::sprintf(name, "silence.str");
    break;
  case EF_STUNATTACK:
    std::sprintf(name, "stun.str");
    break;
  case EF_SLEEPATTACK:
    std::sprintf(name, "sleep.str");
    break;
  case EF_PONG:
    std::sprintf(name, "Pong%d.str", std::rand() % 3 + 1);
    break;
  case EF_POTION1:
    std::sprintf(name, "빨간포션.str");
    break;
  case EF_POTION2:
    std::sprintf(name, "주홍포션.str");
    break;
  case EF_POTION3:
    std::sprintf(name, "노란포션.str");
    break;
  case EF_POTION4:
    std::sprintf(name, "하얀포션.str");
    break;
  case EF_POTION5:
    std::sprintf(name, "파란포션.str");
    break;
  case EF_POTION6:
    std::sprintf(name, "초록포션.str");
    break;
  case EF_POTION7:
    std::sprintf(name, "fruit.str");
    break;
  case EF_POTION8:
    std::sprintf(name, "fruit_.str");
    break;
  case EF_DEFFENDER:
    std::sprintf(name, "Deffender.str");
    break;
  case EF_KEEPING:
    std::sprintf(name, "Keeping.str");
    break;
  case EF_POTION_CON:
    std::sprintf(name, "집중.str");
    break;
  case EF_POTION_:
    std::sprintf(name, "각성.str");
    break;
  case EF_POTION_BERSERK:
  case EF_POTION_BERSERK2:
    std::sprintf(name, "버서크.str");
    break;
  case EF_SPELLBREAKER:
    std::sprintf(name, "spell.str");
    break;
  case EF_DISPELL:
    std::sprintf(name, "디스펠.str");
    break;
  case EF_MAGICROD:
    std::sprintf(name, "매직로드.str");
    break;
  case EF_HOLYCROSS:
    std::sprintf(name, "holy_cross.str");
    break;
  case EF_SHIELDCHARGE:
    std::sprintf(name, "shield_charge.str");
    break;
  case EF_PROVIDENCE:
    std::sprintf(name, "providence.str");
    break;
  case EF_DEVOTION:
    std::sprintf(name, "devotion.str");
    break;
  case EF_FLAMELAUNCHER:
    std::sprintf(name, "enc_fire.str");
    break;
  case EF_FROSTWEAPON:
    std::sprintf(name, "enc_ice.str");
    break;
  case EF_LIGHTNINGLOADER:
    std::sprintf(name, "enc_wind.str");
    break;
  case EF_SEISMICWEAPON:
    std::sprintf(name, "enc_earth.str");
    break;
  case EF_STEALCOIN:
    std::sprintf(name, "steal_coin.str");
    break;
  case EF_STRIPWEAPON:
    std::sprintf(name, "strip_weapon.str");
    break;
  case EF_STRIPSHIELD:
    std::sprintf(name, "strip_shield.str");
    break;
  case EF_STRIPARMOR:
    std::sprintf(name, "strip_armor.str");
    break;
  case EF_STRIPHELM:
    std::sprintf(name, "strip_helm.str");
    break;
  case EF_CHAINCOMBO:
    std::sprintf(name, "연환.str");
    break;
  case EF_PHARMACY_OK:
    std::sprintf(name, "p_success.str");
    break;
  case EF_PHARMACY_FAIL:
    std::sprintf(name, "p_failed.str");
    break;
  case EF_LOUD:
    std::sprintf(name, "loud.str");
    break;
  case EF_ANGEL:
    std::sprintf(name, "angel.str");
    break;
  case EF_DEVIL:
  case EF_DEATHSUMMON:
    std::sprintf(name, "devil.str");
    break;
  case EF_MELTDOWN:
    std::sprintf(name, "melt.str");
    break;
  case EF_CARTBOOST:
    std::sprintf(name, "cart.str");
    break;
  case EF_REJECTSWORD:
    std::sprintf(name, "sword.str");
    break;
  case EF_SOULBURN:
    std::sprintf(name, "소울번.str");
    break;
  case EF_SOULCHANGE:
    std::sprintf(name, "사람효과.str");
    break;
  case EF_ASSUMPTIO2:
    std::sprintf(name, "asum.str");
    break;
  case EF_MOCHI:
    std::sprintf(name, "찹쌀떡.str");
    break;
  case EF_LAMADAN:
    std::sprintf(name, "ramadan.str");
    break;
  case EF_MAPAE:
    std::sprintf(name, "mapae.str");
    break;
  case EF_ITEMPOKJUK:
  case EF_HAPGYEOK:
    std::sprintf(name, "itempokjuk.str");
    break;
  case EF_HFLIMOON1:
    std::sprintf(name, "moonlight_1.str");
    break;
  case EF_HFLIMOON2:
    std::sprintf(name, "moonlight_2.str");
    break;
  case EF_HFLIMOON3:
    std::sprintf(name, "moonlight_3.str");
    break;
  case EF_HO_UP:
    std::sprintf(name, "h_levelup.str");
    break;
  case EF_HAMIDEFENCE:
    std::sprintf(name, "defense.str");
    break;
  case EF_FOOD01:
    std::sprintf(name, "food_str.str");
    break;
  case EF_FOOD02:
    std::sprintf(name, "food_int.str");
    break;
  case EF_FOOD03:
    std::sprintf(name, "food_vit.str");
    break;
  case EF_FOOD04:
    std::sprintf(name, "food_agi.str");
    break;
  case EF_FOOD05:
    std::sprintf(name, "food_dex.str");
    break;
  case EF_FOOD06:
    std::sprintf(name, "food_luk.str");
    break;
  case EF_COOKING_OK:
    std::sprintf(name, "cook_suc.str");
    break;
  case EF_COOKING_FAIL:
    std::sprintf(name, "cook_fail.str");
    break;
  case EF_BAKU:
    std::sprintf(name, "fire dragon.str");
    break;
  case EF_HYOUSYOURAKU:
    std::sprintf(name, "icy.str");
    break;
  case EF_TRACKCASTING:
    std::sprintf(name, "트랙킹.str");
    break;
  case EF_BULLSEYE:
    std::sprintf(name, "불스아이.str");
    break;
  case EF_DRAGONFEAR:
  case EF_WIDECONFUSE:
    std::sprintf(name, "dfear.str");
    break;
  case EF_BLEEDING:
    std::sprintf(name, "wideb.str");
    break;
  case EF_CRITICALWOUND:
    std::sprintf(name, "cwound.str");
    break;
  case EF_FLOWERLEAF:
    std::sprintf(name, "flower_leaf.str");
    break;
  case EF_ITEM315:
    std::sprintf(name, "mobile_ef02.str");
    break;
  case EF_ITEM316:
    std::sprintf(name, "mobile_ef01.str");
    break;
  case EF_ITEM317:
    std::sprintf(name, "mobile_ef03.str");
    break;
  case EF_STORM_MIN:
    std::sprintf(name, "storm_min.str");
    break;
  case EF_POK_JAP:
    std::sprintf(name, "pokjuk_jap.str");
    break;
  case EF_ADO_STR:
    std::sprintf(name, "ado.str");
    break;
  case EF_IGN_STR:
    std::sprintf(name, "이그니션브레이크.str");
    break;
  case EF_CRIMSON_STR:
    std::sprintf(name, "crimson_r.str");
    break;
  case EF_HELL_STR:
    std::sprintf(name, "hell_in.str");
    break;
  case EF_DHOWL_STR:
    std::sprintf(name, "dragon_h.str");
    break;
  case EF_CHAINL_STR:
    std::sprintf(name, "chainlight.str");
    break;
  case EF_AIMED_STR:
    std::sprintf(name, "aimed.str");
    break;
  case EF_ARROWSTORM_STR:
    std::sprintf(name, "arrowstorm.str");
    break;
  case EF_LAULAMUS_STR:
    std::sprintf(name, "laulamus.str");
    break;
  case EF_LAUAGNUS_STR:
    std::sprintf(name, "lauagnus.str");
    break;
  case EF_MILSHIELD_STR:
    std::sprintf(name, "mil_shield.str");
    break;
  case EF_CRASHAXE:
    std::sprintf(name, "powerswing.str");
    break;
  case EF_ENERVATION:
    std::sprintf(name, "enervation.str");
    break;
  case EF_ENERVATION2:
    std::sprintf(name, "groomy.str");
    break;
  case EF_ENERVATION3:
    std::sprintf(name, "ignorance.str");
    break;
  case EF_ENERVATION4:
    std::sprintf(name, "laziness.str");
    break;
  case EF_ENERVATION5:
    std::sprintf(name, "unlucky.str");
    break;
  case EF_ENERVATION6:
    std::sprintf(name, "weakness.str");
    break;
  case EF_FIREWALL2:
    std::sprintf(name, "Firewall_per.str");
    break;
  case EF_SHOCKWAVE2:
    std::sprintf(name, "hunter_shockwave_blue.str");
    break;
  case EF_POISON_MIST:
    std::sprintf(name, "poison_mist.str");
    break;
  case EF_ERASER_CUTTER:
    std::sprintf(name, "eraser_cutter.str");
    break;
  case EF_LAVA_SLIDE:
    std::sprintf(name, "lava_slide.str");
    break;
  case EF_SONIC_CLAW:
    std::sprintf(name, "sonic_claw.str");
    break;
  case EF_TINDER_BREAKER:
    std::sprintf(name, "tinder.str");
    break;
  case EF_MIDNIGHT_FRENZY:
    std::sprintf(name, "mid_frenzy.str");
    break;
  case EF_VOLCANIC_ASH:
    std::sprintf(name, "vash00.str");
    break;
  case EF_2011RWC:
    std::sprintf(name, "rwc2011.str");
    break;
  case EF_2011RWC2:
    std::sprintf(name, "rwc2011_2.str");
    break;
  case EF_NO100_FIRECRACKER:
    std::sprintf(name, "100.str");
    break;
  case EF_RUN_MAKE_OK:
    std::sprintf(name, "rune_success.str");
    break;
  case EF_RUN_MAKE_FAILURE:
    std::sprintf(name, "rune_fail.str");
    break;
  case EF_MIRESULT_MAKE_OK:
    std::sprintf(name, "changematerial_su.str");
    break;
  case EF_MIRESULT_MAKE_FAIL:
    std::sprintf(name, "changematerial_fa.str");
    break;
  case EF_ALL_RAY_OF_PROTECTION:
    std::sprintf(name, "Guardian.str");
    break;
  case EF_VENOMFOG:
    std::sprintf(name, "bubble%d_1.str", std::rand() % 4 + 1);
    break;
  case EF_DUSTSTORM:
    std::sprintf(name, "dust.str");
    effect.loopLimit = 10;
    break;
  case EF_DANCE_BLADE_ATK:
    std::sprintf(name, "dancingblade.str");
    break;
  case EF_INVINCIBLEOFF2:
    std::sprintf(name, "INVINCIBLEOFF2.str");
    break;
  case EF_GC_DARKCROW:
    std::sprintf(name, "gc_darkcrow.str");
    effect.loopLimit = 9999;
    break;
  case EF_ALL_FULL_THROTTLE:
    std::sprintf(name, "all_full_throttle.str");
    break;
  case EF_SR_FLASHCOMBO:
    std::sprintf(name, "sr_flashcombo.str");
    break;
  case EF_RK_LUXANIMA:
    std::sprintf(name, "rk_luxanima.str");
    break;
  case EF_SO_ELEMENTAL_SHIELD:
    std::sprintf(name, "so_elemental_shield.str");
    break;
  case EF_AB_OFFERTORIUM:
    std::sprintf(name, "AB_OFFERTORIUM.str");
    break;
  case EF_WL_TELEKINESIS_INTENSE:
    std::sprintf(name, "WL_TELEKINESIS_INTENSE.str");
    effect.loopLimit = 9999;
    break;
  case EF_GN_ILLUSIONDOPING:
    std::sprintf(name, "GN_ILLUSIONDOPING.str");
    break;
  case EF_NC_MAGMA_ERUPTION:
    std::sprintf(name, "NC_MAGMA_ERUPTION.str");
    effect.loopLimit = 9999;
    break;
  case EF_NPC_CHILL:
    std::sprintf(name, "chill.str");
    effect.loopLimit = 9999;
    break;
  case EF_AB_OFFERTORIUM_RING:
    std::sprintf(name, "AB_OFFERTORIUM_ring.str");
    effect.loopLimit = 9999;
    break;
  default:
    break;
  }
}

// OFFSET: 0x00C1C2D0
// FUNCTION: fn_20074
void InitializeRagEffect(
  RagEffectState& effect, RenderObjectState& render, EffectActor* parent, const RenderObjectState& parentState, int id, float x, float y, float z,
  ServiceOptions& options, ResourceMgr& resources
) {
  effect.type = id;
  effect.parent = parent;
  render.position.x = parentState.position.x;
  render.position.y = parentState.position.y;
  render.position.z = parentState.position.z;
  effect.longitude = parentState.angle;
  effect.duration = EffectLifetime(effect.type);
  effect.deltaPos[0] = x;
  effect.deltaPos[1] = y;
  effect.deltaPos[2] = z;
  char name[64];
  SelectRagEffectResource(effect, options, name);

  if (std::strlen(name) != 0) {
    auto* resource = static_cast<CEZeffectRes*>(resources.Load(name));
    effect.resource = resource;

    if (resource) {
      ++resource->references;

      effect.resourceData = &resource->maximumFrame;
      int count = resource->clipCount;

      if (resource->clips[0].frameCount == 0) {
        --count;
      }

      effect.endLayer = count;
    }
  }
}
