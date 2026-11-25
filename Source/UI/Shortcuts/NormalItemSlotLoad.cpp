#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/UserData/CharacterLinkedUserDataMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindowMgr.inl"
#include "UI/Window/UIWindow.h"

#include <cstring>
#include <json/value.h>

// OFFSET: 0x005E65E0
// FUNCTION: fn_03929
void CCharacterLinkedUserDataMgr::LoadNormalItemSlots(Json::Value& data) {
  Json::Value* const root = &data;
  NormalItemSlotWindow& saved = g_windowMgr.m_normalItemSlotWindow;
  bool& itemSlotActive = g_session.ItemSlotActive();

  const Json::Value& values = (*root)["NormalItemSlot"];

  if (values.isNull()) {
    return;
  }

  g_session.ClearNormalItemSlots();

  for (const Json::Value& value : values) {
    if (!value.isObject()) {
      continue;
    }

    const int item = value.get("ItemID", 0).asInt();
    const int index = value.get("SlotNum", 0).asInt();
    g_session.m_normalItemSlots[index].itemId = static_cast<std::uint32_t>(item);
  }

  const Json::Value& info = (*root)["SlotInfo"];

  if (info.empty()) {
    NormalItemSlotWindow defaults;
    defaults.Reset();
    saved.x = defaults.x;
    saved.y = defaults.y;
    saved.width = defaults.width;
    saved.height = defaults.height;
    saved.show = defaults.show;
  } else {
    const int x = info.get("SlotX", 0).asInt();
    const int y = info.get("SlotY", 0).asInt();
    const int width = info.get("SlotWidth", 0).asInt();
    const int height = info.get("SlotHeight", 0).asInt();
    const int show = info.get("SlotShow", 1).asInt();

    saved.x = x;
    saved.y = y;
    saved.width = width;
    saved.height = height;
    saved.show = show;
  }

  const Json::Value& active = (*root)["ItemSlotActive"];
  itemSlotActive = active.isNull() ? true : active.asBool();
}
