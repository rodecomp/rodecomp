#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EffectResourceSwitcher.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/ChargingPierceEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/EZ2STREffectEx.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/ShowEffect.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <bit>
#include <cstdlib>
#include <format>
#include <memory>
#include <random>
#include <vector>

class CEfstAction_ChangeSize : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_CD_DIVINUS_FLOS : public CSkillAction {
public:
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GenderChange : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_ShowEffect : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_RA_UNLIMIT : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_UNLIMIT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool flag) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_WEAPON : public CEfstAction {
public:
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CEfstAction_DANCING_KNIFE : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_SHC_SHADOW_STAB : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_SHADOW_EXCEED : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SHADOW_EXCEED : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstBeginMessage() override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_ABC_CHASING_BREAK : public CSkillAction {
public:
  bool SuppressAttackSkillArrow() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_CHASING_SHOT : public CSkillAction {
public:
  bool SuppressAttackSkillArrow() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_ABYSS_FLAME_ATK : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_AG_ENERGY_CONVERSION : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_ENCHANTING_SHADOW : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillAction_SHC_IMPACT_CRATER : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_CROSS_SLASH : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_SHC_DANCING_KNIFE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_HIT_AND_SLIDING : public CSkillAction {
public:
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_ABC_ABYSS_FLAME : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_EM_PSYCHIC_STREAM : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_IQ_BLAZING_FLAME_BLAST : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_MAYHEMIC_THORNS : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_EXPLOSIVE_POWDER : public CSkillAction {
public:
  bool KeepSkillAttackSpeed() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_MYSTERY_POWDER : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_MYSTERY_POWDER : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_BO_DUST_EXPLOSION : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_VIGOR : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_VIGOR : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CSkillAction_DK_SERVANTWEAPON_ATK : public CSkillAction {
public:
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_SERVANTWEAPON : public CEfstAction {
public:
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage GetEfstEndMessage() override;
};

class CEfstAction_SERVANT_SIGN : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_DK_SERVANT_W_PHANTOM : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_SERVANT_W_DEMOL : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_STORMSLASH : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_MADNESS_CRUSHER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  bool IsSkillWithoutMotion() override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_DRAGONIC_PIERCE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_DRAGONIC_BREATH : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_CHARGINGPIERCE : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_CHARGINGPIERCE : public CEfstAction {
public:
  EfstActionMessage GetEfstEndMessage() override;
};

class CEfstAction_CHARGINGPIERCE_COUNT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_DK_HACKANDSLASHER : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_HACKANDSLASHER_ATK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_BLOCK_EXPLOSION : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_FROST_FIELD : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_FROST_STORM : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

struct DustExplosionSpawner {
  __declspec(noinline) CEffect* operator()(int quadrant, const vector3d& position) const;
};

// INLINED
template <class Action>
__forceinline Action* CreateAction(const std::map<int, std::function<Action*()>>& factories, int id) {
  const auto found = factories.find(id);
  return factories.end() != found ? found->second() : nullptr;
}

// GLOBAL: gl_24640
CSkillActionMgr* g_skillActionMgr;

// INLINED
CSkillActionMgr::~CSkillActionMgr() = default;

// OFFSET: 0x005B06F0
// FUNCTION: fn_03339
__forceinline CSkillActionMgr* CSkillActionMgr::GetInstance() {
  if (!g_skillActionMgr) {
    g_skillActionMgr = new (std::nothrow) CSkillActionMgr;
  }

  return g_skillActionMgr;
}

// OFFSET: 0x00773480
// FUNCTION: fn_08311
CSkillInfo::CSkillInfo() : available(1), id(0), level(0), name(nullptr), maxLevel(0), learnedLevel(0), apCost(0) {
}

// OFFSET: 0x00773400
// FUNCTION: fn_08310
CSkillInfo::CSkillInfo(int) : available(0), id(0), level(0), name("Unknown-Skill"), maxLevel(0), learnedLevel(0), apCost(0) {
}

// OFFSET: 0x0076FD70
// FUNCTION: fn_08254
CSkillInfo::CSkillInfo(const CSkillInfo& other)
    : available(other.available), id(other.id), type(other.type), level(other.level), spCost(other.spCost),
      upgradable(other.upgradable), range(other.range), name(other.name), treePosition(other.treePosition),
      maxLevel(other.maxLevel), canPlanLevel(other.canPlanLevel), learnedLevel(other.learnedLevel), apCost(other.apCost),
      entries(other.entries) {
}

// OFFSET: 0x009149F0
// FUNCTION: fn_12543
CSkillInfo& CSkillInfo::operator=(const CSkillInfo& other) {
  this->available = other.available;
  this->id = other.id;
  this->type = other.type;
  this->level = other.level;
  this->spCost = other.spCost;
  this->upgradable = other.upgradable;
  this->range = other.range;
  this->name = other.name;
  this->treePosition = other.treePosition;
  this->maxLevel = other.maxLevel;
  this->canPlanLevel = other.canPlanLevel;
  this->learnedLevel = other.learnedLevel;
  this->apCost = other.apCost;

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

  const auto requested = slot->value;
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
const char* CSkillInfo::DisplayName(LuaScript&) const {
  return CSkillInfo::DisplayName(*g_session.m_hotkeyScript, static_cast<int>(this->id));
}

// OFFSET: 0x00773920
// FUNCTION: fn_08318
const char* CSkillInfo::DisplayName(LuaScript&, int skillId) {
  switch (skillId) {
  case SU_CN_METEOR2:
    skillId = SU_CN_METEOR;
    break;
  case SU_LUNATICCARROTBEAT2:
    skillId = SU_LUNATICCARROTBEAT;
    break;
  }

  const char* result = nullptr;
  return g_session.m_hotkeyScript->Call("GetSkillName", "d>s", skillId, &result) ? result : "Unknown-Skill";
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
  switch (skillId) {
  case KR_CHOP_CHOP:
  case AT_FURIOS_STORM:
    return true;
  default:
    return false;
  }
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
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->IsSupport();
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
void CEfstAction_ChangeSize::OnEfstBegin(int, int value, int, int, CRenderObject* actor, bool) {
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

  *reinterpret_cast<float*>(&effect->scaleParameter) = static_cast<float>(value) * 0.01f;
}

// OFFSET: 0x007760D0
// FUNCTION: fn_08344
void CSkillAction_CD_DIVINUS_FLOS::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007747B0
// FUNCTION: fn_08327
void CEfstAction_ChangeSize::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2374);
  }
}

// OFFSET: 0x007747E0
// FUNCTION: fn_08328
void CEfstAction_ChangeSize::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774E20
// FUNCTION: fn_08331
void CEfstAction_GenderChange::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774E60
// FUNCTION: fn_08332
void CEfstAction_ShowEffect::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  const std::uint32_t aid = actor->render.accountId;
  CEffect* effect = g_effectMgr->Find(static_cast<int>(aid), 2375);
  if (!effect) {
    effect = g_effectMgr->Spawn(static_cast<int>(aid), 2375, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (!effect) {
      return;
    }
  }

  static_cast<CShowEffect*>(effect)->Add(efstId, value);
}

// OFFSET: 0x00774ED0
// FUNCTION: fn_08333
void CEfstAction_ShowEffect::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  const std::uint32_t aid = actor->render.accountId;
  if (CShowEffect* effect = static_cast<CShowEffect*>(g_effectMgr->Find(static_cast<int>(aid), 2375))) {
    effect->Remove(efstId);
    if (static_cast<int>(effect->Count()) <= 0) {
      g_effectMgr->Retire(static_cast<int>(aid), 2375);
    }
  }
}

// OFFSET: 0x00774F30
// FUNCTION: fn_08334
void CEfstAction_ShowEffect::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00774FB0
// FUNCTION: fn_08335
void CSkillAction_RA_UNLIMIT::OnSkillCast(int level, CGameActor* source) {
  g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
}

// OFFSET: 0x00775250
// FUNCTION: fn_08337
void CEfstAction_UNLIMIT::OnEfstBegin(int, int, int, int, CRenderObject* actor, bool) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (!g_serviceOptions.Get(229)) {
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007756D0
// FUNCTION: fn_08339
void CEfstAction_UNLIMIT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007764E0
// FUNCTION: fn_08346
EfstActionMessage CEfstAction_SHADOW_WEAPON::GetEfstBeginMessage() {
  return {LookupMessage(MSI_HUNTING_QUEST_INFO_OFF), 0xFFFF00};
}

// OFFSET: 0x00776510
// FUNCTION: fn_08347
EfstActionMessage CEfstAction_SHADOW_WEAPON::GetEfstEndMessage() {
  return {LookupMessage(MSI_TARGET_AP_MAX), 0xFF};
}

// OFFSET: 0x00777910
// FUNCTION: fn_08359
void CEfstAction_DANCING_KNIFE::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00777DD0
// FUNCTION: fn_08364
void CSkillAction_SHC_SHADOW_EXCEED::OnSkillEffect(int, CGameActor* source, CGameActor*) {
  if (source) {
    g_effectMgr->Spawn(static_cast<int>(source->render.accountId), 2467, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777E80
// FUNCTION: fn_08366
void CEfstAction_SHADOW_EXCEED::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  while (effectActor->HasId(1004)) {
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00777EE0
// FUNCTION: fn_08367
void CEfstAction_SHADOW_EXCEED::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, true);
}

// OFFSET: 0x00777F00
// FUNCTION: fn_08368
EfstActionMessage CEfstAction_SHADOW_EXCEED::GetEfstBeginMessage() {
  return {LookupMessage(MSI_SHC_POTENT_VENOM_OFF), 0xFFFF00};
}

// OFFSET: 0x00777F30
// FUNCTION: fn_08369
EfstActionMessage CEfstAction_SHADOW_EXCEED::GetEfstEndMessage() {
  return {LookupMessage(MSI_SHC_ENCHANTING_SHADOW_ON), 0xFF};
}

// OFFSET: 0x00778140
// FUNCTION: fn_08371
bool CSkillAction_ABC_CHASING_BREAK::SuppressAttackSkillArrow() {
  return true;
}

// OFFSET: 0x00778580
// FUNCTION: fn_08375
bool CSkillAction_ABC_CHASING_SHOT::SuppressAttackSkillArrow() {
  return true;
}

// OFFSET: 0x007794D0
// FUNCTION: fn_08385
bool CSkillAction_ABC_ABYSS_FLAME_ATK::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x0077ACC0
// FUNCTION: fn_08404
std::pair<bool, int> CSkillActionMgr::GetSkillCastMotion(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  return !action ? std::pair<bool, int>(false, 8) : std::pair<bool, int>(true, action->GetSkillCastMotion());
}

// OFFSET: 0x0077AD90
// FUNCTION: fn_08405
vector3d CellWorldPosition(int column, int row) {
  if (!g_modeMgr.GetGameMode() || !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene ||
      !static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }

  float z = 0.0f;
  float y = 0.0f;
  float x = 0.0f;
  static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->CellOrigin(column, row, x, z);
  y = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->terrain->Height(x, z);
  return vector3d{x + 2.5f, y + 0.0f, z + 2.5f};
}

// OFFSET: 0x0077AEA0
// FUNCTION: fn_08406
EfstActionMessage CSkillActionMgr::GetEfstBeginMessage(int efstId) {
  std::unique_ptr<CEfstAction> action(CreateAction(this->m_efstActions, efstId));
  if (!action) {
    return {nullptr, 0xFFFF00};
  }

  return action->GetEfstBeginMessage();
}

// OFFSET: 0x0077AF80
// FUNCTION: fn_08407
EfstActionMessage CSkillActionMgr::GetEfstEndMessage(int efstId) {
  std::unique_ptr<CEfstAction> action(CreateAction(this->m_efstActions, efstId));
  if (!action) {
    return {nullptr, 0xFF};
  }

  return action->GetEfstEndMessage();
}

// OFFSET: 0x0077B060
// FUNCTION: fn_08408
int CSkillActionMgr::GetSkillScopeType(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return 0;
  }

  return action->GetSkillScopeType();
}

// OFFSET: 0x0077B210
// FUNCTION: fn_08409
std::pair<bool, int> CSkillActionMgr::GetSkillAttackMotion(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  return !action ? std::pair<bool, int>(false, 7) : std::pair<bool, int>(true, action->GetSkillAttackMotion());
}

// OFFSET: 0x0077B2E0
// FUNCTION: fn_08410
bool CSkillActionMgr::KeepSkillAttackMotion(int skillId, int skillMessageType) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->KeepSkillAttackMotion(skillMessageType);
}

// OFFSET: 0x0077B3A0
// FUNCTION: fn_08411
bool CSkillActionMgr::IsSkillWithoutMotion(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->IsSkillWithoutMotion();
}

// OFFSET: 0x0077B460
// FUNCTION: fn_08412
bool CSkillActionMgr::HasEfstAction(int efstId) {
  return this->m_efstActions.end() != this->m_efstActions.find(efstId);
}

// OFFSET: 0x00774FF0
// FUNCTION: fn_08336
void CSkillAction_RA_UNLIMIT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00775730
// FUNCTION: fn_08340
void CSkillAction_AG_ENERGY_CONVERSION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00775BC0
// FUNCTION: fn_08342
void CSkillAction_CD_DIVINUS_FLOS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00775E60
// FUNCTION: fn_08343
void CSkillAction_CD_DIVINUS_FLOS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776260
// FUNCTION: fn_08345
void CSkillAction_SHC_ENCHANTING_SHADOW::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776550
// FUNCTION: fn_08349
void CSkillAction_SHC_IMPACT_CRATER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007767C0
// FUNCTION: fn_08350
void CSkillAction_SHC_IMPACT_CRATER::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776950
// FUNCTION: fn_08352
void CSkillAction_SHC_CROSS_SLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776AC0
// FUNCTION: fn_08353
void CSkillAction_SHC_CROSS_SLASH::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776C40
// FUNCTION: fn_08355
void CSkillAction_SHC_DANCING_KNIFE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00776EF0
// FUNCTION: fn_08356
void CSkillAction_SHC_DANCING_KNIFE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777C40
// FUNCTION: fn_08363
void CSkillAction_SHC_SHADOW_EXCEED::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shadow_exceed_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007783F0
// FUNCTION: fn_08373
void CSkillAction_ABC_CHASING_BREAK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778150
// FUNCTION: fn_08372
void CSkillAction_ABC_CHASING_BREAK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(2416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  for (int angle = 0; angle < 360; angle += 45) {
    CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->SetAngle(static_cast<float>(angle));
      effect->SetDelay(std::max(0, std::rand() % 150));
      effect->SetScale(0.15f);
      effect->EnableGroundRender();
    }
  }
}

// OFFSET: 0x00778590
// FUNCTION: fn_08376
void CSkillAction_ABC_CHASING_SHOT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00778830
// FUNCTION: fn_08377
void CSkillAction_ABC_CHASING_SHOT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(2416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  for (int angle = 0; angle < 360; angle += 45) {
    CEffect* effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_single"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    if (effect) {
      effect->SetAngle(static_cast<float>(angle));
      effect->SetScale(0.15f);
      effect->EnableGroundRender();
    }
  }
}

// OFFSET: 0x00778AD0
// FUNCTION: fn_08378
void CSkillAction_ABC_CHASING_SHOT::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hitsub"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778C50
// FUNCTION: fn_08379
void CSkillAction_ABC_CHASING_SHOT::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778DD0
// FUNCTION: fn_08381
void CSkillAction_ABC_HIT_AND_SLIDING::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hit_and_sliding_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778F50
// FUNCTION: fn_08382
void CSkillAction_ABC_ABYSS_FLAME::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007791F0
// FUNCTION: fn_08383
void CSkillAction_ABC_ABYSS_FLAME::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779360
// FUNCTION: fn_08384
void CSkillAction_ABC_ABYSS_FLAME::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007794E0
// FUNCTION: fn_08386
void CSkillAction_ABC_ABYSS_FLAME_ATK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_target"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779660
// FUNCTION: fn_08387
void CSkillAction_ABC_ABYSS_FLAME_ATK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779870
// FUNCTION: fn_08388
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00779A00
// FUNCTION: fn_08389
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779C80
// FUNCTION: fn_08390
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779E30
// FUNCTION: fn_08392
void CSkillAction_IQ_BLAZING_FLAME_BLAST::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x0077A2A0
// FUNCTION: fn_08394
void CSkillAction_IQ_BLAZING_FLAME_BLAST::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007754D0
// FUNCTION: fn_08338
void CEfstAction_UNLIMIT::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop_bottom"));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop"));
}

