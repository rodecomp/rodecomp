#ifndef RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYINVITATION_H
#define RAGNAROK_SOURCE_CONTENTS_PARTY_PARTYINVITATION_H

#include "Contents/ForbiddenWords.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <cstring>
#include <string>

class ForbiddenWords;
class WindowManagerWindows;

struct PartyInvitationContext {
  ForbiddenWords& words;
  WindowManagerWindows& windows;
  std::string& pendingName;
};

void ReceivePartyInvitation(const void* packet, PartyInvitationContext& context);

// INLINED
inline void ReceivePartyInvitation(const void* packet, PartyInvitationContext& context) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  char name[25];
  std::strncpy(name, reinterpret_cast<const char*>(bytes + 6), 24);
  name[24] = '\0';

  if (g_forbiddenWords.Contains(name)) {
    std::string unused;
    g_forbiddenWords.Mask(name, &unused);
  }

  context.pendingName.assign(name, std::strlen(name));
  UIWindow* window = context.windows.EnsureWindow(35);
  std::uint32_t identity;
  std::memcpy(&identity, bytes + 2, sizeof(identity));
  window->SendMsg(nullptr, 56, identity, reinterpret_cast<std::intptr_t>(name), 0, 0);
}

#endif
