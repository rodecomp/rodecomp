#ifndef RAGNAROK_SOURCE_NETWORK_SHORTCUTPACKET_H
#define RAGNAROK_SOURCE_NETWORK_SHORTCUTPACKET_H

struct NormalItemSlot;
class CConnection;
struct PacketLengths;

// Exact event313 operation; caller supplies actual bank and special-slot owners.
void SendShortcutUpdate(
  CConnection& connection,
  const PacketLengths& lengths,
  int index,
  int bank,
  const NormalItemSlot* bank0,
  const NormalItemSlot* bank1,
  const NormalItemSlot* special
);

#endif
