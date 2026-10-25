#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Network/Packets/ShortcutPacket.h"

#include <cstdint>
#include <cstring>

// ORIGINAL_SITE: 0x00CFB806 in fn_21426 (fragment)
void SendShortcutUpdate(
  CConnection& connection, const PacketLengths& lengths, int index, int bank, const NormalItemSlot* bank0, const NormalItemSlot* bank1,
  const NormalItemSlot* special
) {
  unsigned char packet[13];
  const std::uint32_t header = 2849;
  std::memcpy(packet, &header, sizeof(header));
  const NormalItemSlot* slot = nullptr;

  if (index >= 36) {
    if (static_cast<unsigned>(index) - 36u <= 1) {
      slot = &special[index - 36];
    }
  } else if (bank == 0) {
    slot = &bank0[index];
  } else if (bank == 1) {
    slot = &bank1[index];
    const std::uint16_t flag = 1;
    std::memcpy(packet + 2, &flag, sizeof(flag));
  }

  if (slot) {
    std::memcpy(packet + 6, slot, sizeof(*slot));
  }

  const auto position = static_cast<std::uint16_t>(index);
  std::memcpy(packet + 4, &position, sizeof(position));
  connection.Send(lengths.FixedLength(2849), packet);
}