// OFFSET: 0x00777520
// FUNCTION: fn_08358
void CEfstAction_DANCING_KNIFE::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_begin"));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_loop"));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_begin"));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_loop"));
}

// OFFSET: 0x00774840
// FUNCTION: fn_08329
void CEfstAction_GenderChange::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetScale(1.5f);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetScale(1.5f);
  }
}

// OFFSET: 0x00774AF0
// FUNCTION: fn_08330
void CEfstAction_GenderChange::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"));
  g_effectMgr->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"));
  if (CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetScale(1.5f);
  }
}

// OFFSET: 0x00777070
// FUNCTION: fn_08357
void CEfstAction_DANCING_KNIFE::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_begin"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_begin"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x007759F0
// FUNCTION: fn_08341
void CSkillAction_AG_ENERGY_CONVERSION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  source->LaunchEffect(2427, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00777940
// FUNCTION: fn_08361
void CSkillAction_SHC_SHADOW_STAB::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shadow_stab"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x00777B00
// FUNCTION: fn_08362
void CSkillAction_SHC_SHADOW_STAB::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
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
void CEfstAction_SHADOW_EXCEED::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor || !argument6) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  if (!effectActor->HasId(1004)) {
    effectActor->LaunchEffect(1004, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x0077A0F0
// FUNCTION: fn_08393
void CSkillAction_IQ_BLAZING_FLAME_BLAST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x0077B560
// FUNCTION: fn_08414
bool CSkillActionMgr::SuppressAttackSkillArrow(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->SuppressAttackSkillArrow();
}

// OFFSET: 0x0077B620
// FUNCTION: fn_08415
bool CSkillActionMgr::SuppressSupportSkillArrow(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->SuppressSupportSkillArrow();
}

// OFFSET: 0x0077B6E0
// FUNCTION: fn_08416
bool CSkillActionMgr::UseGroundSkillLastFrame(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->UseGroundSkillLastFrame();
}

// OFFSET: 0x0077B7A0
// FUNCTION: fn_08417
bool CSkillActionMgr::KeepSkillAttackSpeed(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  return action->KeepSkillAttackSpeed();
}

// OFFSET: 0x0077B860
// FUNCTION: fn_08418
bool CSkillActionMgr::OnSkillDamage(int skillId, int level, CGameActor* source, CGameActor* target) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillDamage(level, source, target);
  return true;
}

// OFFSET: 0x0077B920
// FUNCTION: fn_08419
bool CSkillActionMgr::OnEfstBegin(int efstId, int value1, int value2, int value3, CRenderObject* actor, bool flag) {
  std::unique_ptr<CEfstAction> action(CreateAction(this->m_efstActions, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstBegin(efstId, value1, value2, value3, actor, flag);
  return true;
}

// OFFSET: 0x0077B9F0
// FUNCTION: fn_08420
bool CSkillActionMgr::OnSkillCast(int skillId, int level, CGameActor* source) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillCast(level, source);
  return true;
}

// OFFSET: 0x0077BAB0
// FUNCTION: fn_08421
bool CSkillActionMgr::OnTargetSkillCast(int skillId, int source, int target, int delay) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnTargetSkillCast(source, target, delay);
  return true;
}

// OFFSET: 0x0077BB70
// FUNCTION: fn_08422
bool CSkillActionMgr::OnGroundSkillCast(int skillId, int castDelay, CGameActor* target, short column, short row) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  const vector3d position = CellWorldPosition(column, row);
  action->OnGroundSkillCast(castDelay, target, std::bit_cast<int>(position.x), std::bit_cast<int>(position.y), std::bit_cast<int>(position.z));
  return true;
}

