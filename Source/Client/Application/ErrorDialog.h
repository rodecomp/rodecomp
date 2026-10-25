#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_ERRORDIALOG_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_ERRORDIALOG_H

#include <windows.h>

class CRenderSystemBase;

int ShowErrorDialog(CRenderSystemBase* renderSystem, const char* text, UINT type);

#endif
