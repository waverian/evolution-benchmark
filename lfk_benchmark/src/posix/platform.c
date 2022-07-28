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

#include "platform.h"
#include <string.h>
#include <unistd.h>

#ifdef LFK_USE_CPUID
#include <cpuid.h>
#endif

unsigned get_core_count() { return (int)sysconf(_SC_NPROCESSORS_ONLN); }

char *get_cpu_name() {
#if defined(LFK_USE_CPUID)
  unsigned int brand[12];
  if (!__get_cpuid_max(0x80000004, NULL)) {
    return strdup("No CPUID information");
  }

  __get_cpuid(0x80000002, brand + 0x0, brand + 0x1, brand + 0x2, brand + 0x3);
  __get_cpuid(0x80000003, brand + 0x4, brand + 0x5, brand + 0x6, brand + 0x7);
  __get_cpuid(0x80000004, brand + 0x8, brand + 0x9, brand + 0xa, brand + 0xb);

  // remove trailing whitespaces
  char *ret = (char *)brand;
  while (' ' == *ret) {
    ret++;
  }

  return strdup((char *)brand);
#else
  return strdup("No CPUID information");
#endif
}