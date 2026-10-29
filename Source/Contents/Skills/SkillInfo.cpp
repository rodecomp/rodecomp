#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"


#include <algorithm>
#include <bit>
#include <cstdlib>
#include <format>
#include <memory>

class CEfstAction_ChangeSize : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_CD_DIVINUS_FLOS : public CSkillAction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GenderChange : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_ShowEffect : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_RA_UNLIMIT : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_UNLIMIT : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool flag) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_WEAPON : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CEfstAction_DANCING_KNIFE : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SHC_SHADOW_STAB : public CSkillAction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_SHADOW_EXCEED : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SHADOW_EXCEED : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_ABC_CHASING_BREAK : public CSkillAction {
public:
  bool Slot07() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_CHASING_SHOT : public CSkillAction {
public:
  bool Slot07() override;
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_ABYSS_FLAME_ATK : public CSkillAction {
public:
  bool Slot03(int argument) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AG_ENERGY_CONVERSION : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_ENCHANTING_SHADOW : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
};

class CSkillAction_SHC_IMPACT_CRATER : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_CROSS_SLASH : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_DANCING_KNIFE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_HIT_AND_SLIDING : public CSkillAction {
public:
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_ABYSS_FLAME : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_EM_PSYCHIC_STREAM : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IQ_BLAZING_FLAME_BLAST : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_MAYHEMIC_THORNS : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_EXPLOSIVE_POWDER : public CSkillAction {
public:
  bool Slot05() override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_MYSTERY_POWDER : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_MYSTERY_POWDER : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_BO_DUST_EXPLOSION : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_VIGOR : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_VIGOR : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_DK_SERVANTWEAPON_ATK : public CSkillAction {
public:
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SERVANTWEAPON : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot04() override;
};

class CEfstAction_SERVANT_SIGN : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_DK_SERVANT_W_PHANTOM : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int caster, CGameActor* target, CGameActor* argument3) override;
};

class CSkillAction_DK_SERVANT_W_DEMOL : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int caster, CGameActor* target, CGameActor* argument3) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_STORMSLASH : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_MADNESS_CRUSHER : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  bool Slot04() override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_DRAGONIC_PIERCE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_DRAGONIC_BREATH : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot15(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_CHARGINGPIERCE : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CHARGINGPIERCE : public CEfstAction {
public:
  EfstActionMessage Slot04() override;
};

class CEfstAction_CHARGINGPIERCE_COUNT : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_DK_HACKANDSLASHER : public CSkillAction {
public:
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_HACKANDSLASHER_ATK : public CSkillAction {
public:
  int Slot02() override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_BLOCK_EXPLOSION : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_FROST_FIELD : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
  void Slot11(int level, CGameActor* source, CGameActor* target) override;
  void Slot16(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FROST_STORM : public CEfstAction {
public:
  void Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

__declspec(noinline) CEffect* SpawnDustExplosion(int quadrant, const vector3d& position);

// GLOBAL: gl_24640
CSkillActionMgr* g_skillActionMgr;

// INLINED
CSkillActionMgr::~CSkillActionMgr() = default;

// OFFSET: 0x00773480
// FUNCTION: fn_08311
CSkillInfo::CSkillInfo() : available(1), id(0), level(0), name(nullptr), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
}

// OFFSET: 0x00773400
// FUNCTION: fn_08310
CSkillInfo::CSkillInfo(int) : available(0), id(0), level(0), name("Unknown-Skill"), unknown_0028(0), unknown_0030(0), unknown_0034(0) {
}

// OFFSET: 0x0076FD70
// FUNCTION: fn_08254
CSkillInfo::CSkillInfo(const CSkillInfo& other) {
  *this = other;
}

// OFFSET: 0x009149F0
// FUNCTION: fn_12543
CSkillInfo& CSkillInfo::operator=(const CSkillInfo& other) {
  this->available = other.available;
  if (this == &other) {
    return *this;
  }

  this->id = other.id;
  this->unknown_000C = other.unknown_000C;
  this->level = other.level;
  this->unknown_0014 = other.unknown_0014;
  this->unknown_0018 = other.unknown_0018;
  this->unknown_001C = other.unknown_001C;
  this->name = other.name;
  this->unknown_0024 = other.unknown_0024;
  this->unknown_0028 = other.unknown_0028;
  this->unknown_002C = other.unknown_002C;
  this->unknown_0030 = other.unknown_0030;
  this->unknown_0034 = other.unknown_0034;
  this->entries = other.entries;

  return *this;
}

// OFFSET: 0x00773500
// FUNCTION: fn_08312
CSkillInfo::~CSkillInfo() = default;

// OFFSET: 0x00774710
// FUNCTION: fn_08325
bool CSkillInfo::IsValid() const {
  return this->available != 0 && this->level != 0;
}

// OFFSET: 0x008AC440
// FUNCTION: fn_11732
bool CSkillInfo::MatchesShortcut(const NormalItemSlot* slot) const {
  if (!slot || !this->IsValid()) {
    return false;
  }

  const auto requested = slot->unknown_0005;
  return requested != 0 && (this->id != slot->itemId || this->level >= requested);
}

// OFFSET: 0x00773550
// FUNCTION: fn_08313
int CSkillInfo::UseApAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseApAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x00773620
// FUNCTION: fn_08314
int CSkillInfo::UseSpAmount(LuaScript& script, int skillLevel) const {
  int amount = 0;
  script.Call("GetLevelUseSpAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x007736F0
// FUNCTION: fn_08315
const char* CSkillInfo::DisplayName(LuaScript& script) const {
  return CSkillInfo::DisplayName(script, static_cast<int>(this->id));
}

// OFFSET: 0x00773920
// FUNCTION: fn_08318
const char* CSkillInfo::DisplayName(LuaScript& script, int skillId) {
  if (skillId == SU_CN_METEOR2) {
    skillId = SU_CN_METEOR;
  } else if (skillId == SU_LUNATICCARROTBEAT2) {
    skillId = SU_LUNATICCARROTBEAT;
  }

  const char* result = nullptr;
  return script.Call("GetSkillName", "d>s", skillId, &result) ? result : "Unknown-Skill";
}

// OFFSET: 0x007739F0
// FUNCTION: fn_08319
void __stdcall Sub7739F0() {
}

// OFFSET: 0x00773A00
// FUNCTION: fn_08320
bool IsOptionalSelfTargetSkill(int skillId) {
  switch (skillId) {
  case AL_HEAL:
  case AL_INCAGI:
  case AL_DECAGI:
  case AL_BLESSING:
  case AL_CURE:
  case TF_DETOXIFY:
  case ALL_RESURRECTION:
  case PR_IMPOSITIO:
  case PR_SUFFRAGIUM:
  case PR_ASPERSIO:
  case PR_SLOWPOISON:
  case PR_STRECOVERY:
  case PR_KYRIE:
  case PR_TURNUNDEAD:
  case PR_LEXAETERNA:
  case AS_ENCHANTPOISON:
  case AS_POISONREACT:
  case AM_POTIONPITCHER:
  case AM_CP_WEAPON:
  case AM_CP_SHIELD:
  case AM_CP_ARMOR:
  case AM_CP_HELM:
  case CR_DEVOTION:
  case SA_FLAMELAUNCHER:
  case SA_FROSTWEAPON:
  case SA_LIGHTNINGLOADER:
  case SA_SEISMICWEAPON:
  case SA_DELUGE:
  case SA_DISPELL:
  case PF_SOULCHANGE:
  case PF_MINDBREAKER:
  case SL_KAAHI:
  case SL_KAUPE:
  case SL_KAITE:
  case SL_KAINA:
  case SL_STIN:
  case SL_STUN:
  case SL_SMA:
  case SL_SWOO:
  case SL_SKE:
  case SL_SKA:
  case CR_FULLPROTECTION:
  case MO_KITRANSLATION:
  case RK_ENCHANTBLADE:
  case RK_PHANTOMTHRUST:
  case GC_ANTIDOTE:
  case GC_POISONINGWEAPON:
  case AB_RENOVATIO:
  case AB_HIGHNESSHEAL:
  case AB_CLEARANCE:
  case AB_EXPIATIO:
  case SR_POWERVELOCITY:
  case SR_GENTLETOUCH_QUIET:
  case SR_GENTLETOUCH_CURE:
  case SR_GENTLETOUCH_ENERGYGAIN:
  case SR_GENTLETOUCH_CHANGE:
  case SR_GENTLETOUCH_REVITALIZE:
  case SO_STRIKING:
  case AB_SECRAMENT:
  case ML_DEVOTION:
      return true;
  default:
    return false;
  }
}

// OFFSET: 0x00773CB0
// FUNCTION: fn_08321
bool HidesUseFailureMessage(int skillId) {
  return skillId == KR_CHOP_CHOP || skillId == AT_FURIOS_STORM;
}

// OFFSET: 0x00773CD0
// FUNCTION: fn_08322
bool IsSupportSkill(int skillId) {
  switch (skillId) {
  case SM_RECOVERY:
  case AL_HEAL:
  case AL_INCAGI:
  case AL_BLESSING:
  case AL_CURE:
  case TF_DETOXIFY:
  case ALL_RESURRECTION:
  case PR_IMPOSITIO:
  case PR_SUFFRAGIUM:
  case PR_ASPERSIO:
  case PR_SLOWPOISON:
  case PR_STRECOVERY:
  case PR_KYRIE:
  case BS_REPAIRWEAPON:
  case AS_ENCHANTPOISON:
  case AS_POISONREACT:
  case AM_POTIONPITCHER:
  case AM_CP_WEAPON:
  case AM_CP_SHIELD:
  case AM_CP_ARMOR:
  case AM_CP_HELM:
  case CR_DEVOTION:
  case CR_PROVIDENCE:
  case MO_ABSORBSPIRITS:
  case SA_FLAMELAUNCHER:
  case SA_FROSTWEAPON:
  case SA_LIGHTNINGLOADER:
  case SA_SEISMICWEAPON:
  case HP_ASSUMPTIO:
  case SL_ALCHEMIST:
  case AM_BERSERKPITCHER:
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
  case CR_FULLPROTECTION:
  case SL_HIGH:
  case MB_B_EXCLUDE:
  case SL_DEATHKNIGHT:
  case SL_COLLECTOR:
  case SL_NINJA:
  case SL_GUNNER:
  case DA_RESET:
  case DE_COIL:
  case DA_FIRSTSLOT:
  case DA_HEADDEF:
  case DA_REWARD:
  case DA_EARPLUG:
  case DA_CONTRACT:
  case DA_COPY:
  case DA_CRYSTAL:
  case DA_EXP:
  case DA_REBUILD:
  case DA_JOBCHANGE:
  case GM_SANDMAN:
  case CASH_BLESSING:
  case CASH_INCAGI:
  case CASH_ASSUMPTIO:
  case ALL_ANGEL_PROTECT:
  case NPC_CHEAL:
  case MO_KITRANSLATION:
  case RK_ENCHANTBLADE:
  case RK_CRUSHSTRIKE:
  case GC_ANTIDOTE:
  case AB_CLEMENTIA:
  case AB_CANTO:
  case AB_CHEAL:
  case AB_RENOVATIO:
  case AB_HIGHNESSHEAL:
  case AB_CLEARANCE:
  case AB_EXPIATIO:
  case NC_REPAIR:
  case SC_SHADOWFORM:
  case LG_PIETY:
  case SR_POWERVELOCITY:
  case SR_GENTLETOUCH_CURE:
  case SR_GENTLETOUCH_CHANGE:
  case SR_GENTLETOUCH_REVITALIZE:
  case MI_HARMONIZE:
  case WM_DEADHILLHERE:
  case WM_GLOOMYDAY:
  case SO_STRIKING:
  case GN_BLOOD_SUCKER:
  case GN_WALLOFTHORN:
  case AB_SECRAMENT:
  case ALL_ODINS_POWER:
  case SP_SOULGOLEM:
  case SP_SOULSHADOW:
  case SP_SOULFALCON:
  case SP_SOULFAIRY:
  case SP_SOULREVOLVE:
  case SP_KAUTE:
  case KG_KAGEMUSYA:
  case OB_ZANGETSU:
  case OB_OBOROGENSOU:
  case ECL_SNOWFLIP:
  case ECL_PEONYMAMY:
  case ECL_SADAGUI:
  case ECL_SEQUOIADUST:
  case SU_ARCLOUSEDASH:
  case SU_TUNABELLY:
  case SU_TUNAPARTY:
  case SU_BUNCHOFSHRIMP:
  case SU_FRESHSHRIMP:
  case WE_ONEFOREVER:
  case CD_REPARATIO:
  case CD_MEDIALE_VOTUM:
  case CD_ARGUTUS_VITA:
  case CD_ARGUTUS_TELUM:
  case CD_PRESENS_ACIES:
  case CD_DILECTIO_HEAL:
  case CD_RELIGIO:
  case CD_BENEDICTUM:
  case MT_A_MACHINE:
  case MT_D_MACHINE:
  case BO_ADVANCE_PROTECTION:
  case EM_SPELL_ENCHANTING:
  case EM_INCREASING_ACTIVITY:
  case MH_PAIN_KILLER:
  case MER_REGAIN:
  case MER_TENDER:
  case MER_BENEDICTION:
  case MER_RECUPERATE:
  case MER_MENTALCURE:
  case MER_COMPRESS:
  case MER_KYRIE:
  case MER_BLESSING:
  case MER_INCAGI:
    return true;
  default:
    return CSkillActionMgr::GetInstance()->IsSupportAction(skillId);
  }
}

// OFFSET: 0x0077B4A0
// FUNCTION: fn_08413
bool CSkillActionMgr::IsSupportAction(int skillId) {
  auto it = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(it != this->m_skillActions.end() ? it->second() : nullptr);
  return action ? action->Slot00() : false;
}

// OFFSET: 0x007737C0
// FUNCTION: fn_08316
const char* CSkillInfo::IdName(LuaScript& script) const {
  return CSkillInfo::IdName(script, static_cast<int>(this->id));
}

// OFFSET: 0x00773870
// FUNCTION: fn_08317
const char* CSkillInfo::IdName(LuaScript& script, int skillId) {
  const char* result = nullptr;
  return script.Call("GetSkillIdName", "d>s", skillId, &result) ? result : "Zero Skill";
}

// OFFSET: 0x00774500
// FUNCTION: fn_08323
bool CSkillInfo::UsesLevel(LuaScript& script, int skillId) {
  bool result = false;
  return script.Call("IsLevelUseSkill", "d>b", skillId, &result) ? result : false;
}

// OFFSET: 0x007745B0
// FUNCTION: fn_08324
bool ShowsCastEffect(int skillId) {
  switch (skillId) {
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
  case AL_WARP:
  case AL_INCAGI:
  case AL_DECAGI:
  case AL_CRUCIS:
  case AL_ANGELUS:
  case PR_TURNUNDEAD:
  case WZ_WATERBALL:
  case NPC_DARKSTRIKE:
  case CASH_INCAGI:
  case NPC_DRAGONBREATH:
  case RK_DRAGONBREATH:
  case ALL_ODINS_RECALL:
  case RETURN_TO_ELDICASTES:
  case ALL_GUARDIAN_RECALL:
  case RK_DRAGONBREATH_WATER:
      return false;
  default:
    return true;
  }
}

// OFFSET: 0x00774730
// FUNCTION: fn_08326
void CEfstAction_ChangeSize::Slot00(int, int value, int, int, CRenderObject* actor, bool) {
  if (!actor) {
    return;
  }

  const std::uint32_t aid = actor->render.accountId;
  CEffect* effect = g_effectMgr->Find(static_cast<int>(aid), 2374);
  if (!effect) {
    effect = g_effectMgr->Spawn(static_cast<int>(aid), 2374, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (!effect) {
      return;
    }
  }

  *reinterpret_cast<float*>(&effect->unknown_0048) = static_cast<float>(value) * 0.01f;
}

// OFFSET: 0x007760D0
// FUNCTION: fn_08344
void CSkillAction_CD_DIVINUS_FLOS::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007747B0
// FUNCTION: fn_08327
void CEfstAction_ChangeSize::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2374);
  }
}

// OFFSET: 0x007747E0
// FUNCTION: fn_08328
void CEfstAction_ChangeSize::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774E20
// FUNCTION: fn_08331
void CEfstAction_GenderChange::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774F30
// FUNCTION: fn_08334
void CEfstAction_ShowEffect::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774FB0
// FUNCTION: fn_08335
void CSkillAction_RA_UNLIMIT::Slot08(int level, CGameActor* source) {
  g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
}

// OFFSET: 0x00775250
// FUNCTION: fn_08337
void CEfstAction_UNLIMIT::Slot00(int, int, int, int, CRenderObject* actor, bool) {
  if (!actor) {
    return;
  }

  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (!g_serviceOptions.Get(229)) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007756D0
// FUNCTION: fn_08339
void CEfstAction_UNLIMIT::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007764E0
// FUNCTION: fn_08346
EfstActionMessage CEfstAction_SHADOW_WEAPON::Slot03() {
  return {LookupMessage(3766), 0xFFFF00};
}

// OFFSET: 0x00776510
// FUNCTION: fn_08347
EfstActionMessage CEfstAction_SHADOW_WEAPON::Slot04() {
  return {LookupMessage(3767), 0xFF};
}

// OFFSET: 0x00777910
// FUNCTION: fn_08359
void CEfstAction_DANCING_KNIFE::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00777DD0
// FUNCTION: fn_08364
void CSkillAction_SHC_SHADOW_EXCEED::Slot11(int, CGameActor* source, CGameActor*) {
  if (source) {
    g_effectMgr->Spawn(static_cast<int>(source->render.accountId), 2467, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777E80
// FUNCTION: fn_08366
void CEfstAction_SHADOW_EXCEED::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  while (effectActor->state.Attachments().HasId(1004)) {
    actor->SendMsg(nullptr, 109, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00777EE0
// FUNCTION: fn_08367
void CEfstAction_SHADOW_EXCEED::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, true);
}

// OFFSET: 0x00777F00
// FUNCTION: fn_08368
EfstActionMessage CEfstAction_SHADOW_EXCEED::Slot03() {
  return {LookupMessage(3762), 0xFFFF00};
}

// OFFSET: 0x00777F30
// FUNCTION: fn_08369
EfstActionMessage CEfstAction_SHADOW_EXCEED::Slot04() {
  return {LookupMessage(3763), 0xFF};
}

// OFFSET: 0x00778140
// FUNCTION: fn_08371
bool CSkillAction_ABC_CHASING_BREAK::Slot07() {
  return true;
}

// OFFSET: 0x00778580
// FUNCTION: fn_08375
bool CSkillAction_ABC_CHASING_SHOT::Slot07() {
  return true;
}

// OFFSET: 0x007794D0
// FUNCTION: fn_08385
bool CSkillAction_ABC_ABYSS_FLAME_ATK::Slot03(int argument) {
  return true;
}

// OFFSET: 0x0077ACC0
// FUNCTION: fn_08404
std::pair<bool, int> CSkillActionMgr::ActionSlot01(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (!action) {
    return {false, 8};
  }

  return {true, action->Slot01()};
}

// OFFSET: 0x0077AD90
// FUNCTION: fn_08405
vector3d CellWorldPosition(int column, int row) {
  if (!g_modeMgr.GetGameMode() || !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene ||
      !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }

  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Grid().CellOrigin(column, row, x, z);
  y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Grid().Height(x, z);
  return vector3d{x + 2.5f, y + 0.0f, z + 2.5f};
}

// OFFSET: 0x0077AEA0
// FUNCTION: fn_08406
EfstActionMessage CSkillActionMgr::EfstSlot03(int efstId) {
  const auto found = this->m_efstActions.find(efstId);
  std::unique_ptr<CEfstAction> action(found != this->m_efstActions.end() ? found->second() : nullptr);
  if (!action) {
    return {nullptr, 0xFFFF00};
  }

  return action->Slot03();
}

// OFFSET: 0x0077AF80
// FUNCTION: fn_08407
EfstActionMessage CSkillActionMgr::EfstSlot04(int efstId) {
  const auto found = this->m_efstActions.find(efstId);
  std::unique_ptr<CEfstAction> action(found != this->m_efstActions.end() ? found->second() : nullptr);
  if (!action) {
    return {nullptr, 0xFF};
  }

  return action->Slot04();
}

// OFFSET: 0x0077B060
// FUNCTION: fn_08408
int CSkillActionMgr::ActionSlot14(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot14() : 0;
}

// OFFSET: 0x0077B210
// FUNCTION: fn_08409
std::pair<bool, int> CSkillActionMgr::ActionSlot02(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (!action) {
    return {false, 7};
  }

  return {true, action->Slot02()};
}

// OFFSET: 0x0077B2E0
// FUNCTION: fn_08410
bool CSkillActionMgr::ActionSlot03(int skillId, int argument) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot03(argument) : false;
}

// OFFSET: 0x0077B3A0
// FUNCTION: fn_08411
bool CSkillActionMgr::ActionSlot04(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot04() : false;
}

// OFFSET: 0x0077B460
// FUNCTION: fn_08412
bool CSkillActionMgr::HasEfstAction(int efstId) {
  return this->m_efstActions.find(efstId) != this->m_efstActions.end();
}

// OFFSET: 0x00774FF0
// FUNCTION: fn_08336
void CSkillAction_RA_UNLIMIT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00775730
// FUNCTION: fn_08340
void CSkillAction_AG_ENERGY_CONVERSION::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00775BC0
// FUNCTION: fn_08342
void CSkillAction_CD_DIVINUS_FLOS::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00775E60
// FUNCTION: fn_08343
void CSkillAction_CD_DIVINUS_FLOS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776260
// FUNCTION: fn_08345
void CSkillAction_SHC_ENCHANTING_SHADOW::Slot08(int level, CGameActor* source) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776550
// FUNCTION: fn_08349
void CSkillAction_SHC_IMPACT_CRATER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007767C0
// FUNCTION: fn_08350
void CSkillAction_SHC_IMPACT_CRATER::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776950
// FUNCTION: fn_08352
void CSkillAction_SHC_CROSS_SLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776AC0
// FUNCTION: fn_08353
void CSkillAction_SHC_CROSS_SLASH::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776C40
// FUNCTION: fn_08355
void CSkillAction_SHC_DANCING_KNIFE::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00776EF0
// FUNCTION: fn_08356
void CSkillAction_SHC_DANCING_KNIFE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777C40
// FUNCTION: fn_08363
void CSkillAction_SHC_SHADOW_EXCEED::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shadow_exceed_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007783F0
// FUNCTION: fn_08373
void CSkillAction_ABC_CHASING_BREAK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778150
// FUNCTION: fn_08372
void CSkillAction_ABC_CHASING_BREAK::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(2416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  for (int angle = 0; angle < 360; angle += 45) {
    CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot17(static_cast<float>(angle));
      effect->Slot16(std::max(0, std::rand() % 150));
      effect->Slot22(0.15f);
      effect->Slot32();
    }
  }
}

// OFFSET: 0x00778590
// FUNCTION: fn_08376
void CSkillAction_ABC_CHASING_SHOT::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00778830
// FUNCTION: fn_08377
void CSkillAction_ABC_CHASING_SHOT::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(2416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  for (int angle = 0; angle < 360; angle += 45) {
    CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_single"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->Slot17(static_cast<float>(angle));
      effect->Slot22(0.15f);
      effect->Slot32();
    }
  }
}

// OFFSET: 0x00778AD0
// FUNCTION: fn_08378
void CSkillAction_ABC_CHASING_SHOT::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hitsub"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778C50
// FUNCTION: fn_08379
void CSkillAction_ABC_CHASING_SHOT::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778DD0
// FUNCTION: fn_08381
void CSkillAction_ABC_HIT_AND_SLIDING::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hit_and_sliding_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778F50
// FUNCTION: fn_08382
void CSkillAction_ABC_ABYSS_FLAME::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007791F0
// FUNCTION: fn_08383
void CSkillAction_ABC_ABYSS_FLAME::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779360
// FUNCTION: fn_08384
void CSkillAction_ABC_ABYSS_FLAME::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007794E0
// FUNCTION: fn_08386
void CSkillAction_ABC_ABYSS_FLAME_ATK::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_target"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779660
// FUNCTION: fn_08387
void CSkillAction_ABC_ABYSS_FLAME_ATK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779870
// FUNCTION: fn_08388
void CSkillAction_EM_PSYCHIC_STREAM::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x00779A00
// FUNCTION: fn_08389
void CSkillAction_EM_PSYCHIC_STREAM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779C80
// FUNCTION: fn_08390
void CSkillAction_EM_PSYCHIC_STREAM::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779E30
// FUNCTION: fn_08392
void CSkillAction_IQ_BLAZING_FLAME_BLAST::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x0077A2A0
// FUNCTION: fn_08394
void CSkillAction_IQ_BLAZING_FLAME_BLAST::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007754D0
// FUNCTION: fn_08338
void CEfstAction_UNLIMIT::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop_bottom"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop"));
}

// OFFSET: 0x00777520
// FUNCTION: fn_08358
void CEfstAction_DANCING_KNIFE::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_begin"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_loop"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_begin"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_loop"));
}

