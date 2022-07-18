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

#include "string.h"
#include "time.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "platform.h"
#include "thread.h"

#include "lfk/lfk.h"
#include "lfk/constants.h"

#include "core/lfk_core.h"

typedef struct benchmark_handler_ {
  unsigned core_count;
  double execution_time;
  unsigned optimization_enabled;
  unsigned auto_mode;

  lfk_run_type_e current_run;

  char *compiler_info;
  char *version_info;
  char *timestamp;

  lfk_full_result_t result;

  benchmark_progress_callback_handler_t progress_callback;
} benchmark_handler_;

static void benchmark_fill_timestamp(benchmark_handler_t handler) {
  char buffer[256];
  time_t t;
  struct tm *info;

  time(&t);
  info = localtime(&t);

  snprintf(buffer, sizeof(buffer), "%i:%i %i/%i/%i", info->tm_hour,
           info->tm_min, info->tm_mday, (info->tm_mon + 1),
           (info->tm_year + 1900));

  handler->timestamp = strdup(buffer);
}

benchmark_handler_t benchmark_init() {
  benchmark_handler_t ret = calloc(1, sizeof(benchmark_handler_));

  ret->core_count = 1;
  ret->execution_time = 1;
  ret->optimization_enabled = 1;
  ret->auto_mode = 1;
  ret->progress_callback.data = NULL;
  ret->progress_callback.callback = NULL;
  ret->version_info = strdup(LFK_VERSION);
  ret->compiler_info = strdup(LFK_COMPILER);

  benchmark_fill_timestamp(ret);

  return ret;
}

void benchmark_cleanup(benchmark_handler_t handler) {
  free(handler->version_info);
  free(handler->compiler_info);
  free(handler->timestamp);
  free(handler);
}

void benchmark_set_core_count(benchmark_handler_t handler,
                              unsigned core_count) {
  handler->core_count = core_count ? core_count : get_core_count();
}

void benchmark_set_progress_callback(
    benchmark_handler_t handler,
    benchmark_progress_callback_handler_t callback) {
  handler->progress_callback = callback;
}

void benchmark_set_execution_time(benchmark_handler_t handler,
                                  double execution_time) {
  handler->execution_time = execution_time;
}

void benchmark_set_optimization_enabled(benchmark_handler_t handler,
                                        unsigned optimization_enabled) {
  handler->optimization_enabled = optimization_enabled;
}

unsigned int benchmark_get_core_count(benchmark_handler_t handler) {
  return handler->core_count;
}

void benchmark_proxy_progress_callback(void *handler_as_void, int kernel,
                                       int run) {
  const unsigned TOTAL_KERNELS = 24;
  const unsigned TOTAL_RUNS = 3;
  char buffer[256];

  benchmark_handler_t handler = handler_as_void;
  int progress = (int)(100.0 * (TOTAL_KERNELS * (run) + kernel) /
                       (TOTAL_KERNELS * TOTAL_RUNS));
  if (handler->auto_mode) {
    progress = (int)((progress / 4.0) + (handler->current_run * 25.0));
  }
  snprintf(buffer, sizeof(buffer), "%s | Run %i/3 | Kernel %i/24 | %s",
           RUN_NAMES[handler->current_run], run + 1, kernel,
           KERNEL_NAMES[kernel - 1]);

  handler->progress_callback.callback(handler->progress_callback.data, progress,
                                      buffer);
}

static void benchmark_kernel_runner(benchmark_handler_t handler,
                                    lfk_run_result_t *result_p) {
  unsigned thread_idx;
  int i;

  void *(*benchmark)(void *) = handler->optimization_enabled
                                   ? benchmark_core_optimized
                                   : benchmark_core_nonoptimized;

  benchmark_core_args_t **thread_results =
      calloc(handler->core_count, sizeof(benchmark_core_args_t *));

  lfk_thread_handler_p *thread_pool =
      calloc(handler->core_count, sizeof(lfk_thread_handler_p));

  for (thread_idx = 0; thread_idx < handler->core_count; ++thread_idx) {
    thread_results[thread_idx] = calloc(1, sizeof(benchmark_core_args_t));
    thread_results[thread_idx]->execution_time = handler->execution_time;
    thread_results[thread_idx]->callback_handler.callback = NULL;
    thread_results[thread_idx]->callback_handler.data = NULL;
    if (0 == thread_idx) {
      thread_results[thread_idx]->callback_handler.callback =
          benchmark_proxy_progress_callback;
      thread_results[thread_idx]->callback_handler.data = handler;
    }

    thread_pool[thread_idx] =
        lfk_thread_init(benchmark, thread_results[thread_idx]);
  }
  for (thread_idx = 0; thread_idx < handler->core_count; ++thread_idx) {
    lfk_thread_join(thread_pool[thread_idx]);
  }

  for (thread_idx = 0; thread_idx < handler->core_count; ++thread_idx) {
    for (i = 0; i < 24; i++) {
      result_p->kernel_results[i] +=
          thread_results[thread_idx]->results.kernel_results[i];
    }
    free(thread_results[thread_idx]);
  }
  result_p->maximum = 0.0;
  result_p->average = 0.0;
  result_p->geometric = 0.0;
  result_p->harmonic = 0.0;
  result_p->minimum = result_p->kernel_results[0];

  for (i = 0; i < 24; i++) {
    result_p->average += result_p->kernel_results[i];
    result_p->harmonic += 1 / result_p->kernel_results[i];
    result_p->geometric += log(result_p->kernel_results[i]);
    if (result_p->kernel_results[i] < result_p->minimum) {
      result_p->minimum = result_p->kernel_results[i];
    }
    if (result_p->kernel_results[i] > result_p->maximum) {
      result_p->maximum = result_p->kernel_results[i];
    }
  }

  result_p->average = result_p->average / 24.0;
  result_p->harmonic = 24.0 / result_p->harmonic;
  result_p->geometric = exp(result_p->geometric / 24.0);
}

void benchmark_run(benchmark_handler_t handler) {
  lfk_run_type_e current_run_type;

  if (handler->auto_mode) {
    unsigned core_count = handler->core_count;
    for (current_run_type = single_core_non_optimized;
         current_run_type <= multi_core_optimized; current_run_type++) {
      lfk_run_result_t *result =
          &handler->result.lfk_run_result[current_run_type];
      handler->current_run = current_run_type;
      result->run_type = current_run_type;

      switch (current_run_type) {
      case single_core_non_optimized:
        handler->core_count = 1;
        handler->optimization_enabled = 0;
        break;
      case multi_core_non_optimized:
        handler->core_count = core_count;
        handler->optimization_enabled = 0;
        break;
      case single_core_optimized:
        handler->core_count = 1;
        handler->optimization_enabled = 1;
        break;
      case multi_core_optimized:
        handler->core_count = core_count;
        handler->optimization_enabled = 1;
        break;
      default:
        return;
      }
      benchmark_kernel_runner(handler, result);
    }
  }
  handler->result.singlecore_result =
      handler->result.lfk_run_result[single_core_non_optimized].geometric;

  handler->result.multicore_result =
      handler->result.lfk_run_result[multi_core_non_optimized].geometric;

  handler->progress_callback.callback(NULL, 100, "Done!");
}
const char *benchmark_get_compiler_info(benchmark_handler_t handler) {
  return handler->compiler_info;
}

const char *benchmark_get_version(benchmark_handler_t handler) {
  return handler->version_info;
}

const char *benchmark_get_date(benchmark_handler_t handler) {
  return handler->timestamp;
}

const lfk_full_result_t *benchmark_get_results(benchmark_handler_t handler) {
  return &handler->result;
}