// OFFSET: 0x0077BC60
// FUNCTION: fn_08423
bool CSkillActionMgr::OnEfstEnd(int efstId, int value1, int value2, int value3, CRenderObject* actor) {
  std::unique_ptr<CEfstAction> action(CreateAction(this->m_efstActions, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstEnd(efstId, value1, value2, value3, actor);
  return true;
}

// OFFSET: 0x0077BD20
// FUNCTION: fn_08424
bool CSkillActionMgr::OnGroundSkillEffect(int skillId, int column, int row, int level, int actor) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnGroundSkillEffect(CellWorldPosition(column, row), level, actor);
  return true;
}

// OFFSET: 0x0077BE10
// FUNCTION: fn_08425
bool CSkillActionMgr::OnSkillHit(int skillId, int level, CGameActor* source, CGameActor* target) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillHit(level, source, target);
  return true;
}

// OFFSET: 0x0077BED0
// FUNCTION: fn_08426
bool CSkillActionMgr::OnEfstUpdate(int efstId, int value, int statusValue2, int statusValue3, CRenderObject* actor) {
  std::unique_ptr<CEfstAction> action(CreateAction(this->m_efstActions, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstUpdate(efstId, value, statusValue2, statusValue3, actor);
  return true;
}

// OFFSET: 0x0077BF90
// FUNCTION: fn_08427
bool CSkillActionMgr::OnSkillObjectRemove(int skillId, CRenderObject* object) {
  std::unique_ptr<CSkillObjectAction> action(CreateAction(this->m_skillObjectActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillObjectRemove(object);
  return true;
}

// OFFSET: 0x0077C050
// FUNCTION: fn_08428
bool CSkillActionMgr::OnSkillObjectCreate(int skillId, int level, CRenderObject* object, CGameActor* source) {
  std::unique_ptr<CSkillObjectAction> action(CreateAction(this->m_skillObjectActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillObjectCreate(level, object, source);
  return true;
}

// OFFSET: 0x0077C110
// FUNCTION: fn_08429
bool CSkillActionMgr::OnSkillEffect(int skillId, int level, CGameActor* source, CGameActor* target) {
  std::unique_ptr<CSkillAction> action(CreateAction(this->m_skillActions, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillEffect(level, source, target);
  return true;
}

// OFFSET: 0x0077C1D0
// FUNCTION: fn_08430
void CSkillActionMgr::RegisterEfstAction(int efstId, std::function<CEfstAction*()> factory) {
  this->m_efstActions.insert(std::make_pair(efstId, factory));
}

// OFFSET: 0x0077C360
// FUNCTION: fn_08431
void CSkillActionMgr::RegisterSkillObjectAction(int skillId, std::function<CSkillObjectAction*()> factory) {
  this->m_skillObjectActions.insert(std::make_pair(skillId, factory));
}

// OFFSET: 0x0077C4F0
// FUNCTION: fn_08432
void CSkillActionMgr::RegisterSkillAction(int skillId, std::function<CSkillAction*()> factory) {
  this->m_skillActions.insert(std::make_pair(skillId, factory));
}

// OFFSET: 0x0077CCC0
// FUNCTION: fn_08437
void CSkillAction_BO_MAYHEMIC_THORNS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2228, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, 2229, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x0077CD90
// FUNCTION: fn_08438
void CSkillAction_BO_MAYHEMIC_THORNS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D020
// FUNCTION: fn_08440
void CSkillAction_BO_EXPLOSIVE_POWDER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("explosive_powder"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D190
// FUNCTION: fn_08441
bool CSkillAction_BO_EXPLOSIVE_POWDER::KeepSkillAttackSpeed() {
  return true;
}

// OFFSET: 0x0077D1A0
// FUNCTION: fn_08442
void CSkillAction_BO_MYSTERY_POWDER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D310
// FUNCTION: fn_08443
void CSkillAction_BO_MYSTERY_POWDER::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D490
// FUNCTION: fn_08444
void CEfstAction_MYSTERY_POWDER::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D720
// FUNCTION: fn_08445
void CEfstAction_MYSTERY_POWDER::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop_bottom"));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop"));
  }
}

// OFFSET: 0x0077D940
// FUNCTION: fn_08446
void CEfstAction_MYSTERY_POWDER::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077D960
// FUNCTION: fn_08447
void CSkillAction_BO_DUST_EXPLOSION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!target) {
    return;
  }

  const DustExplosionSpawner spawnDustExplosion{};
  for (int delay = 0; delay < 600; delay += 200) {
    const int quadrant = std::max(std::rand() % 4, 0);
    if (CEffect* effect = spawnDustExplosion(quadrant, target->render.position)) {
      effect->SetDelay(delay);
    }
    if (CEffect* effect = spawnDustExplosion((quadrant + 2) % 4, target->render.position)) {
      effect->SetDelay(delay);
    }
  }
}

// OFFSET: 0x0077D9E0
// FUNCTION: fn_08448
void CSkillAction_BO_DUST_EXPLOSION::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077DC70
// FUNCTION: fn_08449
CEffect* DustExplosionSpawner::operator()(int quadrant, const vector3d& position) const {
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
  EffectLists* effects = g_effectMgr;
  return effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion"), x, y, z, 0, -1, 0, 0);
}

// OFFSET: 0x0077DF70
// FUNCTION: fn_08450
void CSkillAction_DK_VIGOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E0D0
// FUNCTION: fn_08451
void CEfstAction_VIGOR::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E230
// FUNCTION: fn_08452
void CEfstAction_VIGOR::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_buff"));
  }
}

// OFFSET: 0x0077E350
// FUNCTION: fn_08453
void CEfstAction_VIGOR::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077E370
// FUNCTION: fn_08454
EfstActionMessage CEfstAction_VIGOR::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_GUARD_STANCE_ON), 0xFF};
}

