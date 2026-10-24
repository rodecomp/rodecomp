#ifndef RAGNAROK_SOURCE_CONTENTS_EMBLEMDOWNLOAD_H
#define RAGNAROK_SOURCE_CONTENTS_EMBLEMDOWNLOAD_H

#include <cstdint>
#include <string>

class CConnection;
struct AsyncDiagnostic;

void CompleteEmblemDownload(
  AsyncDiagnostic diagnostic,
  int responseCode,
  std::string response,
  std::uint32_t guildId,
  std::uint32_t version,
  CConnection& connection
);

#endif
