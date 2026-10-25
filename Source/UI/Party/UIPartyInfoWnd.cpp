#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/TextFitting.h"
#include "UI/Party/UIPartyInfoWnd.h"

#include <cstdio>

// OFFSET: 0x007BAE20
UIPartyInfoWnd::UIPartyInfoWnd(UIWindowContext& windows, UIButtonContext& buttons, PartyRowContext& row)
    : UIBitmapButton(windows, buttons), tanker(0), dealer(0), healer(0), assist(0), m_row(&row) {
  this->m_drawLabel = 0;
}

// OFFSET: 0x007BB740
// FUNCTION: fn_09285
UIPartyInfoWnd::~UIPartyInfoWnd() = default;

// OFFSET: 0x007BBF00
// FUNCTION: fn_09294
const char* UIPartyInfoWnd::TypeName(int value, ButtonServices& services) {
  if (value == 0) {
    return services.MessageText(3377);
  }

  return value >= 1 && value <= 6 ? services.MessageText(3359 + value) : "";
}

// OFFSET: 0x007C1E20
// FUNCTION: fn_09320
void UIPartyInfoWnd::SetInfo(const CAdvenPartyInfo& value) {
  this->m_disabled = 0;
  this->m_drawLabel = 1;
  this->memo = value.memo;
  char text[10] = {};
  _snprintf_s(text, sizeof(text), _TRUNCATE, "%d~%d", value.minimumLevel, value.maximumLevel);
  this->levels = text;
  this->tanker = value.tanker;
  this->healer = value.healer;
  this->dealer = value.dealer;
  this->assist = value.assist;
  this->type = value.type;
  this->name = value.characterName;
  this->info = value;
}

// OFFSET: 0x007C0420
// FUNCTION: fn_09312
void UIPartyInfoWnd::Reset() {
  this->m_drawLabel = 0;
  this->memo.clear();
  this->levels.clear();
  this->tanker = 1;
  this->dealer = 1;
  this->assist = 1;
  this->type = 0;
  this->name.clear();
  this->m_disabled = 1;
  this->Invalidate();
}

// OFFSET: 0x007BF8C0
// FUNCTION: fn_09304
void UIPartyInfoWnd::Paint() {
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);

  if (this->m_drawLabel == 0) {
    return;
  }

  auto& controls = *this->m_buttonContext;
  CBitmapRes* background = nullptr;

  if (this->geometry.unknown_0030 <= 2) {
    background = static_cast<CBitmapRes*>(controls.resources.Load(this->m_names[this->geometry.unknown_0030].c_str()));
  }

  if (this->m_disabled != 0 && !this->m_names[3].empty()) {
    background = static_cast<CBitmapRes*>(controls.resources.Load(this->m_names[3].c_str()));
  }

  if (background) {
    this->DrawBitmap(0, 0, &background->image, false);
  }

  const auto text = [&](int x, const char* value, int bytes = 0) { this->DrawTextAt(controls.fonts, controls.locale, x, 5, value, bytes, 0, 12, 0, 0, 0); };
  text(11, this->levels.c_str());
  text(64, TypeName(this->type, controls.services));
  constexpr char prefix[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\UIadventurerAgency\\";
  const char* jobs[] = {"def", "heal", "atk", "buf"};
  const bool flags[] = {this->info.tanker, this->info.healer, this->info.dealer, this->info.assist};

  for (int index = 0; index < 4; ++index) {
    const std::string path = std::string(prefix) + "btn_job_" + jobs[index] + (flags[index] ? "_on.bmp" : "_off.bmp");
    auto* resource = static_cast<CBitmapRes*>(controls.resources.Load(path.c_str()));
    this->DrawBitmap(109 + index * 18, 3, resource ? &resource->image : nullptr, false);
  }

  const std::string fitted = FitText(*this->geometry.surface, controls.fonts, controls.locale, this->name, 119);
  text(192, fitted.c_str(), static_cast<int>(fitted.size()));
  text(315, this->memo.c_str(), static_cast<int>(this->memo.size()));

  if (this->m_invalidateParent != 0 && this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }
}

// OFFSET: 0x007BFCA0
// FUNCTION: fn_09308
void UIPartyInfoWnd::Slot31(int x, int y) {
  if (this->m_disabled == 0) {
    this->UIButton::Slot31(x, y);
  }
}

// OFFSET: 0x007BFE90
// FUNCTION: fn_09310
void UIPartyInfoWnd::Slot33(int, int) {
  if (this->m_disabled != 0 || this->info.accountId == this->m_row->accountId || this->info.characterId == this->m_row->characterId) {
    return;
  }

  const std::string name = this->info.characterName;
  const char* relationship = this->m_row->services.IsFriend(name) ? this->m_buttonContext->services.MessageText(102) : nullptr;

  if (this->m_row->services.IsGuildMember(this->info.accountId)) {
    relationship = "GuildMember";
  }

  const std::string code = this->m_row->services.CharacterCode(this->info.characterId);
  this->m_row->services.ShowCharacter(name.c_str(), relationship, code.c_str());
}
