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

#ifndef LFKX_LFK_RESULT_H
#define LFKX_LFK_RESULT_H

typedef enum {
  single_core_non_optimized = 0,
  multi_core_non_optimized,
  single_core_optimized,
  multi_core_optimized,
} lfk_run_type_e;

typedef struct {
  lfk_run_type_e run_type;

  double maximum;
  double average;
  double geometric;
  double harmonic;
  double minimum;

  double kernel_results[24];
} lfk_run_result_t;

typedef struct {
  double singlecore_result;
  double multicore_result;

  lfk_run_result_t lfk_run_result[4];
} lfk_full_result_t;

#endif /* LFKX_LFK_RESULT_H */
