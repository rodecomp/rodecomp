#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Contents/World/WorldScene.h"
#include "Resource/World/TerrainResource.h"
#include "Script/LuaScript.h"


#include <memory>

class CEfstAction_ChangeSize : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_GenderChange : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_ShowEffect : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CSkillAction_RA_UNLIMIT : public CSkillAction {
public:
  void Slot08(int level, CGameActor* source) override;
};

class CEfstAction_UNLIMIT : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_WEAPON : public CEfstAction {
public:
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CEfstAction_DANCING_KNIFE : public CEfstAction {
public:
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
};

class CEfstAction_SHADOW_EXCEED : public CEfstAction {
public:
  void Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) override;
  void Slot02(int efstId, int value, int argument3, int argument4, CRenderObject* actor) override;
  EfstActionMessage Slot03() override;
  EfstActionMessage Slot04() override;
};

class CSkillAction_ABC_CHASING_BREAK : public CSkillAction {
public:
  bool Slot07() override;
};

class CSkillAction_ABC_CHASING_SHOT : public CSkillAction {
public:
  bool Slot07() override;
};

class CSkillAction_ABC_ABYSS_FLAME_ATK : public CSkillAction {
public:
  bool Slot03(int argument) override;
};

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

// OFFSET: 0x00777E80
// FUNCTION: fn_08366
void CEfstAction_SHADOW_EXCEED::Slot01(int efstId, int argument2, int argument3, int argument4, CRenderObject* actor) {
  if (!actor) {
    return;
  }

  auto* effectActor = static_cast<EffectActor*>(actor);
  while (effectActor->state.Attachments().HasId(1004)) {
    actor->SendObjectMessage(nullptr, 109, 0, 1004, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
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
  if (!g_world.ActiveGameMode() || !static_cast<CGameMode*>(g_world.ActiveGameMode())->scene ||
      !static_cast<CGameMode*>(g_world.ActiveGameMode())->scene->terrain) {
    return vector3d{0.0f, 0.0f, 0.0f};
  }

  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
  static_cast<CGameMode*>(g_world.ActiveGameMode())->scene->terrain->Grid().CellOrigin(column, row, x, z);
  y = static_cast<CGameMode*>(g_world.ActiveGameMode())->scene->terrain->Grid().Height(x, z);
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