// OFFSET: 0x0077E3B0
// FUNCTION: fn_08456
void CSkillAction_DK_SERVANTWEAPON_ATK::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_down"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E530
// FUNCTION: fn_08457
void CSkillAction_DK_SERVANTWEAPON_ATK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E7B0
// FUNCTION: fn_08458
void CEfstAction_SERVANTWEAPON::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2460);
  }
}

// OFFSET: 0x0077E7E0
// FUNCTION: fn_08459
EfstActionMessage CEfstAction_SERVANTWEAPON::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_ATTACK_STANCE_ON), 0xFF};
}

// OFFSET: 0x0077E820
// FUNCTION: fn_08461
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillDamage(int actor, CGameActor* targetActor, CGameActor*) {
  CGameActor* source = reinterpret_cast<CGameActor*>(actor);
  if (source) {
    if (CEffect* effect = g_effectMgr->Find(source->render.accountId, 0x99C)) {
      effect->SetLifeTime(0x1453);
    }
  }

  if (targetActor) {
    g_effectMgr->Spawn(targetActor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_down_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(targetActor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_down"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077EAD0
// FUNCTION: fn_08462
void CEfstAction_SERVANT_SIGN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077ED40
// FUNCTION: fn_08463
void CEfstAction_SERVANT_SIGN::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_bottom"));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"));
  }
}

