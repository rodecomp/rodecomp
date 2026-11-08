#include "Client/Session.h"
#include "Contents/Combat/CombatTypes.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"

#include <string>
#include <utility>

// OFFSET: 0x00E13A20
// FUNCTION: fn_23824
std::string CSession::GetRobeActName(int job, int sex, int robe, bool riding, int unused, int costumeJob) {
  return g_equipmentResourceMgr->GetRobeResourceName(0, job, sex, robe, riding, unused, costumeJob);
}

// OFFSET: 0x00E13A90
// FUNCTION: fn_23825
std::string CSession::GetRobeSprName(int job, int sex, int robe, bool riding, int unused, int costumeJob) {
  return g_equipmentResourceMgr->GetRobeResourceName(1, job, sex, robe, riding, unused, costumeJob);
}

// OFFSET: 0x00E13B20
// FUNCTION: fn_23827
std::string CSession::GetRobeName(unsigned int robe) {
  if (robe >= this->m_robeNames.size()) {
    return this->m_robeNames[0];
  }

  return this->m_robeNames[robe];
}

// OFFSET: 0x00E13BF0
// FUNCTION: fn_23828
int CSession::GetItemDescription40(int id) {
  return FindItemDescription(id).viewId;
}

// OFFSET: 0x00E13C10
// FUNCTION: fn_23829
std::uint32_t CSession::GetUnknown0420(int index) {
  return this->hotkeySkillLevels[index];
}

