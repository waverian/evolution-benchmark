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

#define FIRST_COLUMN_LEN 15
#define COLUMN_LEN 13

#define AUTO_PRECISION(x) ((x) > 100 ? 0 : (x) > 10 ? 1 : (x) > 1 ? 2 : 3), (x)

void print_table_divider(FILE *file_handler, int column_count) {
  int i;
  int j;

  fprintf(file_handler, "+");
  for (i = 0; i < FIRST_COLUMN_LEN; i++) {
    fprintf(file_handler, "-");
  }

  for (j = 0; j < column_count; j++) {
    fprintf(file_handler, "+");
    for (i = 0; i < COLUMN_LEN; i++) {
      fprintf(file_handler, "-");
    }
  }
  fprintf(file_handler, "+\r\n");
}

void print_str_first_cell(FILE *file_handler, const char *str) {
  fprintf(file_handler, "| %-*s|", FIRST_COLUMN_LEN - 1, str);
}
void print_str_cell(FILE *file_handler, const char *str) {
  fprintf(file_handler, "%*s |", COLUMN_LEN - 1, str);
}
void print_float_cell(FILE *file_handler, double num) {
  fprintf(file_handler, "%*.*f |", COLUMN_LEN - 1, AUTO_PRECISION(num));
}
void print_int_first_cell(FILE *file_handler, int num) {
  fprintf(file_handler, "| %-*i|", FIRST_COLUMN_LEN - 1, num);
}

BENCHMARK_ERROR_CODE_E
benchmark_print_results_text(const benchmark_handler_t handler, const char *path) {
  int column_count = 0;
  int kernel_idx;
  WB_RUN_TYPE_E run_type;
  WB_OPTIMIZATION_E optimization;
  FILE *file_handler;

  const benchmark_parameters_t parameters = benchmark_get_parameters(handler);
  const benchmark_result_t result = benchmark_get_results(handler);

  file_handler = fopen(path, "wb");

  fprintf(file_handler, "Waverian benchmark report\r\n");
  fprintf(file_handler, "\r\n");
  fprintf(file_handler, "Version            - %s\r\n", parameters.version_info);
  fprintf(file_handler, "Date               - %s\r\n", result.timestamp);
  fprintf(file_handler, "Compiler           - %s\r\n", parameters.compiler_info);
  fprintf(file_handler, "Logical cores      - %u\r\n", result.core_count);
  fprintf(file_handler, "CPU name           - %s\r\n", parameters.cpu_name);
  fprintf(file_handler, "Comment            - %s\r\n", result.comment);
  fprintf(file_handler, "\r\n");
  fprintf(file_handler, "OVERALL RESULT     - %.*f\r\n",
          AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].score));
  fprintf(file_handler, "\r\n");

  for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
    if (result.full_result[WB_OPTIMIZATION_DISABLED].detailed[run_type].valid) {
      fprintf(file_handler, "%-18s - %-6.*f", WB_RUN_TYPE_NAMES[run_type],
              AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_DISABLED].detailed[run_type].score));
      if (run_type != WB_RUN_TYPE_SINGLE_CORE) {
        fprintf(file_handler, " (ratio x%.1f)", result.full_result[WB_OPTIMIZATION_DISABLED].detailed[run_type].ratio);
      }
      fprintf(file_handler, "\r\n");
      column_count++;
    }
  }
  fprintf(file_handler, "\r\n");

  for (optimization = WB_OPTIMIZATION_DISABLED; optimization < WB_OPTIMIZATION_SIZE; optimization++) {
    if (optimization == WB_OPTIMIZATION_ENABLED) {
      fprintf(file_handler, "\r\n");
      fprintf(file_handler, "Optimized score - %.*f (optimized ratio x%.1f)\r\n",
              AUTO_PRECISION(result.full_result[WB_OPTIMIZATION_ENABLED].score),
              result.full_result[WB_OPTIMIZATION_ENABLED].score / result.full_result[WB_OPTIMIZATION_DISABLED].score);
      fprintf(file_handler, "\r\n");
    }

    print_table_divider(file_handler, column_count);

    print_str_first_cell(file_handler, WB_OPTIMIZATION_NAMES[optimization]);
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_str_cell(file_handler, WB_RUN_TYPE_NAMES[run_type]);
      }
    }
    fprintf(file_handler, "\r\n");

    print_table_divider(file_handler, column_count);

    print_str_first_cell(file_handler, "Maximum");
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_float_cell(file_handler, result.full_result[optimization].detailed[run_type].maximum);
      }
    }
    fprintf(file_handler, "\r\n");

    print_str_first_cell(file_handler, "Average");
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_float_cell(file_handler, result.full_result[optimization].detailed[run_type].average);
      }
    }
    fprintf(file_handler, "\r\n");

    print_str_first_cell(file_handler, "Geometric");
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_float_cell(file_handler, result.full_result[optimization].detailed[run_type].geometric);
      }
    }
    fprintf(file_handler, "\r\n");

    print_str_first_cell(file_handler, "Harmonic");
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_float_cell(file_handler, result.full_result[optimization].detailed[run_type].harmonic);
      }
    }
    fprintf(file_handler, "\r\n");

    print_str_first_cell(file_handler, "Minimum");
    for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
      if (result.full_result[optimization].detailed[run_type].valid) {
        print_float_cell(file_handler, result.full_result[optimization].detailed[run_type].minimum);
      }
    }
    fprintf(file_handler, "\r\n");
    print_table_divider(file_handler, column_count);

    for (kernel_idx = 0; kernel_idx < WB_KERNEL_COUNT; kernel_idx++) {
      print_int_first_cell(file_handler, kernel_idx + 1);
      for (run_type = WB_RUN_TYPE_MANUAL; run_type < WB_RUN_TYPE_SIZE; run_type++) {
        if (result.full_result[optimization].detailed[run_type].valid) {
          print_float_cell(file_handler,
                           result.full_result[optimization].detailed[run_type].kernel_results[kernel_idx]);
        }
      }
      fprintf(file_handler, "\r\n");
    }

    print_table_divider(file_handler, column_count);
  }

  fclose(file_handler);
  return BENCHMARK_ERROR_CODE_OK;
}
