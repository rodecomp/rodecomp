#ifndef RAGNAROK_SOURCE_ARCHIVE_PAKAUXILIARY_H
#define RAGNAROK_SOURCE_ARCHIVE_PAKAUXILIARY_H

#include <cstdint>
#include <span>

std::uint64_t ReadAuxiliaryIdentifier(std::span<const std::uint8_t> data);

#endif
