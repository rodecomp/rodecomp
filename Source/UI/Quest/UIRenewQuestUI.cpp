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

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <format>

using RenewQuestOngoingTree = std::_Tree<std::_Tmap_traits<
  unsigned int, RenewQuestOngoingInfo, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, RenewQuestOngoingInfo>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestOngoingInfo>, void*>*>
RenewQuestOngoingTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;

using RenewQuestRecommendedTree = std::_Tree<std::_Tmap_traits<
  unsigned int, RenewQuestRecommendedInfo, std::less<unsigned int>, std::allocator<std::pair<const unsigned int, RenewQuestRecommendedInfo>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestRecommendedInfo>, void*>*>
RenewQuestRecommendedTree::_Find_lower_bound<unsigned int>(const unsigned int&) const;

extern template std::function<void()>& std::function<void()>::operator=(const std::function<void()>&);

// GLOBAL: gl_25249
static bool s_recommendedTabFirstSelect = true;


// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

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
  const std::uint8_t source[] = {0x0E, 0x18, 0x13, 0x8A, 0xAA, 0x01};
  thread_local EncryptedName<sizeof(source), 0x114901C7E37F6D49ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A620
// FUNCTION: fn_16401
__declspec(noinline) EncryptedName<48, 0x136F2F9915D1DBCFULL>& NormalOffButtonName1() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x136F2F9915D1DBCFULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x136F2F9915D1DBCFULL> name(source);
  return name;
}

// OFFSET: 0x00A5A700
// FUNCTION: fn_16402
__declspec(noinline) EncryptedName<47, 0x0B23D10F2F49D72BULL>& NormalOnButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_normal_on.bmp", 0x0B23D10F2F49D72BULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x0B23D10F2F49D72BULL> name(source);
  return name;
}

// OFFSET: 0x00A5A7F0
// FUNCTION: fn_16403
__declspec(noinline) EncryptedName<48, 0x674D9F3975573D41ULL>& NormalOffButtonName2() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x674D9F3975573D41ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x674D9F3975573D41ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A8D0
// FUNCTION: fn_16404
__declspec(noinline) EncryptedName<48, 0x6BDF334F2BFDAD51ULL>& NormalOffButtonName3() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_normal_off.bmp", 0x6BDF334F2BFDAD51ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x6BDF334F2BFDAD51ULL> name(source);
  return name;
}

// OFFSET: 0x00A5A9B0
// FUNCTION: fn_16405
__declspec(noinline) EncryptedName<6, 0xB7AFBD2FD9FD7501ULL>& GulimFontName2() {
  const std::uint8_t source[] = {0x46, 0x00, 0x91, 0xB0, 0x42, 0xBD};
  thread_local EncryptedName<sizeof(source), 0xB7AFBD2FD9FD7501ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AA80
// FUNCTION: fn_16406
__declspec(noinline) EncryptedName<45, 0x79E915E9AF6F55DDULL>& SubOffButtonName1() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0x79E915E9AF6F55DDULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x79E915E9AF6F55DDULL> name(source);
  return name;
}

// OFFSET: 0x00A5AB70
// FUNCTION: fn_16407
__declspec(noinline) EncryptedName<44, 0xB3E307A5F753D5E3ULL>& SubOnButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_sub_on.bmp", 0xB3E307A5F753D5E3ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xB3E307A5F753D5E3ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AC50
// FUNCTION: fn_16408
__declspec(noinline) EncryptedName<45, 0x192D654D29337BE5ULL>& SubOffButtonName2() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0x192D654D29337BE5ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x192D654D29337BE5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AD40
// FUNCTION: fn_16409
__declspec(noinline) EncryptedName<45, 0xF15F8785CDE369BFULL>& SubOffButtonName3() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_sub_off.bmp", 0xF15F8785CDE369BFULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xF15F8785CDE369BFULL> name(source);
  return name;
}

// OFFSET: 0x00A5AE30
// FUNCTION: fn_16410
__declspec(noinline) EncryptedName<6, 0xE33F2FF779878559ULL>& GulimFontName3() {
  const std::uint8_t source[] = {0x1E, 0xF0, 0xEB, 0x10, 0x9A, 0x2F};
  thread_local EncryptedName<sizeof(source), 0xE33F2FF779878559ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AF00
// FUNCTION: fn_16411
__declspec(noinline) EncryptedName<47, 0x49D33529F1556333ULL>& DailyOffButtonName1() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0x49D33529F1556333ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x49D33529F1556333ULL> name(source);
  return name;
}

// OFFSET: 0x00A5AFF0
// FUNCTION: fn_16412
__declspec(noinline) EncryptedName<46, 0x792FFD1325418FB5ULL>& DailyOnButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_daily_on.bmp", 0x792FFD1325418FB5ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x792FFD1325418FB5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B0E0
// FUNCTION: fn_16413
__declspec(noinline) EncryptedName<47, 0xC5B1BBCD9BC9C997ULL>& DailyOffButtonName2() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0xC5B1BBCD9BC9C997ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xC5B1BBCD9BC9C997ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B1D0
// FUNCTION: fn_16414
__declspec(noinline) EncryptedName<47, 0x01CB4F1DF9D3F931ULL>& DailyOffButtonName3() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_daily_off.bmp", 0x01CB4F1DF9D3F931ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x01CB4F1DF9D3F931ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B2C0
// FUNCTION: fn_16415
__declspec(noinline) EncryptedName<6, 0x1767DDE18D779D8BULL>& GulimFontName4() {
  const std::uint8_t source[] = {0xCC, 0xE8, 0x1B, 0xE4, 0x8C, 0xDD};
  thread_local EncryptedName<sizeof(source), 0x1767DDE18D779D8BULL> name(source);
  return name;
}

// OFFSET: 0x00A5B390
// FUNCTION: fn_16416
__declspec(noinline) EncryptedName<47, 0xC1BB8111EDD183E1ULL>& EventOffButtonName1() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0xC1BB8111EDD183E1ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xC1BB8111EDD183E1ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B480
// FUNCTION: fn_16417
__declspec(noinline) EncryptedName<46, 0x271B45771F83DD9FULL>& EventOnButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_event_on.bmp", 0x271B45771F83DD9FULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x271B45771F83DD9FULL> name(source);
  return name;
}

