#ifndef RAGNAROK_SOURCE_CLIENT_ERRORDIALOG_H
#define RAGNAROK_SOURCE_CLIENT_ERRORDIALOG_H

#include <windows.h>

class CRenderSystemBase;

// Obtained through the render-system owner.
// Keep the real owner dependency explicit until its lifecycle is reconstructed.
int ShowErrorDialog(CRenderSystemBase* renderSystem, const char* text, UINT type);

#endif
