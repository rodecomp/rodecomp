#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Quest/UIRenewQuestUI.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Replay/Replay.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/TextEncoding.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Quest/UIOngoingQuestInfo.h"
#include "UI/Quest/UIRecommendedQuestInfo.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>

// GLOBAL: gl_25249
static bool s_recommendedTabFirstSelect = true;

// OFFSET: 0x00A59FE0
// FUNCTION: fn_16371
template <>
__declspec(noinline) const char* EncryptedName<6, 0xE33F2FF779878559ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xE33F2FF779878559ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A010
// FUNCTION: fn_16372
template <>
inline const char* EncryptedName<6, 0xC70319B5EB61598FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xC70319B5EB61598FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A040
// FUNCTION: fn_16373
template <>
__declspec(noinline) const char* EncryptedName<6, 0xB7AFBD2FD9FD7501ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0xB7AFBD2FD9FD7501ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A070
// FUNCTION: fn_16374
template <>
__declspec(noinline) const char* EncryptedName<6, 0x114901C7E37F6D49ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0x114901C7E37F6D49ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A0A0
// FUNCTION: fn_16375
template <>
__declspec(noinline) const char* EncryptedName<6, 0x1767DDE18D779D8BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 6, 0x1767DDE18D779D8BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A0D0
// FUNCTION: fn_16376
template <>
__declspec(noinline) const char* EncryptedName<44, 0xB3E307A5F753D5E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xB3E307A5F753D5E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A100
// FUNCTION: fn_16377
template <>
__declspec(noinline) const char* EncryptedName<44, 0xA597A7E7492B39D5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xA597A7E7492B39D5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A130
// FUNCTION: fn_16378
template <>
__declspec(noinline) const char* EncryptedName<45, 0xD5A78DB38D8DAFABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xD5A78DB38D8DAFABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A160
// FUNCTION: fn_16379
template <>
__declspec(noinline) const char* EncryptedName<45, 0xFFABE3D5D1BBEF73ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xFFABE3D5D1BBEF73ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A190
// FUNCTION: fn_16380
template <>
__declspec(noinline) const char* EncryptedName<45, 0xF15F8785CDE369BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0xF15F8785CDE369BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A1C0
// FUNCTION: fn_16381
template <>
__declspec(noinline) const char* EncryptedName<45, 0x192D654D29337BE5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x192D654D29337BE5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A1F0
// FUNCTION: fn_16382
template <>
__declspec(noinline) const char* EncryptedName<45, 0x0399459DAF454F0FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x0399459DAF454F0FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A220
// FUNCTION: fn_16383
template <>
__declspec(noinline) const char* EncryptedName<45, 0x79E915E9AF6F55DDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 45, 0x79E915E9AF6F55DDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A250
// FUNCTION: fn_16384
template <>
__declspec(noinline) const char* EncryptedName<46, 0x271B45771F83DD9FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0x271B45771F83DD9FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A280
// FUNCTION: fn_16385
template <>
__declspec(noinline) const char* EncryptedName<46, 0x792FFD1325418FB5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0x792FFD1325418FB5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A2B0
// FUNCTION: fn_16386
template <>
__declspec(noinline) const char* EncryptedName<47, 0xC5B1BBCD9BC9C997ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0xC5B1BBCD9BC9C997ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A2E0
// FUNCTION: fn_16387
template <>
__declspec(noinline) const char* EncryptedName<47, 0xC1BB8111EDD183E1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0xC1BB8111EDD183E1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A310
// FUNCTION: fn_16388
template <>
__declspec(noinline) const char* EncryptedName<47, 0x8783C33941E15117ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x8783C33941E15117ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A340
// FUNCTION: fn_16389
template <>
__declspec(noinline) const char* EncryptedName<47, 0x01CB4F1DF9D3F931ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x01CB4F1DF9D3F931ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A370
// FUNCTION: fn_16390
template <>
__declspec(noinline) const char* EncryptedName<47, 0x49D33529F1556333ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x49D33529F1556333ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A3A0
// FUNCTION: fn_16391
template <>
__declspec(noinline) const char* EncryptedName<47, 0x0B23D10F2F49D72BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x0B23D10F2F49D72BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A3D0
// FUNCTION: fn_16392
template <>
__declspec(noinline) const char* EncryptedName<47, 0x0B8745519B1F95D7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 47, 0x0B8745519B1F95D7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A400
// FUNCTION: fn_16393
template <>
__declspec(noinline) const char* EncryptedName<48, 0x136F2F9915D1DBCFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x136F2F9915D1DBCFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A430
// FUNCTION: fn_16394
template <>
__declspec(noinline) const char* EncryptedName<48, 0x674D9F3975573D41ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x674D9F3975573D41ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A460
// FUNCTION: fn_16395
template <>
__declspec(noinline) const char* EncryptedName<48, 0x6BDF334F2BFDAD51ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 48, 0x6BDF334F2BFDAD51ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A490
// FUNCTION: fn_16396
template <>
__declspec(noinline) const char* EncryptedName<49, 0x659975C121557D3DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 49, 0x659975C121557D3DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A4C0
// FUNCTION: fn_16397
template <>
__declspec(noinline) const char* EncryptedName<50, 0x85E1F5A7A5755B7FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 50, 0x85E1F5A7A5755B7FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A4F0
// FUNCTION: fn_16398
template <>
__declspec(noinline) const char* EncryptedName<51, 0xAD6153019F93A527ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 51, 0xAD6153019F93A527ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A520
// FUNCTION: fn_16399
template <>
__declspec(noinline) const char* EncryptedName<52, 0xBFF5C517A1A77F59ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 52, 0xBFF5C517A1A77F59ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00A5A550
// FUNCTION: fn_16400
__declspec(noinline) EncryptedName<6, 0x114901C7E37F6D49ULL>& GulimFontName1() {
  OBFUSCATE_ARRAY_BODY("Gulim", 0x114901C7E37F6D49ULL, std::uint8_t);
}

// OFFSET: 0x00A5A620
// FUNCTION: fn_16401
__declspec(noinline) EncryptedName<48, 0x136F2F9915D1DBCFULL>& NormalOffButtonName1() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x136F2F9915D1DBCFULL);
}

// OFFSET: 0x00A5A700
// FUNCTION: fn_16402
__declspec(noinline) EncryptedName<47, 0x0B23D10F2F49D72BULL>& NormalOnButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_normal_on.bmp", 0x0B23D10F2F49D72BULL);
}

// OFFSET: 0x00A5A7F0
// FUNCTION: fn_16403
__declspec(noinline) EncryptedName<48, 0x674D9F3975573D41ULL>& NormalOffButtonName2() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x674D9F3975573D41ULL);
}

// OFFSET: 0x00A5A8D0
// FUNCTION: fn_16404
__declspec(noinline) EncryptedName<48, 0x6BDF334F2BFDAD51ULL>& NormalOffButtonName3() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x6BDF334F2BFDAD51ULL);
}

// OFFSET: 0x00A5A9B0
// FUNCTION: fn_16405
__declspec(noinline) EncryptedName<6, 0xB7AFBD2FD9FD7501ULL>& GulimFontName2() {
  OBFUSCATE_ARRAY_BODY("Gulim", 0xB7AFBD2FD9FD7501ULL, std::uint8_t);
}

