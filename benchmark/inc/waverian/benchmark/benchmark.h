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

#ifndef WAVERIAN_BENCHMARK_BENCHMARK_H
#define WAVERIAN_BENCHMARK_BENCHMARK_H

#include <waverian/benchmark/details/error_code.h>
#include <waverian/benchmark/details/types.h>

typedef void (*benchmark_progress_callback_t)(void *data, int progress,
                                              const char *message);

typedef struct {
  void *data;
  benchmark_progress_callback_t callback;
} benchmark_progress_callback_handler_t;

benchmark_handler_t benchmark_init();

BENCHMARK_ERROR_CODE_E benchmark_cleanup(benchmark_handler_t handler);

BENCHMARK_ERROR_CODE_E benchmark_set_progress_callback(
    benchmark_handler_t handler,
    benchmark_progress_callback_handler_t callback);

BENCHMARK_ERROR_CODE_E
benchmark_set_core_count_limit(benchmark_handler_t handler, int core_count);

BENCHMARK_ERROR_CODE_E
benchmark_set_execution_time(benchmark_handler_t handler,
                             double execution_time);

BENCHMARK_ERROR_CODE_E benchmark_set_comment(benchmark_handler_t handler,
                                             const char *comment);

BENCHMARK_ERROR_CODE_E
benchmark_set_execution_mode(benchmark_handler_t handler,
                             WB_EXECUTION_MODE_E execution_mode);

BENCHMARK_ERROR_CODE_E
benchmark_set_optimization_enabled(benchmark_handler_t handler,
                                   WB_OPTIMIZATION_E optimization);

BENCHMARK_ERROR_CODE_E benchmark_run(benchmark_handler_t handler);

benchmark_parameters_t benchmark_get_parameters(benchmark_handler_t handler);

benchmark_result_t benchmark_get_results(benchmark_handler_t handler);

#endif /*WAVERIAN_BENCHMARK_BENCHMARK_H*/
