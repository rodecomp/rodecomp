#ifndef RAGNAROK_SOURCE_NETWORK_CLIENTOPTIONPACKET_H
#define RAGNAROK_SOURCE_NETWORK_CLIENTOPTIONPACKET_H

class CConnection;
class PacketLengths;

void SendClientOption(unsigned char value, CConnection& connection, const PacketLengths& lengths);

#endif