// OFFSET: 0x00774840
// FUNCTION: fn_08329
void CEfstAction_GenderChange::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  if (CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot22(1.5f);
  }
  if (CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot22(1.5f);
  }
}

// OFFSET: 0x00774AF0
// FUNCTION: fn_08330
void CEfstAction_GenderChange::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"));
  if (CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot22(1.5f);
  }
}

// OFFSET: 0x00777070
// FUNCTION: fn_08357
void CEfstAction_DANCING_KNIFE::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_begin"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_begin"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x007759F0
// FUNCTION: fn_08341
void CSkillAction_AG_ENERGY_CONVERSION::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  source->LaunchEffect(2427, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00777B00
// FUNCTION: fn_08362
void CSkillAction_SHC_SHADOW_STAB::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  CEZ2STREffectMgr* effects = CEZ2STREffectMgr::GetInstance();
  int variant = std::rand() % 2;
  if (variant < 1) {
    variant = 1;
  }
  g_effectMgr->Spawn(source->render.accountId, effects->EffectId(std::format("shadow_stab_hit{}", variant)), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00777E20
// FUNCTION: fn_08365
void CEfstAction_SHADOW_EXCEED::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor || !argument6) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  if (!effectActor->state.Attachments().HasId(1004)) {
    effectActor->LaunchEffect(1004, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x0077A0F0
// FUNCTION: fn_08393
void CSkillAction_IQ_BLAZING_FLAME_BLAST::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const float scale = source->unknown_0378 * source->render.speed * 16.0f;
  if (CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->Slot16(static_cast<int>(scale));
  }
}

// OFFSET: 0x0077B560
// FUNCTION: fn_08414
bool CSkillActionMgr::ActionSlot07(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot07() : false;
}

// OFFSET: 0x0077B620
// FUNCTION: fn_08415
bool CSkillActionMgr::ActionSlot06(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot06() : false;
}

// OFFSET: 0x0077B6E0
// FUNCTION: fn_08416
bool CSkillActionMgr::ActionSlot13(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot13() : false;
}

// OFFSET: 0x0077B7A0
// FUNCTION: fn_08417
bool CSkillActionMgr::ActionSlot05(int skillId) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  return action ? action->Slot05() : false;
}

// OFFSET: 0x0077B860
// FUNCTION: fn_08418
bool CSkillActionMgr::ActionSlot16(int skillId, int level, CGameActor* source, CGameActor* target) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot16(level, source, target);
  }

  return action != nullptr;
}