// OFFSET: 0x0077EF40
// FUNCTION: fn_08464
void CEfstAction_SERVANT_SIGN::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x0077EF70
// FUNCTION: fn_08466
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x0077EFA0
// FUNCTION: fn_08467
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, 2460);
  }
}

// OFFSET: 0x0077EFD0
// FUNCTION: fn_08468
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) {
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
      effect->SetDelay(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
  } else {
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub_bottom"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
    if (effect) {
      effect->SetDelay(duration);
    }
    effect = g_effectMgr->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub"), offset.x, offset.y, offset.z, 0, -1, 0, 0);
  }

  if (effect) {
    effect->SetDelay(duration);
  }
}

// OFFSET: 0x0077F600
// FUNCTION: fn_08469
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, 2460);
  }
}

// OFFSET: 0x0077F630
// FUNCTION: fn_08470
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillHit(int caster, CGameActor* target, CGameActor* argument3) {
  CGameActor* source = reinterpret_cast<CGameActor*>(caster);
  if (source) {
    const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_collect"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
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
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077FB40
// FUNCTION: fn_08473
void CSkillAction_DK_STORMSLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const float scale = source->render.speed * 50.0f + 30.0f;
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("stormslash_{}", level)), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (!effect) {
    return;
  }

  effect->SetDelay(static_cast<int>(scale));
  for (int frame = 0; level > 0; --level, frame += 15) {
    const int variant = std::rand() % 4 + 1;
    effect->AddEvent(0, std::format("effect\\DK_STORMSLASH{}.wav", variant).c_str(), frame, 0);
  }
}

// OFFSET: 0x0077FD50
// FUNCTION: fn_08475
bool CSkillAction_DK_MADNESS_CRUSHER::IsSkillWithoutMotion() {
  return true;
}

// OFFSET: 0x0077FD60
// FUNCTION: fn_08476
void CSkillAction_DK_MADNESS_CRUSHER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x0077FD90
// FUNCTION: fn_08477
void CSkillAction_DK_MADNESS_CRUSHER::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780020
// FUNCTION: fn_08479
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007801B0
// FUNCTION: fn_08480
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values
  );
  int direction;
  if (angle > 60.0f && angle <= 120.0f) {
    direction = 0;
  } else if (angle > 120.0f && angle <= 240.0f) {
    direction = 1;
  } else if (angle > 240.0f && angle <= 300.0f) {
    direction = 2;
  } else if ((angle > 300.0f && angle <= 360.0f) || (angle >= 0.0f && angle <= 60.0f)) {
    direction = 3;
  } else {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  const bool flip = direction != 1 && direction != 2;
  const bool horizontal = (direction & 1) != 0;
  std::string name = horizontal ? "dragonic_pierce_horizon" : "dragonic_pierce_vertical";
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(name), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
    float offset;
    if (horizontal) {
      effect->SetFlipX(flip);
      offset = flip ? 0.0f : -180.0f;
    } else {
      effect->SetFlipY(flip);
      offset = flip ? -90.0f : -270.0f;
    }
    effect->SetAngle(static_cast<float>((static_cast<int>(angle + offset) + 360) % 360));
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x007805A0
// FUNCTION: fn_08481
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780840
// FUNCTION: fn_08483
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00780870
// FUNCTION: fn_08484
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780AF0
// FUNCTION: fn_08485
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
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
void CSkillAction_DK_CHARGINGPIERCE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00780DE0
// FUNCTION: fn_08488
void CSkillAction_DK_CHARGINGPIERCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781070
// FUNCTION: fn_08489
EfstActionMessage CEfstAction_CHARGINGPIERCE::GetEfstEndMessage() {
  return {LookupMessage(MSI_IG_ATTACK_STANCE_OFF), 0xFF};
}

// OFFSET: 0x007810A0
// FUNCTION: fn_08490
void CEfstAction_CHARGINGPIERCE_COUNT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (argument3) {
    this->OnEfstEnd(efstId, value, argument3, 0, actor);
    if (CRenderObject* target = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->GetGameActorByAidIncludeMe(argument3)) {
      EffectLists* effects = g_effectMgr;
      effects->Spawn(argument3, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
      g_soundMgr->PlayWave(
        "effect\\DK_CHARGINGPIERCE3.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f,
        0
      );
    }
  } else if (actor) {
    if (CEffect* effect = g_effectMgr->Spawn(actor->render.accountId, 2461, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      static_cast<CChargingPierceNewEffect*>(effect)->SetCount(value);
    }
  }
}

// OFFSET: 0x007812F0
// FUNCTION: fn_08491
void CEfstAction_CHARGINGPIERCE_COUNT::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2461);
  }
}

