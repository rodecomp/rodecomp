#ifndef RAGNAROK_SOURCE_CONTENTS_PARTYINVITATION_H
#define RAGNAROK_SOURCE_CONTENTS_PARTYINVITATION_H

#include <string>

struct ForbiddenWords;
class WindowManagerWindows;

struct PartyInvitationContext {
  ForbiddenWords& words;
  WindowManagerWindows& windows;
  std::string& pendingName;
};

void ReceivePartyInvitation(const void* packet, PartyInvitationContext& context);

#endif