// OFFSET: 0x0077B920
// FUNCTION: fn_08419
bool CSkillActionMgr::EfstSlot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  const auto found = this->m_efstActions.find(efstId);
  std::unique_ptr<CEfstAction> action(found != this->m_efstActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot00(efstId, value, argument3, argument4, actor, argument6);
  }

  return action != nullptr;
}

// OFFSET: 0x0077B9F0
// FUNCTION: fn_08420
bool CSkillActionMgr::ActionSlot08(int skillId, int level, CGameActor* source) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot08(level, source);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BAB0
// FUNCTION: fn_08421
bool CSkillActionMgr::ActionSlot09(int skillId, int argument1, int argument2, int argument3) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot09(argument1, argument2, argument3);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BB70
// FUNCTION: fn_08422
bool CSkillActionMgr::ActionSlot10(int skillId, int level, CGameActor* source, short column, short row) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    const vector3d position = CellWorldPosition(column, row);
    action->Slot10(level, source, std::bit_cast<int>(position.x), std::bit_cast<int>(position.y), std::bit_cast<int>(position.z));
  }

  return action != nullptr;
}

// OFFSET: 0x0077BC60
// FUNCTION: fn_08423
bool CSkillActionMgr::EfstSlot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  const auto found = this->m_efstActions.find(efstId);
  std::unique_ptr<CEfstAction> action(found != this->m_efstActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot01(efstId, argument2, argument3, argument4, actor);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BD20
// FUNCTION: fn_08424
bool CSkillActionMgr::ActionSlot12(int skillId, int column, int row, int argument2, int argument3) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot12(CellWorldPosition(column, row), argument2, argument3);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BE10
// FUNCTION: fn_08425
bool CSkillActionMgr::ActionSlot15(int skillId, int level, CGameActor* source, CGameActor* target) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot15(level, source, target);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BED0
// FUNCTION: fn_08426
bool CSkillActionMgr::EfstSlot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  const auto found = this->m_efstActions.find(efstId);
  std::unique_ptr<CEfstAction> action(found != this->m_efstActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot02(efstId, value, argument3, argument4, actor);
  }

  return action != nullptr;
}

