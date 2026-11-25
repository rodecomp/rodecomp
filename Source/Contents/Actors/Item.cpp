#include "Contents/Actors/Item.h"

#include "Contents/Actors/ActorPickNode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/Core/EffectLists.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/ItemResources.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Renderer.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

struct ItemFallEntry {
  /* 0000 */ char itemId[0x20];
  /* 0020 */ std::uint32_t aid;
  /* 0024 */ int column;
  /* 0028 */ int row;
  /* 002C */ int subColumn;
  /* 0030 */ int subRow;
  /* 0034 */ int falling;
  /* 0038 */ int count;
  /* 003C */ unsigned char identified;
  /* 003E */ std::uint16_t type;
  /* 0040 */ unsigned char showEffect;
  /* 0042 */ std::uint16_t grade;
};

// INLINED
static float TerrainHeight(const CGameMode* mode, float x, float z) {
  return mode->scene->terrain->Height(x, z);
}

// OFFSET: 0x00D8BE00
// FUNCTION: fn_22253
void CItem::SetRenderInfo(const float* bounds, float x, float y) {
  enum { ItemPickJob = 32003 };

  if (this->render.trackBounds) {
    // GLOBAL: gl_25149
    static const int minimumSize = static_cast<int>(g_renderer->UiScaleX() * 34.0f);
    ActorPickInfo* info = new ActorPickInfo;
    info->corners[0].x = bounds[0] < bounds[2] ? bounds[0] : bounds[2];
    info->corners[0].y = bounds[1];
    info->corners[0].z = bounds[4] + 300.0f;
    info->corners[1].x = bounds[0] > bounds[2] ? bounds[0] : bounds[2];
    info->corners[1].y = bounds[3];
    info->corners[1].z = bounds[4] + 300.0f;
    info->accountId = this->itemAid;
    info->job = ItemPickJob;
    info->healthState = 0;
    info->kind = 1;

    const int width = static_cast<int>(info->corners[1].x - info->corners[0].x);
    if (width < minimumSize) {
      const int grow = (minimumSize - width) / 2;
      info->corners[1].x += grow;
      info->corners[0].x -= grow;
    }

    const int height = static_cast<int>(info->corners[1].y - info->corners[0].y);
    if (height < minimumSize) {
      const int grow = (minimumSize - height) / 2;
      info->corners[1].y += grow;
      info->corners[0].y -= grow;
    }

    g_actorPickNode.AddPickInfo(info);
  }

  this->SetTlvert(x, y);
}

// OFFSET: 0x00D8B340
// FUNCTION: fn_22246
CItem::CItem() : name() {
  this->SubD8B5B0();
}

// OFFSET: 0x00D8B5A0
// FUNCTION: fn_22248
void CItem::SubD8B5A0() {
}

// OFFSET: 0x00D8B5B0
// FUNCTION: fn_22249
void CItem::SubD8B5B0() {
  this->render.shadowVisible = 1;
  this->render.trackBounds = 1;
  this->render.scale = 1.0f;
  this->render.overrideColor = 0xFFFFFF00;
}

// OFFSET: 0x00D8B5D0
// FUNCTION: fn_22250
bool CItem::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.motionStartTime) / 24.0);
  if (static_cast<int>(elapsed) % 92 < 90) {
    this->render.isSingleColor = 0;
  } else {
    this->render.isSingleColor = 1;
  }

  if (this->isFalling) {
    this->render.position.y = (elapsed * 0.083f + this->fallVelocity) * elapsed + this->fallOrigin;
    CGameMode* mode = g_modeMgr.GetGameMode();
    if (this->render.position.y > TerrainHeight(mode, this->render.position.x, this->render.position.z)) {
      this->render.position.y = TerrainHeight(mode, this->render.position.x, this->render.position.z);
      this->isFalling = 0;
    }
  }

  return true;
}

// OFFSET: 0x00D8B6D0
// FUNCTION: fn_22251
void CItem::Render(int argument) {
  if (this->render.visible) {
    this->DrawShadow(argument, this->render.scale * 0.4f);
    this->DrawSprite(argument, false, 0.0f);
  }
}

// INLINED
static inline vector3d DropEffectPosition(float x, float y, float z) {
  return vector3d{x, y, z};
}

// OFFSET: 0x00D8B530
// FUNCTION: fn_46711
int CItem::GetGradeDropEffect(int grade) const {
  int gradeEffectId = 2485;
  switch (grade) {
  case ITEM_DROP_GRADE_BLUE:
    gradeEffectId = EF_DROPITEM_BLUE;
    break;
  case ITEM_DROP_GRADE_YELLOW:
    gradeEffectId = EF_DROPITEM_YELLOW;
    break;
  case ITEM_DROP_GRADE_PURPLE:
    gradeEffectId = EF_DROPITEM_PURPLE;
    break;
  case ITEM_DROP_GRADE_GREEN:
    gradeEffectId = EF_DROPITEM_GREEN;
    break;
  case ITEM_DROP_GRADE_RED:
    gradeEffectId = EF_DROPITEM_RED;
    break;
  }
  return gradeEffectId;
}

// OFFSET: 0x00D8B720
// FUNCTION: fn_22252
void CItem::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  if (message != 4) {
    this->EffectActor::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }

  const ItemFallEntry* entry = reinterpret_cast<const ItemFallEntry*>(static_cast<std::intptr_t>(b));
  this->itemAid = entry->aid;
  this->name = entry->itemId;
  std::string itemName = this->name;
  mode->SubcellPosition(entry->column, entry->row, entry->subColumn, entry->subRow, this->render.position.x, this->render.position.z);
  this->render.position.y = TerrainHeight(mode, this->render.position.x, this->render.position.z);
  if (entry->falling) {
    this->fallVelocity = -0.6f;
    this->render.position.y -= 15.0f;
    this->isFalling = 1;
  } else {
    this->isFalling = 0;
  }
  this->fallOrigin = this->render.position.y;

  std::string spriteName = g_session.ItemDragResource(this->name.c_str(), entry->identified);
  spriteName += ".spr";
  std::string actionName = g_session.ItemDragResource(this->name.c_str(), entry->identified);
  actionName += ".act";
  this->SetSpriteName(spriteName.c_str());
  this->SetActionName(actionName.c_str());

  ITEM_INFO item;
  this->itemIndex = std::atoi(entry->itemId);
  this->itemEntryFlags = entry->type;
  this->itemId = static_cast<std::uint16_t>(std::atoi(entry->itemId));
  this->identified = entry->identified;
  item.itemId = itemName;
  item.identified = entry->identified;
  item.refiningLevel = 0;
  this->name = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), entry->count));
  this->GetActRes();
  this->render.motionStartTime = timeGetTime();

  if (entry->showEffect == 1) {
    const vector3d position = DropEffectPosition(
        this->render.position.x, TerrainHeight(mode, this->render.position.x, this->render.position.z),
        this->render.position.z);
    const int effectId = g_itemInfoMgr->GetEffectId(this->itemIndex);
    if (effectId >= 0 && !g_effectMgr->Spawn(entry->aid, effectId, position, 0, -1, 0, 0)) {
      if (CRagEffect* effect = this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->render.position = position;
      }
    }

    if (static_cast<std::uint16_t>(entry->grade - 1) <= 4) {
      const int gradeEffectId = GetGradeDropEffect(entry->grade);

      if (!g_effectMgr->Spawn(entry->aid, gradeEffectId, position, 0, -1, 0, 0)) {
        if (CRagEffect* effect = this->LaunchEffect(gradeEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
          effect->render.position = position;
        }
      }
    }
  }
}