// OFFSET: 0x00781330
// FUNCTION: fn_08493
void CSkillAction_DK_HACKANDSLASHER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007815A0
// FUNCTION: fn_08494
void CSkillAction_DK_HACKANDSLASHER::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781710
// FUNCTION: fn_08495
int CSkillAction_DK_HACKANDSLASHER_ATK::GetSkillAttackMotion() {
  return 0;
}

// OFFSET: 0x00781720
// FUNCTION: fn_08496
void CSkillAction_DK_HACKANDSLASHER_ATK::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781B50
// FUNCTION: fn_08497
void CSkillAction_NPC_BLOCK_EXPLOSION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00781B80
// FUNCTION: fn_08498
void CSkillAction_NPC_BLOCK_EXPLOSION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 2406, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, 2407, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781C30
// FUNCTION: fn_08499
void CSkillAction_NPC_FROST_FIELD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x00781C60
// FUNCTION: fn_08500
void CSkillAction_NPC_FROST_FIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
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
void CSkillAction_NPC_FROST_FIELD::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 1310, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781E00
// FUNCTION: fn_08502
void CEfstAction_FROST_STORM::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, 2403, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781E50
// FUNCTION: fn_08503
void CEfstAction_FROST_STORM::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2403);
  }
}

// OFFSET: 0x00781E80
// FUNCTION: fn_08504
void CEfstAction_FROST_STORM::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

class CSkillAction_NPC_LIGHTNING_JUDGEMENT : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_GROGGY : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NPC_RESET_EFST : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_TARGET_MARKER : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_NPC_AIMED_SHOWER : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_BLAZING_ERUPTION : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_NPC_BLOCK_SEAL : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
};

class CEfstAction_BLOCK_SEAL : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_HN_DOUBLEBOWLINGBASH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_MEGA_SONIC_BLOW : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_SHIELD_CHAIN_RUSH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_SPIRAL_PIERCE_MAX : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_METEOR_STORM_BUSTER : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillEffect(vector3d position, int argument2, int argument3) override;
  bool UseGroundSkillLastFrame() override;
  int GetSkillScopeType() override;
};

class CSkillAction_HN_JUPITEL_THUNDER_STORM : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_JACK_FROST_NOVA : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) override;
  int GetSkillScopeType() override;
};

class CSkillObjectAction_HN_JACK_FROST_NOVA : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillAction_HN_HELLS_DRIVE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_GROUND_GRAVITATION : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillCast(int level, CGameActor* source) override;
};

class CSkillObjectAction_HN_GROUND_GRAVITATION : public CSkillObjectAction {
public:
  void OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) override;
  void OnSkillObjectRemove(CRenderObject* object) override;
};

class CSkillAction_HN_NAPALM_VULCAN_STRIKE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillHit(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_HN_BREAKINGLIMIT : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_BREAKINGLIMIT : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_HN_RULEBREAK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CEfstAction_RULEBREAK : public CEfstAction {
public:
  void OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) override;
  void OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_HN_OVERCOMING_CRISIS : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

// OFFSET: 0x00781EA0
// FUNCTION: fn_08505
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00781ED0
// FUNCTION: fn_08506
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\NPC_LIGHTNING_JUDGEMENT.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }
}

// OFFSET: 0x007820B0
// FUNCTION: fn_08507
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 1256, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00782100
// FUNCTION: fn_08508
void CEfstAction_GROGGY::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("groggy_on_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00782270
// FUNCTION: fn_08509
void CEfstAction_GROGGY::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("groggy_on_loop"));
  }
}

// OFFSET: 0x00782390
// FUNCTION: fn_08510
void CEfstAction_GROGGY::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007823B0
// FUNCTION: fn_08511
void CSkillAction_NPC_RESET_EFST::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 2410, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\NPC_RESET_EFST.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }
}

// OFFSET: 0x00782470
// FUNCTION: fn_08512
void CEfstAction_TARGET_MARKER::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("target_marker_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("target_marker_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00782700
// FUNCTION: fn_08513
void CEfstAction_TARGET_MARKER::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("target_marker_loop"));
  }
}

// OFFSET: 0x00782830
// FUNCTION: fn_08514
void CEfstAction_TARGET_MARKER::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00782860
// FUNCTION: fn_08516
void CSkillAction_NPC_AIMED_SHOWER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x00782890
// FUNCTION: fn_08517
void CSkillAction_NPC_AIMED_SHOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 1268, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\NPC_AIMED_SHOWER.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }
}

// OFFSET: 0x00782950
// FUNCTION: fn_08518
void CSkillAction_NPC_AIMED_SHOWER::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 2405, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007829A0
// FUNCTION: fn_08519
void CSkillAction_NPC_BLAZING_ERUPTION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(441, level / 16);
  }
}

// OFFSET: 0x007829D0
// FUNCTION: fn_08520
void CSkillAction_NPC_BLAZING_ERUPTION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, 1270, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, 1266, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(225, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    const CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
    g_soundMgr->PlayWave(
      "effect\\NPC_BLAZING_ERUPTION.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
    );
  }
}