// OFFSET: 0x0077BF90
// FUNCTION: fn_08427
bool CSkillActionMgr::ObjectSlot01(int skillId, CRenderObject* object) {
  const auto found = this->m_skillObjectActions.find(skillId);
  std::unique_ptr<CSkillObjectAction> action(found != this->m_skillObjectActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot01(object);
  }

  return action != nullptr;
}

// OFFSET: 0x0077C050
// FUNCTION: fn_08428
bool CSkillActionMgr::ObjectSlot00(int skillId, int argument, CRenderObject* object, CGameActor* source) {
  const auto found = this->m_skillObjectActions.find(skillId);
  std::unique_ptr<CSkillObjectAction> action(found != this->m_skillObjectActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot00(argument, object, source);
  }

  return action != nullptr;
}

// OFFSET: 0x0077C110
// FUNCTION: fn_08429
bool CSkillActionMgr::ActionSlot11(int skillId, int level, CGameActor* source, CGameActor* target) {
  const auto found = this->m_skillActions.find(skillId);
  std::unique_ptr<CSkillAction> action(found != this->m_skillActions.end() ? found->second() : nullptr);
  if (action) {
    action->Slot11(level, source, target);
  }

  return action != nullptr;
}

// OFFSET: 0x0077C1D0
// FUNCTION: fn_08430
void CSkillActionMgr::RegisterEfstAction(int efstId, std::function<CEfstAction*()> factory) {
  this->m_efstActions.insert({efstId, factory});
}

