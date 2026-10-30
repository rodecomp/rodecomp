#include "Contents/RodexMailStore.h"

#include <new>

// GLOBAL: gl_24911
RodexMailStore* g_rodexMailStore;

// OFFSET: 0x007FE5E0
// FUNCTION: fn_09985
bool RodexMailStore::Create() {
  if (g_rodexMailStore != nullptr) {
    return false;
  }

  g_rodexMailStore = new (std::nothrow) RodexMailStore();
  if (g_rodexMailStore == nullptr) {
    return false;
  }

  if (!g_rodexMailStore->m_isValid) {
    delete g_rodexMailStore;
    g_rodexMailStore = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x007FE7B0
// FUNCTION: fn_09986
void RodexMailStore::Destroy() {
  if (g_rodexMailStore != nullptr) {
    delete g_rodexMailStore;
    g_rodexMailStore = nullptr;
  }
}

// OFFSET: 0x007FE7D0
// FUNCTION: fn_09987
void RodexMailStore::Clear() {
  this->m_mails0.clear();
  this->m_mails1.clear();
  this->m_mails2.clear();
  this->unknown_0008 = 0;
  this->unknown_000C = 0;
  this->unknown_0010 = false;
  this->unknown_0018 = 0;
}

// OFFSET: 0x007FE850
// FUNCTION: fn_09988
void RodexMailStore::PurgeFlagged() {
  for (auto mail = this->m_mails0.begin(); mail != this->m_mails0.end();) {
    if (mail->second.unknown_0190 == 1) {
      mail = this->m_mails0.erase(mail);
    } else {
      ++mail;
    }
  }

  for (auto mail = this->m_mails1.begin(); mail != this->m_mails1.end();) {
    if (mail->second.unknown_0190 == 1) {
      mail = this->m_mails1.erase(mail);
    } else {
      ++mail;
    }
  }

  for (auto mail = this->m_mails2.begin(); mail != this->m_mails2.end();) {
    if (mail->second.unknown_0190 == 1) {
      mail = this->m_mails2.erase(mail);
    } else {
      ++mail;
    }
  }
}