// OFFSET: 0x00782B20
// FUNCTION: fn_08521
void CSkillAction_NPC_BLOCK_SEAL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00782B50
// FUNCTION: fn_08522
void CEfstAction_BLOCK_SEAL::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, 2400, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00782F20
// FUNCTION: fn_08523
void CEfstAction_BLOCK_SEAL::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    g_effectMgr->Retire(actor->render.accountId, 2400);
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop_bottom"));
    effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop"));
  }
}

// OFFSET: 0x00783160
// FUNCTION: fn_08524
void CEfstAction_BLOCK_SEAL::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007832E0
// FUNCTION: fn_08525
int CSkillAction_HN_DOUBLEBOWLINGBASH::GetSkillAttackMotion() {
  return 58;
}

// OFFSET: 0x007832F0
// FUNCTION: fn_08526
void CSkillAction_HN_DOUBLEBOWLINGBASH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00783320
// FUNCTION: fn_08527
void CSkillAction_HN_DOUBLEBOWLINGBASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("double_bowlingbash"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007834A0
// FUNCTION: fn_08528
void CSkillAction_HN_DOUBLEBOWLINGBASH::OnSkillDamage(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(73, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  }
}

// OFFSET: 0x007834E0
// FUNCTION: fn_08529
int CSkillAction_HN_MEGA_SONIC_BLOW::GetSkillAttackMotion() {
  return 58;
}

// OFFSET: 0x007834F0
// FUNCTION: fn_08530
void CSkillAction_HN_MEGA_SONIC_BLOW::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(121, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mega_sonic_blow"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00783690
// FUNCTION: fn_08531
int CSkillAction_HN_SHIELD_CHAIN_RUSH::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x007836A0
// FUNCTION: fn_08532
void CSkillAction_HN_SHIELD_CHAIN_RUSH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(58, level / 16);
  }
}

// OFFSET: 0x007836D0
// FUNCTION: fn_08533
void CSkillAction_HN_SHIELD_CHAIN_RUSH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_chain_rush"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00783850
// FUNCTION: fn_08534
int CSkillAction_HN_SPIRAL_PIERCE_MAX::GetSkillAttackMotion() {
  return 58;
}

// OFFSET: 0x00783860
// FUNCTION: fn_08535
void CSkillAction_HN_SPIRAL_PIERCE_MAX::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(502, level / 16);
  }
}

// OFFSET: 0x00783890
// FUNCTION: fn_08536
void CSkillAction_HN_SPIRAL_PIERCE_MAX::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (!g_serviceOptions.Get(229)) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max_aura_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max_aura"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values);
  const vector3d position = source->render.position;
  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max"), position.x, position.y, position.z, 0, -1, 0, 0);
  if (effect) {
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x00783C90
// FUNCTION: fn_08537
bool CSkillAction_HN_METEOR_STORM_BUSTER::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x00783CA0
// FUNCTION: fn_08538
void CSkillAction_HN_METEOR_STORM_BUSTER::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(55, level / 16);
  }
}

// OFFSET: 0x00783CD0
// FUNCTION: fn_08539
void CSkillAction_HN_METEOR_STORM_BUSTER::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_meteor_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_meteor"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_bottom"), position.x, position.y, position.z, 0, -1, 0, 0);
  g_effectMgr->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster"), position.x, position.y, position.z, 0, -1, 0, 0);
}

// OFFSET: 0x00784100
// FUNCTION: fn_08540
bool CSkillAction_HN_METEOR_STORM_BUSTER::UseGroundSkillLastFrame() {
  return true;
}

// OFFSET: 0x00784110
// FUNCTION: fn_08541
int CSkillAction_HN_METEOR_STORM_BUSTER::GetSkillScopeType() {
  return 7;
}

// OFFSET: 0x00784120
// FUNCTION: fn_08542
int CSkillAction_HN_JUPITEL_THUNDER_STORM::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x00784130
// FUNCTION: fn_08543
void CSkillAction_HN_JUPITEL_THUNDER_STORM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(57, level / 16);
  }
}

// OFFSET: 0x00784160
// FUNCTION: fn_08544
void CSkillAction_HN_JUPITEL_THUNDER_STORM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    static_cast<EffectActor*>(source)->LaunchEffect(12, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_effectMgr->Spawn(source->render.accountId, 2366, source->render.position.x, source->render.position.y, source->render.position.z, target->render.accountId, level, 0, 0);
  }
}

// OFFSET: 0x007841F0
// FUNCTION: fn_08545
int CSkillAction_HN_JACK_FROST_NOVA::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x00784200
// FUNCTION: fn_08546
bool CSkillAction_HN_JACK_FROST_NOVA::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x00784210
// FUNCTION: fn_08547
void CSkillAction_HN_JACK_FROST_NOVA::OnSkillCast(int level, CGameActor* source) {
  if (!source) {
    return;
  }

  static_cast<EffectActor*>(source)->LaunchEffect(1449, level / 16);
  CEffect* effect = g_effectMgr->Spawn(source->render.accountId, g_serviceOptions.Get(229) ? 1543 : 1446, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007842D0
// FUNCTION: fn_08548
void CSkillAction_HN_JACK_FROST_NOVA::OnGroundSkillCast(int level, CGameActor* source, int argument3, int argument4, int argument5) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (mode) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->LaunchDummyEffect(reinterpret_cast<const vector3d*>(&argument3), 0x5A8, level / 24, 0);
  }
}

// OFFSET: 0x00784320
// FUNCTION: fn_08549
int CSkillAction_HN_JACK_FROST_NOVA::GetSkillScopeType() {
  return -2;
}

// OFFSET: 0x00784330
// FUNCTION: fn_08550
void CSkillObjectAction_HN_JACK_FROST_NOVA::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_start_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_loop_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007847E0
// FUNCTION: fn_08551
void CSkillObjectAction_HN_JACK_FROST_NOVA::OnSkillObjectRemove(CRenderObject* object) {
  if (object) {
    const vector3d position = object->render.position;
    EffectLists* effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_end"), position.x, position.y, position.z, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00784950
// FUNCTION: fn_08552
int CSkillAction_HN_HELLS_DRIVE::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x00784960
// FUNCTION: fn_08553
void CSkillAction_HN_HELLS_DRIVE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(1709, level / 16);
  }
}

