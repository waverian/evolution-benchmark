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
#include <waverian/benchmark/details/types.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/core.h"
#include "platform.h"
#include "thread.h"

static char *EMPTY_STR = "";

typedef struct {
  WB_RUN_TYPE_E run_type;
  WB_OPTIMIZATION_E optimization;

  int core_count;
  double execution_time;
} execution_task_t;

typedef struct benchmark_handler_ {
  int core_count_limit;
  double execution_time;
  WB_OPTIMIZATION_E optimization;
  WB_EXECUTION_MODE_E execution_mode;

  char *compiler_info;
  char *version_info;
  char *timestamp;
  char *cpu_name;
  char *comment;

  benchmark_parameters_t parameters;
  run_result_t run_result[WB_OPTIMIZATION_SIZE][WB_RUN_TYPE_SIZE];

  benchmark_progress_callback_handler_t progress_callback;

  struct {
    execution_task_t *current;
    execution_task_t queue[WB_RUN_TYPE_SIZE * WB_OPTIMIZATION_SIZE];
    int size;
  } execution_queue;
} benchmark_handler_;

static void benchmark_setup_execution_queue(benchmark_handler_t handler) {
  WB_OPTIMIZATION_E optimization;
  execution_task_t *task;

  if (handler->execution_mode == WB_EXECUTION_MODE_MANUAL) {
    task = handler->execution_queue.queue + handler->execution_queue.size++;
    task->run_type = WB_RUN_TYPE_MANUAL;
    task->optimization = handler->optimization;
    task->execution_time = handler->execution_time;
    task->core_count = handler->core_count_limit;
    return;
  }

  for (optimization = WB_OPTIMIZATION_DISABLED; optimization < WB_OPTIMIZATION_SIZE; optimization++) {
    task = handler->execution_queue.queue + handler->execution_queue.size++;
    task->run_type = WB_RUN_TYPE_SINGLE_CORE;
    task->optimization = optimization;
    task->execution_time = handler->execution_time;
    task->core_count = 1;

    if (handler->core_count_limit > 4) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = WB_RUN_TYPE_QUAD_CORE;
      task->optimization = optimization;
      task->execution_time = handler->execution_time;
      task->core_count = 4;
    }

    if (handler->core_count_limit > 1) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = WB_RUN_TYPE_MULTI_CORE;
      task->optimization = optimization;
      task->execution_time = handler->execution_time;
      task->core_count = handler->core_count_limit;
    }

    if (handler->execution_mode == WB_EXECUTION_MODE_FULL_AUTO) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = WB_RUN_TYPE_WORKSTATION;
      task->optimization = optimization;
      task->execution_time = handler->execution_time * 8;
      task->core_count = handler->core_count_limit;
    }
  }
}

static void benchmark_fill_timestamp(benchmark_handler_t handler) {
  char buffer[256];
  time_t t;
  struct tm *info;

  if (NULL != handler->timestamp) {
    free(handler->timestamp);
  }

  time(&t);
  info = localtime(&t);

  snprintf(buffer, sizeof(buffer), "%02i:%02i %02i/%02i/%i", info->tm_hour, info->tm_min, info->tm_mday,
           (info->tm_mon + 1), (info->tm_year + 1900));

  handler->timestamp = strdup(buffer);
}

static void benchmark_fill_cpu_name(benchmark_handler_t handler) {
  char *str;
  const char *ptr;
  str = get_cpu_name();
  ptr = str;
  while (strchr("\n\t ", *ptr) != NULL) {
    ptr++;
  }
  handler->cpu_name = strdup(ptr);
  handler->parameters.cpu_name = handler->cpu_name;
  free(str);
}

benchmark_handler_t benchmark_init() {
  benchmark_handler_t handler = calloc(1, sizeof(benchmark_handler_));

  handler->execution_time = 0.5;
  handler->optimization = WB_OPTIMIZATION_DISABLED;
  handler->execution_mode = WB_EXECUTION_MODE_AUTO;
  handler->progress_callback.data = NULL;
  handler->progress_callback.callback = NULL;

  benchmark_fill_cpu_name(handler);
  benchmark_set_core_count_limit(handler, 0);

  handler->comment = strdup(EMPTY_STR);
  handler->version_info = strdup(WB_VERSION);
  handler->compiler_info = strdup(WB_COMPILER);

  handler->parameters.cpu_core_count = handler->core_count_limit;
  handler->parameters.version_info = handler->version_info;
  handler->parameters.compiler_info = handler->compiler_info;

  handler->execution_queue.size = 0;
  handler->execution_queue.current = handler->execution_queue.queue;
  return handler;
}

