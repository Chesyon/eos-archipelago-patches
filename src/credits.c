#include <pmdsky.h>
#include <cot.h>
#include "extern.h"

void PreprocessAndAnalyzeCreditsText(undefined param_1, undefined param_2, undefined param_3, const char* credits_text) {
  char buffer[0x400];
  struct preprocessor_flags flags;
  PreprocessString(buffer, 0x400, credits_text, flags, NULL);
  AnalyzeTextPreamble(param_1, param_2, param_3, buffer);
}
