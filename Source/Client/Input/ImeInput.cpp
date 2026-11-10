#include "Client/Application/ApplicationWindow.h"
#include "Client/Input/Language.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/LanguageCharset.h"
#include "Utility/Text/TextTokens.h"

#include <algorithm>
#include <cstring>
#include <imm.h>
#include <intrin.h>

// OFFSET: 0x00ACEF00
// FUNCTION: fn_17793
LRESULT CLanguageBase::Message(HWND, UINT message, WPARAM value, LPARAM flags, std::uint8_t& handled) {
  switch (message) {
  case WM_INPUTLANGCHANGE: {
    const auto language = static_cast<std::uint16_t>(flags);
    this->language = language;
    this->codePage = g_inlineCharset ? LanguageCodePage(language) : g_textCodePage;
    break;
  }
  case WM_IME_STARTCOMPOSITION:
    this->ResetState();
    this->DeleteSelection();
    break;
  case WM_IME_ENDCOMPOSITION:
    this->EndComposition();
    this->candidate.assign("", 0);
    break;
  case WM_IME_COMPOSITION: {
    if (flags & GCS_COMPSTR) {
      this->RefreshComposition();
      this->ResetCandidate();
    }

    if (flags & GCS_CURSORPOS) {
      HIMC context = ImmGetContext(hWnd);
      if (context) {
        ImmGetCompositionStringA(context, GCS_CURSORPOS, &this->compositionCursor, sizeof(this->compositionCursor));
        ImmReleaseContext(hWnd, context);
      } else {
        this->candidate.clear();
      }
    }

    if (flags & GCS_RESULTSTR) {
      HIMC context = ImmGetContext(hWnd);
      if (context) {
        WCHAR wide[256];
        char converted[256];
        const auto bytes = static_cast<std::uint32_t>(ImmGetCompositionStringW(context, GCS_RESULTSTR, wide, sizeof(wide)));
        const unsigned size = WideCharToMultiByte(this->codePage, 0, wide, bytes >> 1, converted, sizeof(converted), nullptr, nullptr);

        converted[size] = 0;
        ImmReleaseContext(hWnd, context);
        this->Insert(converted);
      } else {
        this->candidate.clear();
      }
    }
    break;
  }
  case WM_IME_NOTIFY:
    switch (value) {
    case IMN_CHANGECANDIDATE:
    case IMN_OPENCANDIDATE: {
      HIMC context = ImmGetContext(hWnd);
      if (context) {
        const auto bytes = ImmGetCandidateListW(context, 0, nullptr, 0);
        if (static_cast<std::int32_t>(bytes) > 0) {
          this->candidateStorage.resize(bytes);
          this->candidateList = reinterpret_cast<CANDIDATELIST*>(this->candidateStorage.empty() ? nullptr : this->candidateStorage.data());
          ImmGetCandidateListW(context, 0, this->candidateList, bytes);
          this->candidateCount = static_cast<int>(this->candidateList->dwCount);
          this->selection = static_cast<int>(this->candidateList->dwSelection);
          this->pageSize = static_cast<int>(std::min<unsigned>(this->candidateList->dwPageSize, 9));
        } else {
          this->candidateCount = 0;
        }

        ImmReleaseContext(hWnd, context);
      } else {
        this->candidateCount = 0;
      }
      break;
    }
    case IMN_CLOSECANDIDATE:
      this->candidateCount = 0;
      break;
    }
    break;
  default:
    handled = 0;
    return 0;
  }

  handled = 1;
  return 0;
}

// OFFSET: 0x00ACD840
// FUNCTION: fn_17770
const char* CLanguageBase::Candidate(int index) const {
  // STATIC: 0x016D3978
  // GLOBAL: gl_24374
  static char output[256];

  if (index < this->candidateCount) {
    const unsigned char* bytes = this->candidateStorage.data();
    const WCHAR* text = reinterpret_cast<const WCHAR*>(bytes + this->candidateList->dwOffset[index]);
    const unsigned size = WideCharToMultiByte(this->codePage, 0, text, -1, output, sizeof(output), nullptr, nullptr);
    output[std::min<unsigned>(size, 255)] = 0;
    return output;
  }

  return "";
}