// OFFSET: 0x00A5AA80
// FUNCTION: fn_16406
__declspec(noinline) EncryptedName<45, 0x79E915E9AF6F55DDULL>& SubOffButtonName1() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0x79E915E9AF6F55DDULL);
}

// OFFSET: 0x00A5AB70
// FUNCTION: fn_16407
__declspec(noinline) EncryptedName<44, 0xB3E307A5F753D5E3ULL>& SubOnButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_sub_on.bmp", 0xB3E307A5F753D5E3ULL);
}

// OFFSET: 0x00A5AC50
// FUNCTION: fn_16408
__declspec(noinline) EncryptedName<45, 0x192D654D29337BE5ULL>& SubOffButtonName2() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0x192D654D29337BE5ULL);
}

// OFFSET: 0x00A5AD40
// FUNCTION: fn_16409
__declspec(noinline) EncryptedName<45, 0xF15F8785CDE369BFULL>& SubOffButtonName3() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0xF15F8785CDE369BFULL);
}

// OFFSET: 0x00A5AE30
// FUNCTION: fn_16410
__declspec(noinline) EncryptedName<6, 0xE33F2FF779878559ULL>& GulimFontName3() {
  OBFUSCATE_ARRAY_BODY("Gulim", 0xE33F2FF779878559ULL, std::uint8_t);
}

// OFFSET: 0x00A5AF00
// FUNCTION: fn_16411
__declspec(noinline) EncryptedName<47, 0x49D33529F1556333ULL>& DailyOffButtonName1() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0x49D33529F1556333ULL);
}

// OFFSET: 0x00A5AFF0
// FUNCTION: fn_16412
__declspec(noinline) EncryptedName<46, 0x792FFD1325418FB5ULL>& DailyOnButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_daily_on.bmp", 0x792FFD1325418FB5ULL);
}

// OFFSET: 0x00A5B0E0
// FUNCTION: fn_16413
__declspec(noinline) EncryptedName<47, 0xC5B1BBCD9BC9C997ULL>& DailyOffButtonName2() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0xC5B1BBCD9BC9C997ULL);
}

// OFFSET: 0x00A5B1D0
// FUNCTION: fn_16414
__declspec(noinline) EncryptedName<47, 0x01CB4F1DF9D3F931ULL>& DailyOffButtonName3() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0x01CB4F1DF9D3F931ULL);
}

// OFFSET: 0x00A5B2C0
// FUNCTION: fn_16415
__declspec(noinline) EncryptedName<6, 0x1767DDE18D779D8BULL>& GulimFontName4() {
  OBFUSCATE_ARRAY_BODY("Gulim", 0x1767DDE18D779D8BULL, std::uint8_t);
}

// OFFSET: 0x00A5B390
// FUNCTION: fn_16416
__declspec(noinline) EncryptedName<47, 0xC1BB8111EDD183E1ULL>& EventOffButtonName1() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0xC1BB8111EDD183E1ULL);
}

// OFFSET: 0x00A5B480
// FUNCTION: fn_16417
__declspec(noinline) EncryptedName<46, 0x271B45771F83DD9FULL>& EventOnButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_event_on.bmp", 0x271B45771F83DD9FULL);
}

// OFFSET: 0x00A5B570
// FUNCTION: fn_16418
__declspec(noinline) EncryptedName<47, 0x8783C33941E15117ULL>& EventOffButtonName2() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0x8783C33941E15117ULL);
}

// OFFSET: 0x00A5B660
// FUNCTION: fn_16419
__declspec(noinline) EncryptedName<47, 0x0B8745519B1F95D7ULL>& EventOffButtonName3() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0x0B8745519B1F95D7ULL);
}

// OFFSET: 0x00A5B750
// FUNCTION: fn_16420
__declspec(noinline) EncryptedName<6, 0xC70319B5EB61598FULL>& GulimFontName5() {
  OBFUSCATE_ARRAY_BODY("Gulim", 0xC70319B5EB61598FULL, std::uint8_t);
}

// OFFSET: 0x00A5B820
// FUNCTION: fn_16421
__declspec(noinline) EncryptedName<45, 0xFFABE3D5D1BBEF73ULL>& EtcOffButtonName1() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0xFFABE3D5D1BBEF73ULL);
}

// OFFSET: 0x00A5B910
// FUNCTION: fn_16422
__declspec(noinline) EncryptedName<44, 0xA597A7E7492B39D5ULL>& EtcOnButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_etc_on.bmp", 0xA597A7E7492B39D5ULL);
}

// OFFSET: 0x00A5B9F0
// FUNCTION: fn_16423
__declspec(noinline) EncryptedName<45, 0xD5A78DB38D8DAFABULL>& EtcOffButtonName2() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0xD5A78DB38D8DAFABULL);
}

// OFFSET: 0x00A5BAE0
// FUNCTION: fn_16424
__declspec(noinline) EncryptedName<45, 0x0399459DAF454F0FULL>& EtcOffButtonName3() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0x0399459DAF454F0FULL);
}

// OFFSET: 0x00A5BBD0
// FUNCTION: fn_16425
__declspec(noinline) EncryptedName<51, 0xAD6153019F93A527ULL>& SearchNormalButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_search_normal.bmp", 0xAD6153019F93A527ULL);
}

// OFFSET: 0x00A5BCB0
// FUNCTION: fn_16426
__declspec(noinline) EncryptedName<50, 0x85E1F5A7A5755B7FULL>& SearchPressButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_search_press.bmp", 0x85E1F5A7A5755B7FULL);
}

// OFFSET: 0x00A5BD90
// FUNCTION: fn_16427
__declspec(noinline) EncryptedName<49, 0x659975C121557D3DULL>& SearchOverButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_search_over.bmp", 0x659975C121557D3DULL);
}

// OFFSET: 0x00A5BE70
// FUNCTION: fn_16428
__declspec(noinline) EncryptedName<52, 0xBFF5C517A1A77F59ULL>& SearchDisableButtonName() {
  OBFUSCATE_STRING_BODY("유저인터페이스\\Renew_QuestUI\\btn_search_disable.bmp", 0xBFF5C517A1A77F59ULL);
}

// OFFSET: 0x00A66BE0
// FUNCTION: fn_16618
void RenewQuestUI::CUIQuestItem::Move(int x, int y) {
  UIWindow::Move(x, y);
  this->baseY = y;
  this->baseX = x;
}

// OFFSET: 0x00A66C10
// FUNCTION: fn_16619
void RenewQuestUI::CUIQuestItem::Paint() {
  if (this->pressed) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bg_questList_press.bmp"))), false);
  } else {
    CUIListItem::Paint();
  }
}

// OFFSET: 0x00A66C50
// FUNCTION: fn_16620
void RenewQuestUI::CUIQuestList::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A66C60
// FUNCTION: fn_16621
void CUIRenewQuestUI::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A66F40
// FUNCTION: fn_16625
void RenewQuestUI::CUIQuestItem::OnLBtnDown(int x, int y) {
  CUIListItem::OnLBtnDown(x, y);
  this->pressed = 1;
  UIWindow::Move(this->baseX, this->baseY + 1);
  this->Invalidate();
}

// OFFSET: 0x00A66F80
// FUNCTION: fn_16626
void RenewQuestUI::CUIQuestItem::OnLBtnUp(int x, int y) {
  CUIListItem::OnLBtnUp(x, y);
  this->pressed = 0;
  UIWindow::Move(this->baseX, this->baseY);
  this->Invalidate();
}

