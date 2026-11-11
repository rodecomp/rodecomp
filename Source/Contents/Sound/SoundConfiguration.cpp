#include "Contents/Sound/SoundConfiguration.h"
#include "Client/Application/Registry.h"
#include "Render/Device/DeviceSelection.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <cstddef>
#include <cstdint>

// INLINED
template <std::uint64_t Key, typename Byte, std::size_t Length>
const char* DecryptText(const Byte (&source)[Length]) {
  thread_local EncryptedName<Length, Key> text(source);
  return text.Text();
}

// INLINED
static const char* RenewSetupSubKey() {
  constexpr std::array<std::uint8_t, 33> sourceArray = CRYPT_NAME("Software\\Gravity Soft\\RenewSetup", 0xC52FB5FBB94FF917ULL);
  const std::uint8_t (&source)[33] = sourceArray._Elems;
  return DecryptText<0xC52FB5FBB94FF917ULL>(source);
}

// GLOBAL: gl_25476
const char* g_renewSetupSubKey = RenewSetupSubKey();

// OFFSET: 0x00E59030
// FUNCTION: fn_24366
int ReadSetup() {
  HKEY key;
  LSTATUS status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_renewSetupSubKey, 0, KEY_QUERY_VALUE, &key);
  RegCloseKey(key);
  if (status != ERROR_SUCCESS) {
    return 0;
  }

  status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &key);
  RegCloseKey(key);
  if (status != ERROR_SUCCESS) {
    return 0;
  }

  HKEY settingsKey;
  DWORD type;
  DWORD size;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &settingsKey) == ERROR_SUCCESS) {
    type = REG_DWORD;
    size = sizeof(int);

    constexpr auto showTipsSourceText = CRYPT_NAME("SHOWTIPSATSTARTUP", 0x0BEB63A5C905B3A7ULL);
    const auto& showTipsSource = showTipsSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x0BEB63A5C905B3A7ULL>(showTipsSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_window315Enabled),
                     &size);

    constexpr auto trilinearFilterSourceText = CRYPT_NAME("TRILINEARFILTER", 0x8B3FEB5B8BD5C3C3ULL);
    const auto& trilinearFilterSource = trilinearFilterSourceText.data;
    if (RegQueryValueExA(settingsKey, DecryptText<0x8B3FEB5B8BD5C3C3ULL>(trilinearFilterSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_textureFiltering), &size) != ERROR_SUCCESS) {
      g_textureFiltering = 0;
    }

    if (g_window315Enabled != 1 && g_window315Enabled != 0) {
      g_window315Enabled = 1;
    }

    type = REG_SZ;
    size = sizeof(g_deviceName);
    constexpr auto deviceNameSourceText = CRYPT_NAME("DEVICENAME", 0xF7530137154D8FE5ULL);
    const auto& deviceNameSource = deviceNameSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0xF7530137154D8FE5ULL>(deviceNameSource), nullptr, &type, reinterpret_cast<BYTE*>(g_deviceName), &size);

    type = REG_BINARY;
    size = sizeof(g_guidDevice);
    constexpr auto guidDeviceSourceText = CRYPT_NAME("GUIDDEVICE", 0x8B65CD61012B5931ULL);
    const auto& guidDeviceSource = guidDeviceSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x8B65CD61012B5931ULL>(guidDeviceSource), nullptr, &type, g_guidDevice, &size);

    type = REG_BINARY;
    size = sizeof(g_guidDriver);
    constexpr auto guidDriverSourceText = CRYPT_NAME("GUIDDRIVER", 0xF5657D1F3DE5CDC7ULL);
    const auto& guidDriverSource = guidDriverSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0xF5657D1F3DE5CDC7ULL>(guidDriverSource), nullptr, &type, g_guidDriver, &size);

    RegCloseKey(settingsKey);
  }

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &settingsKey) == ERROR_SUCCESS) {
    type = REG_DWORD;
    size = sizeof(int);

    constexpr auto soundModeSourceText = CRYPT_NAME("SOUNDMODE", 0x69FD1D9733D3CFC1ULL);
    const auto& soundModeSource = soundModeSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x69FD1D9733D3CFC1ULL>(soundModeSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_soundConfiguration.mode),
                     &size);

    constexpr auto speakerTypeSourceText = CRYPT_NAME("SPEAKERTYPE", 0x7FCD77F9C7CBD14DULL);
    const auto& speakerTypeSource = speakerTypeSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x7FCD77F9C7CBD14DULL>(speakerTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.speakerType), &size);

    constexpr auto digitalRateTypeSourceText = CRYPT_NAME("DIGITALRATETYPE", 0xCB3FBD4183DB8323ULL);
    const auto& digitalRateTypeSource = digitalRateTypeSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0xCB3FBD4183DB8323ULL>(digitalRateTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalRateType), &size);

    constexpr auto digitalBitsTypeSourceText = CRYPT_NAME("DIGITALBITSTYPE", 0x7FC1015DFFCF6D19ULL);
    const auto& digitalBitsTypeSource = digitalBitsTypeSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x7FC1015DFFCF6D19ULL>(digitalBitsTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalBitsType), &size);

    constexpr auto numSampleTypeSourceText = CRYPT_NAME("NUMSAMPLETYPE", 0x0753A7E1EFA9E139ULL);
    const auto& numSampleTypeSource = numSampleTypeSourceText.data;
    if (RegQueryValueExA(settingsKey, DecryptText<0x0753A7E1EFA9E139ULL>(numSampleTypeSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_soundConfiguration.numSampleType), &size) != ERROR_SUCCESS) {
      g_soundConfiguration.numSampleType = 2;
    }

    type = REG_SZ;
    size = sizeof(g_soundConfiguration.providerName);
    constexpr auto providerNameSourceText = CRYPT_NAME("PROVIDERNAME", 0x3F557BE333F95F6DULL);
    const auto& providerNameSource = providerNameSourceText.data;
    RegQueryValueExA(settingsKey, DecryptText<0x3F557BE333F95F6DULL>(providerNameSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(g_soundConfiguration.providerName), &size);

    RegCloseKey(settingsKey);
  }

  return 1;
}
