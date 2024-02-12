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

#ifndef WAVERIAN_EVOLUTION_BENCHMARK_BENCHMARK_H
#define WAVERIAN_EVOLUTION_BENCHMARK_BENCHMARK_H

#include "waverian/evolution_benchmark/error_code.h"
#include "waverian/evolution_benchmark/types.h"

typedef int (*eb_progress_callback_t)(void *data, int progress, const char *message);

typedef struct {
  void *data;
  eb_progress_callback_t callback;
} eb_progress_callback_handler_t;

eb_handler_t eb_init();

EB_ERROR_CODE_E eb_cleanup(eb_handler_t handler);

EB_ERROR_CODE_E
eb_set_progress_callback(eb_handler_t handler, eb_progress_callback_handler_t callback);

EB_ERROR_CODE_E
eb_set_core_count_limit(eb_handler_t handler, int core_count);

EB_ERROR_CODE_E
eb_set_execution_time(eb_handler_t handler, double execution_time);

EB_ERROR_CODE_E
eb_set_comment(eb_handler_t handler, const char *comment);

EB_ERROR_CODE_E
eb_set_execution_mode(eb_handler_t handler, EB_EXECUTION_MODE_E execution_mode);

EB_ERROR_CODE_E
eb_set_optimization(eb_handler_t handler, EB_OPTIMIZATION_E optimization);

EB_ERROR_CODE_E
eb_run_benchmark(eb_handler_t handler, eb_result_t *result);

EB_ERROR_CODE_E
eb_get_system_info(eb_handler_t handler, eb_system_info_t *system_info);

#endif /*WAVERIAN_EVOLUTION_BENCHMARK_BENCHMARK_H*/
