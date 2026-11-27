#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_IMEHELPER_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_IMEHELPER_H

enum SgpResultCode : int {
  SGP_CODE_ALREADY_RUNNING = static_cast<int>(0x8000E001),
  SGP_CODE_DETECTED_MACRO = static_cast<int>(0x8000E003),
  SGP_CODE_DETECTED_SPEEDHACK = static_cast<int>(0x8000E004),
  SGP_CODE_DETECTED_APIHOOK = static_cast<int>(0x8000E005),
  SGP_CODE_DETECTED_MSGHOOK = static_cast<int>(0x8000E006),
};

void NotifyImeHelper();
void HandleImeError(const int* error);

#endif
