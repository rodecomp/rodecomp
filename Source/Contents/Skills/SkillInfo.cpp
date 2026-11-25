#include "UI/Shortcuts/UIShortCutWnd.h"
#include "Client/Mode/ModeMgr.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectResourceSwitcher.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/Skills/ChargingPierceEffect.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Particles/EZ2STREffectEx.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Effects/Core/ShowEffect.h"
#include "Contents/Effects/Skills/MonsterSkillEffects.h"
#include "Contents/Tables/MessageTable.h"
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

class CEfstAction_ChangeSizeMonster : public CEfstAction_ChangeSize {
};

class CEfstAction_ChangeSizeNPC : public CEfstAction_ChangeSize {
};

class CSkillAction_CD_DIVINUS_FLOS : public CSkillAction {
public:
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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

class CEfstAction_ShowEffect1 : public CEfstAction_ShowEffect {
};

class CEfstAction_ShowEffect2 : public CEfstAction_ShowEffect {
};

class CEfstAction_ShowEffect3 : public CEfstAction_ShowEffect {
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
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  int GetSkillAttackMotion() override;
  bool SuppressAttackSkillArrow() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_ABC_CHASING_SHOT : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  bool SuppressAttackSkillArrow() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_ABC_ABYSS_FLAME_ATK : public CSkillAction {
public:
  bool KeepSkillAttackMotion(int argument) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_SHC_CROSS_SLASH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_SHC_DANCING_KNIFE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_ABC_HIT_AND_SLIDING : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_ABC_ABYSS_FLAME : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_EM_PSYCHIC_STREAM : public CSkillAction {
public:
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_IQ_BLAZING_FLAME_BLAST : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_BO_MAYHEMIC_THORNS : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_EXPLOSIVE_POWDER : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  bool KeepSkillAttackSpeed() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_BO_MYSTERY_POWDER : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  int GetSkillAttackMotion() override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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

class CSkillAction_DK_SERVANT_W_SIGN : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnTargetSkillCast(CGameActor* source, CGameActor* target, int delay) override;
};

class CSkillAction_DK_SERVANT_W_PHANTOM : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
};

class CSkillAction_DK_SERVANT_W_DEMOL : public CSkillAction {
public:
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_DK_STORMSLASH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
};

class CSkillAction_DK_MADNESS_CRUSHER : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  bool IsSkillWithoutMotion() override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_DK_DRAGONIC_PIERCE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_DK_DRAGONIC_BREATH : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
};

class CSkillAction_DK_CHARGINGPIERCE : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
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
  int GetSkillAttackMotion() override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
};