// OFFSET: 0x0077C360
// FUNCTION: fn_08431
void CSkillActionMgr::RegisterSkillObjectAction(int skillId, std::function<CSkillObjectAction*()> factory) {
  this->m_skillObjectActions.insert({skillId, factory});
}

// OFFSET: 0x0077C4F0
// FUNCTION: fn_08432
void CSkillActionMgr::RegisterSkillAction(int skillId, std::function<CSkillAction*()> factory) {
  this->m_skillActions.insert({skillId, factory});
}

// OFFSET: 0x0077CCC0
// FUNCTION: fn_08437
void CSkillAction_BO_MAYHEMIC_THORNS::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2228, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2229, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x0077CD90
// FUNCTION: fn_08438
void CSkillAction_BO_MAYHEMIC_THORNS::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D020
// FUNCTION: fn_08440
void CSkillAction_BO_EXPLOSIVE_POWDER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("explosive_powder"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D190
// FUNCTION: fn_08441
bool CSkillAction_BO_EXPLOSIVE_POWDER::Slot05() {
  return true;
}

// OFFSET: 0x0077D1A0
// FUNCTION: fn_08442
void CSkillAction_BO_MYSTERY_POWDER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D310
// FUNCTION: fn_08443
void CSkillAction_BO_MYSTERY_POWDER::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D490
// FUNCTION: fn_08444
void CEfstAction_MYSTERY_POWDER::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D720
// FUNCTION: fn_08445
void CEfstAction_MYSTERY_POWDER::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop_bottom"));
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop"));
  }
}

