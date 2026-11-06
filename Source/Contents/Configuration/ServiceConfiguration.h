#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICECONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICECONFIGURATION_H

#include <string>
#include <vector>

extern signed char g_language;
extern bool g_serviceFlag05;
extern bool g_serviceFlag06;
extern int g_serviceType;
extern char g_homepageUrl[256];

int SetServiceFlags(int first, int second);
extern int g_serviceMode;
extern unsigned char g_startupSecondPhase;
extern const char* g_licenceFileName;
extern const char* g_clientExeName;
extern const char* g_patcherExeName;
extern const char* g_runPatcherMessage;
extern const char* g_windowName;
extern const char* g_serviceHost;
extern const char* g_servicePort;
extern const char* g_serviceOverrideKey;
extern char g_launchServerName[24];
extern int g_serviceFlag07;
extern char g_serviceAddress[129];
extern const int s_adminIds[45];
extern const int s_monsterJobIds[48];
extern std::vector<std::string> g_loadingScreens;

const char* GetLicenceText();
struct XmlNode;

void InitializeServiceStrings();
void ReadClientInfo(XmlNode* clientInfo);

#endif