// OFFSET: 0x00A66FC0
// FUNCTION: fn_16627
void RenewQuestUI::CUIQuestItem::OnMouseLeave() {
  this->pressed = 0;
  UIWindow::Move(this->baseX, this->baseY);
  this->Invalidate();
}

// OFFSET: 0x00A5C170
// FUNCTION: fn_16431
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::ClearItems() {
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A63F60
// FUNCTION: fn_16444
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::OnScroll(int position) {
  if (this->items.empty()) {
    return;
  }
  if (this->data.size() == 0) {
    for (auto* item : this->items) {
      item->index = -1;
      item->Show(0);
    }
    return;
  }

  const int first = this->items.front()->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      const unsigned int shift = first - row;
      if (shift < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - shift, this->items.end());
      }
    }
  }

  auto it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    RenewQuestUI::CUIQuestItem* item = *it;
    if (item) {
      if (this->rebind || row != static_cast<unsigned int>(item->index)) {
        item->index = row;
        if (this->binder) {
          auto node = this->data.begin();
          std::advance(node, static_cast<int>(row));
          this->binder(item, *node);
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(1);
    }
  }

  for (const auto end = this->items.end(); it != end; ++it) {
    auto* const item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A64160
// FUNCTION: fn_16445
template <>
__declspec(noinline) void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (RenewQuestUI::CUIQuestItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  RenewQuestUI::CUIQuestItem* selectedItem = dynamic_cast<RenewQuestUI::CUIQuestItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    int count = static_cast<int>(this->selected);
    if (count > 0) {
      do {
        ++node;
        --count;
      } while (count > 0);
    }
    this->onSelect(selectedItem, *node);
  }
}

class CQuestItemSelector {
public:
  void SetHighlighted(CUIListItem* const& item);
  void Select(CUIListItem* const& item);

private:
  /* 0000 */ void* padding_0000;
  /* 0004 */ CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox;
};

// OFFSET: 0x00A64DE0
// FUNCTION: fn_16497
void CQuestItemSelector::SetHighlighted(CUIListItem* const& item) {
  CUIListItem* highlighted = item;
  for (RenewQuestUI::CUIQuestItem* entry : this->listBox->items) {
    entry->SetHighlighted(highlighted == static_cast<CUIListItem*>(entry));
  }
}

// OFFSET: 0x00A64E80
// FUNCTION: fn_16501
void CQuestItemSelector::Select(CUIListItem* const& item) {
  CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = this->listBox;
  CUIListItem* selection = item;
  if (listBox->layoutFlag) {
    return;
  }
  for (RenewQuestUI::CUIQuestItem* other : listBox->items) {
    if (selection != other) {
      other->SetSelected(0);
    }
  }
  selection->SetSelected(1);
  if (selection->index == static_cast<int>(listBox->selected) && listBox->selectionFlag) {
    return;
  }
  RenewQuestUI::CUIQuestItem* selectedItem = dynamic_cast<RenewQuestUI::CUIQuestItem*>(selection);
  if (!selectedItem) {
    return;
  }
  listBox->selected = selectedItem->index;
  if (listBox->onSelect && static_cast<int>(listBox->selected) >= 0 && listBox->selected < listBox->data.size()) {
    auto node = listBox->data.begin();
    for (int count = static_cast<int>(listBox->selected); count > 0; --count) {
      ++node;
    }
    listBox->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A67CE0
// FUNCTION: fn_16631
template <>
__declspec(noinline) bool CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::SelectIndex(int index) {
  this->scrollBar->SetPosition(this->rowHeight * index);
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    if (index == item->index) {
      this->SelectItem(item);
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00A65F20
// FUNCTION: fn_16603
CUIRenewQuestUI::~CUIRenewQuestUI() {
  g_windowMgr.CloseWindow(WID_RECOMMENDEDQUESTINFO);
  g_windowMgr.CloseWindow(WID_ONGOINGQUESTINFO);
}

// OFFSET: 0x00A65FD0
// FUNCTION: fn_16604
template <>
__declspec(noinline) int CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddData(const unsigned long& value, bool refresh) {
  this->data.push_back(value);
  if (refresh) {
    this->UpdateScrollBar();
  }
  return this->data.size() - 1;
}

// OFFSET: 0x00A66140
// FUNCTION: fn_16610
void RenewQuestUI::CUIQuestItem::OnActiveStateChanged(bool active) {
  const int questId = this->questId;
  if (this->questType == 2) {
    g_renewQuestMgr->SetRecommendedQuestActive(questId, active);
    if (auto* window = static_cast<CUIRenewQuestUI*>(g_windowMgr.LookupWindow(WID_RENEWQUESTUI))) {
      window->UpdateQuestLists(this->questType);
    }
    g_renewQuestMgr->SaveRecommendedActiveList();
  } else {
#pragma pack(push, 1)
    struct QuestActivePacket {
      /* 0000 */ std::int16_t id;
      /* 0002 */ int questId;
      /* 0006 */ bool active;
    };
#pragma pack(pop)

    QuestActivePacket packet;
    packet.id = ID_CZ_ACTIVE_QUEST;
    packet.questId = questId;
    packet.active = active;
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
  }
}

// OFFSET: 0x00A66280
// FUNCTION: fn_16613
void RenewQuestUI::CUIQuestItem::OnSimpleView(bool simpleView) {
  g_renewQuestMgr->SetOngoingSimpleView(this->questId, simpleView);
  g_renewQuestMgr->SaveOngoingSimpleViewList();
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_QUESTDISPLAY)) {
    window->Invalidate();
  }
}

// OFFSET: 0x00A66C70
// FUNCTION: fn_16622
void RenewQuestUI::CUIQuestItem::OnInit() {
  CUIButton* button = this->simpleViewCheckBox;
  std::string tooltip = LookupMessage(MSI_PASSWORD);
  button->tooltip = tooltip;
}

// OFFSET: 0x00A66D90
// FUNCTION: fn_16623
void RenewQuestUI::CUIQuestList::OnInit() {
  this->list->selectionFlag = 0;
}

// OFFSET: 0x00A5C200
// FUNCTION: fn_16432
void CUIRenewQuestUI::Initialize() {
  this->m_ownsBackgroundColor = true;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(381, 466);
  this->Move(
    g_renderer->viewport.centerX - this->geometry.width / 2, g_renderer->viewport.centerY - this->geometry.height / 2
  );
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A67E10
// FUNCTION: fn_16633
void CUIRenewQuestUI::OnFilterButton(UIWindow* button) {
  if (button) {
    this->filter = button->geometry.id;
    this->UpdateQuestLists(0xF);
  }
}

// OFFSET: 0x00A67FA0
// FUNCTION: fn_16634
void CUIRenewQuestUI::OnTabButton(CUITabButton* button) {
}

// OFFSET: 0x00A67FB0
// FUNCTION: fn_16635
void CUIRenewQuestUI::OnPanelButton() {
}

template void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::ClearData();

// OFFSET: 0x00A661F0
// FUNCTION: fn_16612
void UIWindow::RecordWindowFlag(bool value) {
  this->RecordValues(6, 0xd5, value, 0, 0);
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_QUESTDISPLAY)) {
    window->Show(value);
  }
}

// OFFSET: 0x00A66410
// FUNCTION: fn_16615
bool CUIRenewQuestUI::MatchesFilter(const RenewQuestInfo& info) const {
  if (!g_renewQuestMgr->IsInFilter(this->filter, info.m_icon)) {
    return false;
  }

  return this->searchText.empty() || FindSubstring(info.summary, this->searchText) != -1 || FindSubstring(info.m_title, this->searchText) != -1;
}

// OFFSET: 0x00A660E0
// FUNCTION: fn_16605
void CUIRenewQuestUI::OnCallback6() {
}

// OFFSET: 0x00A660F0
// FUNCTION: fn_16606
void CUIRenewQuestUI::OnCallback50() {
}

// OFFSET: 0x00A66110
// FUNCTION: fn_16607
void CUIRenewQuestUI::OnCallback43() {
}

// OFFSET: 0x00A66120
// FUNCTION: fn_16608
void CUIRenewQuestUI::OnCallback80() {
}

// OFFSET: 0x00A66130
// FUNCTION: fn_16609
void CUIRenewQuestUI::OnCallback74() {
}

// OFFSET: 0x00A661E0
// FUNCTION: fn_16611
void CUIRenewQuestUI::OnCallback56() {
}

// OFFSET: 0x00A66FF0
// FUNCTION: fn_16628
void CUIRenewQuestUI::SelectQuest(unsigned int questId) {
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list;
    auto node = std::find(listBox->data.begin(), listBox->data.end(), questId);
    if (node != listBox->data.end()) {
      this->tab->SelectPage(list);
      CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* pageListBox = list->list;

      int index = 0;
      for (auto it = listBox->data.begin(); it != node; ++it) {
        ++index;
      }

      pageListBox->scrollBar->SetPosition(pageListBox->rowHeight * index);

      for (RenewQuestUI::CUIQuestItem* item : pageListBox->items) {
        if (index == item->index) {
          pageListBox->SelectItem(item);
          return;
        }
      }

      return;
    }
  }

  std::string text = std::format("Not found Quest Info = %lu", questId);
  g_windowMgr.Confirm(text.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
}

// OFFSET: 0x00A59940
// FUNCTION: fn_16362
bool sub_A59940() {
  return true;
}

// OFFSET: 0x00A59950
// FUNCTION: fn_16363
bool sub_A59950() {
  return true;
}

// OFFSET: 0x00A59960
// FUNCTION: fn_16364
bool sub_A59960() {
  return true;
}

// OFFSET: 0x00A599F0
// FUNCTION: fn_16366
template <>
CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A59C80
// FUNCTION: fn_16368
template <>
CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::~CUIListBoxEx() {
}

// OFFSET: 0x00A59BB0
// FUNCTION: fn_16367
CUIEditBoxEx::CUIEditBoxEx()
    : CUIControl(), backgroundColor(0xF7F7F7), borderColor(0xC6C6C6), borderWidth(1), editLeft(0), editTop(0), editRight(0), editBottom(0),
      edit(nullptr), onSubmit() {
}

// OFFSET: 0x00A67CA0
// FUNCTION: fn_16630
void RenewQuestUI::CUIQuestItem::SetSelected(bool selected) {
  this->titleText->SetColor(selected ? 0x00E47B2E : 0x00955322);
  CUIListItem::SetSelected(selected);
}

// OFFSET: 0x00A67D40
// FUNCTION: fn_16632
int CUIRenewQuestUI::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    if (a == ID_CHECK) {
      const bool checked = b != 0;
      CUICheckBox* checkBox = this->checkBox;
      checkBox->checked = checked;
      checkBox->Invalidate();
      this->RecordValues(6, a, b != 0, 0, 0);
      if (UIWindow* window = g_windowMgr.LookupWindow(WID_QUESTDISPLAY)) {
        window->Show(checked);
      }
    }
    break;
  case UM_TAB:
    this->tab->SelectIndex(static_cast<int>(std::log2(static_cast<double>(a))));
    break;
  }

  return CUIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x00A5C380
// FUNCTION: fn_16433
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  {
    CUIVScrollBar* bar = this->scrollBar;
    const std::function<void(int)> onChange = [this](int position) { this->OnScroll(position); };
    bar->onChange = onChange;
  }
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A63B90
// FUNCTION: fn_16440
void CUIEditBoxEx::SetOnSubmit(std::function<void()> handler) {
  this->onSubmit = handler;
}

// OFFSET: 0x00A63C00
// FUNCTION: fn_16441
void CUITab::SetOnChange(std::function<void(CUITabButton*)> handler) {
  this->onChange = handler;
}

// OFFSET: 0x00A664C0
// FUNCTION: fn_16616
void RenewQuestUI::CUIQuestList::OnSelectItem(CUIQuestItem* item, const unsigned long& questId) {
  if (item->questId == 0) {
    return;
  }

  if (item->questType == 2) {
    if (CUIRecommendedQuestInfo* info = dynamic_cast<CUIRecommendedQuestInfo*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_RECOMMENDEDQUESTINFO)))) {
      info->SetQuest(item->questId);
      if (UIWindow* other = g_windowMgr.LookupWindow(WID_ONGOINGQUESTINFO)) {
        info->Move(other->geometry.x, other->geometry.y);
        g_windowMgr.CloseWindow(WID_ONGOINGQUESTINFO);
      }
    }
  } else {
    if (CUIOngoingQuestInfo* info = dynamic_cast<CUIOngoingQuestInfo*>(static_cast<UIFrameWnd*>(g_windowMgr.EnsureWindow(WID_ONGOINGQUESTINFO)))) {
      info->SetQuest(item->questId);
      if (UIWindow* other = g_windowMgr.LookupWindow(WID_RECOMMENDEDQUESTINFO)) {
        info->Move(other->geometry.x, other->geometry.y);
        g_windowMgr.CloseWindow(WID_RECOMMENDEDQUESTINFO);
      }
    }
  }
}