// OFFSET: 0x0077D940
// FUNCTION: fn_08446
void CEfstAction_MYSTERY_POWDER::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077D960
// FUNCTION: fn_08447
void CSkillAction_BO_DUST_EXPLOSION::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  for (int delay = 0; delay < 600; delay += 200) {
    const int quadrant = std::max(std::rand() % 4, 0);
    if (CEffect* effect = SpawnDustExplosion(quadrant, target->render.position)) {
      effect->Slot16(delay);
    }
    if (CEffect* effect = SpawnDustExplosion((quadrant + 2) % 4, target->render.position)) {
      effect->Slot16(delay);
    }
  }
}

// OFFSET: 0x0077D9E0
// FUNCTION: fn_08448
void CSkillAction_BO_DUST_EXPLOSION::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077DC70
// FUNCTION: fn_08449
CEffect* SpawnDustExplosion(int quadrant, const vector3d& position) {
  int distance = std::rand() % 20;
  if (distance < 10) {
    distance = 10;
  }

  const int start = quadrant * 90;
  const int end = start + 90;
  int angle = 0;
  if (end != 0) {
    angle = std::max(std::rand() % end, start);
  }
  const float offsetX = ClientSin((static_cast<float>(angle) / 180.0f) * 3.141592f) * static_cast<float>(distance);

  distance = std::rand() % 20;
  if (distance < 10) {
    distance = 10;
  }

  angle = 0;
  if (end != 0) {
    angle = std::max(std::rand() % end, start);
  }
  const float offsetZ = ClientCos((static_cast<float>(angle) / 180.0f) * 3.141592f) * static_cast<float>(distance);

  const float x = offsetX + position.x;
  const float y = position.y + 0.0f;
  const float z = offsetZ + position.z;
  return g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion"), x, y, z, 0, -1, 0, 0);
}

