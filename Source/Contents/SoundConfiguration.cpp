#include "Contents/SoundConfiguration.h"

// OFFSET: 0x00E59030
// FUNCTION: fn_24366
// Sound portion of the setup registry loader.
void SoundConfiguration::LoadRegistry(const char* baseKey) {
  HKEY key;

  if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, baseKey, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS) {
    return;
  }

  // The original reuses type/size outputs across all five DWORD queries;
  // failures are ignored except NUMSAMPLETYPE's explicit fallback to 2.
  DWORD type = REG_DWORD;
  DWORD size = sizeof(int);
  RegQueryValueExA(key, "SOUNDMODE", nullptr, &type, reinterpret_cast<BYTE*>(&this->mode), &size);
  RegQueryValueExA(key, "SPEAKERTYPE", nullptr, &type, reinterpret_cast<BYTE*>(&this->speakerType), &size);
  RegQueryValueExA(key, "DIGITALRATETYPE", nullptr, &type, reinterpret_cast<BYTE*>(&this->digitalRateType), &size);
  RegQueryValueExA(key, "DIGITALBITSTYPE", nullptr, &type, reinterpret_cast<BYTE*>(&this->digitalBitsType), &size);

  if (
    RegQueryValueExA(key, "NUMSAMPLETYPE", nullptr, &type, reinterpret_cast<BYTE*>(&this->numSampleType), &size) !=
    ERROR_SUCCESS
  ) {
    this->numSampleType = 2;
  }

  type = REG_SZ;
  size = sizeof(this->providerName);
  RegQueryValueExA(key, "PROVIDERNAME", nullptr, &type, reinterpret_cast<BYTE*>(this->providerName), &size);
  RegCloseKey(key);
}