// OFFSET: 0x00A665C0
// FUNCTION: fn_16617
void RenewQuestUI::CUIQuestList::BindItem(CUIQuestItem* item, const unsigned long& questId) {
  item->questType = this->type;
  item->questId = questId;
  if (this->type == 2) {
    auto found = g_renewQuestMgr->m_recommendedQuests.find(questId);
    if (found == g_renewQuestMgr->m_recommendedQuests.end()) {
      return;
    }

    const RenewQuestRecommendedInfo& info = found->second;
    item->icon->SetImage(std::format("유저인터페이스\\renew_questui\\{}", info.m_icon.empty() ? std::string("ico_nq.bmp") : info.m_icon));
    item->icon->tooltip = info.m_iconTooltip;
    item->titleText->SetText(info.m_title);
    item->descriptionText->SetText(info.summary);
    item->simpleViewCheckBox->Show(0);
    item->activeCheckBox->Show(1);
    item->activeCheckBox->checked = info.m_active;
    item->activeCheckBox->Invalidate();
    std::string tooltip = LookupMessage(MSI_SAVE_ID);
    item->activeCheckBox->tooltip = tooltip;
  } else {
    auto found = g_renewQuestMgr->m_ongoingQuests.find(questId);
    if (found == g_renewQuestMgr->m_ongoingQuests.end()) {
      return;
    }

    const RenewQuestOngoingInfo& info = found->second;
    item->icon->SetImage(std::format("유저인터페이스\\renew_questui\\{}", info.m_icon.empty() ? std::string("ico_nq.bmp") : info.m_icon));
    item->icon->tooltip = info.m_iconTooltip;
    item->titleText->SetText(info.m_title);
    item->descriptionText->SetText(info.summary);
    item->simpleViewCheckBox->Show(this->type != 8);
    item->simpleViewCheckBox->checked = info.m_simpleView;
    item->simpleViewCheckBox->Invalidate();
    item->activeCheckBox->Show(this->type != 8);
    item->activeCheckBox->checked = info.m_active;
    item->activeCheckBox->Invalidate();
    std::string tooltip = LookupMessage(MSI_OTP_INPUT_TIME);
    item->activeCheckBox->tooltip = tooltip;
  }
}

