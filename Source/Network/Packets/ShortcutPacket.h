#ifndef RAGNAROK_SOURCE_NETWORK_PACKETS_SHORTCUTPACKET_H
#define RAGNAROK_SOURCE_NETWORK_PACKETS_SHORTCUTPACKET_H

struct NormalItemSlot;
class CConnection;
struct PacketLengths;

void SendShortcutUpdate(
  CConnection& connection, const PacketLengths& lengths, int index, int bank, const NormalItemSlot* bank0, const NormalItemSlot* bank1,
  const NormalItemSlot* special
);

#endif