// OFFSET: 0x00A5B570
// FUNCTION: fn_16418
__declspec(noinline) EncryptedName<47, 0x8783C33941E15117ULL>& EventOffButtonName2() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0x8783C33941E15117ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x8783C33941E15117ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B660
// FUNCTION: fn_16419
__declspec(noinline) EncryptedName<47, 0x0B8745519B1F95D7ULL>& EventOffButtonName3() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_event_off.bmp", 0x0B8745519B1F95D7ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x0B8745519B1F95D7ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B750
// FUNCTION: fn_16420
__declspec(noinline) EncryptedName<6, 0xC70319B5EB61598FULL>& GulimFontName5() {
  const std::uint8_t source[] = {0xC8, 0x2C, 0x0D, 0x82, 0xD8, 0x19};
  thread_local EncryptedName<sizeof(source), 0xC70319B5EB61598FULL> name(source);
  return name;
}

// OFFSET: 0x00A5B820
// FUNCTION: fn_16421
__declspec(noinline) EncryptedName<45, 0xFFABE3D5D1BBEF73ULL>& EtcOffButtonName1() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0xFFABE3D5D1BBEF73ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xFFABE3D5D1BBEF73ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B910
// FUNCTION: fn_16422
__declspec(noinline) EncryptedName<44, 0xA597A7E7492B39D5ULL>& EtcOnButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_etc_on.bmp", 0xA597A7E7492B39D5ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xA597A7E7492B39D5ULL> name(source);
  return name;
}

// OFFSET: 0x00A5B9F0
// FUNCTION: fn_16423
__declspec(noinline) EncryptedName<45, 0xD5A78DB38D8DAFABULL>& EtcOffButtonName2() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0xD5A78DB38D8DAFABULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xD5A78DB38D8DAFABULL> name(source);
  return name;
}

// OFFSET: 0x00A5BAE0
// FUNCTION: fn_16424
__declspec(noinline) EncryptedName<45, 0x0399459DAF454F0FULL>& EtcOffButtonName3() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_etc_off.bmp", 0x0399459DAF454F0FULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x0399459DAF454F0FULL> name(source);
  return name;
}

// OFFSET: 0x00A5BBD0
// FUNCTION: fn_16425
__declspec(noinline) EncryptedName<51, 0xAD6153019F93A527ULL>& SearchNormalButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_search_normal.bmp", 0xAD6153019F93A527ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xAD6153019F93A527ULL> name(source);
  return name;
}

// OFFSET: 0x00A5BCB0
// FUNCTION: fn_16426
__declspec(noinline) EncryptedName<50, 0x85E1F5A7A5755B7FULL>& SearchPressButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_search_press.bmp", 0x85E1F5A7A5755B7FULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x85E1F5A7A5755B7FULL> name(source);
  return name;
}

// OFFSET: 0x00A5BD90
// FUNCTION: fn_16427
__declspec(noinline) EncryptedName<49, 0x659975C121557D3DULL>& SearchOverButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_search_over.bmp", 0x659975C121557D3DULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0x659975C121557D3DULL> name(source);
  return name;
}

