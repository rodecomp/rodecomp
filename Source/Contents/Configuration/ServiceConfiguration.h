#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICECONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICECONFIGURATION_H

#include <string>
#include <vector>

extern signed char g_language;
extern bool g_serviceFlag05;
extern bool g_serviceFlag06;
extern int g_serviceType;

int SetServiceFlags(int first, int second);
extern int g_serviceMode;
extern unsigned char g_startupSecondPhase;
extern const char* g_licenceFileName;
extern const char* g_clientExeName;
extern const char* g_patcherExeName;
extern const char* g_runPatcherMessage;
extern const char* g_windowName;
extern char g_launchServerName[24];
extern std::vector<std::string> g_loadingScreens;

const char* GetLicenceText();
void InitializeServiceStrings();

#endif