BENCHMARK_ERROR_CODE_E
benchmark_cleanup(benchmark_handler_t handler) {
  if (NULL == handler) {
    return BENCHMARK_ERROR_CODE_NULL_POINTER;
  }

  free(handler->version_info);
  free(handler->compiler_info);
  free(handler->timestamp);
  free(handler->cpu_name);
  free(handler->comment);
  free(handler);

  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_core_count_limit(benchmark_handler_t handler, int core_count) {
  int actual_core_count = get_core_count();

  handler->parameters.cpu_core_count = actual_core_count;

  if (0 == core_count) {
    handler->core_count_limit = actual_core_count;
  } else {
    handler->core_count_limit = core_count;
  }

  if (actual_core_count < core_count) {
    return BENCHMARK_ERROR_CODE_INVALID_ARGUMENT;
  }
  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_progress_callback(benchmark_handler_t handler, benchmark_progress_callback_handler_t callback) {
  handler->progress_callback = callback;
  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_execution_time(benchmark_handler_t handler, double execution_time) {
  handler->execution_time = execution_time;
  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_execution_mode(benchmark_handler_t handler, WB_EXECUTION_MODE_E execution_mode) {
  handler->execution_mode = execution_mode;
  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_optimization_enabled(benchmark_handler_t handler, WB_OPTIMIZATION_E optimization) {
  handler->optimization = optimization;
  return BENCHMARK_ERROR_CODE_OK;
}

BENCHMARK_ERROR_CODE_E
benchmark_set_comment(benchmark_handler_t handler, const char *comment) {
  if (NULL == comment) {
    return BENCHMARK_ERROR_CODE_INVALID_ARGUMENT;
  }

  handler->comment = strdup(comment);
  return BENCHMARK_ERROR_CODE_OK;
}

void benchmark_proxy_progress_callback(void *handler_as_void, int kernel, int run) {
  static const int TOTAL_KERNELS = 24;
  static const int TOTAL_RUNS = 3;
  int progress;
  char buffer[256];

  benchmark_handler_t handler = handler_as_void;

  progress = (int)(100.0 * (TOTAL_KERNELS * (run) + kernel) / (TOTAL_KERNELS * TOTAL_RUNS));

  progress = (int)(100.0 * (handler->execution_queue.current - handler->execution_queue.queue) /
                   handler->execution_queue.size) +
             progress / handler->execution_queue.size;

  snprintf(buffer, sizeof(buffer), "%s | %s | Run %i/3 | Kernel %i/24 | %s",
           WB_RUN_TYPE_NAMES[handler->execution_queue.current->run_type],
           WB_OPTIMIZATION_NAMES[handler->execution_queue.current->optimization], run + 1, kernel,
           WB_KERNEL_NAMES[kernel - 1]);

  handler->progress_callback.callback(handler->progress_callback.data, progress, buffer);
}

static void benchmark_kernel_runner(benchmark_handler_t handler, run_result_t *result_p) {
  int thread_idx;
  int i;

  void *(*benchmark)(void *) =
      handler->optimization == WB_OPTIMIZATION_ENABLED ? benchmark_core_optimized : benchmark_core_nonoptimized;

  benchmark_core_args_t **thread_results = calloc(handler->core_count_limit, sizeof(benchmark_core_args_t *));

  thread_handler_p *thread_pool = calloc(handler->core_count_limit, sizeof(thread_handler_p));

  for (thread_idx = 0; thread_idx < handler->core_count_limit; ++thread_idx) {
    thread_results[thread_idx] = calloc(1, sizeof(benchmark_core_args_t));
    thread_results[thread_idx]->execution_time = handler->execution_time;
    thread_results[thread_idx]->callback_handler.callback = NULL;
    thread_results[thread_idx]->callback_handler.data = NULL;
    if (0 == thread_idx) {
      thread_results[thread_idx]->callback_handler.callback = benchmark_proxy_progress_callback;
      thread_results[thread_idx]->callback_handler.data = handler;
    }

    thread_pool[thread_idx] = thread_init(benchmark, thread_results[thread_idx]);
  }
  for (thread_idx = 0; thread_idx < handler->core_count_limit; ++thread_idx) {
    thread_join(thread_pool[thread_idx]);
  }
  free(thread_pool);

  for (thread_idx = 0; thread_idx < handler->core_count_limit; ++thread_idx) {
    for (i = 0; i < WB_KERNEL_COUNT; i++) {
      result_p->kernel_results[i] += thread_results[thread_idx]->results.kernel_results[i];
    }
    free(thread_results[thread_idx]);
  }
  free(thread_results);

  result_p->maximum = 0.0;
  result_p->average = 0.0;
  result_p->geometric = 0.0;
  result_p->harmonic = 0.0;
  result_p->minimum = result_p->kernel_results[0];

  for (i = 0; i < WB_KERNEL_COUNT; i++) {
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

  result_p->average = result_p->average / WB_KERNEL_COUNT;
  result_p->harmonic = 24.0 / result_p->harmonic;
  result_p->geometric = exp(result_p->geometric / WB_KERNEL_COUNT);

  result_p->score = result_p->geometric;
  result_p->ratio = 0;
  result_p->valid = 1;
}

static void benchmark_process_execution_queue(benchmark_handler_t handler) {
  execution_task_t *task;
  execution_task_t *last_task;
  run_result_t *result;

  last_task = handler->execution_queue.queue + handler->execution_queue.size;

  while (handler->execution_queue.current != last_task) {
    task = handler->execution_queue.current;
    result = &handler->run_result[task->optimization][task->run_type];
    handler->execution_time = task->execution_time;
    handler->optimization = task->optimization;
    handler->core_count_limit = task->core_count;

    benchmark_kernel_runner(handler, result);
    handler->execution_queue.current++;
  }
}

BENCHMARK_ERROR_CODE_E
benchmark_run(benchmark_handler_t handler) {
  benchmark_setup_execution_queue(handler);
  benchmark_fill_timestamp(handler);
  benchmark_process_execution_queue(handler);
  if (NULL != handler->progress_callback.callback) {
    handler->progress_callback.callback(NULL, 100, "Done!");
  }
  return BENCHMARK_ERROR_CODE_OK;
}

benchmark_result_t benchmark_get_results(benchmark_handler_t handler) {
  benchmark_result_t result = {0};
  WB_RUN_TYPE_E current_run_type;
  WB_OPTIMIZATION_E optimization;
  run_result_t *run_result;
  double score;

  result.core_count = handler->core_count_limit;
  strncpy(result.timestamp, handler->timestamp, sizeof(result.timestamp));
  strncpy(result.comment, handler->comment, sizeof(result.comment));

  // Copy last results from handler
  for (optimization = WB_OPTIMIZATION_DISABLED; optimization < WB_OPTIMIZATION_SIZE; optimization++) {
    for (current_run_type = WB_RUN_TYPE_MANUAL; current_run_type < WB_RUN_TYPE_SIZE; current_run_type++) {
      memcpy(&result.full_result[optimization].detailed[current_run_type],
             &handler->run_result[optimization][current_run_type], sizeof(run_result_t));
    }
  }

  for (optimization = WB_OPTIMIZATION_DISABLED; optimization < WB_OPTIMIZATION_SIZE; optimization++) {
    score = 1;
    for (current_run_type = WB_RUN_TYPE_SINGLE_CORE; current_run_type < WB_RUN_TYPE_WORKSTATION; current_run_type++) {
      // Copy  result if run is not valid
      if (!result.full_result[optimization].detailed[current_run_type].valid) {
        // prioritize multicore if valid. single core othervise.
        run_result = result.full_result[optimization].detailed[WB_RUN_TYPE_MULTI_CORE].valid
                         ? &result.full_result[optimization].detailed[WB_RUN_TYPE_MULTI_CORE]
                         : &result.full_result[optimization].detailed[WB_RUN_TYPE_SINGLE_CORE];
        memcpy(&result.full_result[optimization].detailed[current_run_type], run_result, sizeof(run_result_t));
      }

      // For score calculation consider only SINGLE_CORE MULTI_CORE
      // QUAD_CORE modes
      if (current_run_type == WB_RUN_TYPE_SINGLE_CORE || current_run_type == WB_RUN_TYPE_MULTI_CORE ||
          current_run_type == WB_RUN_TYPE_QUAD_CORE) {
        score *= result.full_result[optimization].detailed[current_run_type].score;
      }
    }
    result.full_result[optimization].score = cbrt(score);
  }

  // Calculate ratio only if single core is valid
  for (optimization = WB_OPTIMIZATION_DISABLED; optimization < WB_OPTIMIZATION_SIZE; optimization++) {
    if (result.full_result[optimization].detailed[WB_RUN_TYPE_SINGLE_CORE].valid) {
      for (current_run_type = WB_RUN_TYPE_MANUAL; current_run_type < WB_RUN_TYPE_SIZE; current_run_type++) {
        result.full_result[optimization].detailed[current_run_type].ratio =
            result.full_result[optimization].detailed[current_run_type].score /
            result.full_result[optimization].detailed[WB_RUN_TYPE_SINGLE_CORE].score;
      }
    }
  }

  return result;
}
benchmark_parameters_t benchmark_get_parameters(benchmark_handler_t handler) {
  return handler->parameters;
}
