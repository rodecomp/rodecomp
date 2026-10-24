#include "Contents/AdventurerAgencyMgr.h"
#include "Contents/LoadPartyListAsyncWork.h"
#include "Network/CURLPostHander.h"
#include "Network/TextEncoding.h"

#include <cstdio>
#include <mmsystem.h>

namespace {
  constexpr char sourceFile[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Contents\\AdventurerAgency\\CLoadPartyLIstAsyncWork.cpp";

  AsyncDiagnostic Diagnostic(bool success, const char* text, std::uint32_t line) {
    std::string message = text;
    return AsyncDiagnostic(&success, &message, sourceFile, &line);
  }
}

// OFFSET: 0x005D1640
// FUNCTION: fn_03698
CLoadPartyListAsyncWork::CLoadPartyListAsyncWork()
    : NAsyncWork("PartyList_Load"), m_accountId(0), m_characterId(0), m_page(0),
      m_diagnostic(Diagnostic(false, "unknwon", 25)), m_windows(nullptr) {
}

// OFFSET: 0x005D1AB0
// FUNCTION: fn_03699
CLoadPartyListAsyncWork::~CLoadPartyListAsyncWork() = default;

// OFFSET: 0x005D1F30
// FUNCTION: fn_03701
void CLoadPartyListAsyncWork::Reset() {
}

// OFFSET: 0x005D1BD0
// FUNCTION: fn_03700
void CLoadPartyListAsyncWork::Bind(
  std::uint32_t accountId,
  std::uint32_t characterId,
  std::string world,
  std::string url,
  std::string token,
  CAdvenPartyInfo filter,
  int search,
  int page,
  WindowManagerWindows& windows
) {
  this->m_group = timeGetTime();
  this->m_accountId = accountId;
  this->m_characterId = characterId;
  this->m_key = 0;
  this->m_world = world;
  this->m_url = url;
  this->m_token = token;
  this->m_filter = filter;
  this->m_page = page;
  this->m_search = search;
  AsyncDiagnostic diagnostic = Diagnostic(false, "unknwon", 54);
  this->m_diagnostic.MoveFrom(diagnostic);
  this->m_windows = &windows;
}

// OFFSET: 0x005D2000
// FUNCTION: fn_03703
bool CLoadPartyListAsyncWork::Execute() {
  char account[64] = {};
  char character[64] = {};
  char page[10] = {};
  _snprintf_s(account, sizeof(account), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_accountId));
  _snprintf_s(character, sizeof(character), _TRUNCATE, "%lu", static_cast<unsigned long>(this->m_characterId));
  _snprintf_s(page, sizeof(page), _TRUNCATE, "%d", this->m_page);
  const std::string world = ToUtf8(this->m_world);
  CCURLPostHander handler(this->m_url);
  handler.AddField("GID", character, 4);
  handler.AddField("AID", account, 4);
  handler.AddField("WorldName", world, 4);
  handler.AddField("AuthToken", this->m_token, 4);

  if (this->m_search == 0) {
    handler.AddField("page", page, 4);
  } else if (this->m_search == 1) {
    char minimum[10] = {};
    char maximum[10] = {};
    _snprintf_s(minimum, sizeof(minimum), _TRUNCATE, "%d", this->m_filter.minimumLevel);
    _snprintf_s(maximum, sizeof(maximum), _TRUNCATE, "%d", this->m_filter.maximumLevel);
    handler.AddField("MinLV", minimum, 4);
    handler.AddField("MaxLV", maximum, 4);

    if (this->m_filter.tanker) {
      handler.AddField("Tanker", "1", 4);
    }

    if (this->m_filter.dealer) {
      handler.AddField("Dealer", "1", 4);
    }

    if (this->m_filter.healer) {
      handler.AddField("Healer", "1", 4);
    }

    if (this->m_filter.assist) {
      handler.AddField("Assist", "1", 4);
    }

    if (this->m_filter.type != 0) {
      char type[10] = {};
      _snprintf_s(type, sizeof(type), _TRUNCATE, "%d", this->m_filter.type);
      handler.AddField("Type", type, 4);
    }

    if (!this->m_filter.memo.empty()) {
      handler.AddField("Memo", ToUtf8(this->m_filter.memo), 4);
    }

    handler.AddField("page", page, 4);
  }

  if (handler.Perform() == 0) {
    this->m_response = FromUtf8(handler.Response());
    AsyncDiagnostic diagnostic = Diagnostic(true, "success", 138);
    this->m_diagnostic.MoveFrom(diagnostic);
  }

  // Transport failure preserves the diagnostic written by Bind.
  return false;
}

// OFFSET: 0x005D1F40
// FUNCTION: fn_03702
void CLoadPartyListAsyncWork::Complete() {
  g_adventurerAgencyMgr->CompleteList(this->m_diagnostic, this->m_response, *this->m_windows);
}
