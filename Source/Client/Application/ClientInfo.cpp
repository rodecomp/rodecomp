#include "Client/Application/ClientInfo.h"

#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "File/File.h"
#include "Resource/XmlNode.h"

#include <cstring>
#include <string>
#include <vector>

// GLOBAL: gl_25522
XmlNode* g_clientInfoRoot;

// GLOBAL: gl_25530
XmlNode* g_clientInfoConnection;

// GLOBAL: gl_25529
char g_clientInfoAddressText[0x100];

// GLOBAL: gl_25531
char g_clientInfoPortText[0x100];

// GLOBAL: gl_25532
char g_clientInfoDomainText[0x100];

// GLOBAL: gl_25533
char g_clientInfoRegistrationWebText[0x100];

// GLOBAL: gl_25534
char g_clientInfoDisplayText[0x100];

// GLOBAL: gl_25539
int g_clientInfoAdminIds[0x40];

// GLOBAL: gl_25540
int g_clientInfoYellowIds[0x40];

// GLOBAL: gl_25537
XmlDocument g_clientInfoDocument;

// GLOBAL: gl_25524
const char* g_clientInfoAddress;

// GLOBAL: gl_25525
const char* g_clientInfoPort;

// GLOBAL: gl_25526
const char* g_clientInfoDomain;

// GLOBAL: gl_25535
const char* g_clientInfoRegistrationWeb;

// GLOBAL: gl_25536
const char* g_clientInfoDisplay;

// GLOBAL: gl_25538
int g_savedServiceType;

// OFFSET: 0x03DE8000
// FUNCTION: fn_45389
__declspec(noinline) static bool SetClientInfoRoot(XmlNode* root) {
  g_clientInfoRoot = root;
  return true;
}

// OFFSET: 0x03DE8300
// FUNCTION: fn_45398
__declspec(noinline) static const char* NodeText(XmlNode* node) {
  if (node->child != nullptr) {
    return nullptr;
  }
  return node->Text().c_str();
}

// OFFSET: 0x03DE829B
// FUNCTION: fn_45395
__declspec(noinline) static const char* CopyNodeText(const char* text, char* output) {
  std::size_t remaining = 0xFF;
  char* cursor = output;
  while (true) {
    const char c = *text++;
    *cursor++ = c;
    if (c == '\0') {
      return output;
    }
    if (--remaining == 0) {
      cursor[-1] = '\0';
      return output;
    }
  }
}

// OFFSET: 0x03DE82BA
// FUNCTION: fn_45396
__declspec(noinline) static int ParseNodeInt(const char* text) {
  while (*text == ' ') {
    ++text;
  }
  bool negative = false;
  if (*text == '-') {
    negative = true;
    ++text;
  } else if (*text == '+') {
    ++text;
  }
  int value = 0;
  while (static_cast<unsigned int>(static_cast<unsigned char>(*text) - '0') <= 9) {
    value = value * 10 + (*text - '0');
    ++text;
  }
  return negative ? -value : value;
}

// OFFSET: 0x03DE82F5
// FUNCTION: fn_45397
__declspec(noinline) static void AssignIds(std::vector<int>& output, const int* first, const int* last) {
  output.assign(first, last);
}

// INLINED
static void ReadIdList(XmlNode* list, int (&ids)[0x40], std::vector<int>& output) {
  int count = 0;
  for (XmlNode* aid = list->FindChild("aid"); aid != nullptr; aid = aid->FindSibling("aid")) {
    if (const char* text = NodeText(aid)) {
      ids[count++] = ParseNodeInt(text);
    }
  }
  AssignIds(output, ids, ids + count);
}

// INLINED
static const char* ReadConnectionText(XmlNode* connection, const char* name, char* buffer) {
  XmlNode* node = connection->FindChild(name);
  if (node == nullptr) {
    return nullptr;
  }
  const char* text = NodeText(node);
  if (text == nullptr) {
    return nullptr;
  }
  return CopyNodeText(text, buffer);
}