// OFFSET: 0x00784990
// FUNCTION: fn_08554
void CSkillAction_HN_HELLS_DRIVE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  // OFFSET: 0x00786500
  // FUNCTION: fn_08577
  const auto shuffledIndices = [](int count) {
    std::vector<int> indices;
    for (int index = 0; index < count; ++index) {
      indices.push_back(index);
    }
    std::mt19937 engine(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), engine);
    return indices;
  };

  const int count = (level - 1) / 3 * 2 + 3;
  std::vector<int> columns = shuffledIndices(count);
  std::vector<int> rows = shuffledIndices(count);
  int frame = 0;
  for (int index = 0; index < count; ++index, frame += 32) {
    float offsetZ = 0.0f;
    if (!rows.empty()) {
      offsetZ = static_cast<float>((rows.front() - count / 2) * 5.0);
      rows.erase(rows.begin());
    }
    float offsetX = 0.0f;
    if (!columns.empty()) {
      offsetX = static_cast<float>((columns.front() - count / 2) * 5.0);
      columns.erase(columns.begin());
    }

    if (CEffect* effect = g_effectMgr->Spawn(
          0, CEZ2STREffectMgr::GetInstance()->EffectId("hells_drive_bottom"), source->render.position.x, source->render.position.y, source->render.position.z, 0, -1, 0, 0
        )) {
      effect->position[0] = offsetX + source->render.position.x;
      effect->position[2] = offsetZ + source->render.position.z;
      effect->position[1] = source->render.position.y + 0.0f;
      effect->SetDelay(frame);
    }

    if (CEffect* effect = g_effectMgr->Spawn(
          0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hells_drive_%02d", std::max(std::rand() % 3, 0))), source->render.position.x,
          source->render.position.y, source->render.position.z, 0, -1, 0, 0
        )) {
      effect->position[0] = offsetX + source->render.position.x;
      effect->position[2] = offsetZ + source->render.position.z;
      effect->position[1] = source->render.position.y + 0.0f;
      effect->SetDelay(frame);
    }
  }
}

// OFFSET: 0x00784E40
// FUNCTION: fn_08555
int CSkillAction_HN_GROUND_GRAVITATION::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x00784E50
// FUNCTION: fn_08556
bool CSkillAction_HN_GROUND_GRAVITATION::KeepSkillAttackMotion(int argument) {
  return true;
}

// OFFSET: 0x00784E60
// FUNCTION: fn_08557
void CSkillAction_HN_GROUND_GRAVITATION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(9, level / 16);
    static_cast<EffectActor*>(source)->LaunchEffect(12, level / 16);
  }
}

// OFFSET: 0x00784EA0
// FUNCTION: fn_08558
void CSkillObjectAction_HN_GROUND_GRAVITATION::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_bottom_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_start"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_bottom_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_loop"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
}

// OFFSET: 0x00785370
// FUNCTION: fn_08559
void CSkillObjectAction_HN_GROUND_GRAVITATION::OnSkillObjectRemove(CRenderObject* object) {
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Find(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_bottom_loop"))) {
    effect->EnableTimeLimit();
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Find(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_loop"))) {
    effect->EnableTimeLimit();
  }
}

// OFFSET: 0x007855C0
// FUNCTION: fn_08560
int CSkillAction_HN_NAPALM_VULCAN_STRIKE::GetSkillAttackMotion() {
  return 59;
}

// OFFSET: 0x007855D0
// FUNCTION: fn_08561
void CSkillAction_HN_NAPALM_VULCAN_STRIKE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(58, level / 16);
  }
}

// OFFSET: 0x00785600
// FUNCTION: fn_08562
void CSkillAction_HN_NAPALM_VULCAN_STRIKE::OnSkillHit(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("napalm_vulcan_strike"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785780
// FUNCTION: fn_08563
int CSkillAction_HN_BREAKINGLIMIT::GetSkillAttackMotion() {
  return 58;
}

// OFFSET: 0x00785790
// FUNCTION: fn_08564
void CSkillAction_HN_BREAKINGLIMIT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2369, level / 16);
  }
}

// OFFSET: 0x007857C0
// FUNCTION: fn_08565
void CSkillAction_HN_BREAKINGLIMIT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("breakinglimit"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785930
// FUNCTION: fn_08566
void CEfstAction_BREAKINGLIMIT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("breakinglimit_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785AB0
// FUNCTION: fn_08567
void CEfstAction_BREAKINGLIMIT::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("breakinglimit_buff"));
  }
}

// OFFSET: 0x00785BE0
// FUNCTION: fn_08568
void CEfstAction_BREAKINGLIMIT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00785C00
// FUNCTION: fn_08569
int CSkillAction_HN_RULEBREAK::GetSkillAttackMotion() {
  return 58;
}

// OFFSET: 0x00785C10
// FUNCTION: fn_08570
void CSkillAction_HN_RULEBREAK::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(2369, level / 16);
  }
}

// OFFSET: 0x00785C40
// FUNCTION: fn_08571
void CSkillAction_HN_RULEBREAK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rulebreak"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785DA0
// FUNCTION: fn_08572
void CEfstAction_RULEBREAK::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rulebreak_buff"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785F10
// FUNCTION: fn_08573
void CEfstAction_RULEBREAK::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rulebreak_buff"));
  }
}

// OFFSET: 0x00786030
// FUNCTION: fn_08574
void CEfstAction_RULEBREAK::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x00786050
// FUNCTION: fn_08575
void CSkillAction_HN_OVERCOMING_CRISIS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("overcoming_crisis"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("overcoming_crisis_bottom"), 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
  }
}


class CSkillObjectActionRegistration {
public:
  CSkillObjectActionRegistration(int skillId, std::function<CSkillObjectAction*()> factory);
};

// OFFSET: 0x00786450
// FUNCTION: fn_08576
CSkillObjectActionRegistration::CSkillObjectActionRegistration(int skillId, std::function<CSkillObjectAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(skillId, factory);
}
