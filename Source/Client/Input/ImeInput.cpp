#include "Client/Input/ImeInput.h"
#include "Client/Input/TraditionalKeyInput.h"
#include "Utility/Text/LanguageCharset.h"

#include <algorithm>
#include <cstring>
#include <intrin.h>

// OFFSET: 0x00ACEF00
// FUNCTION: fn_17793
LRESULT ImeInput::Message(HWND, UINT message, WPARAM value, LPARAM flags, std::uint8_t& handled) {
  const auto options = static_cast<std::uint16_t>(flags);

  switch (message) {
  case WM_INPUTLANGCHANGE:
    this->language = options;
    this->codePage = this->languageCodePage ? LanguageCodePage(options) : this->fallbackCodePage;
    break;
  case WM_IME_STARTCOMPOSITION:
    this->input.DeleteSelection();
    break;
  case WM_IME_ENDCOMPOSITION:
    if (this->traditional) {
      this->traditional->ClearKeys();
    }
    this->input.candidate.clear();
    break;
  case WM_IME_COMPOSITION:
    if (options & GCS_COMPSTR) {
      this->input.RefreshComposition(this->codePage);
      if (this->traditional) {
        this->traditional->FilterCandidate(this->input.candidate);
        this->traditional->RetainSingleKey();
      }
    }

    if (options & GCS_CURSORPOS) {
      HIMC context = ImmGetContext(this->input.window);
      if (context) {
        ImmGetCompositionStringA(context, GCS_CURSORPOS, &this->compositionCursor, sizeof(this->compositionCursor));
        ImmReleaseContext(this->input.window, context);
      } else {
        this->input.candidate.clear();
      }
    }

    if (options & GCS_RESULTSTR) {
      HIMC context = ImmGetContext(this->input.window);
      if (context) {
        WCHAR wide[256];
        char converted[256];
        const auto bytes = static_cast<std::uint32_t>(ImmGetCompositionStringW(context, GCS_RESULTSTR, wide, sizeof(wide)));
        const unsigned size = WideCharToMultiByte(this->codePage, 0, wide, bytes >> 1, converted, sizeof(converted), nullptr, nullptr);

        if (size >= sizeof(converted)) {
          __fastfail(FAST_FAIL_RANGE_CHECK_FAILURE);
        }

        converted[size] = 0;
        ImmReleaseContext(this->input.window, context);
        this->input.Insert(converted);
      } else {
        this->input.candidate.clear();
      }
    }
    break;
  case WM_IME_NOTIFY:
    if (value == IMN_OPENCANDIDATE || value == IMN_CHANGECANDIDATE) {
      HIMC context = ImmGetContext(this->input.window);
      if (context) {
        const auto bytes = ImmGetCandidateListW(context, 0, nullptr, 0);
        if (static_cast<std::int32_t>(bytes) > 0) {
          this->candidateStorage.resize(bytes);
          this->candidateList = reinterpret_cast<CANDIDATELIST*>(this->candidateStorage.data());
          ImmGetCandidateListW(context, 0, this->candidateList, bytes);
          this->input.compositionState = this->candidateList->dwCount;
          this->selection = this->candidateList->dwSelection;
          this->pageSize = std::min<unsigned>(this->candidateList->dwPageSize, 9);
        } else {
          this->input.compositionState = 0;
        }

        ImmReleaseContext(this->input.window, context);
      } else {
        this->input.compositionState = 0;
      }
    } else if (value == IMN_CLOSECANDIDATE) {
      this->input.compositionState = 0;
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
const char* ImeInput::Candidate(int index) const {
  // STATIC: 0x016D3978
  // GLOBAL: gl_24374
  static char output[256];
  if (index >= this->input.compositionState) {
    return "";
  }

  const unsigned char* bytes = this->candidateStorage.data();
  const WCHAR* text = reinterpret_cast<const WCHAR*>(bytes + this->candidateList->dwOffset[index]);
  const unsigned size = WideCharToMultiByte(this->codePage, 0, text, -1, output, sizeof(output), nullptr, nullptr);
  output[std::min<unsigned>(size, 255)] = 0;

  return output;
}
