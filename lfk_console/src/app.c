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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "lfk.h"

#include "utils/html_report_generator.h"
#include "utils/text_report_generator.h"

const char *G_HELP_TEXT =
    ""
    "Livermore Fortran Loops (LFK) benchmark tool."
    "\n"
    "Usage:"
    "\n"
    "\t -o <file>      ..... report text file                      [default "
    "lfk-report.txt]"
    "\n"
    "\t -r <file>.html ..... report html file                      [default "
    "lfk-report.html]"
    "\n"
    "\t -j <cores>     ..... number of cores (0 - auto detection)  [default 1]"
    "\n"
    "\t -t <seconds>   ..... execution time in seconds per kernel  [default 1]"
    "\n";

typedef struct {
  unsigned core_count;
  double execution_time;
  unsigned full_benchmark;

  const char *text_report_file;
  const char *html_report_file;
} program_parameters_t;

void benchmark_callback(void *dummy, int progress, const char *message) {
  static unsigned previous_length = 0;
  unsigned current_length;
  (void)dummy;

  current_length = printf("\r[%3i%%] - %s", progress, message);
  if (previous_length > current_length) {
    printf("%*c\r", previous_length - current_length, ' ');
  }

  previous_length = current_length;
  fflush(stdout);
}

int app_parse_args(program_parameters_t *params, int argc,
                   const char *const argv[]) {
  char *next_str = NULL;
  int i;

  if (NULL == params) {
    return -1;
  }

  /* Setting up default parameters */
  params->full_benchmark = 1;
  params->core_count = 0;
  params->execution_time = 1;
  params->text_report_file = "lfk-report.txt";
  params->html_report_file = "lfk-report.html";

#ifdef LFK_RUNFAST
  params->execution_time = 0.01;
#endif

  for (i = 1; i < argc; i++) {
    next_str = NULL;
    if (0 == strcmp(argv[i], "-h") || (0 == strcmp(argv[i], "--help"))) {
      printf("%s", G_HELP_TEXT);
      exit(0);
    } else if (0 == strcmp(argv[i], "-o")) {
      i++;
      params->text_report_file = argv[i];
    } else if (0 == strcmp(argv[i], "-r")) {
      i++;
      params->html_report_file = argv[i];
    } else if (0 == strcmp(argv[i], "-t")) {
      i++;
      params->execution_time = strtod(argv[i], &next_str);
    } else if (0 == strcmp(argv[i], "-j")) {
      i++;
      params->core_count = strtol(argv[i], &next_str, 10);
    } else if (NULL == next_str) {
      return -1;
    } else {
      return -1;
    }
  }

  return 0;
}

void app_print_run_parameters(const program_parameters_t *params,
                              const benchmark_handler_t handler) {
  printf("Running parameters:\n");
  printf("Report html file: ............. %s\n", params->html_report_file);
  printf("Report text file: ............. %s\n", params->text_report_file);
  printf("CPU: .......................... %s\n",
         benchmark_get_cpu_name(handler));
  printf("Core count: ................... %i%s\n",
         benchmark_get_core_count(handler),
         (params->core_count == 0) ? " (auto)" : "");
  printf("Execution time per kernel: .... %.1f second(s)\n",
         params->execution_time);
}

int app_main(const int argc, const char *const argv[]) {
  program_parameters_t params;
  benchmark_handler_t handler;
  benchmark_progress_callback_handler_t callbackHandler;

  app_parse_args(&params, argc, argv);

  handler = benchmark_init();
  benchmark_set_execution_time(handler, params.execution_time);
  benchmark_set_core_count(handler, params.core_count);

  callbackHandler.data = NULL;
  callbackHandler.callback = benchmark_callback;
  benchmark_set_progress_callback(handler, callbackHandler);

  app_print_run_parameters(&params, handler);

  benchmark_run(handler);
  printf("\n");

  html_report_generator_create_report(handler, params.html_report_file);
  text_report_generator_create_report(handler, params.text_report_file);

  benchmark_cleanup(handler);

  return 0;
}