// OFFSET: 0x00A67290
// FUNCTION: fn_16629
void CUIRenewQuestUI::UpdateQuestLists(unsigned int flags) {
  if (flags & 1) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 1) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned long, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.questType != 1 && entry.second.m_state != 0) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 2) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 2) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned long, RenewQuestRecommendedInfo> entry : g_renewQuestMgr->m_recommendedQuests) {
          if (!this->MatchesFilter(entry.second)) {
            continue;
          }
          if (entry.second.m_active) {
            listBox->AddData(entry.first, false);
          }
        }
        for (std::pair<const unsigned long, RenewQuestRecommendedInfo> entry : g_renewQuestMgr->m_recommendedQuests) {
          if (!this->MatchesFilter(entry.second)) {
            continue;
          }
          if (!entry.second.m_active) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 4) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 4) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned long, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.questType != 1 && entry.second.m_state == 0) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }

  if (flags & 8) {
    for (RenewQuestUI::CUIQuestList* list : this->questLists) {
      if (list->type != 8) {
        continue;
      }

      if (CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* listBox = list->list) {
        const int position = listBox->data.size() == 0 ? -1 : listBox->scrollBar->position;
        listBox->ClearData();
        for (std::pair<const unsigned long, RenewQuestOngoingInfo> entry : g_renewQuestMgr->m_ongoingQuests) {
          if (this->MatchesFilter(entry.second) && entry.second.questType == 1) {
            listBox->AddData(entry.first, false);
          }
        }
        listBox->UpdateScrollBar();
        if (position == -1) {
          listBox->scrollBar->SetPosition(position);
        }
      }
      break;
    }
  }
}

// OFFSET: 0x00A5C0F0
// FUNCTION: fn_16430
CUIFrameWnd* CreateRenewQuestUI() {
  return new CUIRenewQuestUI();
}

// OFFSET: 0x00499210
// FUNCTION: fn_45554
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateRenewQuestUI;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10008, factory);
  return true;
}();

// OFFSET: 0x00A5C590
// FUNCTION: fn_16434
void RenewQuestUI::CUIQuestItem::OnCreate(int width, int height) {
  this->icon = new CUIStaticImage();
  this->icon->SetSize(26, 29);
  this->icon->Move(8, 7);
  this->icon->SetImage(std::string(""));
  this->AddChild(this->icon);

  this->titleText = new CUIStaticText();
  this->titleText->SetSize(260, 12);
  this->titleText->Move(42, 6);
  this->titleText->SetText("");
  this->titleText->SetFitText(1);
  this->titleText->SetAlign(0, 0);
  this->titleText->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0xAD075B8DB599F1CDULL)));
  this->titleText->SetFontHeight(12);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x955322);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->descriptionText = new CUIStaticText();
  this->descriptionText->SetSize(260, 12);
  this->descriptionText->Move(42, 24);
  this->descriptionText->SetText("");
  this->descriptionText->SetFitText(1);
  this->descriptionText->SetAlign(0, 0);
  this->descriptionText->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0xDD3913D5D7F97BE1ULL)));
  this->descriptionText->SetFontHeight(12);
  this->descriptionText->SetBold(0);
  this->descriptionText->SetColor(0x38302E);
  this->descriptionText->SetShadow(0, 0xFFFFFF);
  this->descriptionText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->descriptionText);

  this->simpleViewCheckBox = new CUICheckBox();
  this->simpleViewCheckBox->SetSize(14, 14);
  this->simpleViewCheckBox->Move(312, 3);
  this->simpleViewCheckBox->SetOnCheck([this](bool checked) { this->OnSimpleView(checked); });
  CUICheckBox* simpleViewCheckBox = this->simpleViewCheckBox;
  simpleViewCheckBox->disabled = 0;
  simpleViewCheckBox->Invalidate();
  this->simpleViewCheckBox->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_check_off.bmp", 0x050505679903579BULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_check_on.bmp", 0x83EFBFBB37EDBDBDULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_check_off.bmp", 0xBF2B4B31397FF5A7ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_check_off.bmp", 0xD97D5DBDBB2F2709ULL))
  );
  this->simpleViewCheckBox->checked = false;
  this->simpleViewCheckBox->Invalidate();
  this->AddChild(this->simpleViewCheckBox);

  this->activeCheckBox = new CUICheckBox();
  this->activeCheckBox->SetSize(20, 17);
  this->activeCheckBox->Move(306, 22);
  this->activeCheckBox->SetOnCheck([this](bool checked) { this->OnActiveStateChanged(checked); });
  CUICheckBox* activeCheckBox = this->activeCheckBox;
  activeCheckBox->disabled = 0;
  activeCheckBox->Invalidate();
  this->activeCheckBox->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_lock.bmp", 0xED796785D1DF137FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_lock_open.bmp", 0xF14F79170939C7F1ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_lock.bmp", 0xA5FD29D7AB6DC59FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_lock.bmp", 0x13E33FA1A591AB67ULL))
  );
  this->activeCheckBox->checked = false;
  this->activeCheckBox->Invalidate();
  this->AddChild(this->activeCheckBox);
}

