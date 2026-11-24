#pragma once

#include <vector>
#include "UI/Window/WindowId.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Window/UIEnums.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UISpriteButton.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "Network/TextEncoding.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/CashEmotion/UICashEmotionSprBtn.h"
#include "UI/CashEmotion/UICashEmotionPack.h"
#include "UI/CashEmotion/UICashEmotionPiece.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Items/UILapineScrollBar.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UINormalMB.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"
#include "Contents/Schedule/CheckGDISchedule.h"
#include "Contents/Schedule/EmotionSchedule.h"
#include "Contents/Schedule/TimeScheduler.h"
#include "UI/Emotion/UIEmotionWnd.h"
#include "Client/Mode/CMode.h"
#include "Network/Packets/PacketId.h"
#include "Resource/MSGSTRINGID.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
}
#include <algorithm>
#include <array>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fmt/core.h>
#include <format>
#include <functional>
#include <iterator>
#include <map>
#include <new>
#include <utility>
#include <windows.h>

template __declspec(noinline) std::back_insert_iterator<std::vector<int>> std::copy(
  std::vector<int>::const_iterator, std::vector<int>::const_iterator, std::back_insert_iterator<std::vector<int>>
);
extern template std::vector<int>::vector(const std::vector<int>&);
template __declspec(noinline) std::unordered_map<std::string, CashEmotionCommand*>::iterator
std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::find<void>(const std::string&);
template __declspec(noinline) std::pair<std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::_Nodeptr, bool>
std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::_Try_emplace<const std::string&, CashEmotionCommand*&>(
  const std::string&, CashEmotionCommand*&
);
template __declspec(noinline) std::string fmt::format<int&, int&, int&, int&, int&>(
  fmt::format_string<int&, int&, int&, int&, int&>, int&, int&, int&, int&, int&
);
template __forceinline std::size_t std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::_Min_load_factor_buckets(const std::size_t) const noexcept;
template __forceinline std::size_t std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::_Desired_grow_bucket_count(const std::size_t) const noexcept;
template __forceinline void std::unordered_map<std::string, CashEmotionCommand*>::_Mybase::_Rehash_for_1();
template __forceinline std::function<int()>::~function() noexcept;
template __forceinline std::string fmt::format<int&>(fmt::format_string<int&>, int&);
template __forceinline std::vector<CashEmotionEntry>::~vector();
template __forceinline void std::vector<CashEmotionEntry>::_Tidy() noexcept;
