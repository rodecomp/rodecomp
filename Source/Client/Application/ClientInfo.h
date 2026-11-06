#ifndef RAGNAROK_SOURCE_CLIENT_APPLICATION_CLIENTINFO_H
#define RAGNAROK_SOURCE_CLIENT_APPLICATION_CLIENTINFO_H

struct XmlNode;

extern XmlNode* g_clientInfoRoot;
extern const char* g_clientInfoAddress;
extern const char* g_clientInfoPort;
extern const char* g_clientInfoDomain;
extern const char* g_clientInfoRegistrationWeb;
extern const char* g_clientInfoDisplay;

void LoadClientInfo(char* protection);

#endif