// OFFSET: 0x00A5D850
// FUNCTION: fn_16435
void RenewQuestUI::CUIQuestList::OnCreate(int width, int height) {
  CUIListBoxEx<CUIQuestItem, unsigned long>* listBox = new CUIListBoxEx<CUIQuestItem, unsigned long>();
  this->list = listBox;
  listBox->itemWidth = 330;
  listBox->rowHeight = 46;
  listBox->SetSize(343, 394);
  this->list->Move(0, 0);
  this->list->SetOnSelect([this](CUIQuestItem* item, const unsigned long& questId) { this->OnSelectItem(item, questId); });
  this->list->SetBinder([this](CUIQuestItem* item, const unsigned long& questId) { this->BindItem(item, questId); });
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::string disabled(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0x23BBD58F8B5FF5C9ULL));
    const std::string pressed(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0xCD8F077123FB7D6FULL));
    const std::string hover(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_questList_check.bmp", 0x9949BFD527532943ULL));
    const std::string normal(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0x17A9E5BF39AD4737ULL));
    list->SetImages(normal, hover, pressed, disabled);
  }
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    list->itemX = 0;
    list->itemY = 7;
  }
  this->list->scrollBar->SetSize(7, 394);
  this->list->scrollBar->Move(336, 0);
  this->list->scrollBar->step = 138;
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::string down(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp", 0x03772763F96D9DA5ULL));
    const std::string up(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", 0xC517FB932D95BB6DULL));
    list->SetScrollBarButtonImages(up, down);
  }
  this->list->SetScrollBarBackground("");
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    const std::string bottom(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp", 0xEDC74F6D13DF71EFULL));
    const std::string middle(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp", 0xF161095D0DE18F95ULL));
    const std::string top(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", 0x0B7109397F6B1387ULL));
    list->SetScrollBarThumbImages(top, middle, bottom);
  }
  this->list->AddNewItem();
  this->AddChild(this->list);
}

// OFFSET: 0x00A5E8C0
// FUNCTION: fn_16436
void CUIRenewQuestUI::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(352, 17);
  this->titleBar->Move(29, 0);
  this->titleBar->SetImages(std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x75ADC57D87EFBD8FULL)),
  std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x2BAD156DE3E1F9D9ULL)),
  std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x637B959D55771F69ULL)));
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText(OBFUSCATE_TEXT("QUEST", 0xD1413DBB01CD8B51ULL));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x758F1D0B4F7705B9ULL)));
  title->SetFontHeight(13);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(0, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* systemButton = new CUIImageButton();
  systemButton->SetSize(11, 11);
  systemButton->Move(3, 3);
  systemButton->SetOnClick([this]() { this->OnCallback6(); });
  systemButton->disabled = 0;
  systemButton->Invalidate();
  systemButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x891B232D09451965ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0xEF6D1D893F2B2DC5ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x234977DDB54BC71DULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x6FE153231D879777ULL))
  );
  this->titleBar->AddChild(systemButton);

  this->background = new CUIStaticImage();
  this->background->SetSize(352, 449);
  this->background->Move(29, 17);
  this->background->SetImage(std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_quest.bmp", 0x39A39F87DD5F535BULL)));
  this->AddChild(this->background);

  this->tab = new CUITab();
  this->tab->SetSize(30, 403);
  this->tab->Move(0, 17);
  this->tab->SetOnChange([this](CUITabButton* button) { this->OnTabButton(button); });
  this->AddChild(this->tab);

  CUITabButton* processButton = new CUITabButton();
  processButton->SetSize(30, 100);
  processButton->Move(0, 0);
  processButton->SetOnClick([this]() { this->OnProcessTabClick(); });
  processButton->disabled = 0;
  processButton->Invalidate();
  processButton->SetText(std::string(""));
  processButton->SetFitText(0);
  processButton->SetAlign(0, 0);
  processButton->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x2FEB8739E5975BD1ULL)));
  processButton->SetFontHeight(12);
  processButton->SetBold(0);
  processButton->SetTextColors(0, 0, 0, 0);
  processButton->SetShadow(0, 0);
  processButton->SetOutline(0, 0);
  processButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_process_off.bmp", 0x33133B977BCD03CDULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_process_on.bmp", 0xA12DAB392545D72FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_process_off.bmp", 0x6BA1C1132F033B75ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_process_off.bmp", 0x2DCB7567394DC1ABULL))
  );
  this->tab->AddChild(processButton);

  CUITabButton* recommendedButton = new CUITabButton();
  recommendedButton->SetSize(30, 100);
  recommendedButton->Move(0, 101);
  recommendedButton->SetOnClick([this]() { this->OnRecommendedTabClick(); });
  recommendedButton->disabled = 0;
  recommendedButton->Invalidate();
  recommendedButton->SetText(std::string(""));
  recommendedButton->SetFitText(0);
  recommendedButton->SetAlign(0, 0);
  recommendedButton->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x4F637DC51307B759ULL)));
  recommendedButton->SetFontHeight(12);
  recommendedButton->SetBold(0);
  recommendedButton->SetTextColors(0, 0, 0, 0);
  recommendedButton->SetShadow(0, 0);
  recommendedButton->SetOutline(0, 0);
  recommendedButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_recommended_off.bmp", 0x8D1D0FF1AF2B13F7ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_recommended_on.bmp", 0xB39FA7917BCFBB8DULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_recommended_off.bmp", 0x9171997D6963C9F7ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_recommended_off.bmp", 0x4F99DF03C3EF31B7ULL))
  );
  this->tab->AddChild(recommendedButton);

  CUITabButton* holdButton = new CUITabButton();
  holdButton->SetSize(30, 100);
  holdButton->Move(0, 202);
  holdButton->SetOnClick([this]() { this->OnHoldTabClick(); });
  holdButton->disabled = 0;
  holdButton->Invalidate();
  holdButton->SetText(std::string(""));
  holdButton->SetFitText(0);
  holdButton->SetAlign(0, 0);
  holdButton->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0xE52DF30DCBAF7125ULL)));
  holdButton->SetFontHeight(12);
  holdButton->SetBold(0);
  holdButton->SetTextColors(0, 0, 0, 0);
  holdButton->SetShadow(0, 0);
  holdButton->SetOutline(0, 0);
  holdButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_hold_off.bmp", 0xCFF3899D95AF355FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_hold_on.bmp", 0xFBD599E117C547C1ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_hold_off.bmp", 0x176F138D5DD7F365ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_hold_off.bmp", 0x2D51E3B7236175EDULL))
  );
  this->tab->AddChild(holdButton);

  CUITabButton* waitButton = new CUITabButton();
  waitButton->SetSize(30, 100);
  waitButton->Move(0, 303);
  waitButton->SetOnClick([this]() { this->OnWaitTabClick(); });
  waitButton->disabled = 0;
  waitButton->Invalidate();
  waitButton->SetText(std::string(""));
  waitButton->SetFitText(0);
  waitButton->SetAlign(0, 0);
  waitButton->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x0F41EBBDA32D1B17ULL)));
  waitButton->SetFontHeight(12);
  waitButton->SetBold(0);
  waitButton->SetTextColors(0, 0, 0, 0);
  waitButton->SetShadow(0, 0);
  waitButton->SetOutline(0, 0);
  waitButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_wait_off.bmp", 0x839B27DF8F5967B3ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_wait_on.bmp", 0xF3CB6F7711A71B6DULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_wait_off.bmp", 0x452FEF0D7FC95565ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\tab_wait_off.bmp", 0x9FED7DA189490D5DULL))
  );
  this->tab->AddChild(waitButton);

  this->checkBox = new CUICheckBox();
  this->checkBox->SetSize(10, 10);
  this->checkBox->Move(38, 448);
  this->checkBox->SetOnCheck([this](bool checked) { this->RecordWindowFlag(checked); });
  CUICheckBox* checkBox = this->checkBox;
  checkBox->disabled = 0;
  checkBox->Invalidate();
  this->checkBox->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\checkbox_off.bmp", 0x7DE349D717D7F123ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\checkbox_on.bmp", 0x0F15B16B5BAFD57FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\checkbox_off.bmp", 0x7941D76335CFA14FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\checkbox_off.bmp", 0x7321C3814F8799E1ULL))
  );
  this->checkBox->checked = false;
  this->checkBox->Invalidate();
  this->AddChild(this->checkBox);

  this->filterLabel = new CUIStaticText();
  this->filterLabel->SetSize(100, 12);
  this->filterLabel->Move(52, 447);
  this->filterLabel->SetText("");
  this->filterLabel->SetFitText(0);
  this->filterLabel->SetAlign(0, 0);
  this->filterLabel->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x7115D9F973F913C9ULL)));
  this->filterLabel->SetFontHeight(12);
  this->filterLabel->SetBold(0);
  this->filterLabel->SetColor(0);
  this->filterLabel->SetShadow(0, 0xFFFFFF);
  this->filterLabel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->filterLabel);

  this->closeButton = new CUIImageButton();
  this->closeButton->SetSize(42, 20);
  this->closeButton->Move(327, 442);
  this->closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(WID_RENEWQUESTUI); });
  CUIImageButton* closeButton = this->closeButton;
  closeButton->disabled = 0;
  closeButton->Invalidate();
  this->closeButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\btn_close.bmp", 0xCB1D253B7FE1F563ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\btn_close_b.bmp", 0x793F370985ED674FULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\btn_close_a.bmp", 0xE55723712993051DULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\btn_close.bmp", 0x3FA3AFA7035DE3FFULL))
  );
  this->AddChild(this->closeButton);

  for (int index = 0; index < 4; ++index) {
    RenewQuestUI::CUIQuestList* list = new RenewQuestUI::CUIQuestList();
    list->SetSize(343, 394);
    this->AddChild(list);
    this->questLists.push_back(list);
  }
  this->questLists[0]->Move(38, 17);
  this->questLists[1]->Move(38, 17);
  this->questLists[2]->Move(38, 17);
  this->questLists[3]->Move(38, 17);

  this->searchBackground = new CUIStaticImage();
  this->searchBackground->SetSize(352, 28);
  this->searchBackground->Move(29, 411);
  this->searchBackground->SetImage(std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\bg_search.bmp", 0xDB27CB557D038759ULL)));
  this->AddChild(this->searchBackground);

  this->filterPanel = new CUITab();
  this->filterPanel->SetSize(181, 22);
  this->filterPanel->Move(35, 414);
  this->filterPanel->SetOnChange([this](CUITabButton* button) { this->OnFilterButton(button); });
  this->AddChild(this->filterPanel);

  CUITabButton* allButton = new CUITabButton();
  allButton->SetSize(26, 22);
  allButton->Move(0, 0);
  allButton->SetOnClick([this]() { this->OnCallback50(); });
  allButton->disabled = 0;
  allButton->Invalidate();
  allButton->SetId(0);
  allButton->SetText(std::string(""));
  allButton->SetFitText(0);
  allButton->SetAlign(0, 0);
  allButton->SetFont(std::string(OBFUSCATE_TEXT("Gulim", 0x51BB39B763BBBD8DULL)));
  allButton->SetFontHeight(12);
  allButton->SetBold(0);
  allButton->SetTextColors(0, 0, 0, 0);
  allButton->SetShadow(0, 0);
  allButton->SetOutline(0, 0);
  allButton->SetImages(
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\btn_all_off.bmp", 0x87E3D1C50D572DEFULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\btn_all_on.bmp", 0xDF8117FF49CFF551ULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\btn_all_off.bmp", 0xADC51BF9C98B413DULL)),
    std::string(OBFUSCATE_TEXT("유저인터페이스\\Renew_QuestUI\\btn_all_off.bmp", 0x7D5D0F110B6B8371ULL))
  );
  this->filterPanel->AddChild(allButton);

  CUITabButton* normalButton = new CUITabButton();
  normalButton->SetSize(26, 22);
  normalButton->Move(31, 0);
  normalButton->SetOnClick([this]() { this->OnCallback56(); });
  normalButton->disabled = 0;
  normalButton->Invalidate();
  normalButton->SetId(2);
  normalButton->SetText(std::string(""));
  normalButton->SetFitText(0);
  normalButton->SetAlign(0, 0);
  normalButton->SetFont(std::string(GulimFontName1().Text()));
  normalButton->SetFontHeight(12);
  normalButton->SetBold(0);
  normalButton->SetTextColors(0, 0, 0, 0);
  normalButton->SetShadow(0, 0);
  normalButton->SetOutline(0, 0);
  normalButton->SetImages(
    std::string(NormalOffButtonName1().Text()), std::string(NormalOnButtonName().Text()),
    std::string(NormalOffButtonName2().Text()), std::string(NormalOffButtonName3().Text())
  );
  this->filterPanel->AddChild(normalButton);

  CUITabButton* subButton = new CUITabButton();
  subButton->SetSize(26, 22);
  subButton->Move(62, 0);
  subButton->SetOnClick([this]() { this->OnPanelButton(); });
  subButton->disabled = 0;
  subButton->Invalidate();
  subButton->SetId(3);
  subButton->SetText(std::string(""));
  subButton->SetFitText(0);
  subButton->SetAlign(0, 0);
  subButton->SetFont(std::string(GulimFontName2().Text()));
  subButton->SetFontHeight(12);
  subButton->SetBold(0);
  subButton->SetTextColors(0, 0, 0, 0);
  subButton->SetShadow(0, 0);
  subButton->SetOutline(0, 0);
  subButton->SetImages(
    std::string(SubOffButtonName1().Text()), std::string(SubOnButtonName().Text()),
    std::string(SubOffButtonName2().Text()), std::string(SubOffButtonName3().Text())
  );
  this->filterPanel->AddChild(subButton);

  CUITabButton* dailyButton = new CUITabButton();
  dailyButton->SetSize(26, 22);
  dailyButton->Move(93, 0);
  dailyButton->SetOnClick([this]() { this->OnCallback43(); });
  dailyButton->disabled = 0;
  dailyButton->Invalidate();
  dailyButton->SetId(4);
  dailyButton->SetText(std::string(""));
  dailyButton->SetFitText(0);
  dailyButton->SetAlign(0, 0);
  dailyButton->SetFont(std::string(GulimFontName3().Text()));
  dailyButton->SetFontHeight(12);
  dailyButton->SetBold(0);
  dailyButton->SetTextColors(0, 0, 0, 0);
  dailyButton->SetShadow(0, 0);
  dailyButton->SetOutline(0, 0);
  dailyButton->SetImages(
    std::string(DailyOffButtonName1().Text()), std::string(DailyOnButtonName().Text()),
    std::string(DailyOffButtonName2().Text()), std::string(DailyOffButtonName3().Text())
  );
  this->filterPanel->AddChild(dailyButton);

  CUITabButton* eventButton = new CUITabButton();
  eventButton->SetSize(26, 22);
  eventButton->Move(124, 0);
  eventButton->SetOnClick([this]() { this->OnCallback74(); });
  eventButton->disabled = 0;
  eventButton->Invalidate();
  eventButton->SetId(5);
  eventButton->SetText(std::string(""));
  eventButton->SetFitText(0);
  eventButton->SetAlign(0, 0);
  eventButton->SetFont(std::string(GulimFontName4().Text()));
  eventButton->SetFontHeight(12);
  eventButton->SetBold(0);
  eventButton->SetTextColors(0, 0, 0, 0);
  eventButton->SetShadow(0, 0);
  eventButton->SetOutline(0, 0);
  eventButton->SetImages(
    std::string(EventOffButtonName1().Text()), std::string(EventOnButtonName().Text()),
    std::string(EventOffButtonName2().Text()), std::string(EventOffButtonName3().Text())
  );
  this->filterPanel->AddChild(eventButton);

  CUITabButton* etcButton = new CUITabButton();
  etcButton->SetSize(26, 22);
  etcButton->Move(155, 0);
  etcButton->SetOnClick([this]() { this->OnCallback80(); });
  etcButton->disabled = 0;
  etcButton->Invalidate();
  etcButton->SetId(1);
  etcButton->SetText(std::string(""));
  etcButton->SetFitText(0);
  etcButton->SetAlign(0, 0);
  etcButton->SetFont(std::string(GulimFontName5().Text()));
  etcButton->SetFontHeight(12);
  etcButton->SetBold(0);
  etcButton->SetTextColors(0, 0, 0, 0);
  etcButton->SetShadow(0, 0);
  etcButton->SetOutline(0, 0);
  etcButton->SetImages(
    std::string(EtcOffButtonName1().Text()), std::string(EtcOnButtonName().Text()),
    std::string(EtcOffButtonName2().Text()), std::string(EtcOffButtonName3().Text())
  );
  this->filterPanel->AddChild(etcButton);

  this->searchEdit = new CUIEditBoxEx();
  this->searchEdit->Create(119, 16, false);
  this->searchEdit->Move(223, 417);
  this->searchEdit->SetText(std::string(""));
  this->searchEdit->SetTextColor(0);
  this->searchEdit->SetBackgroundColor(0xFFFF00FF);
  this->searchEdit->SetBorderColor(0xC6C6C6);
  this->searchEdit->SetBorderWidth(1);
  this->searchEdit->SetMaximum(20);
  this->searchEdit->SetEditBounds(0, 0, 0, 0);
  this->searchEdit->SetMask(0);
  this->searchEdit->SetAlignment(0);
  this->searchEdit->SetOnSubmit([this]() {
    this->searchText = this->searchEdit->edit->Text();
    this->UpdateQuestLists(0xF);
  });
  this->AddChild(this->searchEdit);

  this->searchButton = new CUIImageButton();
  this->searchButton->SetSize(23, 18);
  this->searchButton->Move(342, 416);
  this->searchButton->SetOnClick([this]() {
    this->searchText = this->searchEdit->edit->Text();
    this->UpdateQuestLists(0xF);
  });
  CUIImageButton* searchButton = this->searchButton;
  searchButton->disabled = 0;
  searchButton->Invalidate();
  this->searchButton->SetImages(
    std::string(SearchNormalButtonName().Text()), std::string(SearchPressButtonName().Text()),
    std::string(SearchOverButtonName().Text()), std::string(SearchDisableButtonName().Text())
  );
  this->AddChild(this->searchButton);

  processButton->page = this->questLists[0];
  recommendedButton->page = this->questLists[1];
  holdButton->page = this->questLists[2];
  waitButton->page = this->questLists[3];
  this->tab->Select(processButton);
  this->filterPanel->Select(allButton);
}