// OFFSET: 0x0077DF70
// FUNCTION: fn_08450
void CSkillAction_DK_VIGOR::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E0D0
// FUNCTION: fn_08451
void CEfstAction_VIGOR::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E230
// FUNCTION: fn_08452
void CEfstAction_VIGOR::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_buff"));
  }
}

// OFFSET: 0x0077E350
// FUNCTION: fn_08453
void CEfstAction_VIGOR::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077E370
// FUNCTION: fn_08454
EfstActionMessage CEfstAction_VIGOR::Slot04() {
  return {LookupMessage(3747), 0xFF};
}

// OFFSET: 0x0077E3B0
// FUNCTION: fn_08456
void CSkillAction_DK_SERVANTWEAPON_ATK::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_down"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E530
// FUNCTION: fn_08457
void CSkillAction_DK_SERVANTWEAPON_ATK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E7B0
// FUNCTION: fn_08458
void CEfstAction_SERVANTWEAPON::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2460);
  }
}

// OFFSET: 0x0077E7E0
// FUNCTION: fn_08459
EfstActionMessage CEfstAction_SERVANTWEAPON::Slot04() {
  return {LookupMessage(3745), 0xFF};
}

// OFFSET: 0x0077EAD0
// FUNCTION: fn_08462
void CEfstAction_SERVANT_SIGN::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077ED40
// FUNCTION: fn_08463
void CEfstAction_SERVANT_SIGN::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_bottom"));
    g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"));
  }
}

// OFFSET: 0x0077EF40
// FUNCTION: fn_08464
void CEfstAction_SERVANT_SIGN::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077EF70
// FUNCTION: fn_08466
void CSkillAction_DK_SERVANT_W_PHANTOM::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x0077EFA0
// FUNCTION: fn_08467
void CSkillAction_DK_SERVANT_W_PHANTOM::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, 2460);
  }
}

// OFFSET: 0x0077EFD0
// FUNCTION: fn_08468
void CSkillAction_DK_SERVANT_W_PHANTOM::Slot15(int caster, CGameActor* target, CGameActor* argument3) {
  if (!target) {
    return;
  }

  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  const int duration = static_cast<int>(source ? source->render.speed * 100.0f + 100.0f : 500.0f);
  CEffect* sign = g_effectMgr->Find(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"));
  const vector3d offset = {0.0f, 0.0f, 0.0f};
  CEffect* effect;
  if (sign) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_down"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit_bottom"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
  } else {
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub_bottom"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
    if (effect) {
      effect->Slot16(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
  }

  if (effect) {
    effect->Slot16(duration);
  }
}

// OFFSET: 0x0077F600
// FUNCTION: fn_08469
void CSkillAction_DK_SERVANT_W_DEMOL::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, 2460);
  }
}