// OFFSET: 0x00A5BE70
// FUNCTION: fn_16428
__declspec(noinline) EncryptedName<52, 0xBFF5C517A1A77F59ULL>& SearchDisableButtonName() {
  constexpr auto sourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\btn_search_disable.bmp", 0xBFF5C517A1A77F59ULL);
  const auto& source = sourceText.data;
  thread_local EncryptedName<sizeof(source), 0xBFF5C517A1A77F59ULL> name(source);
  return name;
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
  auto it = this->items.begin();
  if (it == this->items.end()) {
    return;
  }
  if (this->data.size() == 0) {
    for (; it != this->items.end(); ++it) {
      RenewQuestUI::CUIQuestItem* item = *it;
      item->index = -1;
      item->Show(0);
    }
    return;
  }

  const int first = (*it)->index;
  int row = position / this->rowHeight;
  if (first != -1) {
    if (first < row) {
      if (static_cast<unsigned int>(row - first) < this->items.size()) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > row) {
      if (static_cast<unsigned int>(first - row) < this->items.size()) {
        std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
      }
    }
  }

  it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || static_cast<unsigned int>(row) >= this->data.size()) {
      break;
    }
    RenewQuestUI::CUIQuestItem* item = *it;
    if (item) {
      if (this->rebind || row != item->index) {
        item->index = row;
        if (this->binder) {
          this->binder(item, *std::next(this->data.begin(), row));
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(1);
    }
  }

  for (; it != this->items.end(); ++it) {
    (*it)->index = -1;
    (*it)->Show(0);
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
    for (auto node = listBox->data.begin(); node != listBox->data.end(); ++node) {
      if (*node == questId) {
        this->tab->SelectPage(list);
        CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>* pageListBox = list->list;

        int index = 0;
        for (auto it = listBox->data.begin(); it != node; ++it) {
          ++index;
        }

        pageListBox->scrollBar->SetPosition(pageListBox->rowHeight * index);

        for (RenewQuestUI::CUIQuestItem* item : pageListBox->items) {
          if (item->index == index) {
            pageListBox->SelectItem(item);
            return;
          }
        }

        return;
      }
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
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
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
          if (!this->MatchesFilter(entry.second)) {
            continue;
          }
          if (entry.second.questType != 1 && entry.second.m_state == 0) {
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
          if (!this->MatchesFilter(entry.second)) {
            continue;
          }
          if (entry.second.questType == 1) {
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
  this->titleText->SetFont(std::string(DecryptName<0xAD075B8DB599F1CDULL>({'\x8A', '\x84', '\xF5', '\xDC', '\xE0', '\x5B'})));
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
  this->descriptionText->SetFont(std::string(DecryptName<0xDD3913D5D7F97BE1ULL>({'\xA6', '\x0E', '\x95', '\xBE', '\xB8', '\x13'})));
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
    std::string(DecryptName<0x050505679903579BULL>({
      '\x5B', '\xF8', '\xC3', '\x63', '\xA7', '\xCB', '\xC0', '\xC8', '\x5D', '\xB3', '\xC3', '\x55', '\xDA', '\xBF', '\x59', '\x57',
      '\xFE', '\x39', '\x66', '\xEE', '\x38', '\x54', '\x70', '\x60', '\xE8', '\x23', '\x56', '\xD0', '\x3B', '\x67', '\x71', '\x5A',
      '\xF8', '\x3F', '\x66', '\xFA', '\x0C', '\x5A', '\x6A', '\x63', '\xFD', '\x79', '\x61', '\xF4', '\x17', '\x05'
    })),
    std::string(DecryptName<0x83EFBFBB37EDBDBDULL>({
      '\x7D', '\x12', '\x2D', '\xCD', '\x7B', '\x71', '\x2A', '\x4E', '\x7B', '\x59', '\x2D', '\xFB', '\x06', '\x05', '\xB3', '\xD1',
      '\xD8', '\xD3', '\x88', '\x40', '\xE4', '\xEE', '\x9A', '\xE6', '\xCE', '\xC9', '\xB8', '\x7E', '\xE7', '\xDD', '\x9B', '\xDC',
      '\xDE', '\xD5', '\x88', '\x54', '\xD0', '\xE0', '\x80', '\xED', '\x93', '\xDF', '\x80', '\x47', '\xBB'
    })),
    std::string(DecryptName<0xBF2B4B31397FF5A7ULL>({
      '\x67', '\x5A', '\xBF', '\xC3', '\xF1', '\x85', '\xEE', '\x72', '\x61', '\x11', '\xBF', '\xF5', '\x8C', '\xF1', '\x77', '\xED',
      '\xC2', '\x9B', '\x1A', '\x4E', '\x6E', '\x1A', '\x5E', '\xDA', '\xD4', '\x81', '\x2A', '\x70', '\x6D', '\x29', '\x5F', '\xE0',
      '\xC4', '\x9D', '\x1A', '\x5A', '\x5A', '\x14', '\x44', '\xD9', '\xC1', '\xDB', '\x1D', '\x54', '\x41', '\x4B'
    })),
    std::string(DecryptName<0xD97D5DBDBB2F2709ULL>({
      '\xC9', '\x88', '\xEF', '\x41', '\x7D', '\x93', '\xB8', '\x14', '\xCF', '\xC3', '\xEF', '\x77', '\x00', '\xE7', '\x21', '\x8B',
      '\x6C', '\x49', '\x4A', '\xCC', '\xE2', '\x0C', '\x08', '\xBC', '\x7A', '\x53', '\x7A', '\xF2', '\xE1', '\x3F', '\x09', '\x86',
      '\x6A', '\x4F', '\x4A', '\xD8', '\xD6', '\x02', '\x12', '\xBF', '\x6F', '\x09', '\x4D', '\xD6', '\xCD', '\x5D'
    }))
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
    std::string(DecryptName<0xED796785D1DF137FULL>({
      '\xBF', '\xBC', '\x1F', '\x2B', '\x45', '\xA9', '\xBC', '\x20', '\xB9', '\xF7', '\x1F', '\x1D', '\x38', '\xDD', '\x25', '\xBF',
      '\x1A', '\x7D', '\xBA', '\xA6', '\xDA', '\x36', '\x0C', '\x88', '\x0C', '\x67', '\x8A', '\x98', '\xD9', '\x05', '\x0D', '\xB2',
      '\x13', '\x7C', '\xBC', '\xBA', '\xAB', '\x05', '\x14', '\x9D', '\x7F'
    })),
    std::string(DecryptName<0xF14F79170939C7F1ULL>({
      '\x31', '\x68', '\xF9', '\xF3', '\xD7', '\xB7', '\x8A', '\x3C', '\x37', '\x23', '\xF9', '\xC5', '\xAA', '\xC3', '\x13', '\xA3',
      '\x94', '\xA9', '\x5C', '\x7E', '\x48', '\x28', '\x3A', '\x94', '\x82', '\xB3', '\x6C', '\x40', '\x4B', '\x1B', '\x3B', '\xAE',
      '\x9D', '\xA8', '\x5A', '\x62', '\x48', '\x16', '\x3F', '\x94', '\x9F', '\xE9', '\x5B', '\x64', '\x67', '\x79'
    })),
    std::string(DecryptName<0xA5FD29D7AB6DC59FULL>({
      '\x5F', '\x6A', '\xAD', '\x51', '\x17', '\xE7', '\x38', '\x68', '\x59', '\x21', '\xAD', '\x67', '\x6A', '\x93', '\xA1', '\xF7',
      '\xFA', '\xAB', '\x08', '\xDC', '\x88', '\x78', '\x88', '\xC0', '\xEC', '\xB1', '\x38', '\xE2', '\x8B', '\x4B', '\x89', '\xFA',
      '\xF3', '\xAA', '\x0E', '\xC0', '\xF9', '\x4B', '\x90', '\xD5', '\x9F'
    })),
    std::string(DecryptName<0x13E33FA1A591AB67ULL>({
      '\xA7', '\x04', '\x51', '\x5F', '\x61', '\xF1', '\x26', '\xDE', '\xA1', '\x4F', '\x51', '\x69', '\x1C', '\x85', '\xBF', '\x41',
      '\x02', '\xC5', '\xF4', '\xD2', '\xFE', '\x6E', '\x96', '\x76', '\x14', '\xDF', '\xC4', '\xEC', '\xFD', '\x5D', '\x97', '\x4C',
      '\x0B', '\xC4', '\xF2', '\xCE', '\x8F', '\x5D', '\x8E', '\x63', '\x67'
    }))
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
    constexpr auto disabledSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0x23BBD58F8B5FF5C9ULL);
    const auto& disabledSource = disabledSourceText.data;
    const std::string disabled(DecryptName<0x23BBD58F8B5FF5C9ULL>(disabledSource));
    constexpr auto pressedSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0xCD8F077123FB7D6FULL);
    const auto& pressedSource = pressedSourceText.data;
    const std::string pressed(DecryptName<0xCD8F077123FB7D6FULL>(pressedSource));
    constexpr auto hoverSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bg_questList_check.bmp", 0x9949BFD527532943ULL);
    const auto& hoverSource = hoverSourceText.data;
    const std::string hover(DecryptName<0x9949BFD527532943ULL>(hoverSource));
    constexpr auto normalSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bg_questList.bmp", 0x17A9E5BF39AD4737ULL);
    const auto& normalSource = normalSourceText.data;
    const std::string normal(DecryptName<0x17A9E5BF39AD4737ULL>(normalSource));
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
    constexpr auto downSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp", 0x03772763F96D9DA5ULL);
    const auto& downSource = downSourceText.data;
    const std::string down(DecryptName<0x03772763F96D9DA5ULL>(downSource));
    constexpr auto upSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", 0xC517FB932D95BB6DULL);
    const auto& upSource = upSourceText.data;
    const std::string up(DecryptName<0xC517FB932D95BB6DULL>(upSource));
    list->SetScrollBarButtonImages(up, down);
  }
  this->list->SetScrollBarBackground("");
  {
    CUIListBoxEx<CUIQuestItem, unsigned long>* list = this->list;
    constexpr auto bottomSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp", 0xEDC74F6D13DF71EFULL);
    const auto& bottomSource = bottomSourceText.data;
    const std::string bottom(DecryptName<0xEDC74F6D13DF71EFULL>(bottomSource));
    constexpr auto middleSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp", 0xF161095D0DE18F95ULL);
    const auto& middleSource = middleSourceText.data;
    const std::string middle(DecryptName<0xF161095D0DE18F95ULL>(middleSource));
    constexpr auto topSourceText = CRYPT_NAME("유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", 0x0B7109397F6B1387ULL);
    const auto& topSource = topSourceText.data;
    const std::string top(DecryptName<0x0B7109397F6B1387ULL>(topSource));
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
  this->titleBar->SetImages(std::string(DecryptName<0x75ADC57D87EFBD8FULL>({
    '\x4F', '\x12', '\x2F', '\x7D', '\xBD', '\x0B', '\x68', '\xB8', '\x49', '\x59', '\x2F', '\x4B', '\xC0', '\x7F', '\xF1', '\x17',
    '\xEE', '\xCE', '\x86', '\xE4', '\x22', '\xAC', '\xC3', '\x01', '\xEA', '\xCF', '\x89', '\xE6', '\x1E', '\xA0', '\xF1', '\x01',
    '\xE6', '\xC9', '\x83', '\xE2', '\x1F', '\xA4', '\xDF', '\x2A', '\xE3', '\xD8', '\x89', '\xF3', '\x53', '\xA7', '\xC0', '\x05',
    '\x8F'
  })),
  std::string(DecryptName<0x2BAD156DE3E1F9D9ULL>({
    '\x19', '\x56', '\x21', '\x19', '\xAD', '\xDB', '\x68', '\xE6', '\x1F', '\x1D', '\x21', '\x2F', '\xD0', '\xAF', '\xF1', '\x49',
    '\xB8', '\x8A', '\x88', '\x80', '\x32', '\x7C', '\xC3', '\x5F', '\xBC', '\x8B', '\x87', '\x82', '\x0E', '\x70', '\xF1', '\x5F',
    '\xB0', '\x8D', '\x8D', '\x86', '\x0F', '\x74', '\xDF', '\x74', '\xB4', '\x90', '\x85', '\xCD', '\x0F', '\x78', '\xDD', '\x2B'
  })),
  std::string(DecryptName<0x637B959D55771F69ULL>({
    '\xA9', '\xB0', '\xB7', '\xAF', '\x5D', '\x5B', '\xBE', '\xAE', '\xAF', '\xFB', '\xB7', '\x99', '\x20', '\x2F', '\x27', '\x01',
    '\x08', '\x6C', '\x1E', '\x36', '\xC2', '\xFC', '\x15', '\x17', '\x0C', '\x6D', '\x11', '\x34', '\xFE', '\xF0', '\x27', '\x17',
    '\x00', '\x6B', '\x1B', '\x30', '\xFF', '\xF4', '\x09', '\x3C', '\x1B', '\x76', '\x10', '\x3D', '\xE9', '\xBB', '\x19', '\x0E',
    '\x19', '\x1F'
  })));
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText(DecryptName<0xD1413DBB01CD8B51ULL>({'\x00', '\xDE', '\x88', '\x52', '\xEF', '\x3D'}));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(std::string(DecryptName<0x758F1D0B4F7705B9ULL>({'\xFE', '\x70', '\x1B', '\x26', '\x66', '\x1D'})));
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
    std::string(DecryptName<0x891B232D09451965ULL>({
      '\xA5', '\xB6', '\x85', '\xF3', '\xED', '\xED', '\xDE', '\x44', '\xA3', '\xFD', '\x85', '\xC5', '\x90', '\x99', '\x47', '\xEB',
      '\x04', '\x6A', '\x2C', '\x6A', '\x72', '\x4A', '\x75', '\xFD', '\x00', '\x6B', '\x23', '\x68', '\x4E', '\x46', '\x47', '\xFA',
      '\x1C', '\x6A', '\x1A', '\x6B', '\x4C', '\x50', '\x7E', '\xD6', '\x0A', '\x7F', '\x23', '\x27', '\x4F', '\x4E', '\x6B', '\x89'
    })),
    std::string(DecryptName<0xEF6D1D893F2B2DC5ULL>({
      '\x05', '\x82', '\xEB', '\xC5', '\x49', '\xD3', '\xA8', '\x22', '\x03', '\xC9', '\xEB', '\xF3', '\x34', '\xA7', '\x31', '\x8D',
      '\xA4', '\x5E', '\x42', '\x5C', '\xD6', '\x74', '\x03', '\x9B', '\xA0', '\x5F', '\x4D', '\x5E', '\xEA', '\x78', '\x31', '\x9C',
      '\xBC', '\x5E', '\x74', '\x5D', '\xE8', '\x6E', '\x08', '\xB0', '\xAA', '\x43', '\x05', '\x5D', '\xE4', '\x6D', '\x6D'
    })),
    std::string(DecryptName<0x234977DDB54BC71DULL>({
      '\xDD', '\x68', '\x8B', '\x4F', '\x1D', '\xB9', '\x8C', '\xEE', '\xDB', '\x23', '\x8B', '\x79', '\x60', '\xCD', '\x15', '\x41',
      '\x7C', '\xB4', '\x22', '\xD6', '\x82', '\x1E', '\x27', '\x57', '\x78', '\xB5', '\x2D', '\xD4', '\xBE', '\x12', '\x15', '\x50',
      '\x64', '\xB4', '\x14', '\xD7', '\xBC', '\x04', '\x2C', '\x7C', '\x72', '\xA9', '\x65', '\xD7', '\xB0', '\x07', '\x49'
    })),
    std::string(DecryptName<0x6FE153231D879777ULL>({
      '\xB7', '\x38', '\x47', '\xE7', '\xE3', '\x9D', '\x24', '\xA2', '\xB1', '\x73', '\x47', '\xD1', '\x9E', '\xE9', '\xBD', '\x0D',
      '\x16', '\xE4', '\xEE', '\x7E', '\x7C', '\x3A', '\x8F', '\x1B', '\x12', '\xE5', '\xE1', '\x7C', '\x40', '\x36', '\xBD', '\x1C',
      '\x0E', '\xE4', '\xD8', '\x7F', '\x42', '\x20', '\x84', '\x30', '\x18', '\xF1', '\xE1', '\x33', '\x41', '\x3E', '\x91', '\x6F'
    }))
  );
  this->titleBar->AddChild(systemButton);

  this->background = new CUIStaticImage();
  this->background->SetSize(352, 449);
  this->background->Move(29, 17);
  this->background->SetImage(std::string(DecryptName<0x39A39F87DD5F535BULL>({
    '\x9B', '\xFC', '\x9F', '\x27', '\x47', '\x51', '\x66', '\xF4', '\x9D', '\xB7', '\x9F', '\x11', '\x3A', '\x25', '\xFF', '\x6B',
    '\x3E', '\x3D', '\x3A', '\xAA', '\xD8', '\xCE', '\xD6', '\x5C', '\x28', '\x27', '\x0A', '\x94', '\xDB', '\xFD', '\xC4', '\x66',
    '\x2A', '\x26', '\x3A', '\xAE', '\xF3', '\xB1', '\xC1', '\x54', '\x2B', '\x53'
  })));
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
  processButton->SetFont(std::string(DecryptName<0x2FEB8739E5975BD1ULL>({'\x96', '\x2E', '\xFB', '\x8C', '\x54', '\x87'})));
  processButton->SetFontHeight(12);
  processButton->SetBold(0);
  processButton->SetTextColors(0, 0, 0, 0);
  processButton->SetShadow(0, 0);
  processButton->SetOutline(0, 0);
  processButton->SetImages(
    std::string(DecryptName<0x33133B977BCD03CDULL>({
      '\x0D', '\xAC', '\x0D', '\x81', '\x57', '\xF5', '\xD6', '\xFE', '\x0B', '\xE7', '\x0D', '\xB7', '\x2A', '\x81', '\x4F', '\x61',
      '\xA8', '\x6D', '\xA8', '\x0C', '\xC8', '\x6A', '\x66', '\x56', '\xBE', '\x77', '\x98', '\x32', '\xCB', '\x4F', '\x72', '\x51',
      '\x92', '\x73', '\xBF', '\x14', '\xF4', '\x5E', '\x60', '\x40', '\x92', '\x6C', '\xAB', '\x1D', '\xB9', '\x59', '\x7E', '\x43',
      '\xCD'
    })),
    std::string(DecryptName<0xA12DAB392545D72FULL>({
      '\xEF', '\x78', '\x85', '\xDF', '\xF9', '\x65', '\xE8', '\x6C', '\xE9', '\x33', '\x85', '\xE9', '\x84', '\x11', '\x71', '\xF3',
      '\x4A', '\xB9', '\x20', '\x52', '\x66', '\xFA', '\x58', '\xC4', '\x5C', '\xA3', '\x10', '\x6C', '\x65', '\xDF', '\x4C', '\xC3',
      '\x70', '\xA7', '\x37', '\x4A', '\x5A', '\xCE', '\x5E', '\xD2', '\x70', '\xB8', '\x2B', '\x0B', '\x5B', '\xC6', '\x5D', '\xA1'
    })),
    std::string(DecryptName<0x6BA1C1132F033B75ULL>({
      '\xB5', '\x94', '\xC3', '\xD5', '\xD3', '\x0F', '\x64', '\xA6', '\xB3', '\xDF', '\xC3', '\xE3', '\xAE', '\x7B', '\xFD', '\x39',
      '\x10', '\x55', '\x66', '\x58', '\x4C', '\x90', '\xD4', '\x0E', '\x06', '\x4F', '\x56', '\x66', '\x4F', '\xB5', '\xC0', '\x09',
      '\x2A', '\x4B', '\x71', '\x40', '\x70', '\xA4', '\xD2', '\x18', '\x2A', '\x54', '\x65', '\x49', '\x3D', '\xA3', '\xCC', '\x1B',
      '\x75'
    })),
    std::string(DecryptName<0x2DCB7567394DC1ABULL>({
      '\x6B', '\x6E', '\x8D', '\xC3', '\xA7', '\xBB', '\x0E', '\xE0', '\x6D', '\x25', '\x8D', '\xF5', '\xDA', '\xCF', '\x97', '\x7F',
      '\xCE', '\xAF', '\x28', '\x4E', '\x38', '\x24', '\xBE', '\x48', '\xD8', '\xB5', '\x18', '\x70', '\x3B', '\x01', '\xAA', '\x4F',
      '\xF4', '\xB1', '\x3F', '\x56', '\x04', '\x10', '\xB8', '\x5E', '\xF4', '\xAE', '\x2B', '\x5F', '\x49', '\x17', '\xA6', '\x5D',
      '\xAB'
    }))
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
  recommendedButton->SetFont(std::string(DecryptName<0x4F637DC51307B759ULL>({'\x1E', '\xC2', '\x6B', '\x7A', '\xA8', '\x7D'})));
  recommendedButton->SetFontHeight(12);
  recommendedButton->SetBold(0);
  recommendedButton->SetTextColors(0, 0, 0, 0);
  recommendedButton->SetShadow(0, 0);
  recommendedButton->SetOutline(0, 0);
  recommendedButton->SetImages(
    std::string(DecryptName<0x8D1D0FF1AF2B13F7ULL>({
      '\x37', '\xBC', '\xEB', '\x55', '\x31', '\xC1', '\xD8', '\x40', '\x31', '\xF7', '\xEB', '\x63', '\x4C', '\xB5', '\x41', '\xDF',
      '\x92', '\x7D', '\x4E', '\xD8', '\xAE', '\x5E', '\x68', '\xE8', '\x84', '\x67', '\x7E', '\xE6', '\xAD', '\x7B', '\x7C', '\xEF',
      '\xA8', '\x61', '\x4E', '\xCC', '\x9E', '\x62', '\x70', '\xE8', '\x99', '\x77', '\x4E', '\xCB', '\xAE', '\x60', '\x7B', '\xEB',
      '\xD9', '\x71', '\x46', '\xDF', '\xF1'
    })),
    std::string(DecryptName<0xB39FA7917BCFBB8DULL>({
      '\x4D', '\x14', '\x0F', '\x81', '\x51', '\x69', '\x5A', '\x7E', '\x4B', '\x5F', '\x0F', '\xB7', '\x2C', '\x1D', '\xC3', '\xE1',
      '\xE8', '\xD5', '\xAA', '\x0C', '\xCE', '\xF6', '\xEA', '\xD6', '\xFE', '\xCF', '\x9A', '\x32', '\xCD', '\xD3', '\xFE', '\xD1',
      '\xD2', '\xC9', '\xAA', '\x18', '\xFE', '\xCA', '\xF2', '\xD6', '\xE3', '\xDF', '\xAA', '\x1F', '\xCE', '\xC8', '\xF1', '\x9D',
      '\xEF', '\xD6', '\xBF', '\x7B'
    })),
    std::string(DecryptName<0x9171997D6963C9F7ULL>({
      '\x37', '\x66', '\xA3', '\x93', '\xBD', '\x57', '\xB4', '\x5C', '\x31', '\x2D', '\xA3', '\xA5', '\xC0', '\x23', '\x2D', '\xC3',
      '\x92', '\xA7', '\x06', '\x1E', '\x22', '\xC8', '\x04', '\xF4', '\x84', '\xBD', '\x36', '\x20', '\x21', '\xED', '\x10', '\xF3',
      '\xA8', '\xBB', '\x06', '\x0A', '\x12', '\xF4', '\x1C', '\xF4', '\x99', '\xAD', '\x06', '\x0D', '\x22', '\xF6', '\x17', '\xF7',
      '\xD9', '\xAB', '\x0E', '\x19', '\x7D'
    })),
    std::string(DecryptName<0x4F99DF03C3EF31B7ULL>({
      '\x77', '\x9E', '\x2F', '\x39', '\xC3', '\x11', '\x5C', '\x82', '\x71', '\xD5', '\x2F', '\x0F', '\xBE', '\x65', '\xC5', '\x1D',
      '\xD2', '\x5F', '\x8A', '\xB4', '\x5C', '\x8E', '\xEC', '\x2A', '\xC4', '\x45', '\xBA', '\x8A', '\x5F', '\xAB', '\xF8', '\x2D',
      '\xE8', '\x43', '\x8A', '\xA0', '\x6C', '\xB2', '\xF4', '\x2A', '\xD9', '\x55', '\x8A', '\xA7', '\x5C', '\xB0', '\xFF', '\x29',
      '\x99', '\x53', '\x82', '\xB3', '\x03'
    }))
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
  holdButton->SetFont(std::string(DecryptName<0xE52DF30DCBAF7125ULL>({'\x62', '\x04', '\xC3', '\xA2', '\x60', '\xF3'})));
  holdButton->SetFontHeight(12);
  holdButton->SetBold(0);
  holdButton->SetTextColors(0, 0, 0, 0);
  holdButton->SetShadow(0, 0);
  holdButton->SetOutline(0, 0);
  holdButton->SetImages(
    std::string(DecryptName<0xCFF3899D95AF355FULL>({
      '\x9F', '\x9A', '\x6F', '\x6F', '\x5D', '\x47', '\x36', '\x02', '\x99', '\xD1', '\x6F', '\x59', '\x20', '\x33', '\xAF', '\x9D',
      '\x3A', '\x5B', '\xCA', '\xE2', '\xC2', '\xD8', '\x86', '\xAA', '\x2C', '\x41', '\xFA', '\xDC', '\xC1', '\xFD', '\x92', '\xAD',
      '\x00', '\x5D', '\xC0', '\xF9', '\xF9', '\xD6', '\x9C', '\xA9', '\x39', '\x1B', '\xCD', '\xF8', '\xED', '\x89'
    })),
    std::string(DecryptName<0xFBD599E117C547C1ULL>({
      '\x01', '\xE8', '\x05', '\xED', '\x21', '\x57', '\x10', '\x36', '\x07', '\xA3', '\x05', '\xDB', '\x5C', '\x23', '\x89', '\xA9',
      '\xA4', '\x29', '\xA0', '\x60', '\xBE', '\xC8', '\xA0', '\x9E', '\xB2', '\x33', '\x90', '\x5E', '\xBD', '\xED', '\xB4', '\x99',
      '\x9E', '\x2F', '\xAA', '\x7B', '\x85', '\xC6', '\xBA', '\x95', '\xEF', '\x25', '\xA8', '\x67', '\xE1'
    })),
    std::string(DecryptName<0x176F138D5DD7F365ULL>({
      '\xA5', '\x5C', '\x17', '\xA7', '\x4D', '\xDD', '\xAA', '\xDA', '\xA3', '\x17', '\x17', '\x91', '\x30', '\xA9', '\x33', '\x45',
      '\x00', '\x9D', '\xB2', '\x2A', '\xD2', '\x42', '\x1A', '\x72', '\x16', '\x87', '\x82', '\x14', '\xD1', '\x67', '\x0E', '\x75',
      '\x3A', '\x9B', '\xB8', '\x31', '\xE9', '\x4C', '\x00', '\x71', '\x03', '\xDD', '\xB5', '\x30', '\xFD', '\x13'
    })),
    std::string(DecryptName<0x2D51E3B7236175EDULL>({
      '\x2D', '\xDA', '\xA1', '\xD9', '\x77', '\x2D', '\x94', '\xE0', '\x2B', '\x91', '\xA1', '\xEF', '\x0A', '\x59', '\x0D', '\x7F',
      '\x88', '\x1B', '\x04', '\x54', '\xE8', '\xB2', '\x24', '\x48', '\x9E', '\x01', '\x34', '\x6A', '\xEB', '\x97', '\x30', '\x4F',
      '\xB2', '\x1D', '\x0E', '\x4F', '\xD3', '\xBC', '\x3E', '\x4B', '\x8B', '\x5B', '\x03', '\x4E', '\xC7', '\xE3'
    }))
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
  waitButton->SetFont(std::string(DecryptName<0x0F41EBBDA32D1B17ULL>({'\x50', '\x6E', '\x41', '\xCA', '\xD0', '\xEB'})));
  waitButton->SetFontHeight(12);
  waitButton->SetBold(0);
  waitButton->SetTextColors(0, 0, 0, 0);
  waitButton->SetShadow(0, 0);
  waitButton->SetOutline(0, 0);
  waitButton->SetImages(
    std::string(DecryptName<0x839B27DF8F5967B3ULL>({
      '\x73', '\xC8', '\x99', '\x75', '\x1F', '\xE9', '\x5E', '\x4E', '\x75', '\x83', '\x99', '\x43', '\x62', '\x9D', '\xC7', '\xD1',
      '\xD6', '\x09', '\x3C', '\xF8', '\x80', '\x76', '\xEE', '\xE6', '\xC0', '\x13', '\x0C', '\xC6', '\x83', '\x53', '\xFA', '\xE1',
      '\xEC', '\x10', '\x38', '\xE6', '\xAB', '\x78', '\xF4', '\xE5', '\xD5', '\x49', '\x3B', '\xE2', '\xAF', '\x27'
    })),
    std::string(DecryptName<0xF3CB6F7711A71B6DULL>({
      '\xAD', '\xB4', '\x67', '\xEB', '\xB7', '\xA1', '\x0E', '\x3E', '\xAB', '\xFF', '\x67', '\xDD', '\xCA', '\xD5', '\x97', '\xA1',
      '\x08', '\x75', '\xC2', '\x66', '\x28', '\x3E', '\xBE', '\x96', '\x1E', '\x6F', '\xF2', '\x58', '\x2B', '\x1B', '\xAA', '\x91',
      '\x32', '\x6C', '\xC6', '\x78', '\x03', '\x30', '\xA4', '\x9D', '\x43', '\x79', '\xCA', '\x61', '\x77'
    })),
    std::string(DecryptName<0x452FEF0D7FC95565ULL>({
      '\xA5', '\xFA', '\x09', '\x85', '\xCD', '\x21', '\xEA', '\x88', '\xA3', '\xB1', '\x09', '\xB3', '\xB0', '\x55', '\x73', '\x17',
      '\x00', '\x3B', '\xAC', '\x08', '\x52', '\xBE', '\x5A', '\x20', '\x16', '\x21', '\x9C', '\x36', '\x51', '\x9B', '\x4E', '\x27',
      '\x3A', '\x22', '\xA8', '\x16', '\x79', '\xB0', '\x40', '\x23', '\x03', '\x7B', '\xAB', '\x12', '\x7D', '\xEF'
    })),
    std::string(DecryptName<0x9FED7DA189490D5DULL>({
      '\x9D', '\xA2', '\x89', '\x73', '\x61', '\xB3', '\x28', '\x52', '\x9B', '\xE9', '\x89', '\x45', '\x1C', '\xC7', '\xB1', '\xCD',
      '\x38', '\x63', '\x2C', '\xFE', '\xFE', '\x2C', '\x98', '\xFA', '\x2E', '\x79', '\x1C', '\xC0', '\xFD', '\x09', '\x8C', '\xFD',
      '\x02', '\x7A', '\x28', '\xE0', '\xD5', '\x22', '\x82', '\xF9', '\x3B', '\x23', '\x2B', '\xE4', '\xD1', '\x7D'
    }))
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
    std::string(DecryptName<0x7DE349D717D7F123ULL>({
      '\xE3', '\x5E', '\x17', '\xED', '\x17', '\x87', '\x26', '\xB0', '\xE5', '\x15', '\x17', '\xDB', '\x6A', '\xF3', '\xBF', '\x2F',
      '\x46', '\x9F', '\xB2', '\x60', '\x88', '\x18', '\x96', '\x18', '\x50', '\x85', '\x82', '\x5E', '\x8B', '\x2A', '\x8B', '\x18',
      '\x40', '\x9A', '\xB5', '\x78', '\xAF', '\x16', '\x8C', '\x1B', '\x45', '\xDF', '\xB5', '\x7A', '\xA7', '\x49'
    })),
    std::string(DecryptName<0x0F15B16B5BAFD57FULL>({
      '\xBF', '\x7A', '\x6F', '\xA1', '\xAB', '\x7F', '\xD0', '\xC2', '\xB9', '\x31', '\x6F', '\x97', '\xD6', '\x0B', '\x49', '\x5D',
      '\x1A', '\xBB', '\xCA', '\x2C', '\x34', '\xE0', '\x60', '\x6A', '\x0C', '\xA1', '\xFA', '\x12', '\x37', '\xD2', '\x7D', '\x6A',
      '\x1C', '\xBE', '\xCD', '\x34', '\x13', '\xEE', '\x7A', '\x61', '\x51', '\xB7', '\xC2', '\x2B', '\x6B'
    })),
    std::string(DecryptName<0x7941D76335CFA14FULL>({
      '\x8F', '\x0E', '\x0F', '\xCF', '\xA3', '\x19', '\x84', '\xB4', '\x89', '\x45', '\x0F', '\xF9', '\xDE', '\x6D', '\x1D', '\x2B',
      '\x2A', '\xCF', '\xAA', '\x42', '\x3C', '\x86', '\x34', '\x1C', '\x3C', '\xD5', '\x9A', '\x7C', '\x3F', '\xB4', '\x29', '\x1C',
      '\x2C', '\xCA', '\xAD', '\x5A', '\x1B', '\x88', '\x2E', '\x1F', '\x29', '\x8F', '\xAD', '\x58', '\x13', '\xD7'
    })),
    std::string(DecryptName<0x7321C3814F8799E1ULL>({
      '\x21', '\x36', '\x47', '\xB5', '\x41', '\x0D', '\xE4', '\xBE', '\x27', '\x7D', '\x47', '\x83', '\x3C', '\x79', '\x7D', '\x21',
      '\x84', '\xF7', '\xE2', '\x38', '\xDE', '\x92', '\x54', '\x16', '\x92', '\xED', '\xD2', '\x06', '\xDD', '\xA0', '\x49', '\x16',
      '\x82', '\xF2', '\xE5', '\x20', '\xF9', '\x9C', '\x4E', '\x15', '\x87', '\xB7', '\xE5', '\x22', '\xF1', '\xC3'
    }))
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
  this->filterLabel->SetFont(std::string(DecryptName<0x7115D9F973F913C9ULL>({'\x8E', '\x66', '\x95', '\x1A', '\x94', '\xD9'})));
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
    std::string(DecryptName<0xCB1D253B7FE1F563ULL>({
      '\xA3', '\x5A', '\x21', '\x85', '\xFB', '\xEB', '\xD8', '\x06', '\xA5', '\x11', '\x21', '\xB3', '\x86', '\x9F', '\x41', '\xA9',
      '\x02', '\x86', '\x88', '\x1C', '\x64', '\x4C', '\x73', '\xBF', '\x06', '\x87', '\x87', '\x1E', '\x58', '\x40', '\x41', '\xA9',
      '\x17', '\x9B', '\xBE', '\x1C', '\x57', '\x4A', '\x6E', '\xAE', '\x4D', '\x97', '\x8C', '\x0F', '\x3B'
    })),
    std::string(DecryptName<0x793F370985ED674FULL>({
      '\x8F', '\xC8', '\x2D', '\x7F', '\xC9', '\xF9', '\xFA', '\xB4', '\x89', '\x83', '\x2D', '\x49', '\xB4', '\x8D', '\x63', '\x1B',
      '\x2E', '\x14', '\x84', '\xE6', '\x56', '\x5E', '\x51', '\x0D', '\x2A', '\x15', '\x8B', '\xE4', '\x6A', '\x52', '\x63', '\x1B',
      '\x3B', '\x09', '\xB2', '\xE6', '\x65', '\x58', '\x4C', '\x1C', '\x10', '\x05', '\xC3', '\xE7', '\x64', '\x47', '\x3F'
    })),
    std::string(DecryptName<0xE55723712993051DULL>({
      '\xDD', '\xAA', '\x53', '\xD3', '\xB1', '\xED', '\x92', '\x28', '\xDB', '\xE1', '\x53', '\xE5', '\xCC', '\x99', '\x0B', '\x87',
      '\x7C', '\x76', '\xFA', '\x4A', '\x2E', '\x4A', '\x39', '\x91', '\x78', '\x77', '\xF5', '\x48', '\x12', '\x46', '\x0B', '\x87',
      '\x69', '\x6B', '\xCC', '\x4A', '\x1D', '\x4C', '\x24', '\x80', '\x42', '\x64', '\xBD', '\x4B', '\x1C', '\x53', '\x57'
    })),
    std::string(DecryptName<0x3FA3AFA7035DE3FFULL>({
      '\x3F', '\x4C', '\x9D', '\xF9', '\x67', '\x61', '\x66', '\xF2', '\x39', '\x07', '\x9D', '\xCF', '\x1A', '\x15', '\xFF', '\x5D',
      '\x9E', '\x90', '\x34', '\x60', '\xF8', '\xC6', '\xCD', '\x4B', '\x9A', '\x91', '\x3B', '\x62', '\xC4', '\xCA', '\xFF', '\x5D',
      '\x8B', '\x8D', '\x02', '\x60', '\xCB', '\xC0', '\xD0', '\x5A', '\xD1', '\x81', '\x30', '\x73', '\xA7'
    }))
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
  this->searchBackground->SetImage(std::string(DecryptName<0xDB27CB557D038759ULL>({
    '\x99', '\x28', '\xC3', '\x87', '\x95', '\x05', '\xE2', '\x16', '\x9F', '\x63', '\xC3', '\xB1', '\xE8', '\x71', '\x7B', '\x89',
    '\x3C', '\xE9', '\x66', '\x0A', '\x0A', '\x9A', '\x52', '\xBE', '\x2A', '\xF3', '\x56', '\x34', '\x09', '\xA9', '\x40', '\x84',
    '\x2A', '\xE2', '\x62', '\x0F', '\x36', '\xA3', '\x09', '\xB9', '\x34', '\xF7', '\x03'
  })));
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
  allButton->SetFont(std::string(DecryptName<0x51BB39B763BBBD8DULL>({'\xCA', '\xC8', '\xD7', '\x0A', '\xDA', '\x39'})));
  allButton->SetFontHeight(12);
  allButton->SetBold(0);
  allButton->SetTextColors(0, 0, 0, 0);
  allButton->SetShadow(0, 0);
  allButton->SetOutline(0, 0);
  allButton->SetImages(
    std::string(DecryptName<0x87E3D1C50D572DEFULL>({
      '\x2F', '\x82', '\x97', '\xF7', '\x05', '\x1F', '\x26', '\x4A', '\x29', '\xC9', '\x97', '\xC1', '\x78', '\x6B', '\xBF', '\xD5',
      '\x8A', '\x43', '\x32', '\x7A', '\x9A', '\x80', '\x96', '\xE2', '\x9C', '\x59', '\x02', '\x44', '\x99', '\xB3', '\x97', '\xE9',
      '\xB0', '\x4C', '\x3B', '\x61', '\x9A', '\xBE', '\x85', '\xE1', '\xC1', '\x4F', '\x3A', '\x7D', '\xC5'
    })),
    std::string(DecryptName<0xDF8117FF49CFF551ULL>({
      '\x91', '\x5A', '\x0F', '\xB3', '\x3F', '\xD9', '\x44', '\x12', '\x97', '\x11', '\x0F', '\x85', '\x42', '\xAD', '\xDD', '\x8D',
      '\x34', '\x9B', '\xAA', '\x3E', '\xA0', '\x46', '\xF4', '\xBA', '\x22', '\x81', '\x9A', '\x00', '\xA3', '\x75', '\xF5', '\xB1',
      '\x0E', '\x94', '\xA3', '\x25', '\xA0', '\x78', '\xEF', '\xF1', '\x33', '\x98', '\xBF', '\x49'
    })),
    std::string(DecryptName<0xADC51BF9C98B413DULL>({
      '\xFD', '\xEE', '\x4B', '\x33', '\x39', '\xD5', '\x00', '\x60', '\xFB', '\xA5', '\x4B', '\x05', '\x44', '\xA1', '\x99', '\xFF',
      '\x58', '\x2F', '\xEE', '\xBE', '\xA6', '\x4A', '\xB0', '\xC8', '\x4E', '\x35', '\xDE', '\x80', '\xA5', '\x79', '\xB1', '\xC3',
      '\x62', '\x20', '\xE7', '\xA5', '\xA6', '\x74', '\xA3', '\xCB', '\x13', '\x23', '\xE6', '\xB9', '\xF9'
    })),
    std::string(DecryptName<0x7D5D0F110B6B8371ULL>({
      '\xB1', '\x2C', '\xAB', '\xF1', '\xD1', '\xC1', '\x98', '\xB0', '\xB7', '\x67', '\xAB', '\xC7', '\xAC', '\xB5', '\x01', '\x2F',
      '\x14', '\xED', '\x0E', '\x7C', '\x4E', '\x5E', '\x28', '\x18', '\x02', '\xF7', '\x3E', '\x42', '\x4D', '\x6D', '\x29', '\x13',
      '\x2E', '\xE2', '\x07', '\x67', '\x4E', '\x60', '\x3B', '\x1B', '\x5F', '\xE1', '\x06', '\x7B', '\x11'
    }))
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
