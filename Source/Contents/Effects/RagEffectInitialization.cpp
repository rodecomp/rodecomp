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

  switch (effect.id) {
  case 10:
    std::sprintf(name, options.Get(229) != 0 ? "memor_min.str" : "Maemor.str");
    break;
  case 13:
  case 315:
    std::sprintf(name, "SafetyWall.str");
    break;
  case 23:
  case 195:
    std::sprintf(name, "StoneCurse.str");
    break;
  case 25:
    std::sprintf(name, "Firewall%d.str", std::rand() % 2 + 1);
    break;
  case 29:
    std::sprintf(name, "Lightning.str");
    break;
  case 30:
    std::sprintf(name, "ThunderStorm.str");
    break;
  case 40:
    std::sprintf(name, "Cross.str");
    break;
  case 41:
  case 717:
    std::sprintf(name, options.Get(229) != 0 ? "jong_mini.str" : "Angelus.str");
    break;
  case 49:
  case 603:
    std::sprintf(name, "FireHit%d.str", std::rand() % 3 + 1);
    break;
  case 52:
    std::sprintf(name, "WindHit%d.str", std::rand() % 3 + 1);
    break;
  case 64:
    std::sprintf(name, "ArrowShot.str");
    break;
  case 65:
    std::sprintf(name, "Invenom.str");
    break;
  case 66:
    std::sprintf(name, options.Get(229) != 0 ? "cure_min.str" : "Cure.str");
    break;
  case 67:
    std::sprintf(name, "Provoke.str");
    break;
  case 68:
    std::sprintf(name, "Mvp.str");
    break;
  case 69:
    std::sprintf(name, "SkidTrap.str");
    break;
  case 70:
    std::sprintf(name, "Brandish.str");
    break;
  case 75:
    std::sprintf(name, options.Get(229) != 0 ? "gloria_min.str" : "Gloria.str");
    break;
  case 76:
    std::sprintf(name, options.Get(229) != 0 ? "magnificat_min.str" : "Magnificat.str");
    break;
  case 77:
    std::sprintf(name, options.Get(229) != 0 ? "resurrection_min.str" : "Resurrection.str");
    break;
  case 78:
    std::sprintf(name, "Recovery.str");
    break;
  case 83:
    std::sprintf(name, "Sanctuary.str");
    break;
  case 84:
    std::sprintf(name, "Impositio.str");
    break;
  case 85:
    std::sprintf(name, options.Get(229) != 0 ? "lexaeterna_min.str" : "LexAeterna.str");
    break;
  case 86:
    std::sprintf(name, "Aspersio.str");
    break;
  case 87:
    std::sprintf(name, "LexDivina.str");
    break;
  case 88:
    std::sprintf(name, options.Get(229) != 0 ? "suffragium_min.str" : "Suffragium.str");
    break;
  case 89:
    std::sprintf(name, "StormGust.str");
    break;
  case 90:
    std::sprintf(name, "Lord.str");
    break;
  case 91:
    std::sprintf(name, "Benedictio.str");
    break;
  case 92:
    std::sprintf(name, "Meteor%d.str", std::rand() % 3 + 1);
    break;
  case 95:
    std::sprintf(name, "Quagmire.str");
    break;
  case 96:
    std::sprintf(name, "FirePillar.str");
    break;
  case 97:
    std::sprintf(name, "FirePillarBomb.str");
    break;
  case 101:
    std::sprintf(name, "RepairWeapon.str");
    break;
  case 102:
    std::sprintf(name, "CrashEarth.str");
    break;
  case 103:
    std::sprintf(name, options.Get(229) != 0 ? "WeaponPerfection_min.str" : "WeaponPerfection.str");
    break;
  case 104:
    std::sprintf(name, options.Get(229) != 0 ? "maximize_min.str" : "MaximizePower.str");
    break;
  case 106:
    std::sprintf(name, "BlastMine.str");
    break;
  case 107:
    std::sprintf(name, "Claymore.str");
    break;
  case 108:
    std::sprintf(name, "Freezing.str");
    break;
  case 109:
    std::sprintf(name, "Bubble%d.str", std::rand() % 4 + 1);
    break;
  case 110:
    std::sprintf(name, "GasPush.str");
    break;
  case 111:
    std::sprintf(name, "Spring.str");
    break;
  case 112:
    std::sprintf(name, options.Get(229) != 0 ? "kyrie_min.str" : "Kyrie.str");
    break;
  case 113:
    std::sprintf(name, "Magnus.str");
    break;
  case 124:
    std::sprintf(name, "VenomDust.str");
    break;
  case 126:
    std::sprintf(name, "PoisonReact_1st.str");
    break;
  case 127:
    std::sprintf(name, "PoisonReact.str");
    break;
  case 129:
    std::sprintf(name, "VenomSplasher.str");
    break;
  case 130:
  case 250:
  case 369:
    std::sprintf(name, "TwoHand.str");
    break;
  case 131:
    std::sprintf(name, "AutoCounter.str");
    break;
  case 133:
    std::sprintf(name, "Freeze.str");
    break;
  case 134:
    std::sprintf(name, "Freezed.str");
    break;
  case 135:
    std::sprintf(name, "IceCrash.str");
    break;
  case 136:
    std::sprintf(name, "slowp.str");
    break;
  case 139:
    std::sprintf(name, "SandMan.str");
    break;
  case 141:
    std::sprintf(name, "Pneuma%d.str", std::rand() % 3 + 1);
    break;
  case 143:
    std::sprintf(name, "SonicBlow.str");
    break;
  case 144:
    std::sprintf(name, "Brandish2.str");
    break;
  case 145:
    std::sprintf(name, "ShockWave.str");
    break;
  case 146:
    std::sprintf(name, "ShockWaveHit.str");
    break;
  case 147:
    std::sprintf(name, "EarthHit.str");
    break;
  case 148:
    std::sprintf(name, "Pierce.str");
    break;
  case 149:
    std::sprintf(name, "Bowling.str");
    break;
  case 150:
    std::sprintf(name, "SpearStab.str");
    break;
  case 151:
    std::sprintf(name, "SpearBoomerang.str");
    break;
  case 152:
    std::sprintf(name, "HolyHit.str");
    break;
  case 153:
  case 750:
    std::sprintf(name, "Concentration.str");
    break;
  case 154:
    std::sprintf(name, "bs_RefineSuccess.str");
    break;
  case 155:
    std::sprintf(name, "bs_RefineFailed.str");
    break;
  case 156:
    std::sprintf(name, "JobChange.str");
    break;
  case 157:
    std::sprintf(name, "LevelUP.str");
    break;
  case 158:
  case 337:
    std::sprintf(name, "JobLvUP.str");
    break;
  case 167:
    std::sprintf(name, "TamingSuccess.str");
    break;
  case 168:
    std::sprintf(name, "TamingFailed.str");
    break;
  case 169:
    std::sprintf(name, "EnergyCoat.str");
    break;
  case 170:
    std::sprintf(name, "CartRevolution.str");
    break;
  case 181:
    std::sprintf(name, "MentalBreak.str");
    break;
  case 182:
    std::sprintf(name, "magical.str");
    break;
  case 183:
    std::sprintf(name, "sui_explosion.str");
    break;
  case 185:
    std::sprintf(name, "suicide.str");
    break;
  case 186:
    std::sprintf(name, "yunta_1.str");
    break;
  case 187:
    std::sprintf(name, "yunta_2.str");
    break;
  case 188:
    std::sprintf(name, "yunta_3.str");
    break;
  case 189:
    std::sprintf(name, "yunta_4.str");
    break;
  case 190:
    std::sprintf(name, "yunta_5.str");
    break;
  case 191:
    std::sprintf(name, "homing.str");
    break;
  case 192:
    std::sprintf(name, "poison.str");
    break;
  case 193:
    std::sprintf(name, "silence.str");
    break;
  case 194:
    std::sprintf(name, "stun.str");
    break;
  case 197:
    std::sprintf(name, "sleep.str");
    break;
  case 199:
    std::sprintf(name, "Pong%d.str", std::rand() % 3 + 1);
    break;
  case 204:
    std::sprintf(name, "빨간포션.str");
    break;
  case 205:
    std::sprintf(name, "주홍포션.str");
    break;
  case 206:
    std::sprintf(name, "노란포션.str");
    break;
  case 207:
    std::sprintf(name, "하얀포션.str");
    break;
  case 208:
    std::sprintf(name, "파란포션.str");
    break;
  case 209:
    std::sprintf(name, "초록포션.str");
    break;
  case 210:
    std::sprintf(name, "fruit.str");
    break;
  case 211:
    std::sprintf(name, "fruit_.str");
    break;
  case 213:
    std::sprintf(name, "Deffender.str");
    break;
  case 214:
    std::sprintf(name, "Keeping.str");
    break;
  case 218:
    std::sprintf(name, "집중.str");
    break;
  case 219:
    std::sprintf(name, "각성.str");
    break;
  case 220:
  case 756:
    std::sprintf(name, "버서크.str");
    break;
  case 234:
    std::sprintf(name, "spell.str");
    break;
  case 235:
    std::sprintf(name, "디스펠.str");
    break;
  case 244:
    std::sprintf(name, "매직로드.str");
    break;
  case 245:
    std::sprintf(name, "holy_cross.str");
    break;
  case 246:
    std::sprintf(name, "shield_charge.str");
    break;
  case 248:
    std::sprintf(name, "providence.str");
    break;
  case 251:
    std::sprintf(name, "devotion.str");
    break;
  case 255:
    std::sprintf(name, "enc_fire.str");
    break;
  case 256:
    std::sprintf(name, "enc_ice.str");
    break;
  case 257:
    std::sprintf(name, "enc_wind.str");
    break;
  case 258:
    std::sprintf(name, "enc_earth.str");
    break;
  case 268:
    std::sprintf(name, "steal_coin.str");
    break;
  case 269:
    std::sprintf(name, "strip_weapon.str");
    break;
  case 270:
    std::sprintf(name, "strip_shield.str");
    break;
  case 271:
    std::sprintf(name, "strip_armor.str");
    break;
  case 272:
    std::sprintf(name, "strip_helm.str");
    break;
  case 273:
    std::sprintf(name, "연환.str");
    break;
  case 305:
    std::sprintf(name, "p_success.str");
    break;
  case 306:
    std::sprintf(name, "p_failed.str");
    break;
  case 311:
    std::sprintf(name, "loud.str");
    break;
  case 371:
    std::sprintf(name, "angel.str");
    break;
  case 372:
  case 1033:
    std::sprintf(name, "devil.str");
    break;
  case 390:
    std::sprintf(name, "melt.str");
    break;
  case 391:
    std::sprintf(name, "cart.str");
    break;
  case 392:
    std::sprintf(name, "sword.str");
    break;
  case 406:
    std::sprintf(name, "소울번.str");
    break;
  case 407:
    std::sprintf(name, "사람효과.str");
    break;
  case 440:
    std::sprintf(name, "asum.str");
    break;
  case 491:
    std::sprintf(name, "찹쌀떡.str");
    break;
  case 492:
    std::sprintf(name, "ramadan.str");
    break;
  case 507:
    std::sprintf(name, "mapae.str");
    break;
  case 508:
  case 612:
    std::sprintf(name, "itempokjuk.str");
    break;
  case 565:
    std::sprintf(name, "moonlight_1.str");
    break;
  case 566:
    std::sprintf(name, "moonlight_2.str");
    break;
  case 567:
    std::sprintf(name, "moonlight_3.str");
    break;
  case 568:
    std::sprintf(name, "h_levelup.str");
    break;
  case 569:
    std::sprintf(name, "defense.str");
    break;
  case 593:
    std::sprintf(name, "food_str.str");
    break;
  case 594:
    std::sprintf(name, "food_int.str");
    break;
  case 595:
    std::sprintf(name, "food_vit.str");
    break;
  case 596:
    std::sprintf(name, "food_agi.str");
    break;
  case 597:
    std::sprintf(name, "food_dex.str");
    break;
  case 598:
    std::sprintf(name, "food_luk.str");
    break;
  case 608:
    std::sprintf(name, "cook_suc.str");
    break;
  case 609:
    std::sprintf(name, "cook_fail.str");
    break;
  case 635:
    std::sprintf(name, "fire dragon.str");
    break;
  case 636:
    std::sprintf(name, "icy.str");
    break;
  case 646:
    std::sprintf(name, "트랙킹.str");
    break;
  case 649:
    std::sprintf(name, "불스아이.str");
    break;
  case 668:
  case 670:
    std::sprintf(name, "dfear.str");
    break;
  case 669:
    std::sprintf(name, "wideb.str");
    break;
  case 677:
    std::sprintf(name, "cwound.str");
    break;
  case 699:
    std::sprintf(name, "flower_leaf.str");
    break;
  case 704:
    std::sprintf(name, "mobile_ef02.str");
    break;
  case 705:
    std::sprintf(name, "mobile_ef01.str");
    break;
  case 706:
    std::sprintf(name, "mobile_ef03.str");
    break;
  case 708:
    std::sprintf(name, "storm_min.str");
    break;
  case 709:
    std::sprintf(name, "pokjuk_jap.str");
    break;
  case 721:
    std::sprintf(name, "ado.str");
    break;
  case 722:
    std::sprintf(name, "이그니션브레이크.str");
    break;
  case 727:
    std::sprintf(name, "crimson_r.str");
    break;
  case 728:
    std::sprintf(name, "hell_in.str");
    break;
  case 731:
    std::sprintf(name, "dragon_h.str");
    break;
  case 734:
    std::sprintf(name, "chainlight.str");
    break;
  case 745:
    std::sprintf(name, "aimed.str");
    break;
  case 746:
    std::sprintf(name, "arrowstorm.str");
    break;
  case 747:
    std::sprintf(name, "laulamus.str");
    break;
  case 748:
    std::sprintf(name, "lauagnus.str");
    break;
  case 749:
    std::sprintf(name, "mil_shield.str");
    break;
  case 795:
    std::sprintf(name, "powerswing.str");
    break;
  case 813:
    std::sprintf(name, "enervation.str");
    break;
  case 814:
    std::sprintf(name, "groomy.str");
    break;
  case 815:
    std::sprintf(name, "ignorance.str");
    break;
  case 816:
    std::sprintf(name, "laziness.str");
    break;
  case 817:
    std::sprintf(name, "unlucky.str");
    break;
  case 818:
    std::sprintf(name, "weakness.str");
    break;
  case 920:
    std::sprintf(name, "Firewall_per.str");
    break;
  case 926:
    std::sprintf(name, "hunter_shockwave_blue.str");
    break;
  case 959:
    std::sprintf(name, "poison_mist.str");
    break;
  case 960:
    std::sprintf(name, "eraser_cutter.str");
    break;
  case 964:
    std::sprintf(name, "lava_slide.str");
    break;
  case 965:
    std::sprintf(name, "sonic_claw.str");
    break;
  case 966:
    std::sprintf(name, "tinder.str");
    break;
  case 967:
    std::sprintf(name, "mid_frenzy.str");
    break;
  case 975:
    std::sprintf(name, "vash00.str");
    break;
  case 987:
    std::sprintf(name, "rwc2011.str");
    break;
  case 988:
    std::sprintf(name, "rwc2011_2.str");
    break;
  case 1006:
    std::sprintf(name, "100.str");
    break;
  case 1015:
    std::sprintf(name, "rune_success.str");
    break;
  case 1016:
    std::sprintf(name, "rune_fail.str");
    break;
  case 1017:
    std::sprintf(name, "changematerial_su.str");
    break;
  case 1018:
    std::sprintf(name, "changematerial_fa.str");
    break;
  case 1019:
    std::sprintf(name, "Guardian.str");
    break;
  case 1020:
    std::sprintf(name, "bubble%d_1.str", std::rand() % 4 + 1);
    break;
  case 1021:
    std::sprintf(name, "dust.str");
    effect.unknown_11C80 = 10;
    break;
  case 1029:
    std::sprintf(name, "dancingblade.str");
    break;
  case 1031:
    std::sprintf(name, "INVINCIBLEOFF2.str");
    break;
  case 1040:
    std::sprintf(name, "gc_darkcrow.str");
    effect.unknown_11C80 = 9999;
    break;
  case 1042:
    std::sprintf(name, "all_full_throttle.str");
    break;
  case 1043:
    std::sprintf(name, "sr_flashcombo.str");
    break;
  case 1044:
    std::sprintf(name, "rk_luxanima.str");
    break;
  case 1046:
    std::sprintf(name, "so_elemental_shield.str");
    break;
  case 1047:
    std::sprintf(name, "AB_OFFERTORIUM.str");
    break;
  case 1048:
    std::sprintf(name, "WL_TELEKINESIS_INTENSE.str");
    effect.unknown_11C80 = 9999;
    break;
  case 1049:
    std::sprintf(name, "GN_ILLUSIONDOPING.str");
    break;
  case 1050:
    std::sprintf(name, "NC_MAGMA_ERUPTION.str");
    effect.unknown_11C80 = 9999;
    break;
  case 1055:
    std::sprintf(name, "chill.str");
    effect.unknown_11C80 = 9999;
    break;
  case 1057:
    std::sprintf(name, "AB_OFFERTORIUM_ring.str");
    effect.unknown_11C80 = 9999;
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
  effect.id = id;
  effect.parent = parent;
  render.position[0] = parentState.position[0];
  render.position[1] = parentState.position[1];
  render.position[2] = parentState.position[2];
  effect.unknown_013C[4] = std::bit_cast<std::uint32_t>(parentState.angle);
  effect.lifetime = EffectLifetime(effect.id);
  effect.offset[0] = x;
  effect.offset[1] = y;
  effect.offset[2] = z;
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

      const auto bits = static_cast<std::uint32_t>(count);
      std::memcpy(effect.unknown_119F8, &bits, sizeof(bits));
    }
  }
}
