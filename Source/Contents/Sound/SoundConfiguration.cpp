#include "Contents/Sound/SoundConfiguration.h"
#include "Client/Application/Registry.h"
#include "Render/Device/DeviceSelection.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"

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
  const std::uint8_t source[] = {0x44, 0x96, 0x29, 0xCD, 0x8C, 0xD4, 0x5D, 0xA0, 0x4B, 0xBE, 0x3D, 0xD8, 0x8D, 0xDC, 0x5B, 0xBC, 0x37,
                                 0xAA, 0x20, 0xDF, 0x8F, 0xE9, 0x7D, 0xA0, 0x79, 0x9C, 0x38, 0xEA, 0x9E, 0xC1, 0x5A, 0xB5, 0x17};
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

    const char showTipsSource[] = {'\xF4', '\xFB', '\x4A', '\x9E', '\xF1', '\x2A', '\xBB', '\x58', '\xE6', '\xE7', '\x56', '\x9D', '\xE4', '\x31', '\xBF', '\x5E', '\xF7', '\xB3'};
    RegQueryValueExA(settingsKey, DecryptText<0x0BEB63A5C905B3A7ULL>(showTipsSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_window315Enabled),
                     &size);

    const char trilinearFilterSource[] = {'\x97', '\x91', '\x9C', '\xC7', '\x12', '\xA5', '\x7A', '\xCA', '\x91', '\x85', '\x9C', '\xC7', '\x0F', '\xAE', '\x6D', '\x8B'};
    if (RegQueryValueExA(settingsKey, DecryptText<0x8B3FEB5B8BD5C3C3ULL>(trilinearFilterSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_textureFiltering), &size) != ERROR_SUCCESS) {
      g_textureFiltering = 0;
    }

    if (g_window315Enabled != 1 && g_window315Enabled != 0) {
      g_window315Enabled = 1;
    }

    type = REG_SZ;
    size = sizeof(g_deviceName);
    const char deviceNameSource[] = {'\xA1', '\xCA', '\x1B', '\x5C', '\x74', '\x44', '\x1D', '\xB6', '\xA8', '\xCA', '\x4D'};
    RegQueryValueExA(settingsKey, DecryptText<0xF7530137154D8FE5ULL>(deviceNameSource), nullptr, &type, reinterpret_cast<BYTE*>(g_deviceName), &size);

    type = REG_BINARY;
    size = sizeof(g_guidDevice);
    const char guidDeviceSource[] = {'\x76', '\x0C', '\x62', '\x45', '\x25', '\x88', '\x33', '\xC2', '\x72', '\x1C', '\x2B'};
    RegQueryValueExA(settingsKey, DecryptText<0x8B65CD61012B5931ULL>(guidDeviceSource), nullptr, &type, g_guidDevice, &size);

    type = REG_BINARY;
    size = sizeof(g_guidDriver);
    const char guidDriverSource[] = {'\x80', '\x98', '\xAC', '\x79', '\x5B', '\x2F', '\x2C', '\xA3', '\x82', '\x9F', '\xE5'};
    RegQueryValueExA(settingsKey, DecryptText<0xF5657D1F3DE5CDC7ULL>(guidDriverSource), nullptr, &type, g_guidDriver, &size);

    RegCloseKey(settingsKey);
  }

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &settingsKey) == ERROR_SUCCESS) {
    type = REG_DWORD;
    size = sizeof(int);

    const char soundModeSource[] = {'\x92', '\x80', '\x86', '\x7D', '\xD3', '\x50', '\xB2', '\x2D', '\x84', '\xCF'};
    RegQueryValueExA(settingsKey, DecryptText<0x69FD1D9733D3CFC1ULL>(soundModeSource), nullptr, &type, reinterpret_cast<BYTE*>(&g_soundConfiguration.mode),
                     &size);

    const char speakerTypeSource[] = {'\x1E', '\x81', '\x8E', '\x86', '\xB2', '\x32', '\x9F', '\x2B', '\x14', '\x81', '\x8E', '\xC7'};
    RegQueryValueExA(settingsKey, DecryptText<0x7FCD77F9C7CBD14DULL>(speakerTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.speakerType), &size);

    const char digitalRateTypeSource[] = {'\x67', '\xCA', '\x9C', '\xCA', '\x15', '\xFC', '\x73', '\x99', '\x62', '\xD7', '\x9E', '\xD7', '\x18', '\xED', '\x7A', '\xCB'};
    RegQueryValueExA(settingsKey, DecryptText<0xCB3FBD4183DB8323ULL>(digitalRateTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalRateType), &size);

    const char digitalBitsTypeSource[] = {'\x5D', '\x24', '\x88', '\xB6', '\x09', '\x40', '\x8D', '\x3D', '\x50', '\x39', '\x9C', '\xAB', '\x04', '\x51', '\x84', '\x7F'};
    RegQueryValueExA(settingsKey, DecryptText<0x7FC1015DFFCF6D19ULL>(digitalBitsTypeSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalBitsType), &size);

    const char numSampleTypeSource[] = {'\x77', '\xB4', '\xE4', '\xBC', '\xA0', '\xEA', '\x03', '\x4B', '\x7C', '\xB5', '\xF0', '\xBF', '\xA4', '\xA7'};
    if (RegQueryValueExA(settingsKey, DecryptText<0x0753A7E1EFA9E139ULL>(numSampleTypeSource), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_soundConfiguration.numSampleType), &size) != ERROR_SUCCESS) {
      g_soundConfiguration.numSampleType = 2;
    }

    type = REG_SZ;
    size = sizeof(g_soundConfiguration.providerName);
    const char providerNameSource[] = {'\x3D', '\x0D', '\xB6', '\x65', '\xAA', '\x3F', '\x10', '\x6D', '\x23', '\x1E', '\xB4', '\x76', '\xE3'};
    RegQueryValueExA(settingsKey, DecryptText<0x3F557BE333F95F6DULL>(providerNameSource), nullptr, &type,
                     reinterpret_cast<BYTE*>(g_soundConfiguration.providerName), &size);

    RegCloseKey(settingsKey);
  }

  return 1;
}
