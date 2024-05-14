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

#ifndef WAVERIAN_EVOLUTION_BENCHMARK_RESULT_H
#define WAVERIAN_EVOLUTION_BENCHMARK_RESULT_H

typedef struct eb_handler_ *eb_handler_t;

typedef enum {
  EB_RUN_TYPE_MANUAL = 0,
  EB_RUN_TYPE_SINGLE_CORE,
  EB_RUN_TYPE_MULTI_CORE,
  EB_RUN_TYPE_QUAD_CORE,
  EB_RUN_TYPE_WORKSTATION,
  EB_RUN_TYPE_SIZE,
} EB_RUN_TYPE_E;

typedef enum {
  EB_EXECUTION_MODE_MANUAL = 0,
  EB_EXECUTION_MODE_AUTO,
  EB_EXECUTION_MODE_FULL_AUTO,
  EB_EXECUTION_MODE_SIZE,
} EB_EXECUTION_MODE_E;

typedef enum {
  EB_OPTIMIZATION_DISABLED = 0,
  EB_OPTIMIZATION_ENABLED,
  EB_OPTIMIZATION_SIZE,
} EB_OPTIMIZATION_E;

typedef struct {
  const char *compiler_info;
  const char *version_info;
  const char *cpu_name;
  int cpu_core_count;
} eb_system_info_t;

typedef struct {
  int valid;

  double score;
  double ratio;

  double maximum;
  double average;
  double geometric;
  double harmonic;
  double minimum;

  double kernels[24];
} eb_run_result_t;

typedef struct {
  double score;
  double optimized_ratio;

  eb_run_result_t detailed[EB_RUN_TYPE_SIZE];
} eb_full_result_t;

typedef struct {
  eb_system_info_t system_info;

  char timestamp[32];
  char comment[128];
  int core_count;

  eb_full_result_t full_result[EB_OPTIMIZATION_SIZE];
} eb_result_t;

#endif /* WAVERIAN_EVOLUTION_BENCHMARK_RESULT_H */
