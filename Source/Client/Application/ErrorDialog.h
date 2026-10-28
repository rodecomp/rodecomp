#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_ERRORDIALOG_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_ERRORDIALOG_H

#include <string>
#include <windows.h>

class CRenderSystemBase;

int ShowErrorDialog(const std::string& text, const UINT& type);
int ShowErrorDialog(CRenderSystemBase* renderSystem, const char* text, UINT type);
int ShowErrorDialog(int code);

#endif
