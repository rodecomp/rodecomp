#include "Contents/Actors/Item.h"

#include "ActorPickNode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Items/ItemResources.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Renderer.h"
#include "Utility/Text/FormatString.h"

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
  /* 003E */ std::uint16_t unknown_003E;
  /* 0040 */ unsigned char showEffect;
  /* 0042 */ std::uint16_t grade;
};

// OFFSET: 0x00D8BE00
// FUNCTION: fn_22253
void CItem::SetRenderInfo(const float* bounds, float x, float y) {
  enum { ItemPickJob = 32003 };

  if (this->render.unknown_00A4Bytes[1]) {
    // GLOBAL: gl_25149
    static const int minimumSize = static_cast<int>(g_renderer->UiScaleX() * 34.0f);
    ActorPickInfo* info = new ActorPickInfo;
    info->left = std::min(bounds[0], bounds[2]);
    info->top = bounds[1];
    info->depth = bounds[4] + 300.0f;
    info->right = std::max(bounds[0], bounds[2]);
    info->bottom = bounds[3];
    info->depthOffset = bounds[4] + 300.0f;
    info->accountId = this->itemAid;
    info->job = ItemPickJob;
    info->unknown_0024 = 0;
    info->kind = 1;

    const int width = static_cast<int>(info->right - info->left);
    if (width < minimumSize) {
      const float grow = static_cast<float>((minimumSize - width) / 2);
      info->left -= grow;
      info->right += grow;
    }

    const int height = static_cast<int>(info->bottom - info->top);
    if (height < minimumSize) {
      const float grow = static_cast<float>((minimumSize - height) / 2);
      info->top -= grow;
      info->bottom += grow;
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
  this->render.unknown_00A4Bytes[0] = 1;
  this->render.unknown_00A4Bytes[1] = 1;
  this->render.scale = 1.0f;
  this->render.overrideColor = 0xFFFFFF00;
}

// OFFSET: 0x00D8B5D0
// FUNCTION: fn_22250
bool CItem::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.isSingleColor = static_cast<int>(elapsed) % 92 >= 90;

  if (this->isFalling) {
    this->render.position.y = (elapsed * 0.083f + this->fallVelocity) * elapsed + this->fallOrigin;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (this->render.position.y > mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z)) {
      this->render.position.y = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
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

// OFFSET: 0x00D8B720
// FUNCTION: fn_22252
void CItem::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  if (message != 4 || a != 0) {
    this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const ItemFallEntry* entry = reinterpret_cast<const ItemFallEntry*>(b);
  this->itemAid = entry->aid;
  this->name = entry->itemId;
  std::string itemName = this->name;
  mode->SubcellPosition(entry->column, entry->row, entry->subColumn, entry->subRow, this->render.position.x, this->render.position.z);
  this->render.position.y = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
  if (entry->falling) {
    this->fallVelocity = -0.6f;
    this->render.position.y -= 15.0f;
    this->isFalling = 1;
  } else {
    this->isFalling = 0;
  }
  this->fallOrigin = this->render.position.y;

  ItemDescriptions descriptions{g_itemDescriptions};
  std::string spriteName = ItemDragResource(descriptions, this->name.c_str(), entry->identified);
  spriteName += ".spr";
  std::string actionName = ItemDragResource(descriptions, this->name.c_str(), entry->identified);
  actionName += ".act";
  this->SetSpriteName(ResourceMgr::Instance(), spriteName.c_str());
  this->SetActionName(ResourceMgr::Instance(), actionName.c_str());

  ITEM_INFO item;
  this->itemIndex = std::atoi(entry->itemId);
  this->unknown_018C = entry->unknown_003E;
  this->itemId = static_cast<std::uint16_t>(std::atoi(entry->itemId));
  this->identified = entry->identified;
  item.itemId = itemName;
  item.identified = entry->identified;
  item.unknown_0060 = 0;
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  this->name = static_cast<const std::string&>(FormatOwnedString(LookupMessage(183), naming.DisplayName(item, 0).c_str(), entry->count));
  this->GetActRes();
  this->render.unknown_008C = timeGetTime();

  if (entry->showEffect == 1) {
    const float x = this->render.position.x;
    const float z = this->render.position.z;
    const float height = mode->scene->terrain->Grid().Height(x, z);
    const int effectId = g_itemInfoMgr->GetEffectId(this->itemIndex);
    if (effectId >= 0 && !g_effectMgr->Spawn(entry->aid, effectId, x, height, z, 0, -1, 0, 0)) {
      if (CRagEffect* effect = this->LaunchEffect(effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
        effect->render.position.x = x;
        effect->render.position.y = height;
        effect->render.position.z = z;
      }
    }

    if (static_cast<std::uint16_t>(entry->grade - 1) <= 4) {
      int gradeEffectId = 2485;
      switch (entry->grade) {
      case 1:
        gradeEffectId = 1869;
        break;
      case 2:
        gradeEffectId = 1189;
        break;
      case 3:
        gradeEffectId = 1190;
        break;
      case 4:
        gradeEffectId = 1870;
        break;
      case 5:
        gradeEffectId = 1871;
        break;
      }

      if (!g_effectMgr->Spawn(entry->aid, gradeEffectId, x, height, z, 0, -1, 0, 0)) {
        if (CRagEffect* effect = this->LaunchEffect(gradeEffectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f)) {
          effect->render.position.x = x;
          effect->render.position.y = height;
          effect->render.position.z = z;
        }
      }
    }
  }
}
