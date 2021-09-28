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

#include "text_report_generator.h"

#include "lfk/constants.h"

#include <stdio.h>

void text_report_generator_create_report(benchmark_handler_t handler,
                                         const char *file_path) {
  const lfk_full_result_t *result = benchmark_get_results(handler);
  const char *format = "|%15s |%11.0f |%11.0f |%11.0f |%11.0f |\n";
  int kernel_idx;

  FILE *file_handler;
  file_handler = fopen(file_path, "w");

  fprintf(file_handler, "LFKX Benchmark report\n\n");
  fprintf(file_handler, "Version    - %s\n", benchmark_get_version(handler));
  fprintf(file_handler, "Date       - %s\n", benchmark_get_date(handler));
  fprintf(file_handler, "Compiler   - %s\n",
          benchmark_get_compiler_info(handler));
  fprintf(file_handler, "Core count - %u\n", benchmark_get_core_count(handler));
  fprintf(file_handler, "\n");
  fprintf(file_handler, "SINGLECORE RESULT - %f\n", result->singlecore_result);
  fprintf(file_handler, "MULTICORE RESULT  - %f\n", result->multicore_result);
  fprintf(file_handler, "\n");

  fprintf(file_handler, "DETAILED RESULTS:\n");
  fprintf(file_handler, "|                |      Non-Optimized      |        "
                        "Optimized        |\n");
  fprintf(file_handler, "|     MFLOPS     | Singlecore | Multicore  | "
                        "Singlecore | Multicore  |\n");

  fprintf(file_handler, format, "Maximum",
          result->lfk_run_result[single_core_non_optimized].maximum,
          result->lfk_run_result[multi_core_non_optimized].maximum,
          result->lfk_run_result[single_core_optimized].maximum,
          result->lfk_run_result[multi_core_optimized].maximum);

  fprintf(file_handler, format, "Average",
          result->lfk_run_result[single_core_non_optimized].average,
          result->lfk_run_result[multi_core_non_optimized].average,
          result->lfk_run_result[single_core_optimized].average,
          result->lfk_run_result[multi_core_optimized].average);

  fprintf(file_handler, format, "Geometric",
          result->lfk_run_result[single_core_non_optimized].geometric,
          result->lfk_run_result[multi_core_non_optimized].geometric,
          result->lfk_run_result[single_core_optimized].geometric,
          result->lfk_run_result[multi_core_optimized].geometric);

  fprintf(file_handler, format, "Harmonic",
          result->lfk_run_result[single_core_non_optimized].harmonic,
          result->lfk_run_result[multi_core_non_optimized].harmonic,
          result->lfk_run_result[single_core_optimized].harmonic,
          result->lfk_run_result[multi_core_optimized].harmonic);

  fprintf(file_handler, format, "Minimum",
          result->lfk_run_result[single_core_non_optimized].minimum,
          result->lfk_run_result[multi_core_non_optimized].minimum,
          result->lfk_run_result[single_core_optimized].minimum,
          result->lfk_run_result[multi_core_optimized].minimum);

  fprintf(file_handler, "\n");
  fprintf(file_handler, "KERNEL RESULTS:\n");
  fprintf(file_handler, "|                |      Non-Optimized      |        "
                        "Optimized        |\n");
  fprintf(file_handler, "|     MFLOPS     | Singlecore | Multicore  | "
                        "Singlecore | Multicore  |\n");
  for (kernel_idx = 0; kernel_idx < KERNEL_COUNT; kernel_idx++) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%i", kernel_idx + 1);
    fprintf(file_handler, format, buffer,
            result->lfk_run_result[single_core_non_optimized]
                .kernel_results[kernel_idx],
            result->lfk_run_result[multi_core_non_optimized]
                .kernel_results[kernel_idx],
            result->lfk_run_result[single_core_optimized]
                .kernel_results[kernel_idx],
            result->lfk_run_result[multi_core_optimized]
                .kernel_results[kernel_idx]);
  }

  fclose(file_handler);
}

//
// printf(" MFLOPS for 24 loops\n", );
// for (i = 0; i < 12; i++) {
// if (result_p->kernel_results[i] < 10000) {
// printf("%7.1f", result_p->kernel_results[i]);
//} else {
// printf("%7.0f", result_p->kernel_results[i]);
//}
//}
// printf("\n");
// for (i = 12; i < 24; i++) {
// if (result_p->kernel_results[i] < 10000) {
// printf("%7.1f", result_p->kernel_results[i]);
//} else {
// printf("%7.0f", result_p->kernel_results[i]);
//}
//}
// printf("\n\n");
//
// printf(" Overall Ratings\n");
// printf(" Maximum Average Geomean Harmean Minimum\n");
// printf("%8.1f%8.1f%8.1f%8.1f%8.1f\n\n",
// result_p->maximum,
// result_p->average,
// result_p->geometric,
// result_p->harmonic,
// result_p->minimum);