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
  const Json::Value& root = data;
  NormalItemSlotWindow& saved = g_windowMgr.m_normalItemSlotWindow;
  bool& itemSlotActive = g_session.ItemSlotActive();

  const Json::Value& values = root["NormalItemSlot"];

  if (values.isNull()) {
    return;
  }

  g_session.ClearNormalItemSlots();

  for (auto entry = values.begin(); entry != values.end(); ++entry) {
    const Json::Value& value = *entry;

    if (!value.isObject()) {
      continue;
    }

    const int item = value.get("ItemID", 0).asInt();
    const int index = value.get("SlotNum", 0).asInt();
    g_session.m_normalItemSlots[index].itemId = static_cast<std::uint32_t>(item);
  }

  const Json::Value& info = root["SlotInfo"];

  if (info.empty()) {
    NormalItemSlotWindow defaults;
    defaults.Reset();
    saved = defaults;
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

  const Json::Value& active = root["ItemSlotActive"];
  itemSlotActive = active.isNull() ? true : active.asBool();

  UIWindow* window = g_windowMgr.LookupWindow(WID_SHORTCUTWND);

  if (!window) {
    return;
  }

  window->geometry.x = saved.x;
  window->geometry.y = saved.y;
  window->geometry.width = saved.width;
  window->geometry.height = saved.height;

  g_windowMgr.ShowShortcutWindow(saved.show);
  window->SendMsg(nullptr, UM_SETINFO, reinterpret_cast<std::intptr_t>(&saved), 0, 0, 0);
  window->SendMsg(nullptr, UM_COMMAND, itemSlotActive ? 588 : 589, 0, 0, 0);
}
