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

#ifndef WAVERIAN_BENCHMARK_PRINTERS_H
#define WAVERIAN_BENCHMARK_PRINTERS_H

#include <waverian/benchmark/details/error_code.h>
#include <waverian/benchmark/details/types.h>

/**
 * This functions prints results of the benchmark run to the HTML file by the
 * given \p path.
 *
 * @param handler
 * @param path target path to output html file
 * @return
 */
BENCHMARK_ERROR_CODE_E
benchmark_print_results_html(const benchmark_handler_t handler,
                             const char *path);

/**
 * This functions prints results of the benchmark run to the text file by the
 * given \p path.
 *
 * @param handler benchmark handler
 * @param path target path to output text file
 * @return
 */
BENCHMARK_ERROR_CODE_E
benchmark_print_results_text(const benchmark_handler_t handler,
                             const char *path);

#endif /*WAVERIAN_BENCHMARK_PRINTERS_H*/
