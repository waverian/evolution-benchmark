/*
 * Copyright (c) 2022 waverian. All rights reserved.
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

#include "app.h"

#include <waverian/evolution_benchmark.h>
#include <waverian/evolution_benchmark/constants.h>
#include <waverian/evolution_benchmark/printers.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTO_PRECISION(x) ((x) > 100 ? 0 : (x) > 10 ? 1 : (x) > 1 ? 2 : 3), (x)

const char *G_HELP_TEXT =
    "Waverian Evolution Benchmark\n"
    "Usage:\n"
    "\t -o <file>    ..... .txt and .html reports file name      [default evolution_benchmark_report]\n"
    "\t -c <comment> ..... comment added to the report\n"
    "\t -j <cores>   ..... number of cores (0 - auto detection)  [default 1]\n"
    "\t -t <seconds> ..... execution time in seconds per kernel  [default 0.5]\n"
    "\t -w           ..... enable workstation mode               [default off]\n";

typedef struct {
  int core_count;
  double execution_time;
  EB_EXECUTION_MODE_E execution_mode;
  const char *comment;

  const char *report_file;
} app_params_t;

int app_callback(void *dummy, int progress, const char *message) {
  static int previous_length = 0;
  (void)dummy;

  printf("\r%*c", previous_length, ' ');
  previous_length = printf("\r[%3i%%] - %s", progress, message);

  fflush(stdout);
  return 0;
}

int app_parse_args(app_params_t *params, int argc, const char *const argv[]) {
  char *next_str = NULL;
  int i;

  if (NULL == params) {
    return -1;
  }

  /* Setting up default parameters */
  params->execution_mode = EB_EXECUTION_MODE_AUTO;
  params->core_count = 0;
  params->execution_time = 0.5;
  params->report_file = "evolution_benchmark_report";
  params->comment = NULL;

#ifdef EB_RUN_FAST
  params->execution_time = 0.001;
#endif

  for (i = 1; i < argc; i++) {
    next_str = NULL;
    if (0 == strcmp(argv[i], "-h") || (0 == strcmp(argv[i], "--help"))) {
      printf("%s", G_HELP_TEXT);
      exit(0);
    } else if (0 == strcmp(argv[i], "-o")) {
      i++;
      params->report_file = argv[i];
    } else if (0 == strcmp(argv[i], "-t")) {
      i++;
      params->execution_mode = EB_EXECUTION_MODE_MANUAL;
      params->execution_time = strtod(argv[i], &next_str);
    } else if (0 == strcmp(argv[i], "-c")) {
      i++;
      params->comment = argv[i];
    } else if (0 == strcmp(argv[i], "-w")) {
      params->execution_mode = EB_EXECUTION_MODE_FULL_AUTO;
    } else if (0 == strcmp(argv[i], "-j")) {
      i++;
      params->core_count = strtol(argv[i], &next_str, 10);
    } else {
      return -1;
    }
  }

  return 0;
}

void app_print_run_parameters(const app_params_t params, const eb_handler_t handler) {
  eb_system_info_t system_info;
  eb_get_system_info(handler, &system_info);

  printf("Running parameters:\n");
  printf("Benchmark version: ............ %s\n", system_info.version_info);
  printf("Report file: .................. %s\n", params.report_file);
  printf("CPU: .......................... %s\n", system_info.cpu_name);
  printf("Logical cores: ................ %i%s\n",
         (params.core_count == 0) ? system_info.cpu_core_count : params.core_count,
         (params.core_count == 0) ? " (auto)" : "");
  printf("Execution time per kernel: .... %.1f second(s)\n", params.execution_time);
  printf("Execution mode: ............... %s\n", EB_EXECUTION_MODE_NAMES[params.execution_mode]);
}

void app_print_run_results(const eb_result_t result) {
  EB_RUN_TYPE_E run_type;
  const char *format = "%-14s - %-6.*f";

  printf(format, "OVERALL RESULT", AUTO_PRECISION(result.full_result[EB_OPTIMIZATION_DISABLED].score));

  printf("\n\n");

  for (run_type = EB_RUN_TYPE_MANUAL; run_type < EB_RUN_TYPE_SIZE; run_type++) {
    if (result.full_result[EB_OPTIMIZATION_DISABLED].detailed[run_type].valid) {
      printf("%-14s - %-6.*f", EB_RUN_TYPE_NAMES[run_type],
             AUTO_PRECISION(result.full_result[EB_OPTIMIZATION_DISABLED].detailed[run_type].score));
      if (run_type != EB_RUN_TYPE_SINGLE_CORE) {
        printf(" (MP ratio x%.1f)", result.full_result[EB_OPTIMIZATION_DISABLED].detailed[run_type].ratio);
      }
      printf("\n");
    }
  }
}

int app_main(const int argc, const char *const argv[]) {
  app_params_t params;
  eb_handler_t handler;
  eb_result_t results;
  char *filename;
  eb_progress_callback_handler_t callback_handler;

  app_parse_args(&params, argc, argv);

  handler = eb_init();

  eb_set_execution_time(handler, params.execution_time);
  eb_set_core_count_limit(handler, params.core_count);
  eb_set_execution_mode(handler, params.execution_mode);
  eb_set_comment(handler, params.comment);

  callback_handler.data = NULL;
  callback_handler.callback = app_callback;
  eb_set_progress_callback(handler, callback_handler);

  app_print_run_parameters(params, handler);

  eb_run_benchmark(handler, &results);
  printf("\n\n");

  app_print_run_results(results);

  filename = malloc(strlen(params.report_file) + 6);

  strcpy(filename, params.report_file);
  strncat(filename, ".html", strlen(filename));
  eb_result_print_html(results, filename);

  strcpy(filename, params.report_file);
  strncat(filename, ".txt", strlen(filename));
  eb_result_print_text(results, filename);

  free(filename);
  eb_cleanup(handler);

  return 0;
}