class CSkillAction_DK_HACKANDSLASHER_ATK : public CSkillAction {
public:
  int GetSkillAttackMotion() override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
static __forceinline int ClampedRandom(int range) {
  const int value = std::rand() % range;
  return value >= 0 ? value : 0;
}

// INLINED
__forceinline CSkillAction* CreateSkillAction(CSkillActionMgr* mgr, int skillId) {
  const auto found = mgr->m_skillActions.find(skillId);
  return mgr->m_skillActions.end() != found ? found->second() : nullptr;
}

// INLINED
__forceinline CEfstAction* CreateEfstAction(CSkillActionMgr* mgr, int efstId) {
  const auto found = mgr->m_efstActions.find(efstId);
  return mgr->m_efstActions.end() != found ? found->second() : nullptr;
}

// INLINED
__forceinline CSkillObjectAction* CreateSkillObjectAction(CSkillActionMgr* mgr, int skillId) {
  const auto found = mgr->m_skillObjectActions.find(skillId);
  return mgr->m_skillObjectActions.end() != found ? found->second() : nullptr;
}

// GLOBAL: gl_24640
CSkillActionMgr* g_skillActionMgr;

// INLINED
CSkillActionMgr::~CSkillActionMgr() = default;

// INLINED
__forceinline CSkillActionMgr* GetSkillActionMgr() {
  if (!g_skillActionMgr) {
    g_skillActionMgr = new (std::nothrow) CSkillActionMgr;
  }

  return g_skillActionMgr;
}

// OFFSET: 0x005B06F0
// FUNCTION: fn_03339
CSkillActionMgr* CSkillActionMgr::GetInstance() {
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

// OFFSET: 0x00773500
// FUNCTION: fn_08312
CSkillInfo::~CSkillInfo() = default;

// OFFSET: 0x00774710
// FUNCTION: fn_08325
bool CSkillInfo::IsValid(const CSkillInfo& skill) {
  return skill.available != 0 && skill.level != 0;
}

// OFFSET: 0x00776540
// FUNCTION: fn_08348
int CSkillAction_SHC_IMPACT_CRATER::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00776940
// FUNCTION: fn_08351
int CSkillAction_SHC_CROSS_SLASH::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00776C30
// FUNCTION: fn_08354
int CSkillAction_SHC_DANCING_KNIFE::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00777930
// FUNCTION: fn_08360
int CSkillAction_SHC_SHADOW_STAB::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00778130
// FUNCTION: fn_08370
int CSkillAction_ABC_CHASING_BREAK::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00778570
// FUNCTION: fn_08374
int CSkillAction_ABC_CHASING_SHOT::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00778DC0
// FUNCTION: fn_08380
int CSkillAction_ABC_HIT_AND_SLIDING::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00779E20
// FUNCTION: fn_08391
int CSkillAction_IQ_BLAZING_FLAME_BLAST::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077CCB0
// FUNCTION: fn_08436
int CSkillAction_BO_MAYHEMIC_THORNS::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077D010
// FUNCTION: fn_08439
int CSkillAction_BO_EXPLOSIVE_POWDER::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077E3A0
// FUNCTION: fn_08455
int CSkillAction_DK_SERVANTWEAPON_ATK::GetSkillAttackMotion() {
  return -1;
}

// OFFSET: 0x0077FB30
// FUNCTION: fn_08472
int CSkillAction_DK_STORMSLASH::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077FD40
// FUNCTION: fn_08474
int CSkillAction_DK_MADNESS_CRUSHER::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00780010
// FUNCTION: fn_08478
int CSkillAction_DK_DRAGONIC_PIERCE::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00780830
// FUNCTION: fn_08482
int CSkillAction_DK_DRAGONIC_BREATH::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00780DA0
// FUNCTION: fn_08486
int CSkillAction_DK_CHARGINGPIERCE::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00781320
// FUNCTION: fn_08492
int CSkillAction_DK_HACKANDSLASHER::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x008AC440
// FUNCTION: fn_11732
bool UIShortCutWnd::MatchesShortcut(const CSkillInfo& skill, const NormalItemSlot* slot) {
  if (!slot || !skill.IsValid()) {
    return false;
  }

  const auto requested = slot->value;
  return 0 != requested && (skill.id != slot->itemId || skill.level >= requested);
}

// OFFSET: 0x00773550
// FUNCTION: fn_08313
int CSkillInfo::UseApAmount(int skillLevel) const {
  int amount = 0;
  g_session.m_hotkeyScript->Call("GetLevelUseApAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x00773620
// FUNCTION: fn_08314
int CSkillInfo::UseSpAmount(int skillLevel) const {
  int amount = 0;
  g_session.m_hotkeyScript->Call("GetLevelUseSpAmount", "dd>d", static_cast<int>(this->id), skillLevel, &amount);
  return amount;
}

// OFFSET: 0x007736F0
// FUNCTION: fn_08315
const char* CSkillInfo::DisplayName() const {
  return CSkillInfo::DisplayName(static_cast<int>(this->id));
}

// OFFSET: 0x00773920
// FUNCTION: fn_08318
const char* CSkillInfo::DisplayName(int skillId) {
  switch (skillId) {
  case SU_CN_METEOR2:
    skillId = SU_CN_METEOR;
    break;
  case SU_LUNATICCARROTBEAT2:
    skillId = SU_LUNATICCARROTBEAT;
    break;
  }

  const char* result = nullptr;
  return g_session.m_hotkeyScript->Call("GetSkillName", "d>s", skillId, &result) == true ? result : "Unknown-Skill";
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
    return GetSkillActionMgr()->IsSupportAction(skillId);
  }
}

// OFFSET: 0x0077B4A0
// FUNCTION: fn_08413
bool CSkillActionMgr::IsSupportAction(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->IsSupport();
}

// OFFSET: 0x007737C0
// FUNCTION: fn_08316
const char* CSkillInfo::IdName() const {
  return CSkillInfo::IdName(static_cast<int>(this->id));
}

// OFFSET: 0x00773870
// FUNCTION: fn_08317
const char* CSkillInfo::IdName(int skillId) {
  const char* result = nullptr;
  return g_session.m_hotkeyScript->Call("GetSkillIdName", "d>s", skillId, &result) ? result : "Zero Skill";
}

// OFFSET: 0x00774500
// FUNCTION: fn_08323
bool CSkillInfo::UsesLevel(int skillId) {
  bool result = false;
  return g_session.m_hotkeyScript->Call("IsLevelUseSkill", "d>b", skillId, &result) ? result : false;
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

  effect->scaleParameter = static_cast<float>(value) * 0.01f;
}

// OFFSET: 0x007760D0
// FUNCTION: fn_08344
void CSkillAction_CD_DIVINUS_FLOS::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (!g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007756D0
// FUNCTION: fn_08339
void CEfstAction_UNLIMIT::OnEfstUpdate(int efstId, int value, int argument3, int argument4, CRenderObject* actor) {
  this->OnEfstBegin(efstId, value, 0, 0, actor, false);
}

// OFFSET: 0x007756F0
// FUNCTION: fn_45569
template <>
CEfstAction* NewAction<CEfstAction_UNLIMIT, CEfstAction>() {
  return new CEfstAction_UNLIMIT;
}

// OFFSET: 0x00775710
// FUNCTION: fn_45570
template <>
CSkillAction* NewAction<CSkillAction_RA_UNLIMIT, CSkillAction>() {
  return new CSkillAction_RA_UNLIMIT;
}

// OFFSET: 0x007764E0
// FUNCTION: fn_08346
EfstActionMessage CEfstAction_SHADOW_WEAPON::GetEfstBeginMessage() {
  return {LookupMessage(MSI_SHC_ENCHANTING_SHADOW_ON), 0xFFFF00};
}

// OFFSET: 0x00776510
// FUNCTION: fn_08347
EfstActionMessage CEfstAction_SHADOW_WEAPON::GetEfstEndMessage() {
  return {LookupMessage(MSI_SHC_ENCHANTING_SHADOW_OFF), 0xFF};
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
    g_effectMgr->Spawn(static_cast<int>(source->render.accountId), 2467, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777E80
// FUNCTION: fn_08366
void CEfstAction_SHADOW_EXCEED::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  while (effectActor->HasId(EF_KAGEMUSYA)) {
    actor->SendMsg(nullptr, AM_DELETE_EFFECT, EF_KAGEMUSYA, 0, 0, 0, 0, 0);
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
  return {LookupMessage(MSI_SHC_SHADOW_EXCEED_ON), 0xFFFF00};
}

// OFFSET: 0x00777F30
// FUNCTION: fn_08369
EfstActionMessage CEfstAction_SHADOW_EXCEED::GetEfstEndMessage() {
  return {LookupMessage(MSI_SHC_SHADOW_EXCEED_OFF), 0xFF};
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
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  return !action ? std::pair<bool, int>(false, 8) : std::pair<bool, int>(true, action->GetSkillCastMotion());
}

// OFFSET: 0x0077AD90
// FUNCTION: fn_08405
vector3d CellWorldPosition(int column, int row) {
  if (!g_modeMgr.GetGameMode()) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }
  if (!g_modeMgr.GetGameMode()->scene) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }
  if (!g_modeMgr.GetGameMode()->scene->terrain) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }
  vector3d position = {0.0f, 0.0f, 0.0f};
  g_modeMgr.GetGameMode()->scene->CellOrigin(column, row, position.x, position.z);
  position.y = g_modeMgr.GetGameMode()->scene->terrain->Height(position.x, position.z);
  return vector3d{position.x + 2.5f, position.y + 0.0f, position.z + 2.5f};
}

// OFFSET: 0x0077AEA0
// FUNCTION: fn_08406
EfstActionMessage CSkillActionMgr::GetEfstBeginMessage(int efstId) {
  std::unique_ptr<CEfstAction> action(CreateEfstAction(this, efstId));
  if (!action) {
    return {nullptr, 0xFFFF00};
  }

  return action->GetEfstBeginMessage();
}

// OFFSET: 0x0077AF80
// FUNCTION: fn_08407
EfstActionMessage CSkillActionMgr::GetEfstEndMessage(int efstId) {
  std::unique_ptr<CEfstAction> action(CreateEfstAction(this, efstId));
  if (!action) {
    return {nullptr, 0xFF};
  }

  return action->GetEfstEndMessage();
}

// OFFSET: 0x0077B060
// FUNCTION: fn_08408
int CSkillActionMgr::GetSkillScopeType(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return 0;
  }

  return action->GetSkillScopeType();
}

// OFFSET: 0x0077B210
// FUNCTION: fn_08409
std::pair<bool, int> CSkillActionMgr::GetSkillAttackMotion(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  return !action ? std::pair<bool, int>(false, 7) : std::pair<bool, int>(true, action->GetSkillAttackMotion());
}

// OFFSET: 0x0077B2E0
// FUNCTION: fn_08410
bool CSkillActionMgr::KeepSkillAttackMotion(int skillId, int skillMessageType) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->KeepSkillAttackMotion(skillMessageType);
}

// OFFSET: 0x0077B3A0
// FUNCTION: fn_08411
bool CSkillActionMgr::IsSkillWithoutMotion(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
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
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("unlimit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00775730
// FUNCTION: fn_08340
void CSkillAction_AG_ENERGY_CONVERSION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00775BC0
// FUNCTION: fn_08342
void CSkillAction_CD_DIVINUS_FLOS::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00775E60
// FUNCTION: fn_08343
void CSkillAction_CD_DIVINUS_FLOS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("divinus_flos_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776260
// FUNCTION: fn_08345
void CSkillAction_SHC_ENCHANTING_SHADOW::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("enchanting_shadow"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776550
// FUNCTION: fn_08349
void CSkillAction_SHC_IMPACT_CRATER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007767C0
// FUNCTION: fn_08350
void CSkillAction_SHC_IMPACT_CRATER::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("impact_crater_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776950
// FUNCTION: fn_08352
void CSkillAction_SHC_CROSS_SLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776AC0
// FUNCTION: fn_08353
void CSkillAction_SHC_CROSS_SLASH::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("cross_slash_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00776C40
// FUNCTION: fn_08355
void CSkillAction_SHC_DANCING_KNIFE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00776EF0
// FUNCTION: fn_08356
void CSkillAction_SHC_DANCING_KNIFE::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00777C40
// FUNCTION: fn_08363
void CSkillAction_SHC_SHADOW_EXCEED::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shadow_exceed_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007783F0
// FUNCTION: fn_08373
void CSkillAction_ABC_CHASING_BREAK::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778150
// FUNCTION: fn_08372
void CSkillAction_ABC_CHASING_BREAK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(EF_ABC_CHASING_LINE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  for (int index = 0; index < 8; ++index) {
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_break"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    if (effect) {
      effect->SetAngle(static_cast<float>(index * 45));
      effect->SetDelay(ClampedRandom(150));
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
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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

  if (CRagEffect* effect = static_cast<EffectActor*>(source)->LaunchEffect(EF_ABC_CHASING_LINE, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
    effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&target->render.position), 0, 0, 0, 0);
  }

  const vector3d offset{0.0f, 0.0f, 0.0f};
  for (int angle = 0; angle < 360; angle += 45) {
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_single"), offset, 0, -1, 0, 0);
    if (effect) {
      effect->SetAngle(static_cast<float>(angle));
      effect->SetScale(0.15f);
      effect->EnableGroundRender();
    }
  }
}

// OFFSET: 0x00778AD0
// FUNCTION: fn_08378
void CSkillAction_ABC_CHASING_SHOT::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hitsub"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778C50
// FUNCTION: fn_08379
void CSkillAction_ABC_CHASING_SHOT::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chasing_shot_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778DD0
// FUNCTION: fn_08381
void CSkillAction_ABC_HIT_AND_SLIDING::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hit_and_sliding_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00778F50
// FUNCTION: fn_08382
void CSkillAction_ABC_ABYSS_FLAME::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x007791F0
// FUNCTION: fn_08383
void CSkillAction_ABC_ABYSS_FLAME::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779360
// FUNCTION: fn_08384
void CSkillAction_ABC_ABYSS_FLAME::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007794E0
// FUNCTION: fn_08386
void CSkillAction_ABC_ABYSS_FLAME_ATK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_target"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779660
// FUNCTION: fn_08387
void CSkillAction_ABC_ABYSS_FLAME_ATK::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("abyss_flame_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779870
// FUNCTION: fn_08388
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x00779A00
// FUNCTION: fn_08389
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779C80
// FUNCTION: fn_08390
void CSkillAction_EM_PSYCHIC_STREAM::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("psychic_stream_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00779E30
// FUNCTION: fn_08392
void CSkillAction_IQ_BLAZING_FLAME_BLAST::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x0077A2A0
// FUNCTION: fn_08394
void CSkillAction_IQ_BLAZING_FLAME_BLAST::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  if (CEffect* effect = effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetScale(1.5f);
  }
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetScale(1.5f);
  }
}

// OFFSET: 0x00774AF0
// FUNCTION: fn_08330
void CEfstAction_GenderChange::OnEfstEnd(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_loop"));
  effects = g_effectMgr;
  effects->Retire(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change_cast"));
  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("gender_change"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_begin"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_bottom_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_begin"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dancing_knife_skill_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x007759F0
// FUNCTION: fn_08341
void CSkillAction_AG_ENERGY_CONVERSION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  source->LaunchEffect(EF_HEAL_BLUE_2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("energy_conversion"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
}

// OFFSET: 0x00777940
// FUNCTION: fn_08361
void CSkillAction_SHC_SHADOW_STAB::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values
  );
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shadow_stab"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetAngle(angle);
  }
}

// OFFSET: 0x00777B00
// FUNCTION: fn_08362
void CSkillAction_SHC_SHADOW_STAB::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (!target) {
    return;
  }

  EffectLists* effects = g_effectMgr;
  effects->Spawn(
    target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("shadow_stab_hit{}", static_cast<int>(std::max(std::rand() % 2, 1)))), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0
  );
}

// OFFSET: 0x00777E20
// FUNCTION: fn_08365
void CEfstAction_SHADOW_EXCEED::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (!actor || !argument6) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  if (!effectActor->HasId(EF_KAGEMUSYA)) {
    effectActor->LaunchEffect(EF_KAGEMUSYA, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("blazing_flame_blast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x0077B560
// FUNCTION: fn_08414
bool CSkillActionMgr::SuppressAttackSkillArrow(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->SuppressAttackSkillArrow();
}

// OFFSET: 0x0077B620
// FUNCTION: fn_08415
bool CSkillActionMgr::SuppressSupportSkillArrow(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->SuppressSupportSkillArrow();
}

// OFFSET: 0x0077B6E0
// FUNCTION: fn_08416
bool CSkillActionMgr::UseGroundSkillLastFrame(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->UseGroundSkillLastFrame();
}

// OFFSET: 0x0077B7A0
// FUNCTION: fn_08417
bool CSkillActionMgr::KeepSkillAttackSpeed(int skillId) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  return action->KeepSkillAttackSpeed();
}

// OFFSET: 0x0077B860
// FUNCTION: fn_08418
bool CSkillActionMgr::OnSkillDamage(int skillId, CGameActor* source, CGameActor* target, int hitIndex) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillDamage(source, target, hitIndex);
  return true;
}

// OFFSET: 0x0077B920
// FUNCTION: fn_08419
bool CSkillActionMgr::OnEfstBegin(int efstId, int value1, int value2, int value3, CRenderObject* actor, bool flag) {
  std::unique_ptr<CEfstAction> action(CreateEfstAction(this, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstBegin(efstId, value1, value2, value3, actor, flag);
  return true;
}

// OFFSET: 0x0077B9F0
// FUNCTION: fn_08420
bool CSkillActionMgr::OnSkillCast(int skillId, int level, CGameActor* source) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillCast(level, source);
  return true;
}

// OFFSET: 0x0077BAB0
// FUNCTION: fn_08421
bool CSkillActionMgr::OnTargetSkillCast(int skillId, CGameActor* source, CGameActor* target, int delay) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnTargetSkillCast(source, target, delay);
  return true;
}

// OFFSET: 0x0077BB70
// FUNCTION: fn_08422
bool CSkillActionMgr::OnGroundSkillCast(int skillId, int castDelay, CGameActor* target, short column, short row) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnGroundSkillCast(castDelay, target, CellWorldPosition(column, row));
  return true;
}

// OFFSET: 0x0077BC60
// FUNCTION: fn_08423
bool CSkillActionMgr::OnEfstEnd(int efstId, int value1, int value2, int value3, CRenderObject* actor) {
  std::unique_ptr<CEfstAction> action(CreateEfstAction(this, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstEnd(efstId, value1, value2, value3, actor);
  return true;
}

// OFFSET: 0x0077BD20
// FUNCTION: fn_08424
bool CSkillActionMgr::OnGroundSkillEffect(int skillId, int column, int row, int level, int actor) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnGroundSkillEffect(CellWorldPosition(column, row), level, actor);
  return true;
}

// OFFSET: 0x0077BE10
// FUNCTION: fn_08425
bool CSkillActionMgr::OnSkillHit(int skillId, CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillHit(source, target, targetSkill);
  return true;
}

// OFFSET: 0x0077BED0
// FUNCTION: fn_08426
bool CSkillActionMgr::OnEfstUpdate(int efstId, int value, int statusValue2, int statusValue3, CRenderObject* actor) {
  std::unique_ptr<CEfstAction> action(CreateEfstAction(this, efstId));
  if (!action) {
    return false;
  }

  action->OnEfstUpdate(efstId, value, statusValue2, statusValue3, actor);
  return true;
}

// OFFSET: 0x0077BF90
// FUNCTION: fn_08427
bool CSkillActionMgr::OnSkillObjectRemove(int skillId, CRenderObject* object) {
  std::unique_ptr<CSkillObjectAction> action(CreateSkillObjectAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillObjectRemove(object);
  return true;
}

// OFFSET: 0x0077C050
// FUNCTION: fn_08428
bool CSkillActionMgr::OnSkillObjectCreate(int skillId, int level, CRenderObject* object, CGameActor* source) {
  std::unique_ptr<CSkillObjectAction> action(CreateSkillObjectAction(this, skillId));
  if (!action) {
    return false;
  }

  action->OnSkillObjectCreate(level, object, source);
  return true;
}

// OFFSET: 0x0077C110
// FUNCTION: fn_08429
bool CSkillActionMgr::OnSkillEffect(int skillId, int level, CGameActor* source, CGameActor* target) {
  std::unique_ptr<CSkillAction> action(CreateSkillAction(this, skillId));
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
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, EF_ACIDIFIED_ZONE_GROUND_CAST, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
    if (CEffect* effect = g_effectMgr->Spawn(source->render.accountId, EF_ACIDIFIED_ZONE_GROUND_CAST_BOTTOM, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// OFFSET: 0x0077CD90
// FUNCTION: fn_08438
void CSkillAction_BO_MAYHEMIC_THORNS::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mayhemic_thorns_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D020
// FUNCTION: fn_08440
void CSkillAction_BO_EXPLOSIVE_POWDER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("explosive_powder"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D310
// FUNCTION: fn_08443
void CSkillAction_BO_MYSTERY_POWDER::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077D490
// FUNCTION: fn_08444
void CEfstAction_MYSTERY_POWDER::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mistery_powder_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  const vector3d& position = target->render.position;
  for (int delay = 0; delay < 600; delay += 200) {
    const int quadrant = std::ranges::max(std::rand() % 4, 0);
    if (CEffect* effect = spawnDustExplosion(quadrant, position)) {
      effect->SetDelay(delay);
    }
    if (CEffect* effect = spawnDustExplosion((quadrant + 2) % 4, position)) {
      effect->SetDelay(delay);
    }
  }
}

// OFFSET: 0x0077D9E0
// FUNCTION: fn_08448
void CSkillAction_BO_DUST_EXPLOSION::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// INLINED
static int RandomInRange(int low, int high) {
  if (high == 0) {
    return 0;
  }

  const int value = std::rand() % high;
  return value >= low ? value : low;
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
  int angle = RandomInRange(start, end);
  const float offsetX = ClientSin((static_cast<float>(angle) / 180.0f) * 3.141592f) * static_cast<float>(distance);

  distance = std::rand() % 20;
  if (distance < 10) {
    distance = 10;
  }

  angle = RandomInRange(start, end);
  const float offsetZ = ClientCos((static_cast<float>(angle) / 180.0f) * 3.141592f) * static_cast<float>(distance);

  const auto at = [&](float dx, float dz) { return vector3d{dx + position.x, position.y + 0.0f, dz + position.z}; };
  const vector3d spawnPosition = at(offsetX, offsetZ);
  EffectLists* effects = g_effectMgr;
  return effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("dust_explosion"), spawnPosition, 0, -1, 0, 0);
}

// OFFSET: 0x0077DED0
// FUNCTION: fn_45571
template <>
CEfstAction* NewAction<CEfstAction_MYSTERY_POWDER, CEfstAction>() {
  return new CEfstAction_MYSTERY_POWDER;
}

// OFFSET: 0x0077DF70
// FUNCTION: fn_08450
void CSkillAction_DK_VIGOR::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E0D0
// FUNCTION: fn_08451
void CEfstAction_VIGOR::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("vigor_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  return {LookupMessage(MSI_DK_VIGOR_OFF), 0xFF};
}

// OFFSET: 0x0077E3B0
// FUNCTION: fn_08456
void CSkillAction_DK_SERVANTWEAPON_ATK::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_down"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077E530
// FUNCTION: fn_08457
void CSkillAction_DK_SERVANTWEAPON_ATK::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  return {LookupMessage(MSI_DK_SERVANTWEAPON_OFF), 0xFF};
}

// OFFSET: 0x0077E810
// FUNCTION: fn_08460
int CSkillAction_DK_SERVANT_W_SIGN::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077E820
// FUNCTION: fn_08461
void CSkillAction_DK_SERVANT_W_SIGN::OnTargetSkillCast(CGameActor* source, CGameActor* targetActor, int) {
  if (source) {
    if (CServantWeaponNewEffect* effect = static_cast<CServantWeaponNewEffect*>(g_effectMgr->Find(source->render.accountId, EF_DK_SERVANTWEAPON))) {
      effect->Shoot(0x1453);
    }
  }

  if (targetActor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(targetActor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_down_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(targetActor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_down"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077EAD0
// FUNCTION: fn_08462
void CEfstAction_SERVANT_SIGN::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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

// OFFSET: 0x0077EF60
// FUNCTION: fn_08465
int CSkillAction_DK_SERVANT_W_PHANTOM::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x0077EF70
// FUNCTION: fn_08466
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x0077EFA0
// FUNCTION: fn_08467
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, EF_DK_SERVANTWEAPON);
  }
}

// OFFSET: 0x0077EFD0
// FUNCTION: fn_08468
void CSkillAction_DK_SERVANT_W_PHANTOM::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (!target) {
    return;
  }

  const int duration = static_cast<int>(!source ? 500.0f : source->render.speed * 100.0f + 100.0f);
  EffectLists* effects = g_effectMgr;
  CEffect* sign = effects->Find(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_sign"));
  if (sign) {
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_down"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servantweapon_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
    }
  } else {
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
    }
    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_phantom_sub"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
    }
  }
}

// OFFSET: 0x0077F600
// FUNCTION: fn_08469
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Find(source->render.accountId, EF_DK_SERVANTWEAPON);
  }
}

// OFFSET: 0x0077F630
// FUNCTION: fn_08470
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (source) {
    const int duration = static_cast<int>(source->render.speed * 100.0f + 100.0f);
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_collect"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetDelay(duration);
    }
  }

  if (target) {
    const int roll = std::rand() % 2;
    const int side = roll >= 0 ? roll : 0;
    if (side == 0) {
      EffectLists* effects = g_effectMgr;
      effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_leftdown"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    } else {
      EffectLists* effects = g_effectMgr;
      effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_rightdown"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    }
  }
}

// OFFSET: 0x0077F9B0
// FUNCTION: fn_08471
void CSkillAction_DK_SERVANT_W_DEMOL::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("servant_demolition_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x0077FB40
// FUNCTION: fn_08473
void CSkillAction_DK_STORMSLASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source) {
    return;
  }

  const int delay = static_cast<int>(source->render.speed * 50.0f + 30.0f);
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(std::format("stormslash_{}", level)), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (!effect) {
    return;
  }

  effect->SetDelay(delay);
  for (int i = 0; i < level; ++i) {
    const int variant = std::rand() % 4 + 1;
    effect->AddEvent(0, std::format("effect\\DK_STORMSLASH{}.wav", variant).c_str(), i * 15, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x0077FD90
// FUNCTION: fn_08477
void CSkillAction_DK_MADNESS_CRUSHER::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("madness_crusher"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780020
// FUNCTION: fn_08479
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
      effect->SetLifeTime(level);
    }
  }
}

// INLINED
static int ScreenDirection(float angle) {
  if (angle > 60.0f && angle <= 120.0f) {
    return 0;
  }
  if (angle > 120.0f && angle <= 240.0f) {
    return 1;
  }
  if (angle > 240.0f && angle <= 300.0f) {
    return 2;
  }
  if ((angle > 300.0f && angle <= 360.0f) || (angle >= 0.0f && angle <= 60.0f)) {
    return 3;
  }
  return -1;
}

// OFFSET: 0x007801B0
// FUNCTION: fn_08480
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  const float angle = ScreenAngle(
    &source->render.position.x, &target->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values
  );
  const int direction = ScreenDirection(angle);
  if (direction == -1) {
    return;
  }

  const float scale = source->attackMotion * source->render.speed * 16.0f;
  bool flip = true;
  if (direction == 1 || direction == 2) {
    flip = false;
  }
  const bool horizontal = (direction & 1) != 0;
  const std::string name(horizontal ? "dragonic_pierce_horizon" : "dragonic_pierce_vertical");
  EffectLists* effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId(name), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
    float offset;
    if (horizontal) {
      effect->SetFlipX(flip);
      offset = flip ? 0.0f : -180.0f;
    } else {
      effect->SetFlipY(flip);
      offset = flip ? -90.0f : -270.0f;
    }
    float rotated = angle;
    rotated += offset;
    const int rotation = (static_cast<int>(rotated) + 360) % 360;
    effect->SetAngle(static_cast<float>(rotation));
  }

  effects = g_effectMgr;
  if (CEffect* effect = effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
    effect->SetDelay(static_cast<int>(scale));
  }
}

// OFFSET: 0x007805A0
// FUNCTION: fn_08481
void CSkillAction_DK_DRAGONIC_PIERCE::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_pierce_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780840
// FUNCTION: fn_08483
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00780870
// FUNCTION: fn_08484
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00780AF0
// FUNCTION: fn_08485
void CSkillAction_DK_DRAGONIC_BREATH::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (!target) {
    return;
  }

  for (int i = 0; i < 4; ++i) {
    const vector3d position = target->render.position;
    EffectLists* effects = g_effectMgr;
    CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("dragonic_breath_hit"), position, 0, -1, 0, 0);
    if (!effect) {
      break;
    }

    const int first = std::rand() % 10;
    const int second = std::rand() % 10;
    const int jitterX = second >= 0 ? second : 0;
    const int jitterY = first >= 0 ? first : 0;
    effect->positionVector = vector3d{static_cast<float>((i % 2) * 10 - jitterX) + target->render.position.x, static_cast<float>((i % 4) / 2 * 10 - jitterY) + target->render.position.y, target->render.position.z + 0.0f};
  }
}

