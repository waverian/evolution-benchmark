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

#ifndef LFKX_LFK_H
#define LFKX_LFK_H

#include "lfk/types.h"

typedef struct benchmark_handler_ *benchmark_handler_t;

typedef void (*benchmark_progress_callback_t)(void *data, int progress,
                                              const char *message);

typedef struct {
  void *data;
  benchmark_progress_callback_t callback;
} benchmark_progress_callback_handler_t;

benchmark_handler_t benchmark_init();

void benchmark_cleanup(benchmark_handler_t handler);

void benchmark_set_progress_callback(
    benchmark_handler_t handler,
    benchmark_progress_callback_handler_t callback);

void benchmark_set_core_count(benchmark_handler_t handler, unsigned core_count);

void benchmark_set_execution_time(benchmark_handler_t handler,
                                  double execution_time);

void benchmark_set_optimization_enabled(benchmark_handler_t handler,
                                        unsigned optimization_enabled);

void benchmark_run(benchmark_handler_t handler);

const char *benchmark_get_version(benchmark_handler_t handler);

const char *benchmark_get_compiler_info(benchmark_handler_t handler);

const char *benchmark_get_date(benchmark_handler_t handler);

const lfk_full_result_t *benchmark_get_results(benchmark_handler_t handler);

unsigned int benchmark_get_core_count(benchmark_handler_t handler);

#endif /*LFKX_LFK_H*/