// OFFSET: 0x00A63930
// FUNCTION: fn_16439
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (RenewQuestUI::CUIQuestItem*& slot : this->items) {
      RenewQuestUI::CUIQuestItem* item = new RenewQuestUI::CUIQuestItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (RenewQuestUI::CUIQuestItem* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A63CD0
// FUNCTION: fn_16442
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (RenewQuestUI::CUIQuestItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A63EE0
// FUNCTION: fn_16443
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A659B0
// FUNCTION: fn_16597
RenewQuestUI::CUIQuestItem::CUIQuestItem()
    : CUIListItem(), icon(nullptr), titleText(nullptr), descriptionText(nullptr), simpleViewCheckBox(nullptr), activeCheckBox(nullptr), questType(1),
      questId(0), pressed(0), baseX(0), baseY(0) {
}

// OFFSET: 0x00A65A70
// FUNCTION: fn_16598
RenewQuestUI::CUIQuestList::CUIQuestList() : CUIControl(), list(nullptr), type(1) {
}

// OFFSET: 0x00A67E90
// FUNCTION: fn_45515
void CUIRenewQuestUI::OnProcessTabClick() {
  this->RecordValues(0x16, 1, 0, 0, 0);
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    if (list->type == 1) {
      if (list->list) {
        list->list->scrollBar->SetPosition(0);
      }
      return;
    }
  }
}

// OFFSET: 0x00A67EE0
// FUNCTION: fn_45516
void CUIRenewQuestUI::OnRecommendedTabClick() {
  this->RecordValues(0x16, 2, 0, 0, 0);
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    if (list->type == 2) {
      if (list->list) {
        if (s_recommendedTabFirstSelect) {
          list->list->SelectIndex(0);
          s_recommendedTabFirstSelect = false;
          return;
        }
        list->list->scrollBar->SetPosition(0);
      }
      return;
    }
  }
}

// OFFSET: 0x00A67E40
// FUNCTION: fn_45517
void CUIRenewQuestUI::OnHoldTabClick() {
  this->RecordValues(0x16, 4, 0, 0, 0);
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    if (list->type == 4) {
      if (list->list) {
        list->list->scrollBar->SetPosition(0);
      }
      return;
    }
  }
}

