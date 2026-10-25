#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYJOINRESULT_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYJOINRESULT_H

struct AgencyRequestContext;
struct StopRecruitingContext;
class WindowNotifications;

struct PartyJoinResultContext {
  WindowNotifications& notifications;
  AgencyRequestContext& requests;
  StopRecruitingContext& stop;
  const int& partyMember;
};

void ReceivePartyJoinResult(const void* packet, PartyJoinResultContext& context);

#endif
