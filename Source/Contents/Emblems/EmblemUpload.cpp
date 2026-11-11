#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Async/AsyncDiagnostic.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"

#include "Resource/MSGSTRINGID.h"
#include <json/reader.h>
#include <json/value.h>

namespace {
#pragma pack(push, 1)
  struct EmblemVersionPacket {
    std::uint16_t id;
    std::uint32_t guildId;
    std::int32_t version;
  };
#pragma pack(pop)
  static_assert(sizeof(EmblemVersionPacket) == 10);

  // INLINED
  template <std::uint64_t Key, std::size_t Length>
  const char* DecryptName(const std::uint8_t (&source)[Length]) {
    thread_local EncryptedName<Length, Key> name(source);
    return name.Text();
  }
}

// OFFSET: 0x005EBAD0
// FUNCTION: fn_03976
void CEmblemDataMgr::CompleteUpload(AsyncDiagnostic diagnostic, std::string response, std::uint32_t guildId) {
  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(response.c_str(), root, true)) {
    return;
  }

  switch (root[DecryptName<0xC52FB5FBB94FF917ULL>({0x43, 0x80, 0x3F, 0xDC, 0xFB})].asInt()) {
  case 1: {
    EmblemVersionPacket packet{0x0B46, guildId, root[DecryptName<0x812BE72BCD5707B7ULL>({0xC1, 0x62, 0x25, 0xBE, 0x42, 0x88, 0x45, 0x81})].asInt()};
    GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.id), &packet);
    break;
  }
  case 2:
  case 3:
  case 4: {
    const char* message = LookupMessage(MSI_RODEX_BUTTON_DELETE_ALL);
    g_windowMgr.Confirm(message, 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }
  default:
    break;
  }

  this->m_uploading = false;
}