// OFFSET: 0x00A67F50
// FUNCTION: fn_45518
void CUIRenewQuestUI::OnWaitTabClick() {
  this->RecordValues(0x16, 8, 0, 0, 0);
  for (RenewQuestUI::CUIQuestList* list : this->questLists) {
    if (list->type == 8) {
      if (list->list) {
        list->list->scrollBar->SetPosition(0);
      }
      return;
    }
  }
}

// OFFSET: 0x00A66230
// FUNCTION: fn_45521
void CUIRenewQuestUI::OnSearchSubmit() {
  this->searchText = this->searchEdit->edit->Text();
  this->UpdateQuestLists(0xF);
}

// OFFSET: 0x00A66100
// FUNCTION: fn_45519
void CUIRenewQuestUI::OnClose() {
  g_windowMgr.CloseWindow(WID_RENEWQUESTUI);
}

// OFFSET: 0x00A66370
// FUNCTION: fn_45520
void CUIRenewQuestUI::OnSearchButton() {
  this->searchText = this->searchEdit->edit->Text();
  this->UpdateQuestLists(0xF);
}

// OFFSET: 0x00A65AF0
// FUNCTION: fn_16599
CUIRenewQuestUI::CUIRenewQuestUI()
    : tab(nullptr), checkBox(nullptr), filterLabel(nullptr), closeButton(nullptr), questLists(), searchBackground(nullptr), filterPanel(nullptr), searchEdit(nullptr), searchButton(nullptr), filter(0), searchText(),
      titleBar(nullptr), background(nullptr) {
}

// OFFSET: 0x00A66DA0
// FUNCTION: fn_16624
void CUIRenewQuestUI::OnInit() {
  g_windowMgr.RecordWindowPacket(0x2718);
  g_windowMgr.CloseWindow(WID_NOTIFYQUESTWND);

  this->titleBar->SetTitle(LookupMessage(MSI_RENEW_QUEST_DETAIL_REWARD));
  this->filterLabel->SetText(LookupMessage(MSI_QUEST_DISPLAY_ONOFF));

  bool visible = false;
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_QUESTDISPLAY)) {
    visible = window->geometry.visible != 0;
  }
  CUICheckBox* checkBox = this->checkBox;
  checkBox->checked = visible;
  checkBox->Invalidate();

  for (unsigned int index = 0; index < this->questLists.size(); ++index) {
    RenewQuestUI::CUIQuestList* list = this->questLists[index];
    list->type = 1 << index;
  }
  this->UpdateQuestLists(0xF);

  for (UIWindow* child : this->filterPanel->m_children) {
    const std::string& name = g_renewQuestMgr->GetFilterName(child->geometry.id);
    static_cast<CUITabButton*>(child)->tooltipText = name;
  }
}
