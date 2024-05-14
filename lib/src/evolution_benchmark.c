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

#include <waverian/evolution_benchmark.h>
#include <waverian/evolution_benchmark/constants.h>
#include <waverian/evolution_benchmark/types.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core/core.h"
#include "platform.h"
#include "thread.h"

static const char *EMPTY_STR = "";
static const char *COMPILER = EB_COMPILER;
static const char *VERSION = EB_VERSION;

typedef struct {
  EB_RUN_TYPE_E run_type;
  EB_OPTIMIZATION_E optimization;

  int core_count;
  double execution_time;
} execution_task_t;

typedef struct eb_handler_ {
  int core_count_limit;
  double execution_time;
  EB_OPTIMIZATION_E optimization;
  EB_EXECUTION_MODE_E execution_mode;

  char *timestamp;
  char *cpu_name;
  int cpu_logical_cores_count;
  char *comment;

  eb_progress_callback_handler_t progress_callback;

  struct {
    execution_task_t *current;
    execution_task_t queue[EB_RUN_TYPE_SIZE * EB_OPTIMIZATION_SIZE];
    int size;
  } execution_queue;
} eb_handler_;

static void benchmark_setup_execution_queue(eb_handler_t handler) {
  EB_OPTIMIZATION_E optimization;
  execution_task_t *task;

  if (handler->execution_mode == EB_EXECUTION_MODE_MANUAL) {
    task = handler->execution_queue.queue + handler->execution_queue.size++;
    task->run_type = EB_RUN_TYPE_MANUAL;
    task->optimization = handler->optimization;
    task->execution_time = handler->execution_time;
    task->core_count = handler->core_count_limit;
    return;
  }

  for (optimization = EB_OPTIMIZATION_DISABLED; optimization < EB_OPTIMIZATION_SIZE; optimization++) {
    task = handler->execution_queue.queue + handler->execution_queue.size++;
    task->run_type = EB_RUN_TYPE_SINGLE_CORE;
    task->optimization = optimization;
    task->execution_time = handler->execution_time;
    task->core_count = 1;

    if (handler->core_count_limit > 4) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = EB_RUN_TYPE_QUAD_CORE;
      task->optimization = optimization;
      task->execution_time = handler->execution_time;
      task->core_count = 4;
    }

    if (handler->core_count_limit > 1) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = EB_RUN_TYPE_MULTI_CORE;
      task->optimization = optimization;
      task->execution_time = handler->execution_time;
      task->core_count = handler->core_count_limit;
    }

    if (handler->execution_mode == EB_EXECUTION_MODE_FULL_AUTO) {
      task = handler->execution_queue.queue + handler->execution_queue.size++;
      task->run_type = EB_RUN_TYPE_WORKSTATION;
      task->optimization = optimization;
      task->execution_time = handler->execution_time * 8;
      task->core_count = handler->core_count_limit;
    }
  }
}

