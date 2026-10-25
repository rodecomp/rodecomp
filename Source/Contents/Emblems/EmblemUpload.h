#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMUPLOAD_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMS_EMBLEMUPLOAD_H

#include <cstdint>

class CConnection;
struct PacketLengths;
struct WindowDialogs;

struct EmblemUploadContext {
  CConnection& connection;
  const PacketLengths& lengths;
  WindowDialogs& dialogs;
  const bool& languagePrefix;
  const int& serviceType;
  const std::uint16_t& language;
};

#endif