// OFFSET: 0x03DE831E
// FUNCTION: fn_45399
__declspec(noinline) static void ApplyClientInfoRoot(XmlNode* root) {
  ReadClientInfo(root);
  g_savedServiceType = g_serviceType;
  g_serviceType = g_serviceFlag07;
  if (g_monsterJobs.empty()) {
    AssignIds(g_monsterJobs, g_adminAccountIds.data(), g_adminAccountIds.data() + g_adminAccountIds.size());
  }
}

// OFFSET: 0x03DE8027
// FUNCTION: fn_45390
__declspec(noinline) static void ReadConnectionInfo(int connectionIndex) {
  if (g_clientInfoRoot == nullptr) {
    return;
  }
  XmlNode* connection = g_clientInfoRoot->FindChild("connection");
  if (connection == nullptr) {
    return;
  }
  for (; connectionIndex > 0; --connectionIndex) {
    connection = connection->FindSibling("connection");
    if (connection == nullptr) {
      return;
    }
  }
  g_clientInfoConnection = connection;

  if (const char* text = ReadConnectionText(connection, "address", g_clientInfoAddressText)) {
    g_clientInfoAddress = text;
  }
  if (const char* text = ReadConnectionText(connection, "port", g_clientInfoPortText)) {
    g_clientInfoPort = text;
  }
  if (const char* text = ReadConnectionText(connection, "domain", g_clientInfoDomainText)) {
    g_clientInfoDomain = text;
  }
  if (const char* text = ReadConnectionText(connection, "registrationweb", g_clientInfoRegistrationWebText)) {
    g_clientInfoRegistrationWeb = text;
  }
  if (const char* text = ReadConnectionText(connection, "display", g_clientInfoDisplayText)) {
    g_clientInfoDisplay = text;
  }
  if (XmlNode* node = connection->FindChild("langtype")) {
    if (const char* text = NodeText(node)) {
      g_serviceFlag07 = ParseNodeInt(text);
    }
  }
  if (XmlNode* admin = g_clientInfoConnection->FindChild("admin")) {
    ReadIdList(admin, g_clientInfoAdminIds, g_adminAccountIds);
  }
  if (XmlNode* yellow = g_clientInfoConnection->FindChild("yellow")) {
    ReadIdList(yellow, g_clientInfoYellowIds, g_monsterJobs);
  }

  ApplyClientInfoRoot(g_clientInfoRoot);
}

// OFFSET: 0x03DE8E00
// FUNCTION: fn_45401
__declspec(noinline) static void ReadLoadingScreens() {
  int added = 0;
  XmlNode* connection = g_clientInfoConnection;
  if (connection == nullptr && g_clientInfoRoot != nullptr) {
    connection = g_clientInfoRoot->FindChild("connection");
  }
  if (connection != nullptr) {
    if (XmlNode* loading = connection->FindChild("loading")) {
      for (XmlNode* image = loading->FindChild("image"); image != nullptr; image = image->FindSibling("image")) {
        const std::string& name = image->Text();
        if (name.empty()) {
          break;
        }
        g_loadingScreens.push_back(std::string("유저인터페이스\\") + name);
        ++added;
      }
    }
  }
  if (added == 0) {
    g_loadingScreens.push_back(std::string("유저인터페이스\\loading00.jpg"));
  }
}

// OFFSET: 0x03DE8B20
// FUNCTION: fn_45400
__declspec(noinline) static void ApplyClientInfo(int connectionIndex) {
  ReadConnectionInfo(connectionIndex);
  ReadLoadingScreens();
  g_clientInfoConnection = nullptr;
}

// OFFSET: 0x03DE96A0
// FUNCTION: fn_45409
void LoadClientInfo(char* protection) {
  CFile file;
  if (file.Open("clientinfo.xml", 0, true, false)) {
    const char* data = static_cast<const char*>(file.GetData());
    g_clientInfoDocument.Parse(data, data + file.GetSize());
    file.Close();
    if (XmlNode* root = g_clientInfoDocument.FindChild("clientinfo")) {
      SetClientInfoRoot(root);
      ApplyClientInfo(0);
    }
  }
  InitializeServiceStrings();
  (void)protection;
}