// OFFSET: 0x00780DB0
// FUNCTION: fn_08487
void CSkillAction_DK_CHARGINGPIERCE::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00780DE0
// FUNCTION: fn_08488
void CSkillAction_DK_CHARGINGPIERCE::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_cast"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781070
// FUNCTION: fn_08489
EfstActionMessage CEfstAction_CHARGINGPIERCE::GetEfstEndMessage() {
  return {LookupMessage(MSI_DK_CHARGINGPIERCE_OFF), 0xFF};
}

// OFFSET: 0x007810A0
// FUNCTION: fn_08490
void CEfstAction_CHARGINGPIERCE_COUNT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (argument3) {
    const int targetId = argument3;
    this->OnEfstEnd(efstId, value, targetId, 0, actor);
    if (CRenderObject* target = g_modeMgr.GetGameMode()->scene->GetGameActorByAidIncludeMe(targetId)) {
      EffectLists* effects = g_effectMgr;
      effects->Spawn(targetId, CEZ2STREffectMgr::GetInstance()->EffectId("chargingpierce_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
      const CPlayer* player = g_modeMgr.GetGameMode()->scene->player;
      g_soundMgr->PlayWave(
        "effect\\DK_CHARGINGPIERCE3.wav", target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z, 250, 40, 1.0f,
        0
      );
    }
  } else if (actor) {
    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(actor->render.accountId, EF_DK_CHARGINGPIERCE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0)) {
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
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007815A0
// FUNCTION: fn_08494
void CSkillAction_DK_HACKANDSLASHER::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781710
// FUNCTION: fn_08495
int CSkillAction_DK_HACKANDSLASHER_ATK::GetSkillAttackMotion() {
  return 0;
}

// OFFSET: 0x00781720
// FUNCTION: fn_08496
void CSkillAction_DK_HACKANDSLASHER_ATK::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("hackandslash_hit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781B50
// FUNCTION: fn_08497
void CSkillAction_NPC_BLOCK_EXPLOSION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00781B80
// FUNCTION: fn_08498
void CSkillAction_NPC_BLOCK_EXPLOSION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_THE_W_PROTECTION_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, EF_ELEMENTAL_BUSTER_FIRE_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781C30
// FUNCTION: fn_08499
void CSkillAction_NPC_FROST_FIELD::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// OFFSET: 0x00781C60
// FUNCTION: fn_08500
void CSkillAction_NPC_FROST_FIELD::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_DESTRUCTIVE_HURRICANE_BOTTOM_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, EF_CRYSTAL_IMPACT_BOTTOM_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    const CPlayer* player = g_modeMgr.GetGameMode()->scene->player;
    const vector3d position = {target->render.position.x - player->render.position.x, 0.0f, target->render.position.z - player->render.position.z};
    g_soundMgr->PlayWave("effect\\NPC_FROST_FIELD.wav", position.x, position.y, position.z, 250, 40, 1.0f, 0);
  }
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, EF_NEW_DIAMONDDUST_BOTTOM, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781DB0
// FUNCTION: fn_08501
void CSkillAction_NPC_FROST_FIELD::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, EF_NEW_RAMPAGEBLASTER, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00781E00
// FUNCTION: fn_08502
void CEfstAction_FROST_STORM::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    g_effectMgr->Spawn(actor->render.accountId, EF_NEW_BATTLE_W_HIT_2, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  int GetSkillAttackMotion() override;
  void OnSkillCast(int level, CGameActor* source) override;
  void OnSkillEffect(int level, CGameActor* source, CGameActor* target) override;
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) override;
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
  void OnGroundSkillCast(int level, CGameActor* source, vector3d position) override;
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
  void OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) override;
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

// OFFSET: 0x00782850
// FUNCTION: fn_08515
int CSkillAction_NPC_AIMED_SHOWER::GetSkillAttackMotion() {
  return 2;
}

// OFFSET: 0x00781EA0
// FUNCTION: fn_08505
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00781ED0
// FUNCTION: fn_08506
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spark_blaster_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    const auto relativePosition = [&](const vector3d& listener) {
      return vector3d{target->render.position.x - listener.x, 0.0f, target->render.position.z - listener.z};
    };
    const vector3d position = relativePosition(g_modeMgr.GetGameMode()->scene->player->render.position);
    g_soundMgr->PlayWave("effect\\NPC_LIGHTNING_JUDGEMENT.wav", position.x, position.y, position.z, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x007820B0
// FUNCTION: fn_08507
void CSkillAction_NPC_LIGHTNING_JUDGEMENT::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, EF_UNK_1256, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00782100
// FUNCTION: fn_08508
void CEfstAction_GROGGY::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("groggy_on_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    g_effectMgr->Spawn(source->render.accountId, EF_NEW_JUDEX_TARGET_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    const vector3d& listener = g_modeMgr.GetGameMode()->scene->player->render.position;
    const vector3d position = {target->render.position.x - listener.x, 0.0f, target->render.position.z - listener.z};
    g_soundMgr->PlayWave("effect\\NPC_RESET_EFST.wav", position.x, position.y, position.z, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00782470
// FUNCTION: fn_08512
void CEfstAction_TARGET_MARKER::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("target_marker_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("target_marker_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// OFFSET: 0x00782890
// FUNCTION: fn_08517
void CSkillAction_NPC_AIMED_SHOWER::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_TETRA_VORTEX_WIND_EXP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    const vector3d& listener = g_modeMgr.GetGameMode()->scene->player->render.position;
    const vector3d position = {target->render.position.x - listener.x, 0.0f, target->render.position.z - listener.z};
    g_soundMgr->PlayWave("effect\\NPC_AIMED_SHOWER.wav", position.x, position.y, position.z, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00782950
// FUNCTION: fn_08518
void CSkillAction_NPC_AIMED_SHOWER::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    g_effectMgr->Spawn(target->render.accountId, EF_NEW_PRAMEN_REUSE, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007829A0
// FUNCTION: fn_08519
void CSkillAction_NPC_BLAZING_ERUPTION::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BLUECASTING, level / 16);
  }
}

// OFFSET: 0x007829D0
// FUNCTION: fn_08520
void CSkillAction_NPC_BLAZING_ERUPTION::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    g_effectMgr->Spawn(source->render.accountId, EF_TETRA_VORTEX_FIRE_GROUND, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(source->render.accountId, EF_TETRA_VORTEX_FIRE_EXP, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_VOLCANO, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    const vector3d& listener = g_modeMgr.GetGameMode()->scene->player->render.position;
    const vector3d position = {target->render.position.x - listener.x, 0.0f, target->render.position.z - listener.z};
    g_soundMgr->PlayWave("effect\\NPC_BLAZING_ERUPTION.wav", position.x, position.y, position.z, 250, 40, 1.0f, 0);
  }
}

// OFFSET: 0x00782B20
// FUNCTION: fn_08521
void CSkillAction_NPC_BLOCK_SEAL::OnSkillCast(int level, CGameActor* source) {
  if (source) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00782B50
// FUNCTION: fn_08522
void CEfstAction_BLOCK_SEAL::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("npc_block_seal_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    g_effectMgr->Spawn(actor->render.accountId, EF_UNK_2400, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00783320
// FUNCTION: fn_08527
void CSkillAction_HN_DOUBLEBOWLINGBASH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("double_bowlingbash"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007834A0
// FUNCTION: fn_08528
void CSkillAction_HN_DOUBLEBOWLINGBASH::OnSkillDamage(CGameActor* source, CGameActor* target, int hitIndex) {
  if (target) {
    static_cast<EffectActor*>(target)->LaunchEffect(EF_BOWLINGBASH, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_SONICBLOW, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("mega_sonic_blow"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x007836D0
// FUNCTION: fn_08533
void CSkillAction_HN_SHIELD_CHAIN_RUSH::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("shield_chain_rush"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_PIERCEBODY, level / 16);
  }
}

// OFFSET: 0x00783890
// FUNCTION: fn_08536
void CSkillAction_HN_SPIRAL_PIERCE_MAX::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (!source || !target) {
    return;
  }

  if (!g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF)) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max_aura_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
  EffectLists* effects = g_effectMgr;
  effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max_aura"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  const float angle = ScreenAngle(&source->render.position.x, &target->render.position.x, g_modeMgr.GetGameMode()->camera->frame.view.values);
  const vector3d position = source->render.position;
  effects = g_effectMgr;
  CEffect* effect = effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("spiral_pierce_max"), position, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL3, level / 16);
  }
}

// INLINED
static CEffect* SpawnAt(EffectLists* effects, int id, const vector3d& position) {
  return effects->Spawn(0, id, position, 0, -1, 0, 0);
}

// OFFSET: 0x00783CD0
// FUNCTION: fn_08539
void CSkillAction_HN_METEOR_STORM_BUSTER::OnGroundSkillEffect(vector3d position, int argument2, int argument3) {
  EffectLists* effects = g_effectMgr;
  SpawnAt(effects, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_meteor_bottom"), position);
  effects = g_effectMgr;
  SpawnAt(effects, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_meteor"), position);
  effects = g_effectMgr;
  SpawnAt(effects, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster_bottom"), position);
  effects = g_effectMgr;
  SpawnAt(effects, CEZ2STREffectMgr::GetInstance()->EffectId("meteor_storm_buster"), position);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL5, level / 16);
  }
}

// OFFSET: 0x00784160
// FUNCTION: fn_08544
void CSkillAction_HN_JUPITEL_THUNDER_STORM::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source && target) {
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    g_effectMgr->Spawn(source->render.accountId, EF_HN_JUPITEL_THUNDER_STORM, source->render.position, target->render.accountId, level, 0, 0);
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

  static_cast<EffectActor*>(source)->LaunchEffect(EF_NEW_FROSTMISTY, level / 16);
  EffectLists* effects = g_effectMgr;
  CEffect* effect = effects->Spawn(source->render.accountId, g_serviceOptions.Get(TT_MIN_EFFECT_ON_OFF) ? 1543 : 1446, vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  if (effect) {
    effect->SetLifeTime(level);
  }
}

// OFFSET: 0x007842D0
// FUNCTION: fn_08548
void CSkillAction_HN_JACK_FROST_NOVA::OnGroundSkillCast(int level, CGameActor* source, vector3d position) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  if (mode) {
    g_modeMgr.GetGameMode()->LaunchDummyEffect(&position, 0x5A8, level / 24, 0);
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
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_start_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_loop_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x007847E0
// FUNCTION: fn_08551
void CSkillObjectAction_HN_JACK_FROST_NOVA::OnSkillObjectRemove(CRenderObject* object) {
  if (object) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(0, CEZ2STREffectMgr::GetInstance()->EffectId("jack_frost_nova_end"), object->render.position, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_NEW_EARTHDRIVE_CAST, level / 16);
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
  for (int index = 0; index < count; ++index) {
    float offsetZ;
    if (rows.empty()) {
      offsetZ = 0.0f;
    } else {
      offsetZ = static_cast<float>((rows.front() - count / 2) * 5.0);
      rows.erase(rows.begin());
    }
    float offsetX;
    if (columns.empty()) {
      offsetX = 0.0f;
    } else {
      offsetX = static_cast<float>((columns.front() - count / 2) * 5.0);
      columns.erase(columns.begin());
    }

    EffectLists* effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          0, CEZ2STREffectMgr::GetInstance()->EffectId("hells_drive_bottom"), source->render.position, 0, -1, 0, 0
        )) {
      const vector3d position = {offsetX + source->render.position.x, source->render.position.y + 0.0f, offsetZ + source->render.position.z};
      effect->position[0] = position.x;
      effect->position[1] = position.y;
      effect->position[2] = position.z;
      effect->SetDelay(index * 32);
    }

    effects = g_effectMgr;
    if (CEffect* effect = effects->Spawn(
          0, CEZ2STREffectMgr::GetInstance()->EffectId(FormatOwnedString("hells_drive_%02d", RandomInRange(0, 3))), source->render.position, 0, -1, 0, 0
        )) {
      const vector3d position = {offsetX + source->render.position.x, source->render.position.y + 0.0f, offsetZ + source->render.position.z};
      effect->position[0] = position.x;
      effect->position[1] = position.y;
      effect->position[2] = position.z;
      effect->SetDelay(index * 32);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_ENHANCE, level / 16);
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL, level / 16);
  }
}

// OFFSET: 0x00784EA0
// FUNCTION: fn_08558
void CSkillObjectAction_HN_GROUND_GRAVITATION::OnSkillObjectCreate(int argument, CRenderObject* object, CGameActor* source) {
  EffectLists* effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_bottom_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_start"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_bottom_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  effects = g_effectMgr;
  effects->Spawn(object->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("ground_gravitation_loop"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL6, level / 16);
  }
}

// OFFSET: 0x00785600
// FUNCTION: fn_08562
void CSkillAction_HN_NAPALM_VULCAN_STRIKE::OnSkillHit(CGameActor* source, CGameActor* target, CRenderObject* targetSkill) {
  if (target) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(target->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("napalm_vulcan_strike"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_RING_WHITE, level / 16);
  }
}

// OFFSET: 0x007857C0
// FUNCTION: fn_08565
void CSkillAction_HN_BREAKINGLIMIT::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("breakinglimit"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785930
// FUNCTION: fn_08566
void CEfstAction_BREAKINGLIMIT::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("breakinglimit_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    static_cast<EffectActor*>(source)->LaunchEffect(EF_BEGINSPELL_RING_WHITE, level / 16);
  }
}

// OFFSET: 0x00785C40
// FUNCTION: fn_08571
void CSkillAction_HN_RULEBREAK::OnSkillEffect(int level, CGameActor* source, CGameActor* target) {
  if (source) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rulebreak"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}

// OFFSET: 0x00785DA0
// FUNCTION: fn_08572
void CEfstAction_RULEBREAK::OnEfstBegin(int efstId, int value, int argument3, int argument4, CRenderObject* actor, bool argument6) {
  if (actor) {
    EffectLists* effects = g_effectMgr;
    effects->Spawn(actor->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("rulebreak_buff"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
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
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("overcoming_crisis"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
    effects = g_effectMgr;
    effects->Spawn(source->render.accountId, CEZ2STREffectMgr::GetInstance()->EffectId("overcoming_crisis_bottom"), vector3d{0.0f, 0.0f, 0.0f}, 0, -1, 0, 0);
  }
}


// OFFSET: 0x00786450
// FUNCTION: fn_08576
CSkillObjectActionRegistration::CSkillObjectActionRegistration(int skillId, std::function<CSkillObjectAction*()> factory) {
  CSkillActionMgr::GetInstance()->RegisterSkillObjectAction(skillId, factory);
}

// GLOBAL: gl_25720
static EfstActionRegistrar s_efstActionChangeSizeMonster{EFST_CHANGE_SIZE_MONSTER, NewAction<CEfstAction_ChangeSizeMonster, CEfstAction>};
// GLOBAL: gl_25721
static EfstActionRegistrar s_efstActionChangeSizeNPC{EFST_CHANGE_SIZE, NewAction<CEfstAction_ChangeSizeNPC, CEfstAction>};
// GLOBAL: gl_25722
static EfstActionRegistrar s_efstActionGenderChange(1559, NewAction<CEfstAction_GenderChange, CEfstAction>);
// GLOBAL: gl_25723
static EfstActionRegistrar s_efstActionShowEffect1{EFST_SHOW_EFFECT1, NewAction<CEfstAction_ShowEffect1, CEfstAction>};
// GLOBAL: gl_25724
static EfstActionRegistrar s_efstActionShowEffect2{EFST_SHOW_EFFECT2, NewAction<CEfstAction_ShowEffect2, CEfstAction>};
// GLOBAL: gl_25725
static EfstActionRegistrar s_efstActionShowEffect3{EFST_SHOW_EFFECT3, NewAction<CEfstAction_ShowEffect3, CEfstAction>};

struct EfstActionRegistration {
  // INLINED
  __forceinline EfstActionRegistration(int efstId, std::function<CEfstAction*()> factory) {
    CSkillActionMgr::GetInstance()->RegisterEfstAction(efstId, factory);
  }
};

struct SkillActionRegistration {
  // INLINED
  __forceinline SkillActionRegistration(int skillId, std::function<CSkillAction*()> factory) {
    CSkillActionMgr::GetInstance()->RegisterSkillAction(skillId, factory);
  }
};

// OFFSET: 0x00453F10
// FUNCTION: fn_45562
static EfstActionRegistration s_efstActionRegistered722(722, NewAction<CEfstAction_UNLIMIT, CEfstAction>);
// OFFSET: 0x00453FE0
// FUNCTION: fn_45563
static SkillActionRegistration s_skillActionRegistered5002(5002, NewAction<CSkillAction_RA_UNLIMIT, CSkillAction>);

// GLOBAL: gl_25726
static SkillActionRegistrar s_skillActionAgEnergyConversion{AG_ENERGY_CONVERSION, NewAction<CSkillAction_AG_ENERGY_CONVERSION, CSkillAction>};
// GLOBAL: gl_25727
static SkillActionRegistrar s_skillActionCdDivinusFlos{CD_DIVINUS_FLOS, NewAction<CSkillAction_CD_DIVINUS_FLOS, CSkillAction>};
// GLOBAL: gl_25728
static EfstActionRegistrar s_efstActionDancingKnife{EFST_DANCING_KNIFE, NewAction<CEfstAction_DANCING_KNIFE, CEfstAction>};
// GLOBAL: gl_25729
static EfstActionRegistrar s_efstActionShadowExceed{EFST_SHADOW_EXCEED, NewAction<CEfstAction_SHADOW_EXCEED, CEfstAction>};
// GLOBAL: gl_25730
static EfstActionRegistrar s_efstActionShadowWeapon{EFST_SHADOW_WEAPON, NewAction<CEfstAction_SHADOW_WEAPON, CEfstAction>};
// GLOBAL: gl_25731
static SkillActionRegistrar s_skillActionShcCrossSlash{SHC_CROSS_SLASH, NewAction<CSkillAction_SHC_CROSS_SLASH, CSkillAction>};
// GLOBAL: gl_25732
static SkillActionRegistrar s_skillActionShcDancingKnife{SHC_DANCING_KNIFE, NewAction<CSkillAction_SHC_DANCING_KNIFE, CSkillAction>};
// GLOBAL: gl_25733
static SkillActionRegistrar s_skillActionShcEnchantingShadow{SHC_ENCHANTING_SHADOW, NewAction<CSkillAction_SHC_ENCHANTING_SHADOW, CSkillAction>};
// GLOBAL: gl_25734
static SkillActionRegistrar s_skillActionShcImpactCrater{SHC_IMPACT_CRATER, NewAction<CSkillAction_SHC_IMPACT_CRATER, CSkillAction>};
// GLOBAL: gl_25735
static SkillActionRegistrar s_skillActionShcShadowExceed{SHC_SHADOW_EXCEED, NewAction<CSkillAction_SHC_SHADOW_EXCEED, CSkillAction>};
// GLOBAL: gl_25736
static SkillActionRegistrar s_skillActionShcShadowStab{SHC_SHADOW_STAB, NewAction<CSkillAction_SHC_SHADOW_STAB, CSkillAction>};
// GLOBAL: gl_25738
static SkillActionRegistrar s_skillActionAbcAbyssFlame{ABC_ABYSS_FLAME, NewAction<CSkillAction_ABC_ABYSS_FLAME, CSkillAction>};
// GLOBAL: gl_25739
static SkillActionRegistrar s_skillActionAbcAbyssFlameAtk{ABC_ABYSS_FLAME_ATK, NewAction<CSkillAction_ABC_ABYSS_FLAME_ATK, CSkillAction>};
// GLOBAL: gl_25740
static SkillActionRegistrar s_skillActionAbcChasingBreak{ABC_CHASING_BREAK, NewAction<CSkillAction_ABC_CHASING_BREAK, CSkillAction>};
// GLOBAL: gl_25741
static SkillActionRegistrar s_skillActionAbcChasingShot{ABC_CHASING_SHOT, NewAction<CSkillAction_ABC_CHASING_SHOT, CSkillAction>};
// GLOBAL: gl_25742
static SkillActionRegistrar s_skillActionAbcHitAndSliding{ABC_HIT_AND_SLIDING, NewAction<CSkillAction_ABC_HIT_AND_SLIDING, CSkillAction>};
// GLOBAL: gl_25743
static SkillActionRegistrar s_skillActionEmPsychicStream{EM_PSYCHIC_STREAM, NewAction<CSkillAction_EM_PSYCHIC_STREAM, CSkillAction>};
// GLOBAL: gl_25744
static SkillActionRegistrar s_skillActionIqBlazingFlameBlast{IQ_BLAZING_FLAME_BLAST, NewAction<CSkillAction_IQ_BLAZING_FLAME_BLAST, CSkillAction>};

// OFFSET: 0x00457500
// FUNCTION: fn_45564
static EfstActionRegistration s_efstActionRegistered1665(1665, NewAction<CEfstAction_MYSTERY_POWDER, CEfstAction>);

// GLOBAL: gl_25745
static SkillActionRegistrar s_skillActionBoDustExplosion{BO_DUST_EXPLOSION, NewAction<CSkillAction_BO_DUST_EXPLOSION, CSkillAction>};
// GLOBAL: gl_25746
static SkillActionRegistrar s_skillActionBoExplosivePowder{BO_EXPLOSIVE_POWDER, NewAction<CSkillAction_BO_EXPLOSIVE_POWDER, CSkillAction>};
// GLOBAL: gl_25747
static SkillActionRegistrar s_skillActionBoMayhemicThorns{BO_MAYHEMIC_THORNS, NewAction<CSkillAction_BO_MAYHEMIC_THORNS, CSkillAction>};
// GLOBAL: gl_25748
static SkillActionRegistrar s_skillActionBoMysteryPowder{BO_MYSTERY_POWDER, NewAction<CSkillAction_BO_MYSTERY_POWDER, CSkillAction>};
// GLOBAL: gl_25749
static EfstActionRegistrar s_efstActionChargingpierce{EFST_CHARGINGPIERCE, NewAction<CEfstAction_CHARGINGPIERCE, CEfstAction>};
// GLOBAL: gl_25750
static EfstActionRegistrar s_efstActionChargingpierceCount{EFST_CHARGINGPIERCE_COUNT, NewAction<CEfstAction_CHARGINGPIERCE_COUNT, CEfstAction>};
// GLOBAL: gl_25751
static EfstActionRegistrar s_efstActionServantweapon{EFST_SERVANTWEAPON, NewAction<CEfstAction_SERVANTWEAPON, CEfstAction>};
// GLOBAL: gl_25752
static EfstActionRegistrar s_efstActionServantSign{EFST_SERVANT_SIGN, NewAction<CEfstAction_SERVANT_SIGN, CEfstAction>};
// GLOBAL: gl_25753
static EfstActionRegistrar s_efstActionVigor{EFST_VIGOR, NewAction<CEfstAction_VIGOR, CEfstAction>};
// GLOBAL: gl_25754
static SkillActionRegistrar s_skillActionDkChargingpierce{DK_CHARGINGPIERCE, NewAction<CSkillAction_DK_CHARGINGPIERCE, CSkillAction>};
// GLOBAL: gl_25755
static SkillActionRegistrar s_skillActionDkDragonicBreath{DK_DRAGONIC_BREATH, NewAction<CSkillAction_DK_DRAGONIC_BREATH, CSkillAction>};
// GLOBAL: gl_25756
static SkillActionRegistrar s_skillActionDkDragonicPierce{DK_DRAGONIC_PIERCE, NewAction<CSkillAction_DK_DRAGONIC_PIERCE, CSkillAction>};
// GLOBAL: gl_25757
static SkillActionRegistrar s_skillActionDkHackandslasher{DK_HACKANDSLASHER, NewAction<CSkillAction_DK_HACKANDSLASHER, CSkillAction>};
// GLOBAL: gl_25758
static SkillActionRegistrar s_skillActionDkHackandslasherAtk{DK_HACKANDSLASHER_ATK, NewAction<CSkillAction_DK_HACKANDSLASHER_ATK, CSkillAction>};
// GLOBAL: gl_25759
static SkillActionRegistrar s_skillActionDkMadnessCrusher{DK_MADNESS_CRUSHER, NewAction<CSkillAction_DK_MADNESS_CRUSHER, CSkillAction>};
// GLOBAL: gl_25760
static SkillActionRegistrar s_skillActionDkServantweaponAtk{DK_SERVANTWEAPON_ATK, NewAction<CSkillAction_DK_SERVANTWEAPON_ATK, CSkillAction>};
// GLOBAL: gl_25761
static SkillActionRegistrar s_skillActionDkServantWDemol{DK_SERVANT_W_DEMOL, NewAction<CSkillAction_DK_SERVANT_W_DEMOL, CSkillAction>};
// GLOBAL: gl_25762
static SkillActionRegistrar s_skillActionDkServantWPhantom{DK_SERVANT_W_PHANTOM, NewAction<CSkillAction_DK_SERVANT_W_PHANTOM, CSkillAction>};
// GLOBAL: gl_25763
static SkillActionRegistrar s_skillActionDkServantWSign{DK_SERVANT_W_SIGN, NewAction<CSkillAction_DK_SERVANT_W_SIGN, CSkillAction>};
// GLOBAL: gl_25764
static SkillActionRegistrar s_skillActionDkStormslash{DK_STORMSLASH, NewAction<CSkillAction_DK_STORMSLASH, CSkillAction>};
// GLOBAL: gl_25765
static SkillActionRegistrar s_skillActionDkVigor{DK_VIGOR, NewAction<CSkillAction_DK_VIGOR, CSkillAction>};
// GLOBAL: gl_25766
static EfstActionRegistrar s_efstActionBlockSeal{EFST_BLOCK_SEAL, NewAction<CEfstAction_BLOCK_SEAL, CEfstAction>};
// GLOBAL: gl_25767
static EfstActionRegistrar s_efstActionFrostStorm{EFST_FROST_STORM, NewAction<CEfstAction_FROST_STORM, CEfstAction>};
// GLOBAL: gl_25768
static EfstActionRegistrar s_efstActionGroggy{EFST_GROGGY, NewAction<CEfstAction_GROGGY, CEfstAction>};
// GLOBAL: gl_25769
static EfstActionRegistrar s_efstActionTargetMarker{EFST_TARGET_MARKER, NewAction<CEfstAction_TARGET_MARKER, CEfstAction>};
// GLOBAL: gl_25770
static SkillActionRegistrar s_skillActionNpcAimedShower{NPC_AIMED_SHOWER, NewAction<CSkillAction_NPC_AIMED_SHOWER, CSkillAction>};
// GLOBAL: gl_25771
static SkillActionRegistrar s_skillActionNpcBlazingEruption{NPC_BLAZING_ERUPTION, NewAction<CSkillAction_NPC_BLAZING_ERUPTION, CSkillAction>};
// GLOBAL: gl_25772
static SkillActionRegistrar s_skillActionNpcBlockExplosion{NPC_BLOCK_EXPLOSION, NewAction<CSkillAction_NPC_BLOCK_EXPLOSION, CSkillAction>};
// GLOBAL: gl_25773
static SkillActionRegistrar s_skillActionNpcBlockSeal{NPC_BLOCK_SEAL, NewAction<CSkillAction_NPC_BLOCK_SEAL, CSkillAction>};
// GLOBAL: gl_25774
static SkillActionRegistrar s_skillActionNpcFrostField{NPC_FROST_FIELD, NewAction<CSkillAction_NPC_FROST_FIELD, CSkillAction>};
// GLOBAL: gl_25775
static SkillActionRegistrar s_skillActionNpcLightningJudgement{NPC_LIGHTNING_JUDGEMENT, NewAction<CSkillAction_NPC_LIGHTNING_JUDGEMENT, CSkillAction>};
// GLOBAL: gl_25776
static SkillActionRegistrar s_skillActionNpcResetEfst{NPC_RESET_EFST, NewAction<CSkillAction_NPC_RESET_EFST, CSkillAction>};
// GLOBAL: gl_25777
static EfstActionRegistrar s_efstActionBreakinglimit{EFST_BREAKINGLIMIT, NewAction<CEfstAction_BREAKINGLIMIT, CEfstAction>};
// GLOBAL: gl_25778
static EfstActionRegistrar s_efstActionRulebreak{EFST_RULEBREAK, NewAction<CEfstAction_RULEBREAK, CEfstAction>};
// GLOBAL: gl_25779
static CSkillObjectActionRegistration s_skillObjectAction296(296, NewAction<CSkillObjectAction_HN_GROUND_GRAVITATION, CSkillObjectAction>);
// GLOBAL: gl_25780
static CSkillObjectActionRegistration s_skillObjectAction295(295, NewAction<CSkillObjectAction_HN_JACK_FROST_NOVA, CSkillObjectAction>);
// GLOBAL: gl_25781
static SkillActionRegistrar s_skillActionHnBreakinglimit{HN_BREAKINGLIMIT, NewAction<CSkillAction_HN_BREAKINGLIMIT, CSkillAction>};
// GLOBAL: gl_25782
static SkillActionRegistrar s_skillActionHnDoublebowlingbash{HN_DOUBLEBOWLINGBASH, NewAction<CSkillAction_HN_DOUBLEBOWLINGBASH, CSkillAction>};
// GLOBAL: gl_25783
static SkillActionRegistrar s_skillActionHnGroundGravitation{HN_GROUND_GRAVITATION, NewAction<CSkillAction_HN_GROUND_GRAVITATION, CSkillAction>};
// GLOBAL: gl_25784
static SkillActionRegistrar s_skillActionHnHellsDrive{HN_HELLS_DRIVE, NewAction<CSkillAction_HN_HELLS_DRIVE, CSkillAction>};
// GLOBAL: gl_25785
static SkillActionRegistrar s_skillActionHnJackFrostNova{HN_JACK_FROST_NOVA, NewAction<CSkillAction_HN_JACK_FROST_NOVA, CSkillAction>};
// GLOBAL: gl_25786
static SkillActionRegistrar s_skillActionHnJupitelThunderStorm{HN_JUPITEL_THUNDER_STORM, NewAction<CSkillAction_HN_JUPITEL_THUNDER_STORM, CSkillAction>};
// GLOBAL: gl_25787
static SkillActionRegistrar s_skillActionHnMegaSonicBlow{HN_MEGA_SONIC_BLOW, NewAction<CSkillAction_HN_MEGA_SONIC_BLOW, CSkillAction>};
// GLOBAL: gl_25788
static SkillActionRegistrar s_skillActionHnMeteorStormBuster{HN_METEOR_STORM_BUSTER, NewAction<CSkillAction_HN_METEOR_STORM_BUSTER, CSkillAction>};
// GLOBAL: gl_25789
static SkillActionRegistrar s_skillActionHnNapalmVulcanStrike{HN_NAPALM_VULCAN_STRIKE, NewAction<CSkillAction_HN_NAPALM_VULCAN_STRIKE, CSkillAction>};
// GLOBAL: gl_25790
static SkillActionRegistrar s_skillActionHnOvercomingCrisis{HN_OVERCOMING_CRISIS, NewAction<CSkillAction_HN_OVERCOMING_CRISIS, CSkillAction>};
// GLOBAL: gl_25791
static SkillActionRegistrar s_skillActionHnRulebreak{HN_RULEBREAK, NewAction<CSkillAction_HN_RULEBREAK, CSkillAction>};
// GLOBAL: gl_25792
static SkillActionRegistrar s_skillActionHnShieldChainRush{HN_SHIELD_CHAIN_RUSH, NewAction<CSkillAction_HN_SHIELD_CHAIN_RUSH, CSkillAction>};
// GLOBAL: gl_25793
static SkillActionRegistrar s_skillActionHnSpiralPierceMax{HN_SPIRAL_PIERCE_MAX, NewAction<CSkillAction_HN_SPIRAL_PIERCE_MAX, CSkillAction>};
