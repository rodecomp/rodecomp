#include "Contents/Sound/SoundConfiguration.h"
#include "Client/Registry.h"
#include "Render/DeviceSelection.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Utility/Cipher/XorBytes.h"

#include <cstddef>
#include <cstdint>

template <std::size_t Length, std::uint64_t Key>
class EncryptedText {
public:
  explicit EncryptedText(const std::uint8_t (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedText() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptText(const std::uint8_t (&source)[Length]) {
  thread_local EncryptedText<Length, Key> text(source);
  return text.Text();
}

// INLINED
static const char* RenewSetupSubKey() {
  const std::uint8_t source[] = {0x44, 0x96, 0x29, 0xCD, 0x8C, 0xD4, 0x5D, 0xA0, 0x4B, 0xBE, 0x3D, 0xD8, 0x8D, 0xDC, 0x5B, 0xBC, 0x37,
                                 0xAA, 0x20, 0xDF, 0x8F, 0xE9, 0x7D, 0xA0, 0x79, 0x9C, 0x38, 0xEA, 0x9E, 0xC1, 0x5A, 0xB5, 0x17};
  return DecryptText<0xC52FB5FBB94FF917ULL>(source);
}

// GLOBAL: gl_25476
const char* g_renewSetupSubKey = RenewSetupSubKey();

// OFFSET: 0x00E59030
// FUNCTION: fn_24366
int sub_E59030() {
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

    const std::uint8_t showTipsSource[] = {0xF4, 0xFB, 0x4A, 0x9E, 0xF1, 0x2A, 0xBB, 0x58, 0xE6,
                                           0xE7, 0x56, 0x9D, 0xE4, 0x31, 0xBF, 0x5E, 0xF7, 0xB3};
    RegQueryValueExA(settingsKey, DecryptText<0x0BEB63A5C905B3A7ULL>(showTipsSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_window315Enabled),
                     &size);

    const std::uint8_t trilinearFilterSource[] = {0x97, 0x91, 0x9C, 0xC7, 0x12, 0xA5, 0x7A, 0xCA, 0x91, 0x85, 0x9C, 0xC7, 0x0F, 0xAE, 0x6D, 0x8B};
    if (RegQueryValueExA(settingsKey, DecryptText<0x8B3FEB5B8BD5C3C3ULL>(trilinearFilterSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_textureFiltering), &size) != ERROR_SUCCESS) {
      g_textureFiltering = 0;
    }

    if (g_window315Enabled != 1 && g_window315Enabled != 0) {
      g_window315Enabled = 1;
    }

    type = REG_SZ;
    size = sizeof(g_deviceName);
    const std::uint8_t deviceNameSource[] = {0xA1, 0xCA, 0x1B, 0x5C, 0x74, 0x44, 0x1D, 0xB6, 0xA8, 0xCA, 0x4D};
    RegQueryValueExA(settingsKey, DecryptText<0xF7530137154D8FE5ULL>(deviceNameSource), nullptr, &type, reinterpret_cast<BYTE*>(g_deviceName), &size);

    type = REG_BINARY;
    size = sizeof(g_guidDevice);
    const std::uint8_t guidDeviceSource[] = {0x76, 0x0C, 0x62, 0x45, 0x25, 0x88, 0x33, 0xC2, 0x72, 0x1C, 0x2B};
    RegQueryValueExA(settingsKey, DecryptText<0x8B65CD61012B5931ULL>(guidDeviceSource), nullptr, &type, g_guidDevice, &size);

    type = REG_BINARY;
    size = sizeof(g_guidDriver);
    const std::uint8_t guidDriverSource[] = {0x80, 0x98, 0xAC, 0x79, 0x5B, 0x2F, 0x2C, 0xA3, 0x82, 0x9F, 0xE5};
    RegQueryValueExA(settingsKey, DecryptText<0xF5657D1F3DE5CDC7ULL>(guidDriverSource), nullptr, &type, g_guidDriver, &size);

    RegCloseKey(settingsKey);
  }

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &settingsKey) == ERROR_SUCCESS) {
    type = REG_DWORD;
    size = sizeof(int);

    const std::uint8_t soundModeSource[] = {0x92, 0x80, 0x86, 0x7D, 0xD3, 0x50, 0xB2, 0x2D, 0x84, 0xCF};
    RegQueryValueExA(settingsKey, DecryptText<0x69FD1D9733D3CFC1ULL>(soundModeSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_soundConfiguration.mode),
                     &size);

    const std::uint8_t speakerTypeSource[] = {0x1E, 0x81, 0x8E, 0x86, 0xB2, 0x32, 0x9F, 0x2B, 0x14, 0x81, 0x8E, 0xC7};
    RegQueryValueExA(settingsKey, DecryptText<0x7FCD77F9C7CBD14DULL>(speakerTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.speakerType), &size);

    const std::uint8_t digitalRateTypeSource[] = {0x67, 0xCA, 0x9C, 0xCA, 0x15, 0xFC, 0x73, 0x99, 0x62, 0xD7, 0x9E, 0xD7, 0x18, 0xED, 0x7A, 0xCB};
    RegQueryValueExA(settingsKey, DecryptText<0xCB3FBD4183DB8323ULL>(digitalRateTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalRateType), &size);

    const std::uint8_t digitalBitsTypeSource[] = {0x5D, 0x24, 0x88, 0xB6, 0x09, 0x40, 0x8D, 0x3D, 0x50, 0x39, 0x9C, 0xAB, 0x04, 0x51, 0x84, 0x7F};
    RegQueryValueExA(settingsKey, DecryptText<0x7FC1015DFFCF6D19ULL>(digitalBitsTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalBitsType), &size);

    const std::uint8_t numSampleTypeSource[] = {0x77, 0xB4, 0xE4, 0xBC, 0xA0, 0xEA, 0x03, 0x4B, 0x7C, 0xB5, 0xF0, 0xBF, 0xA4, 0xA7};
    if (RegQueryValueExA(settingsKey, DecryptText<0x0753A7E1EFA9E139ULL>(numSampleTypeSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_soundConfiguration.numSampleType), &size) != ERROR_SUCCESS) {
      g_soundConfiguration.numSampleType = 2;
    }

    type = REG_SZ;
    size = sizeof(g_soundConfiguration.providerName);
    const std::uint8_t providerNameSource[] = {0x3D, 0x0D, 0xB6, 0x65, 0xAA, 0x3F, 0x10, 0x6D, 0x23, 0x1E, 0xB4, 0x76, 0xE3};
    RegQueryValueExA(settingsKey, DecryptText<0x3F557BE333F95F6DULL>(providerNameSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(g_soundConfiguration.providerName), &size);

    RegCloseKey(settingsKey);
  }

  return 1;
}
