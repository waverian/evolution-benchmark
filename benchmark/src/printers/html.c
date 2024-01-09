/*
 * Copyright (c) 2022 waverian
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <waverian/benchmark/benchmark.h>
#include <waverian/benchmark/details/constants.h>
#include <waverian/benchmark/printers.h>

#include <stdio.h>

#define AUTO_PRECISION(x) ((x) > 100 ? 0 : (x) > 10 ? 1 : (x) > 1 ? 2 : 3), (x)

BENCHMARK_ERROR_CODE_E
benchmark_print_results_html(const benchmark_handler_t handler, const char *file_path) {
  int i;
  WB_RUN_TYPE_E j;

  const benchmark_parameters_t parameters = benchmark_get_parameters(handler);
  const benchmark_result_t result = benchmark_get_results(handler);
  FILE *file_handler = fopen(file_path, "w");

  if (NULL == file_handler) {
    return BENCHMARK_ERROR_CODE_FILE_WRITE_ERROR;
  }

  // clang-format off
  // This insert the code in the html_generated.c directly to this file.
  // The html_generated.c file is autogenerated by the template_processor.py
  // based on the templated html report_template.html
  #include "html_generated.c"
  // clang-format on

  fclose(file_handler);

  return BENCHMARK_ERROR_CODE_OK;
}
