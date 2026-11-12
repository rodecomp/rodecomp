#include "Contents/Sound/SoundConfiguration.h"
#include "Client/Application/Registry.h"
#include "Render/Device/DeviceSelection.h"
#include "UI/Dialogs/UITipOfTheDayWnd.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <cstddef>
#include <cstdint>

// INLINED
static const char* RenewSetupSubKey() {
  return OBFUSCATE_BYTES_TEXT("Software\\Gravity Soft\\RenewSetup", 0xC52FB5FBB94FF917ULL);
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

    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("SHOWTIPSATSTARTUP", 0x0BEB63A5C905B3A7ULL), nullptr, &type, reinterpret_cast<BYTE*>(&g_window315Enabled),
                     &size);

    if (RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("TRILINEARFILTER", 0x8B3FEB5B8BD5C3C3ULL), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_textureFiltering), &size) != ERROR_SUCCESS) {
      g_textureFiltering = 0;
    }

    if (g_window315Enabled != 1 && g_window315Enabled != 0) {
      g_window315Enabled = 1;
    }

    type = REG_SZ;
    size = sizeof(g_deviceName);
    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("DEVICENAME", 0xF7530137154D8FE5ULL), nullptr, &type, reinterpret_cast<BYTE*>(g_deviceName), &size);

    type = REG_BINARY;
    size = sizeof(g_guidDevice);
    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("GUIDDEVICE", 0x8B65CD61012B5931ULL), nullptr, &type, g_guidDevice, &size);

    type = REG_BINARY;
    size = sizeof(g_guidDriver);
    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("GUIDDRIVER", 0xF5657D1F3DE5CDC7ULL), nullptr, &type, g_guidDriver, &size);

    RegCloseKey(settingsKey);
  }

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, g_subKey, 0, KEY_QUERY_VALUE, &settingsKey) == ERROR_SUCCESS) {
    type = REG_DWORD;
    size = sizeof(int);

    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("SOUNDMODE", 0x69FD1D9733D3CFC1ULL), nullptr, &type, reinterpret_cast<BYTE*>(&g_soundConfiguration.mode),
                     &size);

    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("SPEAKERTYPE", 0x7FCD77F9C7CBD14DULL), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.speakerType), &size);

    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("DIGITALRATETYPE", 0xCB3FBD4183DB8323ULL), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalRateType), &size);

    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("DIGITALBITSTYPE", 0x7FC1015DFFCF6D19ULL), nullptr, &type,
                     reinterpret_cast<BYTE*>(&g_soundConfiguration.digitalBitsType), &size);

    if (RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("NUMSAMPLETYPE", 0x0753A7E1EFA9E139ULL), nullptr, &type,
                         reinterpret_cast<BYTE*>(&g_soundConfiguration.numSampleType), &size) != ERROR_SUCCESS) {
      g_soundConfiguration.numSampleType = 2;
    }

    type = REG_SZ;
    size = sizeof(g_soundConfiguration.providerName);
    RegQueryValueExA(settingsKey, OBFUSCATE_TEXT("PROVIDERNAME", 0x3F557BE333F95F6DULL), nullptr, &type,
                     reinterpret_cast<BYTE*>(g_soundConfiguration.providerName), &size);

    RegCloseKey(settingsKey);
  }

  return 1;
}