// OFFSET: 0x0077F630
// FUNCTION: fn_08470
void CSkillAction_DK_SERVANT_W_DEMOL::Slot15(int caster, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  if (source) {
    const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_collect"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot16(duration);
    }
  }

  if (target) {
    const int side = std::max(std::rand() % 2, 0);
    if (side == 0) {
      g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_leftdown"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    } else {
      g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_rightdown"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x0077F9B0
// FUNCTION: fn_08471
void CSkillAction_DK_SERVANT_W_DEMOL::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077FB40
// FUNCTION: fn_08473
void CSkillAction_DK_STORMSLASH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const float scale = source->render.speed * 50.0f + 30.0f;
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("stormslash_{}", level)), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (!effect) {
    return;
  }

  effect->Slot16(static_cast<int>(scale));
  for (int frame = 0; level > 0; --level, frame += 15) {
    const int variant = std::rand() % 4 + 1;
    effect->AddEvent(0, std::format("effect\\DK_STORMSLASH{}.wav", variant).c_str(), frame, 0);
  }
}

// OFFSET: 0x0077FD50
// FUNCTION: fn_08475
bool CSkillAction_DK_MADNESS_CRUSHER::Slot04() {
  return true;
}

// OFFSET: 0x0077FD60
// FUNCTION: fn_08476
void CSkillAction_DK_MADNESS_CRUSHER::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x0077FD90
// FUNCTION: fn_08477
void CSkillAction_DK_MADNESS_CRUSHER::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780020
// FUNCTION: fn_08479
void CSkillAction_DK_DRAGONIC_PIERCE::Slot08(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->Slot14(level);
    }
  }
}

// OFFSET: 0x007805A0
// FUNCTION: fn_08481
void CSkillAction_DK_DRAGONIC_PIERCE::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780840
// FUNCTION: fn_08483
void CSkillAction_DK_DRAGONIC_BREATH::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00780870
// FUNCTION: fn_08484
void CSkillAction_DK_DRAGONIC_BREATH::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780AF0
// FUNCTION: fn_08485
void CSkillAction_DK_DRAGONIC_BREATH::Slot15(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  for (int i = 0; i < 4; ++i) {
    const vector3d position = source->render.position;
    CEffect* effect = g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath_hit"), position.x, position.y, position.z, 0, -1, 0, 0);
    if (!effect) {
      break;
    }

    const int first = std::rand();
    const int second = std::rand();
    const int jitterX = std::max(second % 10, 0);
    const int jitterY = std::max(first % 10, 0);
    effect->position[0] = static_cast<float>((i % 2) * 10 - jitterX) + source->render.position.x;
    effect->position[2] = source->render.position.z + 0.0f;
    effect->position[1] = static_cast<float>((i % 4) / 2 * 10 - jitterY) + source->render.position.y;
  }
}

// OFFSET: 0x00780DB0
// FUNCTION: fn_08487
void CSkillAction_DK_CHARGINGPIERCE::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00780DE0
// FUNCTION: fn_08488
void CSkillAction_DK_CHARGINGPIERCE::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781070
// FUNCTION: fn_08489
EfstActionMessage CEfstAction_CHARGINGPIERCE::Slot04() {
  return {LookupMessage(3746), 0xFF};
}

// OFFSET: 0x007812F0
// FUNCTION: fn_08491
void CEfstAction_CHARGINGPIERCE_COUNT::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2461);
  }
}

// OFFSET: 0x00781330
// FUNCTION: fn_08493
void CSkillAction_DK_HACKANDSLASHER::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007815A0
// FUNCTION: fn_08494
void CSkillAction_DK_HACKANDSLASHER::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781710
// FUNCTION: fn_08495
int CSkillAction_DK_HACKANDSLASHER_ATK::Slot02() {
  return 0;
}

// OFFSET: 0x00781720
// FUNCTION: fn_08496
void CSkillAction_DK_HACKANDSLASHER_ATK::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781B50
// FUNCTION: fn_08497
void CSkillAction_NPC_BLOCK_EXPLOSION::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00781B80
// FUNCTION: fn_08498
void CSkillAction_NPC_BLOCK_EXPLOSION::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 2406, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, 2407, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781C30
// FUNCTION: fn_08499
void CSkillAction_NPC_FROST_FIELD::Slot08(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x00781C60
// FUNCTION: fn_08500
void CSkillAction_NPC_FROST_FIELD::Slot11(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 2408, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 2409, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\NPC_FROST_FIELD.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, 1283, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781DB0
// FUNCTION: fn_08501
void CSkillAction_NPC_FROST_FIELD::Slot16(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 1310, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781E00
// FUNCTION: fn_08502
void CEfstAction_FROST_STORM::Slot00(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, 2403, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781E50
// FUNCTION: fn_08503
void CEfstAction_FROST_STORM::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2403);
  }
}

// OFFSET: 0x00781E80
// FUNCTION: fn_08504
void CEfstAction_FROST_STORM::Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->Slot00(efstId, value, 0, 0, actor, false);
}