// OFFSET: 0x00E13C30
// FUNCTION: fn_23830
void CSession::GetSkillActionInfo(int skillId, int& effectId, int& motion, int variant, int level) {
  effectId = 16;
  motion = 8;
  const int beginEffect = this->GetBeginEffectId(skillId);
  switch (skillId) {
  case KO_BAKURETSU:
  case KO_HUUMARANKA:
    motion = 36;
    break;
  case KO_MEIKYOUSISUI:
  case KO_KYOUGAKU:
  case KO_JYUSATSU:
  case KO_KAHU_ENTEN:
  case KO_HYOUHU_HUBUKI:
  case KO_KAZEHU_SEIRAN:
  case KO_DOHU_KOUKAI:
  case KO_GENWAKU:
  case OB_ZANGETSU:
  case OB_OBOROGENSOU:
  case OB_AKAITSUKI:
    motion = 37;
    break;
  }

  if (beginEffect != -1) {
    effectId = beginEffect;
    return;
  }

  switch (skillId) {
  case KN_AUTOCOUNTER:
  case TF_THROWSTONE:
  case AS_VENOMKNIFE:
    effectId = -1;
    motion = 11;
    break;
  case TF_SPRINKLESAND:
    motion = 11;
    break;
  case KN_BRANDISHSPEAR:
    effectId = 144;
    motion = 2;
    break;
  case SM_BASH:
  case SM_MAGNUM:
  case MC_MAMMONITE:
  case AC_DOUBLE:
  case BS_HAMMERFALL:
  case HT_POWER:
  case HT_PHANTASMIC:
  case MS_MAGNUM:
    motion = 2;
    break;
  case KN_SPEARBOOMERANG:
    effectId = -1;
    motion = 12;
    break;
  case AC_CONCENTRATION:
    effectId = 43;
    return;
  case AC_SHOWER:
    effectId = -1;
    motion = 18;
    break;
  case TF_PICKSTONE:
    effectId = -1;
    motion = 5;
    break;
  case MO_EXTREMITYFIST:
    effectId = level < 4016 ? 328 : 510;
    break;
  case AM_CALLHOMUN:
  case AM_REST:
  case AM_RESURRECTHOMUN:
    motion = 0;
    effectId = 342;
    return;
  case DC_UGLYDANCE:
  case DC_HUMMING:
  case DC_DONTFORGETME:
  case DC_FORTUNEKISS:
  case DC_SERVICEFORYOU:
  case WE_MALE:
  case WE_CALLPARTNER:
  case WE_BABY:
  case WE_CALLPARENT:
  case WE_CALLBABY:
  case WE_CALLALLFAMILY:
  case WE_ONEFOREVER:
  case WE_CHEERUP:
    effectId = 342;
    return;
  case ST_CHASEWALK:
    motion = 0;
    effectId = 1195;
    return;
  case WE_FEMALE:
    effectId = 343;
    return;
  case SN_SHARPSHOOTING:
  case CG_ARROWVULCAN:
    motion = 18;
    effectId = 58;
    return;
  case BD_LULLABY:
  case BD_ETERNALCHAOS:
  case BD_ROKISWEIL:
  case HP_ASSUMPTIO:
  case HP_BASILICA:
  case PA_PRESSURE:
  case SN_WINDWALK:
  case PA_SHIELDCHAIN:
  case HW_GANBANTEIN:
  case CG_TAROTCARD:
  case CASH_ASSUMPTIO:
  case RK_REFRESH:
  case AB_RENOVATIO:
  case AB_SECRAMENT:
  case SJ_NOVAEXPLOSING:
  case SJ_GRAVITYCONTROL:
  case SJ_BOOKOFCREATINGSTAR:
  case AB_CONVENIO:
  case EM_ELEMENTAL_VEIL:
    effectId = 58;
    return;
  case LK_SPIRALPIERCE:
    effectId = 502;
    motion = 11;
    break;
  case AL_BLESSING:
  case CASH_BLESSING:
    effectId = 12;
    motion = 0;
    break;
  case AM_ACIDTERROR:
  case AM_POTIONPITCHER:
  case AM_CANNIBALIZE:
  case AM_CP_WEAPON:
  case AM_CP_SHIELD:
  case AM_CP_ARMOR:
  case AM_CP_HELM:
  case MO_STEELBODY:
  case CR_FULLPROTECTION:
    motion = 0;
    effectId = 12;
    goto variant;
  case MG_SRECOVERY:
  case MG_SIGHT:
  case MG_NAPALMBEAT:
  case MG_SAFETYWALL:
  case MG_SOULSTRIKE:
  case MG_COLDBOLT:
  case MG_FROSTDIVER:
  case MG_STONECURSE:
  case MG_FIREBALL:
  case MG_FIREWALL:
  case MG_FIREBOLT:
  case MG_LIGHTNINGBOLT:
  case MG_THUNDERSTORM:
  case AL_DP:
  case AL_DEMONBANE:
  case AL_RUWACH:
  case AL_PNEUMA:
  case AL_TELEPORT:
  case AL_WARP:
  case AL_HEAL:
  case AL_DECAGI:
  case AL_HOLYWATER:
  case AL_CRUCIS:
  case AL_ANGELUS:
  case AL_CURE:
  case ALL_RESURRECTION:
  case PR_IMPOSITIO:
  case PR_SUFFRAGIUM:
  case PR_ASPERSIO:
  case PR_BENEDICTIO:
  case PR_SANCTUARY:
  case PR_STRECOVERY:
  case PR_KYRIE:
  case PR_MAGNIFICAT:
  case PR_GLORIA:
  case PR_LEXDIVINA:
  case PR_TURNUNDEAD:
  case PR_LEXAETERNA:
  case PR_MAGNUS:
  case WZ_FIREPILLAR:
  case WZ_SIGHTRASHER:
  case WZ_FIREIVY:
  case WZ_METEOR:
  case WZ_JUPITEL:
  case WZ_VERMILION:
  case WZ_WATERBALL:
  case WZ_ICEWALL:
  case WZ_FROSTNOVA:
  case WZ_STORMGUST:
  case WZ_EARTHSPIKE:
  case WZ_HEAVENDRIVE:
  case WZ_QUAGMIRE:
  case BS_REPAIRWEAPON:
  case AL_HOLYLIGHT:
  case MG_ENERGYCOAT:
  case CR_GRANDCROSS:
  case CR_DEVOTION:
  case CR_PROVIDENCE:
  case MO_CALLSPIRITS:
  case MO_ABSORBSPIRITS:
  case MO_INVESTIGATE:
  case MO_FINGEROFFENSIVE:
  case SA_SPELLBREAKER:
  case SA_AUTOSPELL:
  case SA_FLAMELAUNCHER:
  case SA_FROSTWEAPON:
  case SA_LIGHTNINGLOADER:
  case SA_SEISMICWEAPON:
  case SA_VOLCANO:
  case SA_DELUGE:
  case SA_VIOLENTGALE:
  case SA_LANDPROTECTOR:
  case SA_DISPELL:
  case SA_ABRACADABRA:
  case BD_RICHMANKIM:
  case BD_DRUMBATTLEFIELD:
  case BD_RINGNIBELUNGEN:
  case BD_INTOABYSS:
  case BD_SIEGFRIED:
  case NPC_GRANDDARKNESS:
  case NPC_DARKSTRIKE:
  case NPC_DARKTHUNDER:
  case NPC_STOP:
  case HW_MAGICCRASHER:
  case PF_HPCONVERSION:
  case PF_SOULCHANGE:
  case PF_SOULBURN:
  case HW_NAPALMVULCAN:
  case CH_SOULCOLLECT:
  case PF_MINDBREAKER:
  case PF_MEMORIZE:
  case PF_SPIDERWEB:
  case PF_DOUBLECASTING:
  case HW_GRAVITATION:
  case NPC_WIDESIGHT:
  case ALL_ANGEL_PROTECT:
  case NPC_MILLENNIUMSHIELD:
  case NPC_STORMGUST2:
  case NPC_CHEAL:
  case NPC_DRAGONBREATH:
  case NPC_VENOMIMPRESS:
  case NPC_IGNITIONBREAK:
  case NPC_PHANTOMTHRUST:
  case NPC_RAINOFMETEOR:
  case NPC_GROUNDDRIVE:
  case WZ_SIGHTBLASTER:
  case MO_KITRANSLATION:
  case RK_ENCHANTBLADE:
  case RK_HUNDREDSPEAR:
  case RK_IGNITIONBREAK:
  case RK_DRAGONBREATH:
  case RK_MILLENNIUMSHIELD:
  case RK_CRUSHSTRIKE:
  case RK_GIANTGROWTH:
  case RK_STONEHARDSKIN:
  case RK_FIGHTINGSPIRIT:
  case RK_ABUNDANCE:
  case RK_PHANTOMTHRUST:
  case GC_VENOMIMPRESS:
  case GC_ROLLINGCUTTER:
  case AB_ADORAMUS:
  case AB_CHEAL:
  case AB_EPICLESIS:
  case AB_ORATIO:
  case AB_HIGHNESSHEAL:
  case AB_EXPIATIO:
  case AB_DUPLELIGHT:
  case AB_SILENTIUM:
  case SC_CHAOSPANIC:
  case ALL_ODINS_RECALL:
  case RETURN_TO_ELDICASTES:
  case ALL_GUARDIAN_RECALL:
  case ALL_ODINS_POWER:
  case SJ_NEWMOONKICK:
  case SJ_FALLINGSTAR:
  case RK_DRAGONBREATH_WATER:
  case SU_CN_METEOR:
  case SU_CN_METEOR2:
  case DK_DRAGONIC_AURA:
  case CD_REPARATIO:
  case MER_MAGNIFICAT:
  case EM_EL_FLAMEROCK:
  case EM_EL_AGE_OF_ICE:
  case EM_EL_STORM_WIND:
  case EM_EL_AVALANCHE:
  case EM_EL_DEADLY_POISON:
    effectId = 12;
    goto variant;
  case CR_SLIMPITCHER:
  case CR_ACIDDEMONSTRATION:
    motion = 28;
    effectId = 12;
    goto variant;
  case ST_PRESERVE:
    motion = 0;
    effectId = 59;
    return;
  case AS_SPLASHER:
  case NPC_ARMORBRAKE:
  case NPC_HELMBRAKE:
  case NPC_SHIELDBRAKE:
  case NPC_AGIUP:
  case ASC_BREAKER:
  case ASC_METEORASSAULT:
  case NPC_PULSESTRIKE:
  case NPC_PULSESTRIKE2:
    effectId = 59;
    return;
  case TK_SEVENWIND:
  case TK_MISSION:
    motion = 0;
    effectId = 441;
    return;
  case BA_DISSONANCE:
  case BA_WHISTLE:
  case BA_ASSASSINCROSS:
  case BA_POEMBRAGI:
  case BA_APPLEIDUN:
  case TK_RUN:
  case SG_FEEL:
  case SG_HATE:
  case SG_FUSION:
  case SL_ALCHEMIST:
  case SL_MONK:
  case SL_STAR:
  case SL_SAGE:
  case SL_CRUSADER:
  case SL_SUPERNOVICE:
  case SL_KNIGHT:
  case SL_WIZARD:
  case SL_PRIEST:
  case SL_BARDDANCER:
  case SL_ROGUE:
  case SL_ASSASIN:
  case SL_BLACKSMITH:
  case SL_HUNTER:
  case SL_SOULLINKER:
  case SL_KAIZEL:
  case SL_KAAHI:
  case SL_KAUPE:
  case SL_KAITE:
  case SL_STIN:
  case SL_STUN:
  case SL_SMA:
  case SL_SWOO:
  case SL_SKE:
  case SL_SKA:
  case SL_HIGH:
  case SL_DEATHKNIGHT:
  case SL_COLLECTOR:
  case SL_NINJA:
  case SL_GUNNER:
  case AB_ANCILLA:
  case SC_MANHOLE:
  case SC_DIMENSIONDOOR:
  case SC_BLOODYLUST:
  case SP_SOULGOLEM:
  case SP_SOULSHADOW:
  case SP_SOULFALCON:
  case SP_SOULFAIRY:
  case SP_SOULUNITY:
  case SP_SOULREAPER:
  case SP_SOULREVOLVE:
    effectId = 441;
    return;
  case AM_BERSERKPITCHER:
    motion = 12;
    effectId = 12;
    goto variant;
  case AM_TWILIGHT1:
    motion = 7;
    effectId = 573;
    return;
  case AM_TWILIGHT2:
    motion = 7;
    effectId = 574;
    return;
  case AM_TWILIGHT3:
    motion = 7;
    effectId = 575;
    return;
  case NJ_HUUMA:
  case NJ_BUNSINJYUTSU:
  case NJ_NEN:
    motion = 37;
    effectId = 441;
    return;
  case NJ_KOUENKA:
  case NJ_KAENSIN:
  case NJ_BAKUENRYU:
    motion = 37;
    effectId = 55;
    return;
  case SA_ELEMENTFIRE:
    effectId = 55;
    return;
  case NJ_HYOUSENSOU:
  case NJ_SUITON:
  case NJ_HYOUSYOURAKU:
    motion = 37;
    effectId = 54;
    return;
  case NJ_HUUJIN:
  case NJ_RAIGEKISAI:
  case NJ_KAMAITACHI:
    motion = 37;
    effectId = 57;
    return;
  case SA_ELEMENTWIND:
    effectId = 57;
    return;
  case GS_BULLSEYE:
  case GS_MADNESSCANCEL:
  case GS_ADJUSTMENT:
  case GS_TRACKING:
  case GS_DISARM:
  case GS_PIERCINGSHOT:
  case GS_RAPIDSHOWER:
  case GS_GATLINGFEVER:
    motion = 41;
    effectId = 441;
    return;
  case GS_DUST:
  case GS_FULLBUSTER:
  case GS_SPREADATTACK:
  case GS_GROUNDDRIFT:
    motion = 42;
    effectId = 441;
    return;
  case KN_CHARGEATK:
  case BA_PANGVOICE:
  case PR_REDEMPTIO:
    motion = 0;
    effectId = 58;
    return;
  case AB_CLEARANCE:
    effectId = 1560;
    return;
  case KN_PIERCE:
  case KN_SPEARSTAB:
  case KN_BOWLINGBASH:
  case AB_DUPLELIGHT_MELEE:
    effectId = -1;
    motion = 2;
    break;
  case RK_SONICWAVE:
    effectId = 12;
    motion = 2;
    break;
  case HT_SKIDTRAP:
  case HT_LANDMINE:
  case HT_ANKLESNARE:
  case HT_SHOCKWAVE:
  case HT_SANDMAN:
  case HT_FLASHER:
  case HT_FREEZINGTRAP:
  case HT_BLASTMINE:
  case HT_CLAYMORETRAP:
  case HT_REMOVETRAP:
  case HT_TALKIEBOX:
  case GM_SANDMAN:
  case RA_CLUSTERBOMB:
  case RA_FIRINGTRAP:
  case RA_ICEBOUNDTRAP:
    motion = 5;
    break;
  case SR_DRAGONCOMBO:
  case SR_SKYNETBLOW:
  case SR_EARTHSHAKER:
  case SR_FALLENEMPIRE:
  case SR_TIGERCANNON:
  case SR_RAMPAGEBLASTER:
  case SR_RAISINGDRAGON:
  case SR_ASSIMILATEPOWER:
  case SR_POWERVELOCITY:
  case SR_GENTLETOUCH_QUIET:
  case SR_GENTLETOUCH_CURE:
  case SR_GENTLETOUCH_ENERGYGAIN:
  case SR_GENTLETOUCH_CHANGE:
  case SR_GENTLETOUCH_REVITALIZE:
  case SR_RIDEINLIGHTNING:
    motion = -1;
    effectId = 831;
    return;
  case MB_M_TELEPORT:
  case SC_INVISIBILITY:
  case SC_FEINTBOMB:
    motion = 0;
    effectId = 501;
    return;
  case SC_ENERVATION:
  case SC_GROOMY:
  case SC_IGNORANCE:
  case SC_LAZINESS:
  case SC_UNLUCKY:
  case SC_WEAKNESS:
  case SC_STRIPACCESSARY:
    motion = 7;
    effectId = 441;
    return;
  case SC_MAELSTROM:
    effectId = 1556;
    return;
  case RL_FALLEN_ANGEL:
    motion = 46;
    effectId = -1;
    return;
  case TF_HIDING:
  case KN_SPEARMASTERY:
  case KN_TWOHANDQUICKEN:
  case KN_RIDING:
  case KN_CAVALIERMASTERY:
  case WZ_ESTIMATION:
  case NPC_PIERCINGATT:
  case NPC_MENTALBREAKER:
  case NPC_RANGEATTACK:
  case NPC_ATTRICHANGE:
  case NPC_CHANGEWATER:
  case NPC_CHANGEGROUND:
  case NPC_CHANGEFIRE:
  case NPC_CHANGEWIND:
  case NPC_CHANGEPOISON:
  case NPC_CHANGEHOLY:
  case NPC_CHANGEDARKNESS:
  case NPC_CHANGETELEKINESIS:
  case NPC_CRITICALSLASH:
  case NPC_COMBOATTACK:
  case NPC_GUIDEDATTACK:
  case NPC_SELFDESTRUCTION:
  case NPC_SPLASHATTACK:
  case NPC_SUICIDE:
  case NPC_POISON:
  case NPC_BLINDATTACK:
  case NPC_SILENCEATTACK:
  case NPC_STUNATTACK:
  case NPC_PETRIFYATTACK:
  case NPC_CURSEATTACK:
  case NPC_SLEEPATTACK:
  case NPC_RANDOMATTACK:
  case NPC_WATERATTACK:
  case NPC_GROUNDATTACK:
  case NPC_FIREATTACK:
  case NPC_WINDATTACK:
  case NPC_POISONATTACK:
  case NPC_HOLYATTACK:
  case NPC_DARKNESSATTACK:
  case NPC_TELEKINESISATTACK:
  case NPC_MAGICALATTACK:
  case CR_SPEARQUICKEN:
  case MO_CHAINCOMBO:
  case MO_COMBOFINISH:
  case NPC_DARKCROSS:
  case NPC_UNDEADATTACK:
  case PA_SACRIFICE:
  case CH_PALMSTRIKE:
  case CH_TIGERFIST:
  case CH_CHAINCRUSH:
  case LK_HEADCRUSH:
  case LK_JOINTBEAT:
  case KN_ONEHAND:
  case NPC_FIREBREATH:
  case NPC_WIDESILENCE:
  case NPC_WIDESTONE:
  case NPC_WIDECONFUSE:
  case NPC_WIDESLEEP:
  case NPC_STONESKIN:
  case NPC_ANTIMAGIC:
  case NPC_WIDESTUN:
  case NPC_WIDEWEB:
  case NPC_FIRESTORM:
  case NPC_WIDESILENCE2:
  case NPC_WIDESTUN2:
  case NPC_WIDESTONE2:
  case NPC_WIDESLEEP2:
  case NPC_WIDECONFUSE2:
  case NPC_CURSE_OF_BLUE_CUBE:
  case NPC_KILLING_AURA:
  case MO_BALKYOUNG:
  case RK_WINDCUTTER:
  case AB_JUDEX:
  case NC_AXEBOOMERANG:
  case AG_DEADLY_PROJECTION:
  case AG_DESTRUCTIVE_HURRICANE:
  case AG_RAIN_OF_CRYSTAL:
  case AG_MYSTERY_ILLUSION:
  case AG_VIOLENT_QUAKE:
  case AG_SOUL_VC_STRIKE:
  case AG_STRANTUM_TREMOR:
  case AG_ALL_BLOOM:
  case AG_CRYSTAL_IMPACT:
  case AG_TORNADO_STORM:
  case AG_FLORAL_FLARE_ROAD:
  case AG_ASTRAL_STRIKE:
  case AG_CLIMAX:
  case AG_ROCK_DOWN:
  case AG_STORM_CANNON:
  case AG_CRIMSON_ARROW:
  case AG_FROZEN_SLASH:
  case IQ_OLEUM_SANCTUM:
  case IQ_EXPOSION_BLASTER:
  case IQ_FIRST_FAITH_POWER:
  case IQ_JUDGE:
  case IQ_THIRD_EXOR_FLAME:
  case CD_MEDIALE_VOTUM:
  case CD_ARGUTUS_VITA:
  case CD_ARGUTUS_TELUM:
  case CD_ARBITRIUM:
  case CD_PRESENS_ACIES:
  case CD_COMPETENTIA:
  case CD_DILECTIO_HEAL:
  case CD_RELIGIO:
  case CD_BENEDICTUM:
  case ABC_STRIP_SHADOW:
  case ABC_UNLUCKY_RUSH:
  case ABC_CHAIN_REACTION_SHOT:
  case ABC_ABYSS_SLAYER:
  case ABC_ABYSS_STRIKE:
  case WH_HAWKBOOMERANG:
  case WH_SOLIDTRAP:
  case BO_THE_WHOLE_PROTECTION:
  case BO_ADVANCE_PROTECTION:
  case BO_ACIDIFIED_ZONE_WATER:
  case BO_ACIDIFIED_ZONE_GROUND:
  case BO_ACIDIFIED_ZONE_WIND:
  case BO_ACIDIFIED_ZONE_FIRE:
  case BO_WOODENWARRIOR:
  case BO_WOODEN_FAIRY:
  case BO_CREEPER:
  case BO_RESEARCHREPORT:
  case BO_HELLTREE:
  case TR_SOUNDBLEND:
  case TR_MUSICAL_INTERLUDE:
  case EM_ACTIVITY_BURN:
  case EM_INCREASING_ACTIVITY:
  case EM_DIAMOND_STORM:
  case EM_LIGHTNING_LAND:
  case EM_VENOM_SWAMP:
  case EM_CONFLAGRATION:
  case EM_TERRA_DRIVE:
  case EM_SUMMON_ELEMENTAL_ARDOR:
  case EM_SUMMON_ELEMENTAL_DILUVIO:
  case EM_SUMMON_ELEMENTAL_PROCELLA:
  case EM_SUMMON_ELEMENTAL_TERREMOTUS:
  case EM_SUMMON_ELEMENTAL_SERPENS:
  case EM_ELEMENTAL_BUSTER:
  case ABC_CHAIN_REACTION_SHOT_ATK:
  case EM_ELEMENTAL_BUSTER_FIRE:
  case EM_ELEMENTAL_BUSTER_WATER:
  case EM_ELEMENTAL_BUSTER_WIND:
  case EM_ELEMENTAL_BUSTER_GROUND:
  case EM_ELEMENTAL_BUSTER_POISON:
  case EM_EL_FLAMEARMOR:
  case EM_EL_CRYSTAL_ARMOR:
  case EM_EL_EYES_OF_STORM:
  case EM_EL_STRONG_PROTECTION:
  case EM_EL_POISON_SHIELD:
  case ABR_NET_REPAIR:
  case ABR_NET_SUPPORT:
  case ABR_INFINITY_BUSTER:
    effectId = -1;
    return;
  case NPC_ELECTRICWALK:
  case SO_ELECTRICWALK:
    motion = -1;
    effectId = 57;
    return;
  case SO_EARTHGRAVE:
    motion = -1;
    effectId = 1664;
    return;
  case SO_VARETYR_SPEAR:
    motion = 7;
    effectId = 1565;
    return;
  case NPC_MANDRAGORA:
  case GN_CART_TORNADO:
  case GN_BLOOD_SUCKER:
  case GN_CHANGEMATERIAL:
  case GN_MIX_COOKING:
  case GN_MAKEBOMB:
  case GN_S_PHARMACY:
    motion = -1;
    effectId = 911;
    return;
  case GN_CARTBOOST:
    motion = -1;
    effectId = 1677;
    return;
  case SR_GATEOFHELL:
  case GN_MANDRAGORA:
  case SR_HOWLINGOFLION:
    motion = -1;
    effectId = -1;
    return;
  case GN_SPORE_EXPLOSION:
  case GN_DEMONIC_FIRE:
    motion = 12;
    effectId = 911;
    return;
  case GN_THORNS_TRAP:
  case GN_CRAZYWEED:
  case GN_FIRE_EXPANSION:
  case GN_HELLS_PLANT:
    motion = 7;
    effectId = 911;
    return;
  case GN_WALLOFTHORN:
    motion = 7;
    effectId = -1;
    return;
  case WL_JACKFROST:
    effectId = 1449;
    motion = 7;
    break;
  case WL_COMET:
    effectId = 1418;
    motion = 7;
    break;
  case WL_TETRAVORTEX:
    effectId = 805;
    motion = 7;
    break;
  case WL_SUMMONFB:
  case WL_SUMMONBL:
  case WL_SUMMONWB:
  case WL_SUMMONSTONE:
    effectId = g_serviceOptions.Get(229) ? 1430 : 1260;
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    motion = 7;
    break;
  case SC_TRIANGLESHOT:
    effectId = 441;
    motion = 18;
    break;
  case SC_SHADOWFORM:
    effectId = 826;
    motion = 7;
    break;
  case AL_INCAGI:
  case CASH_INCAGI:
  case NPC_COMET:
  case NPC_JACKFROST:
  case NPC_ARROWSTORM:
  case WL_WHITEIMPRISON:
  case WL_FROSTMISTY:
  case WL_MARSHOFABYSS:
  case WL_SIENNAEXECRATE:
  case WL_DRAINLIFE:
  case WL_CRIMSONROCK:
  case WL_EARTHSTRAIN:
  case WL_RELEASE:
  case RA_ARROWSTORM:
  case RA_DETONATOR:
  case RA_WUGSTRIKE:
  case RA_WUGBITE:
  case RA_SENSITIVEKEEN:
    effectId = 12;
    motion = 7;
    break;
  case WL_STASIS:
  case RA_WUGMASTERY:
  case SC_AUTOSHADOWSPELL:
    effectId = 441;
    motion = 7;
    break;
  case AS_SONICBLOW:
  case NPC_RAYOFGENESIS:
  case GC_CROSSIMPACT:
  case AB_DUPLELIGHT_MAGIC:
  case WL_SOULEXPANSION:
  case WL_HELLINFERNO:
  case WL_CHAINLIGHTNING:
  case RA_AIMEDBOLT:
  case LG_OVERBRAND:
  case LG_RAYOFGENESIS:
    effectId = -1;
    motion = 7;
    break;
  case LG_EARTHDRIVE:
    effectId = 1709;
    motion = 7;
    break;
  case LG_PIETY:
    effectId = 830;
    motion = 7;
    break;
  case RA_WUGRIDER:
  case RA_WUGDASH:
  case GN_CARTCANNON:
  case LG_OVERBRAND_PLUSATK:
    effectId = -1;
    motion = -1;
    break;
  case LG_EXEEDBREAK:
    effectId = 882;
    motion = 7;
    break;
  case NC_AXETORNADO:
    motion = 2;
    effectId = 775;
    return;
  case NC_FLAMELAUNCHER:
    motion = 2;
    effectId = 12;
    goto variant;
  case NC_ARMSCANNON:
    motion = 2;
    effectId = -1;
    return;
  case NPC_PSYCHIC_WAVE:
  case NPC_CLOUD_KILL:
  case NPC_POISON_BUSTER:
  case NC_BOOSTKNUCKLE:
  case NC_VULCANARM:
  case NC_COLDSLOWER:
  case NC_ACCELERATION:
  case NC_SHAPESHIFT:
  case SO_DIAMONDDUST:
  case SO_POISON_BUSTER:
  case SO_PSYCHIC_WAVE:
  case SO_CLOUD_KILL:
  case SO_STRIKING:
  case SO_WARMER:
  case SO_VACUUM_EXTREME:
  case SO_ARRULLO:
  case SO_EL_CONTROL:
  case SO_SUMMON_AGNI:
  case SO_SUMMON_AQUA:
  case SO_SUMMON_VENTUS:
  case SO_SUMMON_TERA:
    motion = -1;
    effectId = 12;
    goto variant;
  case NC_SELFDESTRUCTION:
    motion = -1;
    effectId = 775;
    return;
  case NPC_FIREWALK:
  case NC_INFRAREDSCAN:
  case SO_FIREWALK:
    motion = -1;
    effectId = 55;
    return;
  case NC_MAGNETICFIELD:
    motion = -1;
    effectId = 54;
    return;
  case NC_ANALYZE:
  case NC_NEUTRALBARRIER:
  case NC_REPAIR:
    motion = -1;
    effectId = 58;
    return;
  case NC_STEALTHFIELD:
    motion = -1;
    effectId = 441;
    return;
  case WA_SWING_DANCE:
  case WA_SYMPHONY_OF_LOVER:
  case WM_DOMINION_IMPULSE:
  case WM_VOICEOFSIREN:
  case WM_LULLABY_DEEPSLEEP:
  case WM_RANDOMIZESPELL:
  case WM_GLOOMYDAY:
    motion = 16;
    effectId = 840;
    return;
  case WA_MOONLIT_SERENADE:
    motion = 16;
    effectId = 840;
    g_soundMgr->PlayWave("effect\\달빛세레나데.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case WM_POEMOFNETHERWORLD:
    motion = 16;
    effectId = 840;
    g_soundMgr->PlayWave("effect\\나락의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case WM_REVERBERATION:
    motion = 16;
    effectId = 1656;
    return;
  case DC_WINKCHARM:
  case WM_METALICSOUND:
    motion = 16;
    effectId = -1;
    return;
  case WM_SEVERE_RAINSTORM:
    motion = 16;
    effectId = 1286;
    return;
  case WM_DEADHILLHERE:
  case WM_SIRCLEOFNATURE:
    motion = 16;
    effectId = 841;
    return;
  case MI_RUSH_WINDMILL:
  case MI_ECHOSONG:
    motion = 16;
    effectId = 844;
    return;
  case WM_GREAT_ECHO:
  case WM_SONG_OF_MANA:
  case WM_DANCE_WITH_WUG:
  case WM_SATURDAY_NIGHT_FEVER:
  case WM_MELODYOFSINK:
  case WM_BEYOND_OF_WARCRY:
  case WM_UNLIMITED_HUMMING_VOICE:
    motion = 16;
    effectId = 843;
    return;
  case WM_LERADS_DEW:
    motion = 16;
    effectId = 843;
    g_soundMgr->PlayWave("effect\\레라드의이슬.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case WM_SOUND_OF_DESTRUCTION:
    effectId = -1;
    motion = 16;
    break;
  case MI_HARMONIZE:
    motion = 16;
    effectId = 441;
    return;
  case SJ_STAREMPEROR:
    motion = 19;
    effectId = 58;
    return;
  case SP_CURSEEXPLOSION:
  case SP_SOULCURSE:
  case SP_SPA:
  case SP_SHA:
  case SP_SWHOO:
  case SP_SOULDIVISION:
  case SP_SOULEXPLOSION:
    effectId = 454;
    return;
  case TK_HIGHJUMP:
  case SU_LOPE:
    motion = 19;
    effectId = -1;
    return;
  case SU_SV_STEMSPEAR:
  case SU_CN_POWDERING:
  case SU_SCAROFTAROU:
  case SU_PICKYPECK:
  case SU_ARCLOUSEDASH:
  case SU_LUNATICCARROTBEAT:
  case SU_TUNABELLY:
  case SU_BUNCHOFSHRIMP:
  case SU_LUNATICCARROTBEAT2:
    motion = 50;
    effectId = -1;
    return;
  case AB_CLEMENTIA:
  case AB_CANTO:
  case AB_PRAEFATIO:
  case AB_LAUDAAGNUS:
  case AB_LAUDARAMUS:
  case AB_VITUPERATUM:
    effectId = 830;
    return;
  case WL_RECOGNIZEDSPELL:
  case WL_READING_SB:
  case LG_FORCEOFVANGUARD:
  case LG_RAGEBURST:
  case LG_SHIELDSPELL:
  case LG_PRESTIGE:
  case LG_BANDING:
  case LG_MOONSLASHER:
  case LG_HESPERUSLIT:
  case LG_INSPIRATION:
  case WL_READING_SB_READING:
    effectId = 58;
    motion = 7;
    break;
  case IQ_SINCERE_FAITH:
    effectId = 1963;
    return;
  case IQ_POWERFUL_FAITH:
    effectId = 1986;
    return;
  case IQ_FIRM_FAITH:
    effectId = 1998;
    return;
  case CD_PNEUMATICUS_PROCELLA:
    effectId = 1959;
    return;
  case CD_FRAMEN:
    effectId = 1932;
    return;
  case WH_FLAMETRAP:
    effectId = 1923;
    return;
  case MT_SUMMON_ABR_INFINITY:
    effectId = 1910;
    return;
  case MT_SUMMON_ABR_BATTLE_WARIOR:
    effectId = 1938;
    return;
  case MT_SUMMON_ABR_DUAL_CANNON:
    effectId = 1918;
    return;
  case MT_SUMMON_ABR_MOTHER_NET:
    effectId = 1902;
    return;
  case WH_DEEPBLINDTRAP:
    effectId = 1872;
    return;
  case WH_SWIFTTRAP:
    effectId = 1876;
    return;
  case ABC_ABYSS_SQUARE:
    effectId = -1;
    motion = -1;
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case GC_POISONSMOKE:
  case RA_FEARBREEZE:
  case RA_UNLIMIT:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case SHC_FATAL_SHADOW_CROW:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = -1;
    return;
  case NPC_BO_HELL_DUSTY:
    effectId = 2240;
    return;
  case NPC_BO_THROWROCK:
  case NPC_BO_WOODEN_ATTACK:
    effectId = 2249;
    return;
  case NPC_BO_FAIRY_DUSTY:
    effectId = 2255;
    return;
  case NPC_SUMMONSLAVE:
  case NPC_SUMMONMONSTER:
  case CR_AUTOGUARD:
  case MO_EXPLOSIONSPIRITS:
  case NPC_POWERUP:
  case ML_AUTOGUARD:
    motion = 0;
    effectId = -1;
    return;
  case EL_CIRCLE_OF_FIRE:
  case EL_FIRE_ARROW:
  case EL_FIRE_BOMB:
  case EL_FIRE_WAVE:
    motion = 7;
    effectId = 55;
    return;
  case EL_ICE_NEEDLE:
  case EL_WATER_SCREW:
  case EL_TIDAL_WEAPON:
    motion = 7;
    effectId = 54;
    return;
  case EL_WIND_SLASH:
  case EL_HURRICANE:
    motion = 7;
    effectId = 57;
    return;
  case EL_STONE_RAIN:
    motion = 7;
    effectId = 56;
    return;
  case SA_ELEMENTGROUND:
    effectId = 56;
    return;
  default:
    if (static_cast<unsigned int>(skillId - 5401) <= 106 || static_cast<unsigned int>(skillId - 6001) <= 5 || static_cast<unsigned int>(skillId - 6525) <= 81) {
      effectId = -1;
    }
    {
      const std::pair<bool, int> action = CSkillActionMgr::GetInstance()->GetSkillCastMotion(skillId);
      if (action.first == true) {
        motion = action.second;
      }
    }
    break;
  case NV_BASIC:
  case SM_SWORD:
  case SM_TWOHAND:
  case SM_RECOVERY:
  case SM_PROVOKE:
  case SM_ENDURE:
  case MC_INCCARRY:
  case MC_DISCOUNT:
  case MC_OVERCHARGE:
  case MC_PUSHCART:
  case MC_IDENTIFY:
  case MC_VENDING:
  case AC_OWL:
  case AC_VULTURE:
  case TF_DOUBLE:
  case TF_MISS:
  case TF_STEAL:
  case TF_POISON:
  case TF_DETOXIFY:
  case BS_IRON:
  case BS_STEEL:
  case BS_ENCHANTEDSTONE:
  case BS_ORIDEOCON:
  case BS_DAGGER:
  case BS_SWORD:
  case BS_TWOHANDSWORD:
  case BS_AXE:
  case BS_MACE:
  case BS_KNUCKLE:
  case BS_SPEAR:
  case BS_HILTBINDING:
  case BS_FINDINGORE:
  case BS_WEAPONRESEARCH:
  case BS_SKINTEMPER:
  case BS_ADRENALINE:
  case BS_WEAPONPERFECT:
  case BS_OVERTHRUST:
  case BS_MAXIMIZE:
  case HT_BEASTBANE:
  case HT_FALCON:
  case HT_STEELCROW:
  case HT_BLITZBEAT:
  case HT_DETECTING:
  case HT_SPRINGTRAP:
  case AS_RIGHT:
  case AS_LEFT:
  case AS_KATAR:
  case AS_CLOAKING:
  case AS_GRIMTOOTH:
  case AS_ENCHANTPOISON:
  case AS_POISONREACT:
  case AS_VENOMDUST:
  case BS_ADRENALINE2:
  case WS_OVERTHRUSTMAX:
  case GC_CLOAKINGEXCEED:
    break;
  case SA_ELEMENTWATER:
    effectId = 54;
    return;
  }

  if (effectId != 12) {
    return;
  }

variant:
  switch (variant) {
  case ELE_WATER:
    effectId = 54;
    return;
  case ELE_EARTH:
    effectId = 56;
    return;
  case ELE_FIRE:
    effectId = 55;
    return;
  case ELE_WIND:
    effectId = 57;
    return;
  case ELE_POISON:
    effectId = 59;
    return;
  case ELE_HOLY:
    effectId = 58;
    return;
  }
}

// OFFSET: 0x00E15270
// FUNCTION: fn_23831
void CSession::GetSkillActionInfo2(int skillId, int& effectId, int& motion) {
  effectId = 16;
  motion = 7;
  const int beginEffect = this->GetBeginEffectId(skillId);
  switch (skillId) {
  case KO_JYUMONJIKIRI:
    motion = 9;
    break;
  case KO_SETSUDAN:
  case KO_BAKURETSU:
  case KO_HAPPOKUNAI:
  case KO_HUUMARANKA:
  case KO_ZENKAI:
  case KO_IZAYOI:
  case KG_KAGEHUMI:
    motion = 36;
    break;
  case KO_MEIKYOUSISUI:
  case KO_KYOUGAKU:
  case KO_JYUSATSU:
  case KO_KAHU_ENTEN:
  case KO_HYOUHU_HUBUKI:
  case KO_KAZEHU_SEIRAN:
  case KO_DOHU_KOUKAI:
  case KO_GENWAKU:
  case OB_ZANGETSU:
  case OB_OBOROGENSOU:
  case OB_AKAITSUKI:
    motion = 37;
    break;
  }

  if (beginEffect != -1) {
    effectId = beginEffect;
    return;
  }

  switch (skillId) {
  case NV_BASIC:
  case SM_SWORD:
  case SM_TWOHAND:
  case SM_RECOVERY:
  case SM_PROVOKE:
  case SM_ENDURE:
  case MC_INCCARRY:
  case MC_DISCOUNT:
  case MC_OVERCHARGE:
  case MC_PUSHCART:
  case MC_IDENTIFY:
  case MC_VENDING:
  case AC_OWL:
  case AC_VULTURE:
  case TF_DOUBLE:
  case TF_MISS:
  case TF_STEAL:
  case TF_POISON:
  case TF_DETOXIFY:
  case LK_AURABLADE:
    return;
  case SM_BASH:
  case SM_MAGNUM:
  case MC_MAMMONITE:
  case AC_DOUBLE:
  case HW_MAGICPOWER:
  case HT_POWER:
  case HT_PHANTASMIC:
  case MS_MAGNUM:
    motion = 2;
    return;
  case MG_SRECOVERY:
  case MG_SIGHT:
  case MG_NAPALMBEAT:
  case MG_SAFETYWALL:
  case MG_SOULSTRIKE:
  case MG_COLDBOLT:
  case MG_FROSTDIVER:
  case MG_STONECURSE:
  case MG_FIREBALL:
  case MG_FIREWALL:
  case MG_FIREBOLT:
  case MG_LIGHTNINGBOLT:
  case MG_THUNDERSTORM:
  case AL_DP:
  case AL_DEMONBANE:
  case AL_RUWACH:
  case AL_PNEUMA:
  case AL_TELEPORT:
  case AL_WARP:
  case AL_DECAGI:
  case AL_HOLYWATER:
  case AL_CRUCIS:
  case AL_ANGELUS:
  case AL_CURE:
  case ALL_RESURRECTION:
  case PR_MACEMASTERY:
  case PR_IMPOSITIO:
  case PR_SUFFRAGIUM:
  case PR_ASPERSIO:
  case PR_BENEDICTIO:
  case PR_SANCTUARY:
  case PR_SLOWPOISON:
  case PR_STRECOVERY:
  case PR_KYRIE:
  case PR_MAGNIFICAT:
  case PR_GLORIA:
  case PR_LEXDIVINA:
  case PR_TURNUNDEAD:
  case PR_LEXAETERNA:
  case PR_MAGNUS:
  case WZ_FIREPILLAR:
  case WZ_SIGHTRASHER:
  case WZ_FIREIVY:
  case WZ_METEOR:
  case WZ_JUPITEL:
  case WZ_VERMILION:
  case WZ_WATERBALL:
  case WZ_ICEWALL:
  case WZ_FROSTNOVA:
  case WZ_STORMGUST:
  case WZ_EARTHSPIKE:
  case WZ_HEAVENDRIVE:
  case WZ_QUAGMIRE:
  case NPC_DARKSTRIKE:
  case NPC_DARKTHUNDER:
  case NPC_STOP:
  case HW_NAPALMVULCAN:
  case HW_GANBANTEIN:
  case NPC_WIDESIGHT:
  case ALL_ANGEL_PROTECT:
  case NPC_STORMGUST2:
  case NPC_RAINOFMETEOR:
  case NPC_GROUNDDRIVE:
  case WZ_SIGHTBLASTER:
  case ALL_ODINS_RECALL:
  case RETURN_TO_ELDICASTES:
  case ALL_GUARDIAN_RECALL:
  case ALL_ODINS_POWER:
  case MER_MAGNIFICAT:
    effectId = 12;
    return;
  case AL_HEAL:
  case AL_BLESSING:
  case AS_SONICBLOW:
  case AL_HOLYLIGHT:
  case MG_ENERGYCOAT:
  case WE_MALE:
  case WE_FEMALE:
  case WE_BABY:
  case CG_LONGINGFREEDOM:
  case CG_TAROTCARD:
  case CASH_BLESSING:
  case AB_HIGHNESSHEAL:
  case WE_CALLALLFAMILY:
  case WE_ONEFOREVER:
  case WE_CHEERUP:
    effectId = -1;
    motion = 7;
    return;
  case AL_INCAGI:
  case TF_PICKSTONE:
  case BD_ADAPTATION:
  case CASH_INCAGI:
    effectId = -1;
    motion = 0;
    return;
  case AC_CONCENTRATION:
    effectId = 43;
    return;
  case AC_SHOWER:
  case RG_BACKSTAP:
  case RG_RAID:
  case RG_INTIMIDATE:
  case CR_HOLYCROSS:
  case MO_INVESTIGATE:
  case MO_FINGEROFFENSIVE:
  case NPC_DARKCROSS:
  case PA_PRESSURE:
  case PA_SACRIFICE:
  case ASC_BREAKER:
  case SN_SHARPSHOOTING:
  case ASC_METEORASSAULT:
  case GS_TRIPLEACTION:
  case GS_BULLSEYE:
  case GS_TRACKING:
  case GS_DISARM:
  case GS_PIERCINGSHOT:
  case GS_RAPIDSHOWER:
  case GS_DUST:
  case GS_FULLBUSTER:
  case GS_SPREADATTACK:
  case NPC_PULSESTRIKE:
  case NPC_EXPULSION:
  case NPC_PULSESTRIKE2:
  case NPC_ARROWSTORM:
  case NPC_FATALMENACE:
  case NPC_IGNITIONBREAK:
  case KN_CHARGEATK:
  case RG_CLOSECONFINE:
  case RK_HUNDREDSPEAR:
  case RK_WINDCUTTER:
  case RK_IGNITIONBREAK:
  case RK_STORMBLAST:
  case GC_DARKILLUSION:
  case GC_WEAPONCRUSH:
  case GC_VENOMPRESSURE:
  case GC_ROLLINGCUTTER:
  case GC_CROSSRIPPERSLASHER:
  case RA_ARROWSTORM:
  case RA_AIMEDBOLT:
  case NC_FLAMELAUNCHER:
  case NC_ARMSCANNON:
  case NC_AXETORNADO:
  case SC_FATALMENACE:
  case SC_TRIANGLESHOT:
  case SC_STRIPACCESSARY:
  case LG_RAGEBURST:
  case LG_OVERBRAND:
  case LG_MOONSLASHER:
  case LG_HESPERUSLIT:
  case SR_CRESCENTELBOW_AUTOSPELL:
  case GN_CART_TORNADO:
  case IQ_FIRST_BRAND:
  case IQ_SECOND_FLAME:
  case IQ_SECOND_FAITH:
  case IQ_SECOND_JUDGEMENT:
  case CD_EFFLIGO:
  case CD_PETITIO:
  case SHC_SAVAGE_IMPACT:
  case SHC_ETERNAL_SLASH:
  case SHC_FATAL_SHADOW_CROW:
  case MT_AXE_STOMP:
  case MT_RUSH_QUAKE:
  case BO_WOODENWARRIOR:
  case BO_WOODEN_FAIRY:
  case BO_CREEPER:
  case BO_HELLTREE:
  case ABR_BATTLE_BUSTER:
  case ABR_DUAL_CANNON_FIRE:
  case ABR_NET_REPAIR:
  case ABR_NET_SUPPORT:
  case ABR_INFINITY_BUSTER:
    motion = 2;
    effectId = -1;
    return;
  case TF_HIDING:
  case KN_SPEARMASTERY:
  case KN_TWOHANDQUICKEN:
  case KN_RIDING:
  case KN_CAVALIERMASTERY:
  case WZ_ESTIMATION:
  case BS_IRON:
  case BS_STEEL:
  case BS_ENCHANTEDSTONE:
  case BS_ORIDEOCON:
  case BS_DAGGER:
  case BS_SWORD:
  case BS_TWOHANDSWORD:
  case BS_AXE:
  case BS_MACE:
  case BS_KNUCKLE:
  case BS_SPEAR:
  case BS_HILTBINDING:
  case BS_FINDINGORE:
  case BS_WEAPONRESEARCH:
  case BS_REPAIRWEAPON:
  case BS_HAMMERFALL:
  case BS_ADRENALINE:
  case BS_WEAPONPERFECT:
  case BS_OVERTHRUST:
  case BS_MAXIMIZE:
  case HT_BEASTBANE:
  case HT_FALCON:
  case HT_STEELCROW:
  case HT_BLITZBEAT:
  case HT_DETECTING:
  case HT_SPRINGTRAP:
  case AS_RIGHT:
  case AS_LEFT:
  case AS_KATAR:
  case AS_CLOAKING:
  case AS_GRIMTOOTH:
  case AS_ENCHANTPOISON:
  case AS_POISONREACT:
  case AS_VENOMDUST:
  case AS_SPLASHER:
  case MC_CARTREVOLUTION:
  case NPC_PIERCINGATT:
  case NPC_MENTALBREAKER:
  case NPC_RANGEATTACK:
  case NPC_ATTRICHANGE:
  case NPC_CHANGEWATER:
  case NPC_CHANGEGROUND:
  case NPC_CHANGEFIRE:
  case NPC_CHANGEWIND:
  case NPC_CHANGEPOISON:
  case NPC_CHANGEHOLY:
  case NPC_CHANGEDARKNESS:
  case NPC_CHANGETELEKINESIS:
  case NPC_CRITICALSLASH:
  case NPC_COMBOATTACK:
  case NPC_GUIDEDATTACK:
  case NPC_SELFDESTRUCTION:
  case NPC_SPLASHATTACK:
  case NPC_SUICIDE:
  case NPC_POISON:
  case NPC_BLINDATTACK:
  case NPC_SILENCEATTACK:
  case NPC_STUNATTACK:
  case NPC_PETRIFYATTACK:
  case NPC_CURSEATTACK:
  case NPC_SLEEPATTACK:
  case NPC_RANDOMATTACK:
  case NPC_WATERATTACK:
  case NPC_GROUNDATTACK:
  case NPC_FIREATTACK:
  case NPC_WINDATTACK:
  case NPC_POISONATTACK:
  case NPC_HOLYATTACK:
  case NPC_DARKNESSATTACK:
  case NPC_TELEKINESISATTACK:
  case NPC_MAGICALATTACK:
  case AM_CP_WEAPON:
  case AM_CP_SHIELD:
  case AM_CP_ARMOR:
  case AM_CP_HELM:
  case CR_SPEARQUICKEN:
  case MO_BODYRELOCATION:
  case MO_EXTREMITYFIST:
  case MO_CHAINCOMBO:
  case MO_COMBOFINISH:
  case CH_TIGERFIST:
  case SN_WINDWALK:
  case BS_ADRENALINE2:
  case CR_FULLPROTECTION:
  case WS_OVERTHRUSTMAX:
  case KN_ONEHAND:
  case NJ_ISSEN:
  case NPC_FIREBREATH:
  case NPC_WIDESILENCE:
  case NPC_WIDESTONE:
  case NPC_WIDECONFUSE:
  case NPC_WIDESLEEP:
  case NPC_STONESKIN:
  case NPC_ANTIMAGIC:
  case NPC_WIDESTUN:
  case NPC_WIDEWEB:
  case NPC_FIRESTORM:
  case NPC_WIDESILENCE2:
  case NPC_WIDESTUN2:
  case NPC_WIDESTONE2:
  case NPC_WIDESLEEP2:
  case NPC_WIDECONFUSE2:
  case NPC_CANE_OF_EVIL_EYE:
  case NPC_KILLING_AURA:
  case GC_CLOAKINGEXCEED:
  case AB_JUDEX:
  case AB_ADORAMUS:
  case AB_DUPLELIGHT_MELEE:
  case AB_DUPLELIGHT_MAGIC:
  case SR_DRAGONCOMBO:
  case SR_EARTHSHAKER:
  case SR_FALLENEMPIRE:
  case SR_RAMPAGEBLASTER:
  case SR_LIGHTNINGWALK:
  case SR_RAISINGDRAGON:
  case SR_POWERVELOCITY:
  case LG_OVERBRAND_BRANDISH:
  case SJ_FALLINGSTAR:
  case SJ_SOLARBURST:
  case SP_SPA:
  case SP_SWHOO:
  case SU_SV_ROOTTWIST_ATK:
  case SU_LUNATICCARROTBEAT:
  case SU_LUNATICCARROTBEAT2:
  case DK_SERVANT_W_DEMOL:
  case DK_DRAGONIC_AURA:
  case AG_DEADLY_PROJECTION:
  case AG_SOUL_VC_STRIKE:
  case AG_ASTRAL_STRIKE:
  case AG_ROCK_DOWN:
  case AG_STORM_CANNON:
  case AG_CRIMSON_ARROW:
  case AG_FROZEN_SLASH:
  case IQ_THIRD_PUNISH:
  case IQ_THIRD_FLAME_BOMB:
  case IQ_THIRD_CONSECRATION:
  case IG_SHIELD_SHOOTING:
  case CD_ARBITRIUM:
  case CD_ARBITRIUM_ATK:
  case CD_FRAMEN:
  case SHC_POTENT_VENOM:
  case WH_HAWKRUSH:
  case WH_HAWKBOOMERANG:
  case BO_BIONIC_PHARMACY:
  case BO_THE_WHOLE_PROTECTION:
  case BO_ADVANCE_PROTECTION:
  case BO_RESEARCHREPORT:
  case TR_METALIC_FURY:
  case TR_SOUNDBLEND:
  case EM_DIAMOND_STORM:
  case EM_TERRA_DRIVE:
  case EM_ELEMENTAL_BUSTER:
  case EM_ELEMENTAL_BUSTER_FIRE:
  case EM_ELEMENTAL_BUSTER_WATER:
  case EM_ELEMENTAL_BUSTER_WIND:
  case EM_ELEMENTAL_BUSTER_GROUND:
  case EM_ELEMENTAL_BUSTER_POISON:
  case EL_FIRE_ARROW:
  case EL_FIRE_BOMB:
  case EL_FIRE_WAVE:
  case EL_ICE_NEEDLE:
  case EL_WATER_SCREW:
  case EL_TIDAL_WEAPON:
  case EL_WIND_SLASH:
  case EL_HURRICANE:
  case EL_STONE_RAIN:
  case ELEMENTAL_LAST:
  case EM_EL_FLAMEARMOR:
  case EM_EL_FLAMEROCK:
  case EM_EL_COLD_FORCE:
  case EM_EL_CRYSTAL_ARMOR:
  case EM_EL_AGE_OF_ICE:
  case EM_EL_GRACE_BREEZE:
  case EM_EL_EYES_OF_STORM:
  case EM_EL_STORM_WIND:
  case EM_EL_EARTH_CARE:
  case EM_EL_STRONG_PROTECTION:
  case EM_EL_AVALANCHE:
  case EM_EL_DEEP_POISONING:
  case EM_EL_POISON_SHIELD:
  case EM_EL_DEADLY_POISON:
    effectId = -1;
    return;
  case KN_PIERCE:
  case KN_SPEARSTAB:
  case KN_AUTOCOUNTER:
  case KN_BOWLINGBASH:
  case BS_SKINTEMPER:
  case MO_TRIPLEATTACK:
  case CH_PALMSTRIKE:
  case CH_CHAINCRUSH:
  case LK_SPIRALPIERCE:
  case LK_HEADCRUSH:
  case LK_JOINTBEAT:
  case HW_GRAVITATION:
  case MO_BALKYOUNG:
  case RK_SONICWAVE:
  case GC_CROSSIMPACT:
  case ABC_ABYSS_DAGGER:
  case ABC_UNLUCKY_RUSH:
  case ABC_CHAIN_REACTION_SHOT:
  case ABC_DEFT_STAB:
  case ABC_FRENZY_SHOT:
  case ABC_CHAIN_REACTION_SHOT_ATK:
    effectId = -1;
    motion = 2;
    return;
  case KN_BRANDISHSPEAR:
    effectId = 144;
    motion = 2;
    return;
  case KN_SPEARBOOMERANG:
  case TF_SPRINKLESAND:
  case TF_THROWSTONE:
  case AS_VENOMKNIFE:
    effectId = -1;
    motion = 12;
    return;
  case HT_SKIDTRAP:
  case HT_LANDMINE:
  case HT_ANKLESNARE:
  case HT_SHOCKWAVE:
  case HT_SANDMAN:
  case HT_FLASHER:
  case HT_FREEZINGTRAP:
  case HT_BLASTMINE:
  case HT_CLAYMORETRAP:
  case HT_REMOVETRAP:
  case HT_TALKIEBOX:
  case GM_SANDMAN:
  case RA_ELECTRICSHOCKER:
  case RA_CLUSTERBOMB:
  case RA_MAGENTATRAP:
  case RA_COBALTTRAP:
  case RA_MAIZETRAP:
  case RA_VERDURETRAP:
  case RA_FIRINGTRAP:
  case RA_ICEBOUNDTRAP:
  case WH_FLAMETRAP:
    motion = 5;
    return;
  case NPC_SUMMONSLAVE:
  case NPC_SUMMONMONSTER:
  case CR_AUTOGUARD:
  case CR_REFLECTSHIELD:
  case CR_DEFENDER:
  case MO_STEELBODY:
  case NPC_POWERUP:
  case NPC_AGIUP:
  case LK_PARRYING:
  case SN_SIGHT:
  case WS_MELTDOWN:
  case WS_CARTBOOST:
  case TK_RUN:
  case SG_FUSION:
  case MS_PARRYING:
  case ML_DEFENDER:
  case ML_AUTOGUARD:
    motion = 0;
    effectId = -1;
    return;
  case AM_DEMONSTRATION:
  case AM_ACIDTERROR:
  case AM_POTIONPITCHER:
  case AM_CANNIBALIZE:
  case CR_SHIELDCHARGE:
  case CR_SHIELDBOOMERANG:
  case PA_SHIELDCHAIN:
  case GN_BLOOD_SUCKER:
  case GN_SLINGITEM:
  case BO_ACIDIFIED_ZONE_WATER:
  case BO_ACIDIFIED_ZONE_GROUND:
  case BO_ACIDIFIED_ZONE_WIND:
  case BO_ACIDIFIED_ZONE_FIRE:
    motion = 12;
    effectId = -1;
    return;
  case AM_CALLHOMUN:
  case AM_REST:
  case AM_RESURRECTHOMUN:
  case ITM_TOMAHAWK:
  case NPC_ARMORBRAKE:
  case NPC_HELMBRAKE:
  case NPC_SHIELDBRAKE:
  case HW_MAGICCRASHER:
  case PF_HPCONVERSION:
  case PF_SOULCHANGE:
  case PF_SOULBURN:
  case SN_FALCONASSAULT:
  case ST_CHASEWALK:
  case PF_MINDBREAKER:
  case PF_MEMORIZE:
  case TK_SEVENWIND:
  case SG_FEEL:
  case SG_HATE:
  case SL_KAIZEL:
  case SL_KAAHI:
  case SL_KAUPE:
  case SL_KAITE:
  case SL_STIN:
  case SL_STUN:
  case SL_SMA:
  case SL_SWOO:
  case SL_SKE:
  case SL_SKA:
  case WS_CARTTERMINATION:
  case TK_MISSION:
  case NPC_JACKFROST:
  case NPC_DRAGONBREATH:
  case NPC_MANDRAGORA:
  case NPC_PSYCHIC_WAVE:
  case NPC_RAYOFGENESIS:
  case NPC_VENOMIMPRESS:
  case NPC_CLOUD_KILL:
  case NPC_PHANTOMTHRUST:
  case NPC_POISON_BUSTER:
  case NPC_ELECTRICWALK:
  case NPC_FIREWALK:
  case RK_ENCHANTBLADE:
  case RK_DRAGONBREATH:
  case RK_DRAGONHOWLING:
  case RK_CRUSHSTRIKE:
  case RK_REFRESH:
  case RK_GIANTGROWTH:
  case RK_ABUNDANCE:
  case RK_PHANTOMTHRUST:
  case GC_VENOMIMPRESS:
  case WL_SOULEXPANSION:
  case WL_FROSTMISTY:
  case WL_JACKFROST:
  case WL_CRIMSONROCK:
  case WL_HELLINFERNO:
  case WL_CHAINLIGHTNING:
  case WL_TETRAVORTEX:
  case WL_RELEASE:
  case WL_READING_SB:
  case RA_DETONATOR:
  case RA_WUGSTRIKE:
  case RA_WUGBITE:
  case RA_SENSITIVEKEEN:
  case SC_AUTOSHADOWSPELL:
  case SC_SHADOWFORM:
  case SC_BODYPAINT:
  case SC_INVISIBILITY:
  case SC_DEADLYINFECT:
  case SC_ENERVATION:
  case SC_GROOMY:
  case SC_IGNORANCE:
  case SC_LAZINESS:
  case SC_UNLUCKY:
  case SC_WEAKNESS:
  case SC_MANHOLE:
  case SC_DIMENSIONDOOR:
  case SC_CHAOSPANIC:
  case SC_MAELSTROM:
  case SC_BLOODYLUST:
  case SC_FEINTBOMB:
  case LG_TRAMPLE:
  case LG_SHIELDPRESS:
  case LG_SHIELDSPELL:
  case LG_EXEEDBREAK:
  case LG_PRESTIGE:
  case LG_BANDING:
  case LG_RAYOFGENESIS:
  case LG_PIETY:
  case LG_INSPIRATION:
  case WA_SWING_DANCE:
  case WA_SYMPHONY_OF_LOVER:
  case WA_MOONLIT_SERENADE:
  case MI_RUSH_WINDMILL:
  case MI_ECHOSONG:
  case MI_HARMONIZE:
  case WM_METALICSOUND:
  case WM_REVERBERATION:
  case WM_DOMINION_IMPULSE:
  case WM_SEVERE_RAINSTORM:
  case WM_POEMOFNETHERWORLD:
  case WM_VOICEOFSIREN:
  case WM_LULLABY_DEEPSLEEP:
  case WM_GREAT_ECHO:
  case WM_SOUND_OF_DESTRUCTION:
  case SO_FIREWALK:
  case SO_ELECTRICWALK:
  case SO_EARTHGRAVE:
  case SO_DIAMONDDUST:
  case SO_POISON_BUSTER:
  case SO_PSYCHIC_WAVE:
  case SO_CLOUD_KILL:
  case SO_STRIKING:
  case SO_WARMER:
  case SO_VACUUM_EXTREME:
  case SO_VARETYR_SPEAR:
  case SO_ARRULLO:
  case SO_EL_CONTROL:
  case SO_SUMMON_AGNI:
  case SO_SUMMON_AQUA:
  case SO_SUMMON_VENTUS:
  case SO_SUMMON_TERA:
  case GN_CARTCANNON:
  case GN_THORNS_TRAP:
  case GN_CRAZYWEED:
  case GN_MANDRAGORA:
  case SR_RIDEINLIGHTNING:
  case RK_DRAGONBREATH_WATER:
  case WL_READING_SB_READING:
  case HFLI_MOON:
  case HFLI_SBR44:
  case EL_CIRCLE_OF_FIRE:
    motion = 7;
    effectId = -1;
    return;
  case CR_PROVIDENCE:
  case MO_CALLSPIRITS:
  case MO_BLADESTOP:
  case PA_GOSPEL:
  case CH_SOULCOLLECT:
  case NPC_SR_CURSEDCIRCLE:
  case MO_KITRANSLATION:
  case SR_CURSEDCIRCLE:
    motion = 0;
    return;
  case BD_LULLABY:
  case BD_RICHMANKIM:
  case BD_ETERNALCHAOS:
  case BD_DRUMBATTLEFIELD:
  case BD_RINGNIBELUNGEN:
  case BD_ROKISWEIL:
  case BD_INTOABYSS:
  case BD_SIEGFRIED:
  case DC_UGLYDANCE:
  case DC_HUMMING:
  case DC_DONTFORGETME:
  case DC_FORTUNEKISS:
  case DC_SERVICEFORYOU:
  case CG_MOONLIT:
  case CG_MARIONETTE:
  case CG_HERMODE:
    effectId = -1;
    motion = 16;
    return;
  case BA_MUSICALSTRIKE:
  case DC_THROWARROW:
  case CG_ARROWVULCAN:
  case NJ_KIRIKAGE:
  case NC_VULCANARM:
  case LG_CANNONSPEAR:
  case LG_BANISHINGPOINT:
  case LG_PINPOINTATTACK:
  case WH_GALESTORM:
    motion = 9;
    effectId = -1;
    return;
  case BA_DISSONANCE:
  case BA_WHISTLE:
  case BA_ASSASSINCROSS:
  case BA_POEMBRAGI:
  case BA_APPLEIDUN:
    effectId = -1;
    motion = 17;
    return;
  case NPC_UNDEADATTACK:
    effectId = 454;
    return;
  case LK_CONCENTRATION:
  case LK_BERSERK:
  case LK_FURY:
  case HP_ASSUMPTIO:
  case HP_BASILICA:
  case PF_SPIDERWEB:
  case PF_DOUBLECASTING:
  case CASH_ASSUMPTIO:
  case MS_BERSERK:
  case EL_HEATER:
  case EL_TROPIC:
  case EL_AQUAPLAY:
  case EL_COOLER:
  case EL_CHILLY_AIR:
  case EL_GUST:
  case EL_BLAST:
  case EL_WILD_STORM:
  case EL_PETROLOGY:
  case EL_CURSED_SOIL:
  case EL_UPHEAVAL:
    motion = 7;
    return;
  case HW_SOULDRAIN:
    motion = 1;
    effectId = -1;
    return;
  case TK_READYSTORM:
  case RK_STONEHARDSKIN:
  case SC_REPRODUCE:
    motion = 21;
    effectId = -1;
    return;
  case TK_STORMKICK:
  case SR_WINDMILL:
  case SJ_FULLMOONKICK:
    motion = 30;
    effectId = -1;
    return;
  case TK_READYDOWN:
  case GN_CARTBOOST:
    motion = 22;
    effectId = -1;
    return;
  case TK_DOWNKICK:
  case LG_FORCEOFVANGUARD:
    motion = 31;
    effectId = -1;
    return;
  case TK_READYTURN:
  case SL_ALCHEMIST:
  case SL_MONK:
  case SL_STAR:
  case SL_SAGE:
  case SL_CRUSADER:
  case SL_SUPERNOVICE:
  case SL_KNIGHT:
  case SL_WIZARD:
  case SL_PRIEST:
  case SL_BARDDANCER:
  case SL_ROGUE:
  case SL_ASSASIN:
  case SL_BLACKSMITH:
  case SL_HUNTER:
  case SL_SOULLINKER:
  case SL_HIGH:
  case SL_DEATHKNIGHT:
  case SL_COLLECTOR:
  case SL_NINJA:
  case SL_GUNNER:
  case SR_CRESCENTELBOW:
  case SR_ASSIMILATEPOWER:
  case SP_SOULGOLEM:
  case SP_SOULSHADOW:
  case SP_SOULFALCON:
  case SP_SOULFAIRY:
    motion = 23;
    effectId = -1;
    return;
  case TK_TURNKICK:
  case LG_REFLECTDAMAGE:
  case SR_GENTLETOUCH_QUIET:
  case SR_GENTLETOUCH_CURE:
  case SR_GENTLETOUCH_ENERGYGAIN:
  case SR_GENTLETOUCH_CHANGE:
  case SR_GENTLETOUCH_REVITALIZE:
  case SJ_PROMINENCEKICK:
    motion = 32;
    effectId = -1;
    return;
  case TK_READYCOUNTER:
  case RK_FIGHTINGSPIRIT:
  case SR_GATEOFHELL:
    motion = 24;
    effectId = -1;
    return;
  case TK_COUNTER:
    motion = 26;
    effectId = -1;
    return;
  case TK_DODGE:
  case LG_EARTHDRIVE:
    motion = 25;
    effectId = -1;
    return;
  case TK_JUMPKICK:
  case SR_KNUCKLEARROW:
    motion = 27;
    effectId = -1;
    return;
  case TK_HIGHJUMP:
  case SJ_STAREMPEROR:
  case SU_LOPE:
    motion = 20;
    effectId = -1;
    return;
  case SG_SUN_WARM:
  case SG_MOON_WARM:
  case SG_STAR_WARM:
    motion = 34;
    effectId = -1;
    return;
  case SG_SUN_COMFORT:
  case SG_MOON_COMFORT:
  case SG_STAR_COMFORT:
    motion = 33;
    effectId = -1;
    return;
  case AM_BERSERKPITCHER:
  case CR_SLIMPITCHER:
  case CR_ACIDDEMONSTRATION:
  case WL_CHAINLIGHTNING_ATK:
  case WL_TETRAVORTEX_FIRE:
  case WL_TETRAVORTEX_WATER:
  case WL_TETRAVORTEX_WIND:
  case WL_TETRAVORTEX_GROUND:
  case RA_WUGMASTERY:
  case RA_WUGRIDER:
  case RA_WUGDASH:
  case WM_REVERBERATION_MELEE:
  case WM_REVERBERATION_MAGIC:
  case GN_SPORE_EXPLOSION:
  case GN_WALLOFTHORN:
  case GN_CRAZYWEED_ATK:
  case GN_DEMONIC_FIRE:
  case GN_FIRE_EXPANSION:
  case GN_HELLS_PLANT:
  case WM_SEVERE_RAINSTORM_MELEE:
  case AG_VIOLENT_QUAKE_ATK:
  case AG_ALL_BLOOM_ATK:
  case AG_ALL_BLOOM_ATK2:
  case AG_CRYSTAL_IMPACT_ATK:
  case AG_ASTRAL_STRIKE_ATK:
  case AG_CRIMSON_ARROW_ATK:
  case IQ_MASSIVE_F_BLASTER:
  case IQ_EXPOSION_BLASTER:
    motion = -1;
    effectId = -1;
    return;
  case GS_GLITTERING:
  case NC_BOOSTKNUCKLE:
  case NC_COLDSLOWER:
  case NC_ANALYZE:
  case NC_MAGNETICFIELD:
    motion = 39;
    effectId = -1;
    return;
  case GS_FLING:
  case GS_MADNESSCANCEL:
  case GS_ADJUSTMENT:
  case GS_INCREASING:
  case GS_DESPERADO:
  case GS_GATLINGFEVER:
    motion = 38;
    effectId = -1;
    return;
  case GS_CRACKER:
  case GS_GROUNDDRIFT:
    motion = 40;
    effectId = -1;
    return;
  case NJ_SYURIKEN:
  case NJ_KUNAI:
  case NJ_HUUMA:
  case NJ_ZENYNAGE:
  case NJ_UTSUSEMI:
  case NJ_KOUENKA:
  case NJ_HYOUSYOURAKU:
  case NJ_HUUJIN:
  case NJ_RAIGEKISAI:
  case NJ_KAMAITACHI:
  case NJ_NEN:
  case KO_BAKURETSU:
  case KO_HAPPOKUNAI:
  case KO_MUCHANAGE:
    motion = 36;
    effectId = -1;
    return;
  case NJ_TATAMIGAESHI:
  case NJ_KASUMIKIRI:
  case NJ_SHADOWJUMP:
  case NJ_BUNSINJYUTSU:
  case NJ_KAENSIN:
  case NJ_BAKUENRYU:
  case NJ_HYOUSENSOU:
  case NJ_SUITON:
    motion = 35;
    effectId = -1;
    return;
  case NPC_MILLENNIUMSHIELD:
  case RK_DEATHBOUND:
  case RK_MILLENNIUMSHIELD:
    motion = 11;
    return;
  case NPC_HELLBURNING:
    effectId = 49;
    return;
  case DC_WINKCHARM:
    motion = 16;
    effectId = -1;
    return;
  case BS_GREED:
    motion = 5;
    effectId = -1;
    return;
  case GC_POISONINGWEAPON:
    motion = 7;
    effectId = 768;
    return;
  case GC_WEAPONBLOCKING:
    motion = -1;
    return;
  case GC_COUNTERSLASH:
    motion = 2;
    effectId = 768;
    return;
  case GC_POISONSMOKE:
    motion = 7;
    effectId = 771;
    return;
  case GC_PHANTOMMENACE:
  case NC_ACCELERATION:
  case EL_WATER_SCREEN:
  case EL_WATER_BARRIER:
    motion = 7;
    effectId = 441;
    return;
  case NC_PILEBUNKER:
  case SO_SPELLFIST:
    motion = 39;
    effectId = 441;
    return;
  case NC_HOVERING:
  case NC_F_SIDESLIDE:
  case NC_B_SIDESLIDE:
    motion = -1;
    effectId = 441;
    return;
  case NC_SELFDESTRUCTION:
    motion = 5;
    effectId = 441;
    return;
  case NC_SHAPESHIFT:
  case NC_NEUTRALBARRIER:
  case NC_STEALTHFIELD:
    motion = 43;
    effectId = 441;
    return;
  case NC_EMERGENCYCOOL:
    motion = 25;
    effectId = 441;
    return;
  case NC_INFRAREDSCAN:
    motion = 44;
    effectId = 548;
    return;
  case NC_REPAIR:
    motion = 43;
    effectId = 58;
    return;
  case NC_AXEBOOMERANG:
    motion = 12;
    effectId = 12;
    return;
  case NC_POWERSWING:
    motion = 2;
    effectId = 12;
    return;
  case SR_SKYNETBLOW:
  case SR_TIGERCANNON:
    motion = 18;
    effectId = -1;
    return;
  case GN_CHANGEMATERIAL:
  case GN_MIX_COOKING:
  case GN_MAKEBOMB:
  case GN_S_PHARMACY:
  case EL_WIND_STEP:
  case EL_ZEPHYR:
    motion = 7;
    effectId = 911;
    return;
  case LG_OVERBRAND_PLUSATK:
    motion = -1;
    effectId = 548;
    return;
  case SJ_FLASHKICK:
    motion = 51;
    effectId = -1;
    return;
  case SU_SCRATCH:
    motion = 49;
    effectId = -1;
    return;
  case EL_SOLID_SKIN:
    motion = 7;
    effectId = 56;
    return;
  }

  if (static_cast<unsigned int>(skillId - 5401) <= 106 || static_cast<unsigned int>(skillId - 6001) <= 5 || static_cast<unsigned int>(skillId - 6525) <= 81) {
    effectId = -1;
  }

  const std::pair<bool, int> action = CSkillActionMgr::GetInstance()->GetSkillAttackMotion(skillId);
  if (action.first == true) {
    motion = action.second;
  }
}