static void benchmark_fill_timestamp(eb_handler_t handler) {
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

static void benchmark_fill_cpu_name(eb_handler_t handler) {
  char *str;
  const char *ptr;
  str = get_cpu_name();
  ptr = str;
  while (strchr("\n\t ", *ptr) != NULL) {
    ptr++;
  }
  handler->cpu_name = strdup(ptr);
  free(str);
}

eb_handler_t eb_init() {
  eb_handler_t handler = calloc(1, sizeof(eb_handler_));

  handler->execution_time = 0.5;
  handler->optimization = EB_OPTIMIZATION_DISABLED;
  handler->execution_mode = EB_EXECUTION_MODE_AUTO;
  handler->progress_callback.data = NULL;
  handler->progress_callback.callback = NULL;

  benchmark_fill_cpu_name(handler);
  handler->cpu_logical_cores_count = get_core_count();

  eb_set_core_count_limit(handler, 0);

  handler->comment = strdup(EMPTY_STR);
  handler->timestamp = strdup(EMPTY_STR);

  handler->execution_queue.size = 0;
  handler->execution_queue.current = handler->execution_queue.queue;
  return handler;
}

EB_ERROR_CODE_E
eb_cleanup(eb_handler_t handler) {
  if (NULL == handler) {
    return EB_ERROR_CODE_NULL_POINTER;
  }

  free(handler->timestamp);
  free(handler->cpu_name);
  free(handler->comment);
  free(handler);

  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_core_count_limit(eb_handler_t handler, int core_count) {
  if (0 == core_count) {
    handler->core_count_limit = handler->cpu_logical_cores_count;
  } else {
    handler->core_count_limit = core_count;
  }

  if (handler->cpu_logical_cores_count < core_count) {
    return EB_ERROR_CODE_INVALID_ARGUMENT;
  }
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_progress_callback(eb_handler_t handler, eb_progress_callback_handler_t callback) {
  handler->progress_callback = callback;
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_execution_time(eb_handler_t handler, double execution_time) {
  handler->execution_time = execution_time;
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_execution_mode(eb_handler_t handler, EB_EXECUTION_MODE_E execution_mode) {
  handler->execution_mode = execution_mode;
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_optimization(eb_handler_t handler, EB_OPTIMIZATION_E optimization) {
  handler->optimization = optimization;
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E
eb_set_comment(eb_handler_t handler, const char *comment) {
  if (NULL == comment) {
    return EB_ERROR_CODE_INVALID_ARGUMENT;
  }

  handler->comment = strdup(comment);
  return EB_ERROR_CODE_OK;
}

int benchmark_proxy_progress_callback(void *handler_as_void, int kernel, int run) {
  static const int TOTAL_KERNELS = 24;
  static const int TOTAL_RUNS = 3;
  int progress;
  char buffer[256];

  eb_handler_t handler = handler_as_void;

  progress = (int)(100.0 * (TOTAL_KERNELS * (run) + kernel) / (TOTAL_KERNELS * TOTAL_RUNS));

  progress = (int)(100.0 * (handler->execution_queue.current - handler->execution_queue.queue) /
                   handler->execution_queue.size) +
             progress / handler->execution_queue.size;

  snprintf(buffer, sizeof(buffer), "%s | %s | Run %i/3 | Kernel %i/24 | %s",
           EB_RUN_TYPE_NAMES[handler->execution_queue.current->run_type],
           EB_OPTIMIZATION_NAMES[handler->execution_queue.current->optimization], run + 1, kernel,
           EB_KERNEL_NAMES[kernel - 1]);

  return handler->progress_callback.callback(handler->progress_callback.data, progress, buffer);
}

static void benchmark_kernel_runner(eb_handler_t handler, eb_run_result_t *result_p) {
  int thread_idx;
  int i;

  void *(*benchmark)(void *) =
      handler->optimization == EB_OPTIMIZATION_ENABLED ? benchmark_core_optimized : benchmark_core_nonoptimized;

  eb_core_args_t **thread_results = calloc(handler->core_count_limit, sizeof(eb_core_args_t *));

  thread_handler_p *thread_pool = calloc(handler->core_count_limit, sizeof(thread_handler_p));

  for (thread_idx = 0; thread_idx < handler->core_count_limit; ++thread_idx) {
    thread_results[thread_idx] = calloc(1, sizeof(eb_core_args_t));
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
    for (i = 0; i < EB_KERNEL_COUNT; i++) {
      result_p->kernels[i] += thread_results[thread_idx]->results.kernels[i];
    }
    free(thread_results[thread_idx]);
  }
  free(thread_results);

  result_p->maximum = 0.0;
  result_p->average = 0.0;
  result_p->geometric = 0.0;
  result_p->harmonic = 0.0;
  result_p->minimum = result_p->kernels[0];

  for (i = 0; i < EB_KERNEL_COUNT; i++) {
    result_p->average += result_p->kernels[i];
    result_p->harmonic += 1 / result_p->kernels[i];
    result_p->geometric += log(result_p->kernels[i]);
    if (result_p->kernels[i] < result_p->minimum) {
      result_p->minimum = result_p->kernels[i];
    }
    if (result_p->kernels[i] > result_p->maximum) {
      result_p->maximum = result_p->kernels[i];
    }
  }

  result_p->average = result_p->average / EB_KERNEL_COUNT;
  result_p->harmonic = 24.0 / result_p->harmonic;
  result_p->geometric = exp(result_p->geometric / EB_KERNEL_COUNT);

  result_p->score = result_p->geometric;
  result_p->ratio = 0;
  result_p->valid = 1;
}

static void benchmark_process_execution_queue(eb_handler_t handler, eb_result_t *result) {
  execution_task_t *task;
  execution_task_t *last_task;
  eb_run_result_t *run_result;

  last_task = handler->execution_queue.queue + handler->execution_queue.size;

  while (handler->execution_queue.current != last_task) {
    task = handler->execution_queue.current;
    run_result = &result->full_result[task->optimization].detailed[task->run_type];
    handler->execution_time = task->execution_time;
    handler->optimization = task->optimization;
    handler->core_count_limit = task->core_count;

    benchmark_kernel_runner(handler, run_result);
    handler->execution_queue.current++;
  }

  handler->execution_queue.size = 0;
  handler->execution_queue.current = handler->execution_queue.queue;
}

static void benchmark_fill_results(eb_handler_t handler, eb_result_t *result) {
  EB_RUN_TYPE_E current_run_type;
  EB_OPTIMIZATION_E optimization;
  eb_run_result_t *run_result;
  double score;

  result->core_count = handler->core_count_limit;
  strncpy(result->timestamp, handler->timestamp, sizeof(result->timestamp));
  strncpy(result->comment, handler->comment, sizeof(result->comment));

  for (optimization = EB_OPTIMIZATION_DISABLED; optimization < EB_OPTIMIZATION_SIZE; optimization++) {
    score = 1;
    for (current_run_type = EB_RUN_TYPE_SINGLE_CORE; current_run_type < EB_RUN_TYPE_WORKSTATION; current_run_type++) {
      // Copy  result if run is not valid
      if (!result->full_result[optimization].detailed[current_run_type].valid) {
        // prioritize multicore if valid. single core othervise.
        run_result = result->full_result[optimization].detailed[EB_RUN_TYPE_MULTI_CORE].valid
                         ? &result->full_result[optimization].detailed[EB_RUN_TYPE_MULTI_CORE]
                         : &result->full_result[optimization].detailed[EB_RUN_TYPE_SINGLE_CORE];
        memcpy(&result->full_result[optimization].detailed[current_run_type], run_result, sizeof(eb_run_result_t));
      }

      // For score calculation consider only SINGLE_CORE MULTI_CORE
      // QUAD_CORE modes
      if (current_run_type == EB_RUN_TYPE_SINGLE_CORE || current_run_type == EB_RUN_TYPE_MULTI_CORE ||
          current_run_type == EB_RUN_TYPE_QUAD_CORE) {
        score *= result->full_result[optimization].detailed[current_run_type].score;
      }
    }
    result->full_result[optimization].score = cbrt(score);
  }

  // Calculate ratio only if single core is valid
  for (optimization = EB_OPTIMIZATION_DISABLED; optimization < EB_OPTIMIZATION_SIZE; optimization++) {
    if (result->full_result[optimization].detailed[EB_RUN_TYPE_SINGLE_CORE].valid) {
      for (current_run_type = EB_RUN_TYPE_MANUAL; current_run_type < EB_RUN_TYPE_SIZE; current_run_type++) {
        result->full_result[optimization].detailed[current_run_type].ratio =
            result->full_result[optimization].detailed[current_run_type].score /
            result->full_result[optimization].detailed[EB_RUN_TYPE_SINGLE_CORE].score;
      }
    }
  }

  // Calculate optimization ratio, skip disabled optimization
  for (optimization = EB_OPTIMIZATION_DISABLED; optimization < EB_OPTIMIZATION_SIZE; optimization++) {
    result->full_result[optimization].optimized_ratio = result->full_result[optimization].score / result->full_result[EB_OPTIMIZATION_DISABLED].score;
  }
}

EB_ERROR_CODE_E
eb_run_benchmark(eb_handler_t handler, eb_result_t *result) {
  EB_ERROR_CODE_E error_code;
  if (NULL == handler) {
    return EB_ERROR_CODE_NULL_POINTER;
  }
  if (NULL == result) {
    return EB_ERROR_CODE_NULL_POINTER;
  }

  memset(result, 0, sizeof(eb_result_t));

  benchmark_setup_execution_queue(handler);
  benchmark_fill_timestamp(handler);
  benchmark_process_execution_queue(handler, result);
  benchmark_fill_results(handler, result);

  error_code = eb_get_system_info(handler, &result->system_info);

  if (EB_ERROR_CODE_OK != error_code) {
    return error_code;
  }

  if (NULL != handler->progress_callback.callback) {
    handler->progress_callback.callback(handler->progress_callback.data, 100, "Done!");
  }
  return EB_ERROR_CODE_OK;
}

EB_ERROR_CODE_E eb_get_system_info(eb_handler_t handler, eb_system_info_t *system_info) {
  if (NULL == handler) {
    return EB_ERROR_CODE_NULL_POINTER;
  }
  if (NULL == system_info) {
    return EB_ERROR_CODE_NULL_POINTER;
  }

  system_info->compiler_info = COMPILER;
  system_info->version_info = VERSION;
  system_info->cpu_name = handler->cpu_name;
  system_info->cpu_core_count = handler->cpu_logical_cores_count;

  return EB_ERROR_CODE_OK;
}
