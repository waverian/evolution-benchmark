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

#ifndef WAVERIAN_BENCHMARK_RESULT_H
#define WAVERIAN_BENCHMARK_RESULT_H

typedef struct benchmark_handler_ *benchmark_handler_t;

typedef enum {
  WB_RUN_TYPE_MANUAL = 0,
  WB_RUN_TYPE_SINGLE_CORE,
  WB_RUN_TYPE_MULTI_CORE,
  WB_RUN_TYPE_QUAD_CORE,
  WB_RUN_TYPE_WORKSTATION,
  WB_RUN_TYPE_SIZE,
} WB_RUN_TYPE_E;

typedef enum {
  WB_EXECUTION_MODE_MANUAL = 0,
  WB_EXECUTION_MODE_AUTO,
  WB_EXECUTION_MODE_FULL_AUTO,
  WB_EXECUTION_MODE_SIZE,
} WB_EXECUTION_MODE_E;

typedef enum {
  WB_OPTIMIZATION_DISABLED = 0,
  WB_OPTIMIZATION_ENABLED,
  WB_OPTIMIZATION_SIZE,
} WB_OPTIMIZATION_E;

typedef struct {
  int valid;

  double score;
  double ratio;

  double maximum;
  double average;
  double geometric;
  double harmonic;
  double minimum;

  double kernel_results[24];
} run_result_t;

typedef struct {
  double score;
  run_result_t detailed[WB_RUN_TYPE_SIZE];
} full_result_t[WB_OPTIMIZATION_SIZE];

typedef struct {
  char timestamp[32];
  char comment[128];
  int core_count;

  full_result_t full_result;
} benchmark_result_t;

typedef struct {
  const char *compiler_info;
  const char *version_info;
  const char *cpu_name;
  int cpu_core_count;
} benchmark_parameters_t;

#endif /* WAVERIAN_BENCHMARK_RESULT_H */
