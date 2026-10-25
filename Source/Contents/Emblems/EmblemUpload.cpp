#include "Contents/Emblems/EmblemDataMgr.h"
#include "Contents/Emblems/EmblemUpload.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Async/AsyncDiagnostic.h"

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
}

// OFFSET: 0x005EBAD0
// FUNCTION: fn_03976
void CEmblemDataMgr::CompleteUpload(AsyncDiagnostic diagnostic, std::string response, std::uint32_t guildId, EmblemUploadContext& context) {
  if (!diagnostic.success) {
    return;
  }

  Json::Value root;
  Json::Reader reader;
  const std::string document(response.c_str());

  if (!reader.parse(document, root, true)) {
    return;
  }

  switch (root["Type"].asInt()) {
  case 1: {
    EmblemVersionPacket packet{0x0B46, guildId, root["version"].asInt()};
    context.connection.Send(context.lengths.FixedLength(packet.id), &packet);
    break;
  }
  case 2:
  case 3:
  case 4: {
    const char* message = LookupMessage(3586, context.languagePrefix, context.serviceType, context.language);
    context.dialogs.Confirm(message, 0, true, false, 0, "Message", 280, 120, 0);
    break;
  }
  default:
    break;
  }

  this->m_uploading = false;
}
