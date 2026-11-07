#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Text/TextFitting.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

// OFFSET: 0x007BAE20
// FUNCTION: fn_09277
UIPartyInfoWnd::UIPartyInfoWnd()
    : UIBitmapButton(), tanker(0), dealer(0), healer(0), assist(0) {
  this->m_drawLabel = 0;
}

UIPartyInfoWnd::~UIPartyInfoWnd() = default;

// OFFSET: 0x007BBEF0
// FUNCTION: fn_09293
CAdvenPartyInfo* UIPartyInfoWnd::Info() {
  return &this->info;
}

// OFFSET: 0x007BBF00
// FUNCTION: fn_09294
const char* UIPartyInfoWnd::TypeName(int value) {
  switch (value) {
  case 0:
    return LookupMessage(3377);
  case 1:
    return LookupMessage(3360);
  case 2:
    return LookupMessage(3361);
  case 3:
    return LookupMessage(3362);
  case 4:
    return LookupMessage(3363);
  case 5:
    return LookupMessage(3364);
  case 6:
    return LookupMessage(3365);
  default:
    return "";
  }
}

// OFFSET: 0x007C1E20
// FUNCTION: fn_09320
void UIPartyInfoWnd::SetInfo(const CAdvenPartyInfo& value) {
  this->m_disabled = 0;
  this->m_drawLabel = 1;
  this->memo = value.memo;
  char text[10] = {};
  StringFormat(text, sizeof(text), _TRUNCATE, "%d~%d", value.minimumLevel, value.maximumLevel);
  this->levels = text;
  this->tanker = value.tanker;
  this->healer = value.healer;
  this->dealer = value.dealer;
  this->assist = value.assist;
  this->type = value.type;
  this->name = value.characterName;
  this->info = value;
}

// OFFSET: 0x007BF8C0
// FUNCTION: fn_09304
void UIPartyInfoWnd::Paint() {
  this->ClearSurface(0xFFFF00FFu);

  if (this->m_drawLabel == 0) {
    return;
  }

  CBitmapRes* background = nullptr;

  switch (this->geometry.unknown_0030) {
  case 0:
    background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[0].c_str())));
    break;
  case 1:
    background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[1].c_str())));
    break;
  case 2:
    background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[2].c_str())));
    break;
  }

  if (this->m_disabled != 0 && !this->m_names[3].empty()) {
    background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[3].c_str())));
  }

  if (background) {
    this->DrawBitmap(0, 0, &background->image, false);
  }

  const auto text = [&](int x, const char* value, int bytes = 0) { this->DrawTextAt(x, 5, value, bytes, 0, 12, 0, 0, 0); };
  text(11, this->levels.c_str());
  text(64, TypeName(this->type));

  CBitmapRes* job = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.tanker ? "유저인터페이스\\UIadventurerAgency\\btn_job_def_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_def_off.bmp")));
  this->DrawBitmap(109, 3, job, false);
  job = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.healer ? "유저인터페이스\\UIadventurerAgency\\btn_job_heal_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_heal_off.bmp")));
  this->DrawBitmap(127, 3, job, false);
  job = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.dealer ? "유저인터페이스\\UIadventurerAgency\\btn_job_atk_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_atk_off.bmp")));
  this->DrawBitmap(145, 3, job, false);
  job = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(
      Resolve(this->info.assist ? "유저인터페이스\\UIadventurerAgency\\btn_job_buf_on.bmp" : "유저인터페이스\\UIadventurerAgency\\btn_job_buf_off.bmp")));
  this->DrawBitmap(163, 3, job, false);

  const std::string fitted = FitText(*this->geometry.surface, this->name, 119);
  text(192, fitted.c_str(), static_cast<int>(fitted.size()));
  text(315, this->memo.c_str(), static_cast<int>(this->memo.size()));

  if (this->m_invalidateParent != 0 && this->parent) {
    this->parent->Invalidate();
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
  if (this->m_disabled != 0 || this->info.accountId == g_session.m_aid || this->info.characterId == g_session.m_gid) {
    return;
  }

  const std::string name = this->info.characterName;
  const char* relationship = g_windowMgr.IsFriend(name.c_str()) ? LookupMessage(102) : nullptr;

  if (g_session.IsGuildMemberAccount(this->info.accountId)) {
    relationship = "GuildMember";
  }

  std::string code;
  code = ShortcutFilename(this->info.characterId);
  g_windowMgr.SendNotice(14, reinterpret_cast<std::intptr_t>(name.c_str()), reinterpret_cast<std::intptr_t>(relationship), reinterpret_cast<std::intptr_t>(code.c_str()), 0, 0);
}
